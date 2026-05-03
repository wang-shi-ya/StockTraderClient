#include "dbconnectionpool.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMutexLocker>
#include <QProcessEnvironment>

DbConnectionPool::DbConnectionPool(QObject *parent)
    : QObject(parent)
    , m_port(3306)
    , m_minConnections(2)
    , m_maxConnections(10)
    , m_cleanupTimer(nullptr)
    , m_initialized(false)
{
    // 延迟初始化定时器，直到QApplication创建后
}

DbConnectionPool::~DbConnectionPool() {
    closeAll();
}

DbConnectionPool& DbConnectionPool::instance() {
    static DbConnectionPool instance;
    return instance;
}

bool DbConnectionPool::initialize(int minConnections, int maxConnections) {
    QMutexLocker locker(&m_mutex);
    
    if (m_initialized) {
        return true;
    }
    
    // 读取环境变量，获取数据库配置
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        m_host = env.value("DB_HOST", "127.0.0.1");
        const QString portStr = env.value("DB_PORT", "3306");
        m_databaseName = env.value("DB_NAME", "stock_trader");
        m_userName = env.value("DB_USER", "root");
        
        m_password = env.value("DB_PASSWORD");
        if (m_password.isEmpty()) {
            m_password = env.value("db_password");
        }

        if (m_password.isEmpty()) {
            qWarning() << "DB_PASSWORD environment variable not set, database connection may fail";
        }
    
    bool ok = false;
    m_port = portStr.toInt(&ok);
    if (!ok) {
        m_port = 3306;
    }
    
    m_minConnections = qMax(1, minConnections);
    m_maxConnections = qMax(m_minConnections, maxConnections);

    // 注意：QSqlDatabase 连接与线程绑定，不能跨线程复用。
    // 这里仅为“当前线程”预热创建 minConnections 个连接；其他线程会在首次 getConnection() 时按需创建。
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    QQueue<QString> &avail = m_availableConnectionsByThread[tid];
    for (int i = 0; i < m_minConnections; ++i) {
        const QString connectionName = QString("PoolConnection_%1_%2").arg(tid).arg(i);
        QSqlDatabase db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_connectionOwnerThread.insert(connectionName, tid);
            avail.enqueue(connectionName);
        } else {
            qWarning() << "Failed to create initial connection:" << db.lastError();
        }
    }

    if (avail.isEmpty()) {
        qWarning() << "Failed to create any initial connections for thread" << tid;
        return false;
    }
    
    // 初始化定时器，每5分钟清理一次无效连接
    if (!m_cleanupTimer) {
        m_cleanupTimer = new QTimer(this);
        connect(m_cleanupTimer, &QTimer::timeout, this, &DbConnectionPool::cleanupInvalidConnections);
        m_cleanupTimer->start(300000); // 5分钟
    }
    
    m_initialized = true;
    qDebug() << "Connection pool initialized with" << avail.size() << "connections (thread" << tid << ")";
    return true;
}

QSqlDatabase DbConnectionPool::getConnection() {
    QMutexLocker locker(&m_mutex);
    
    if (!m_initialized) {
        if (!initialize()) {
            return QSqlDatabase();
        }
    }
    
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    QString connectionName;

    QQueue<QString> &avail = m_availableConnectionsByThread[tid];
    QSet<QString> &used = m_usedConnectionsByThread[tid];

    // 尝试从当前线程的可用连接队列获取
    if (!avail.isEmpty()) {
        connectionName = avail.dequeue();
        QSqlDatabase db = QSqlDatabase::database(connectionName);

        // 检查连接是否有效
        if (isConnectionValid(db)) {
            used.insert(connectionName);
            return db;
        }

        // 连接无效，移除并尝试重建（同一线程内）
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }
        db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_connectionOwnerThread.insert(connectionName, tid);
            used.insert(connectionName);
            return db;
        }
    }

    // 如果没有可用连接，尝试创建新连接（按线程限制最大连接数）
    const int totalInThread = avail.size() + used.size();
    if (totalInThread < m_maxConnections) {
        connectionName = QString("PoolConnection_%1_%2").arg(tid).arg(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_connectionOwnerThread.insert(connectionName, tid);
            used.insert(connectionName);
            return db;
        }
    }

    // 如果无法创建新连接，等待当前线程可用连接
    int waitCount = 0;
    while (avail.isEmpty() && waitCount < 100) {
        m_waitCondition.wait(&m_mutex, 100);
        waitCount++;
    }

    if (!avail.isEmpty()) {
        connectionName = avail.dequeue();
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (isConnectionValid(db)) {
            used.insert(connectionName);
            return db;
        }
    }
    
    qWarning() << "Failed to get connection from pool";
    return QSqlDatabase();
}

void DbConnectionPool::releaseConnection(const QSqlDatabase &connection) {
    if (!connection.isValid()) {
        return;
    }
    
    QMutexLocker locker(&m_mutex);
    
    QString connectionName = connection.connectionName();

    const quintptr ownerTid = m_connectionOwnerThread.value(connectionName, 0);
    if (ownerTid == 0) {
        return;
    }

    QSet<QString> &used = m_usedConnectionsByThread[ownerTid];
    if (!used.contains(connectionName)) {
        return;
    }

    used.remove(connectionName);

    // 检查连接是否仍然有效
    if (isConnectionValid(connection)) {
        QQueue<QString> &avail = m_availableConnectionsByThread[ownerTid];
        avail.enqueue(connectionName);
        m_waitCondition.wakeOne();
        return;
    }

    // 连接无效：removeDatabase 必须在所属线程调用，否则会触发 Qt 的跨线程警告
    const quintptr currentTid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    if (currentTid != ownerTid) {
        qWarning() << "releaseConnection called from different thread. connection:"
                   << connectionName << "owner:" << ownerTid << "current:" << currentTid;
        return;
    }
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }
    m_connectionOwnerThread.remove(connectionName);
}

int DbConnectionPool::availableConnections() const {
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    return m_availableConnectionsByThread.value(tid).size();
}

int DbConnectionPool::activeConnections() const {
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    return m_usedConnectionsByThread.value(tid).size();
}

int DbConnectionPool::totalConnections() const {
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    return m_availableConnectionsByThread.value(tid).size()
        + m_usedConnectionsByThread.value(tid).size();
}

void DbConnectionPool::closeAll() {
    QMutexLocker locker(&m_mutex);
    
    if (m_cleanupTimer) {
        m_cleanupTimer->stop();
    }

    // 最佳努力清理：仅清理当前线程拥有的连接，避免跨线程 removeDatabase 的 Qt 警告。
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());

    QQueue<QString> &avail = m_availableConnectionsByThread[tid];
    while (!avail.isEmpty()) {
        const QString connectionName = avail.dequeue();
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }
        m_connectionOwnerThread.remove(connectionName);
    }

    QSet<QString> &used = m_usedConnectionsByThread[tid];
    for (const QString &connectionName : used) {
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }
        m_connectionOwnerThread.remove(connectionName);
    }
    used.clear();
    
    m_initialized = false;
}

QSqlDatabase DbConnectionPool::createConnection(const QString &connectionName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setHostName(m_host);
    db.setPort(m_port);
    db.setDatabaseName(m_databaseName);
    db.setUserName(m_userName);
    db.setPassword(m_password);
    
    if (!db.open()) {
        qWarning() << "Failed to open connection" << connectionName << ":" << db.lastError();
        return QSqlDatabase();
    }
    
    return db;
}

bool DbConnectionPool::isConnectionValid(const QSqlDatabase &connection) {
    if (!connection.isValid() || !connection.isOpen()) {
        return false;
    }
    
    // 执行一个简单的查询来测试连接
    QSqlQuery query(connection);
    if (!query.exec("SELECT 1")) {
        qWarning() << "Connection validation failed:" << query.lastError();
        return false;
    }
    
    return true;
}

void DbConnectionPool::cleanupInvalidConnections() {
    QMutexLocker locker(&m_mutex);
    
    QQueue<QString> validConnections;
    
    const quintptr tid = reinterpret_cast<quintptr>(QThread::currentThreadId());
    QQueue<QString> &avail = m_availableConnectionsByThread[tid];

    // 检查当前线程的可用连接
    while (!avail.isEmpty()) {
        QString connectionName = avail.dequeue();
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase db = QSqlDatabase::database(connectionName);
            if (isConnectionValid(db)) {
                validConnections.enqueue(connectionName);
            } else {
                // 连接无效，移除
                QSqlDatabase::removeDatabase(connectionName);
                m_connectionOwnerThread.remove(connectionName);
            }
        }
    }
    
    avail = validConnections;
    
    // 确保至少保持最小连接数
    int currentTotal = avail.size() + m_usedConnectionsByThread.value(tid).size();
    while (currentTotal < m_minConnections) {
        QString connectionName = QString("PoolConnection_%1_%2").arg(tid).arg(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_connectionOwnerThread.insert(connectionName, tid);
            avail.enqueue(connectionName);
            currentTotal++;
        } else {
            break; // 无法创建连接，退出循环
        }
    }
}

