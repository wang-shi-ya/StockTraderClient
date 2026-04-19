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
    QQueue<QString> m_availableConnections; // 可用连接名称队列
    QSet<QString> m_usedConnections;        // 正在使用的连接名称
    mutable QMutex m_mutex;
    QWaitCondition m_waitCondition;
    
    // 定时器，定期清理无效连接
    QTimer *m_cleanupTimer;
    
    bool m_initialized;
};

