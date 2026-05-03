#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <QDateTime>
#include <QSet>
#include <QHash>
#include <QThread>
#include <QMutexLocker>

class DbConnectionPool : public QObject {
    Q_OBJECT

public:
    static DbConnectionPool& instance();
    
    // 初始化连接池
    bool initialize(int minConnections = 2, int maxConnections = 10);
    
    // 获取连接
    QSqlDatabase getConnection();
    
    // 释放连接（将连接返回到池中）
    void releaseConnection(const QSqlDatabase &connection);
    
    // 获取当前连接池状态
    int availableConnections() const;
    int activeConnections() const;
    int totalConnections() const;
    
    // 关闭所有连接
    void closeAll();

private:
    DbConnectionPool(QObject *parent = nullptr);
    ~DbConnectionPool() override;
    DbConnectionPool(const DbConnectionPool&) = delete;
    DbConnectionPool& operator=(const DbConnectionPool&) = delete;
    
    // 创建新连接
    QSqlDatabase createConnection(const QString &connectionName);
    
    // 检查连接是否有效
    bool isConnectionValid(const QSqlDatabase &connection);
    
    // 清理无效连接
    void cleanupInvalidConnections();
    
    // 连接配置
    QString m_host;
    int m_port;
    QString m_databaseName;
    QString m_userName;
    QString m_password;
    
    // 连接池配置
    int m_minConnections;
    int m_maxConnections;
    
    // 连接管理
    // Qt SQL connections are thread-affine: a connection created in one thread
    // must only be used in that same thread. So the pool is maintained per-thread.
    QHash<quintptr, QQueue<QString>> m_availableConnectionsByThread; // threadId -> available names
    QHash<quintptr, QSet<QString>> m_usedConnectionsByThread;        // threadId -> used names
    QHash<QString, quintptr> m_connectionOwnerThread;                // connectionName -> owner threadId
    mutable QMutex m_mutex;
    QWaitCondition m_waitCondition;
    
    // 定时器，定期清理无效连接
    QTimer *m_cleanupTimer;
    
    bool m_initialized;
};

