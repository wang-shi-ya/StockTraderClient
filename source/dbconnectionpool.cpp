#include "dbconnectionpool.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QProcessEnvironment>
#include <QDebug>
#include <QThread>

DbConnectionPool::DbConnectionPool(QObject *parent)
    : QObject(parent)
    , m_port(3306)
    , m_minConnections(2)
    , m_maxConnections(10)
    , m_cleanupTimer(new QTimer(this))
    , m_initialized(false)
{
    // 设置定时器，每5分钟清理一次无效连接
    connect(m_cleanupTimer, &QTimer::timeout, this, &DbConnectionPool::cleanupInvalidConnections);
    m_cleanupTimer->start(300000); // 5分钟
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
    m_password = env.value("DB_PASSWORD", "HBsc75820306@");
    
    bool ok = false;
    m_port = portStr.toInt(&ok);
    if (!ok) {
        m_port = 3306;
    }
    
    m_minConnections = qMax(1, minConnections);
    m_maxConnections = qMax(m_minConnections, maxConnections);
    
    // 创建初始连接
    for (int i = 0; i < m_minConnections; ++i) {
        QString connectionName = QString("PoolConnection_%1").arg(i);
        QSqlDatabase db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_availableConnections.enqueue(connectionName);
        } else {
            qWarning() << "Failed to create initial connection:" << db.lastError();
        }
    }
    
    if (m_availableConnections.isEmpty()) {
        qWarning() << "Failed to create any initial connections";
        return false;
    }
    
    m_initialized = true;
    qDebug() << "Connection pool initialized with" << m_availableConnections.size() << "connections";
    return true;
}

QSqlDatabase DbConnectionPool::getConnection() {
    QMutexLocker locker(&m_mutex);
    
    if (!m_initialized) {
        if (!initialize()) {
            return QSqlDatabase();
        }
    }
    
    QString connectionName;
    
    // 尝试从可用连接队列获取
    if (!m_availableConnections.isEmpty()) {
        connectionName = m_availableConnections.dequeue();
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        
        // 检查连接是否有效
        if (isConnectionValid(db)) {
            m_usedConnections.insert(connectionName);
            return db;
        } else {
            // 连接无效，移除并尝试重建
            QSqlDatabase::removeDatabase(connectionName);
            db = createConnection(connectionName);
            if (db.isValid() && db.isOpen()) {
                m_usedConnections.insert(connectionName);
                return db;
            }
        }
    }
    
    // 如果没有可用连接，尝试创建新连接（不超过最大连接数）
    int totalConnections = m_availableConnections.size() + m_usedConnections.size();
    if (totalConnections < m_maxConnections) {
        connectionName = QString("PoolConnection_%1").arg(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_usedConnections.insert(connectionName);
            return db;
        }
    }
    
    // 如果无法创建新连接，等待可用连接
    int waitCount = 0;
    while (m_availableConnections.isEmpty() && waitCount < 100) {
        m_waitCondition.wait(&m_mutex, 100); // 等待100ms
        waitCount++;
    }
    
    if (!m_availableConnections.isEmpty()) {
        connectionName = m_availableConnections.dequeue();
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (isConnectionValid(db)) {
            m_usedConnections.insert(connectionName);
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
    
    if (m_usedConnections.contains(connectionName)) {
        m_usedConnections.remove(connectionName);
        
        // 检查连接是否仍然有效
        if (isConnectionValid(connection)) {
            m_availableConnections.enqueue(connectionName);
            m_waitCondition.wakeOne(); // 唤醒等待的线程
        } else {
            // 连接无效，移除并关闭
            QSqlDatabase::removeDatabase(connectionName);
        }
    }
}

int DbConnectionPool::availableConnections() const {
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    return m_availableConnections.size();
}

int DbConnectionPool::activeConnections() const {
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    return m_usedConnections.size();
}

int DbConnectionPool::totalConnections() const {
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    return m_availableConnections.size() + m_usedConnections.size();
}

void DbConnectionPool::closeAll() {
    QMutexLocker locker(&m_mutex);
    
    if (m_cleanupTimer) {
        m_cleanupTimer->stop();
    }
    
    // 关闭所有可用连接
    while (!m_availableConnections.isEmpty()) {
        QString connectionName = m_availableConnections.dequeue();
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }
    }
    
    // 关闭所有正在使用的连接（注意：这可能会导致问题，但清理时应该已经释放）
    for (const QString &connectionName : m_usedConnections) {
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }
    }
    m_usedConnections.clear();
    
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
    
    // 检查可用连接
    while (!m_availableConnections.isEmpty()) {
        QString connectionName = m_availableConnections.dequeue();
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase db = QSqlDatabase::database(connectionName);
            if (isConnectionValid(db)) {
                validConnections.enqueue(connectionName);
            } else {
                // 连接无效，移除
                QSqlDatabase::removeDatabase(connectionName);
            }
        }
    }
    
    m_availableConnections = validConnections;
    
    // 确保至少保持最小连接数
    int currentTotal = m_availableConnections.size() + m_usedConnections.size();
    while (currentTotal < m_minConnections) {
        QString connectionName = QString("PoolConnection_%1").arg(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase db = createConnection(connectionName);
        if (db.isValid() && db.isOpen()) {
            m_availableConnections.enqueue(connectionName);
            currentTotal++;
        } else {
            break; // 无法创建连接，退出循环
        }
    }
}

