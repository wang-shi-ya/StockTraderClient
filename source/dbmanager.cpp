#include "dbmanager.h"
#include "dbconnectionpool.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QProcessEnvironment>
#include <QDateTime>

DbManager::DbManager(QObject *parent) 
    : QObject(parent)
    , m_pool(nullptr)
    , m_usePool(false)
{
    // 默认使用连接池
    m_pool = &DbConnectionPool::instance();
    m_usePool = true;
}

DbManager::~DbManager() {
	// Note: We don't close the connection here because it might be shared
	// The connection will be closed when the application exits
	// If you need to explicitly close, use QSqlDatabase::removeDatabase()
}

bool DbManager::initialize() {
    if (m_usePool && m_pool) {
        // 使用连接池
        if (!m_pool->initialize(2, 10)) {
            qWarning() << "Failed to initialize connection pool";
            return false;
        }
        
        // 从连接池获取一个连接用于schema检查
        m_db = m_pool->getConnection();
        if (!m_db.isValid() || !m_db.isOpen()) {
            qWarning() << "Failed to get connection from pool";
            return false;
        }
        
        // 确保schema存在
        bool schemaOk = ensureSchema();
        
        // 释放连接（schema检查完成后）
        m_pool->releaseConnection(m_db);
        
        return schemaOk;
    } else {
        // 使用单连接模式（向后兼容）
        static const QString connectionName = "StockTraderDBConnection";
        
        if (QSqlDatabase::contains(connectionName)) {
            m_db = QSqlDatabase::database(connectionName);
            if (m_db.isOpen()) {
                // Connection exists and is open, just verify schema
                return ensureSchema();
            }
            // Connection exists but is closed, remove it and recreate
            QSqlDatabase::removeDatabase(connectionName);
        }
        
        // Read env vars, with defaults
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        const QString host = env.value("DB_HOST", "127.0.0.1");
        const QString portStr = env.value("DB_PORT", "3306");
        const QString dbName = env.value("DB_NAME", "stock_trader");
        const QString user = env.value("DB_USER", "root");
        const QString password = env.value("DB_PASSWORD", "HBsc75820306@");

        bool okPort = false;
        int port = portStr.toInt(&okPort);
        if (!okPort) port = 3306;

        m_db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        m_db.setHostName(host);
        m_db.setPort(port);
        m_db.setDatabaseName(dbName);
        m_db.setUserName(user);
        m_db.setPassword(password);

        if (!m_db.open()) {
            qWarning() << "DB open failed:" << m_db.lastError();
            return false;
        }
        return ensureSchema();
    }
    
    return false; // 不应该到达这里
}

bool DbManager::ensureConnection() {
    if (m_usePool && m_pool) {
        // 使用连接池时，每次获取新连接
        if (!m_db.isValid() || !m_db.isOpen()) {
            m_db = m_pool->getConnection();
            return m_db.isValid() && m_db.isOpen();
        }
        return true;
    } else {
        // 单连接模式
        static const QString connectionName = "StockTraderDBConnection";
        
        if (!m_db.isValid() || !m_db.isOpen()) {
            if (QSqlDatabase::contains(connectionName)) {
                m_db = QSqlDatabase::database(connectionName);
                if (m_db.isOpen()) {
                    return true;
                }
            }
            // Try to reinitialize
            return initialize();
        }
        return true;
    }
}

QSqlDatabase DbManager::database() {
    if (m_usePool && m_pool) {
        // 使用连接池，每次获取新连接
        if (!m_db.isValid() || !m_db.isOpen()) {
            m_db = m_pool->getConnection();
        }
        return m_db;
    } else {
        // 单连接模式
        ensureConnection();
        return m_db;
    }
}

void DbManager::releaseConnection() {
    if (m_usePool && m_pool && m_db.isValid()) {
        m_pool->releaseConnection(m_db);
        m_db = QSqlDatabase(); // 清空连接引用
    }
}

bool DbManager::ensureSchema() {
    // 确保有有效连接
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        qWarning() << "No valid database connection for schema check";
        return false;
    }
    
    QSqlQuery q(db);
    const char *usersSql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  username VARCHAR(64) UNIQUE NOT NULL,"
        "  password_hash VARCHAR(255) NOT NULL,"
        "  salt VARCHAR(64) NULL,"
        "  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";
    if (!q.exec(QLatin1String(usersSql))) {
        qWarning() << "Create users failed:" << q.lastError();
        return false;
    }
    q.finish();

    // Ensure salt column exists on older MySQL versions (without IF NOT EXISTS)
    QSqlQuery checkSalt(db);
    // INFORMATION_SCHEMA works across MySQL versions
    checkSalt.prepare(
        "SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS "
        "WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'users' AND COLUMN_NAME = 'salt'"
    );
    if (checkSalt.exec() && checkSalt.next()) {
        const int hasSalt = checkSalt.value(0).toInt();
        if (hasSalt == 0) {
            QSqlQuery addSalt(db);
            if (!addSalt.exec("ALTER TABLE users ADD COLUMN salt VARCHAR(64) AFTER password_hash")) {
                qWarning() << "Add salt column failed:" << addSalt.lastError();
            }
        }
    } else {
        qWarning() << "Failed to check salt column existence:" << checkSalt.lastError();
    }

    // Ensure personal info columns exist
    QStringList personalInfoColumns = {"real_name", "email", "phone", "id_card", "address", "last_login_time", "status"};
    QStringList columnTypes = {"VARCHAR(64)", "VARCHAR(128)", "VARCHAR(32)", "VARCHAR(32)", "VARCHAR(255)", "TIMESTAMP NULL", "VARCHAR(16)"};
    QStringList columnDefaults = {"", "", "", "", "", "NULL", "'正常'"};
    
    for (int i = 0; i < personalInfoColumns.size(); ++i) {
        QSqlQuery checkCol(db);
        checkCol.prepare(
            "SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS "
            "WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'users' AND COLUMN_NAME = ?"
        );
        checkCol.addBindValue(personalInfoColumns[i]);
        if (checkCol.exec() && checkCol.next()) {
            const int hasCol = checkCol.value(0).toInt();
            if (hasCol == 0) {
                QSqlQuery addCol(db);
                QString alterSql = QString("ALTER TABLE users ADD COLUMN %1 %2").arg(personalInfoColumns[i]).arg(columnTypes[i]);
                if (!columnDefaults[i].isEmpty() && columnDefaults[i] != "NULL") {
                    alterSql += QString(" DEFAULT %1").arg(columnDefaults[i]);
                }
                if (!addCol.exec(alterSql)) {
                    qWarning() << "Add column" << personalInfoColumns[i] << "failed:" << addCol.lastError();
                }
            }
        }
    }

	const char *ordersSql =
		"CREATE TABLE IF NOT EXISTS orders ("
		"  id INT AUTO_INCREMENT PRIMARY KEY,"
		"  order_id VARCHAR(32) UNIQUE NOT NULL,"
		"  username VARCHAR(64) NOT NULL,"
		"  symbol VARCHAR(32) NOT NULL,"
		"  side VARCHAR(8) NOT NULL,"
		"  quantity INT NOT NULL,"
		"  price DOUBLE NOT NULL,"
		"  status VARCHAR(16) NOT NULL,"
		"  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
		"  INDEX idx_user(username),"
		"  INDEX idx_symbol(symbol),"
		"  INDEX idx_created_at(created_at)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";
	if (!q.exec(QLatin1String(ordersSql))) {
		qWarning() << "Create orders failed:" << q.lastError();
		return false;
	}

    q.finish();

    const char *positionsSql =
        "CREATE TABLE IF NOT EXISTS positions ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  username VARCHAR(64) NOT NULL,"
        "  symbol VARCHAR(32) NOT NULL,"
        "  quantity BIGINT NOT NULL DEFAULT 0,"
        "  avg_price DOUBLE NOT NULL DEFAULT 0,"
        "  UNIQUE KEY uniq_user_symbol(username, symbol)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";
    if (!q.exec(QLatin1String(positionsSql))) {
        qWarning() << "Create positions failed:" << q.lastError();
        return false;
    }

    q.finish();

    const char *tradesSql =
        "CREATE TABLE IF NOT EXISTS trades ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  order_id VARCHAR(32) NOT NULL,"
        "  username VARCHAR(64) NOT NULL,"
        "  symbol VARCHAR(32) NOT NULL,"
        "  side VARCHAR(8) NOT NULL,"
        "  quantity INT NOT NULL,"
        "  price DOUBLE NOT NULL,"
        "  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "  INDEX idx_order(order_id)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";
    if (!q.exec(QLatin1String(tradesSql))) {
        qWarning() << "Create trades failed:" << q.lastError();
        return false;
    }

    q.finish();

    const char *companiesSql =
        "CREATE TABLE IF NOT EXISTS companies ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  symbol VARCHAR(32) UNIQUE NOT NULL,"
        "  name VARCHAR(128) NOT NULL,"
        "  industry VARCHAR(64),"
        "  sector VARCHAR(64),"
        "  total_shares DOUBLE NOT NULL DEFAULT 0,"
        "  circulating_shares DOUBLE NOT NULL DEFAULT 0,"
        "  market_cap DOUBLE NOT NULL DEFAULT 0,"
        "  circulating_cap DOUBLE NOT NULL DEFAULT 0,"
        "  pe DOUBLE,"
        "  pb DOUBLE,"
        "  roe DOUBLE,"
        "  revenue DOUBLE,"
        "  net_profit DOUBLE,"
        "  report_date DATETIME,"
        "  updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,"
        "  INDEX idx_symbol(symbol)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";
    if (!q.exec(QLatin1String(companiesSql))) {
        qWarning() << "Create companies failed:" << q.lastError();
        return false;
    }

    q.finish();

    return true;
}

bool DbManager::saveCompanyInfo(const QString &symbol, const QString &name, const QString &industry,
                                 const QString &sector, double totalShares, double circulatingShares,
                                 double marketCap, double circulatingCap, double pe, double pb,
                                 double roe, double revenue, double netProfit, const QDateTime &reportDate) {
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO companies (symbol, name, industry, sector, total_shares, circulating_shares, "
        "market_cap, circulating_cap, pe, pb, roe, revenue, net_profit, report_date) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
        "ON DUPLICATE KEY UPDATE "
        "name=VALUES(name), industry=VALUES(industry), sector=VALUES(sector), "
        "total_shares=VALUES(total_shares), circulating_shares=VALUES(circulating_shares), "
        "market_cap=VALUES(market_cap), circulating_cap=VALUES(circulating_cap), "
        "pe=VALUES(pe), pb=VALUES(pb), roe=VALUES(roe), "
        "revenue=VALUES(revenue), net_profit=VALUES(net_profit), report_date=VALUES(report_date)"
    );
    q.addBindValue(symbol);
    q.addBindValue(name);
    q.addBindValue(industry);
    q.addBindValue(sector);
    q.addBindValue(totalShares);
    q.addBindValue(circulatingShares);
    q.addBindValue(marketCap);
    q.addBindValue(circulatingCap);
    q.addBindValue(pe);
    q.addBindValue(pb);
    q.addBindValue(roe);
    q.addBindValue(revenue);
    q.addBindValue(netProfit);
    q.addBindValue(reportDate);
    
    bool success = q.exec();
    if (!success) {
        qWarning() << "Save company info failed:" << q.lastError();
    }
    
    // 如果使用连接池，释放连接
    if (m_usePool && m_pool) {
        releaseConnection();
    }
    
    return success;
}

bool DbManager::loadCompanyInfo(const QString &symbol, QString &name, QString &industry, QString &sector,
                                 double &totalShares, double &circulatingShares, double &marketCap,
                                 double &circulatingCap, double &pe, double &pb, double &roe,
                                 double &revenue, double &netProfit, QDateTime &reportDate) {
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare(
        "SELECT name, industry, sector, total_shares, circulating_shares, market_cap, "
        "circulating_cap, pe, pb, roe, revenue, net_profit, report_date "
        "FROM companies WHERE symbol=?"
    );
    q.addBindValue(symbol);
    
    if (!q.exec() || !q.next()) {
        return false;
    }
    
    name = q.value(0).toString();
    industry = q.value(1).toString();
    sector = q.value(2).toString();
    totalShares = q.value(3).toDouble();
    circulatingShares = q.value(4).toDouble();
    marketCap = q.value(5).toDouble();
    circulatingCap = q.value(6).toDouble();
    pe = q.value(7).toDouble();
    pb = q.value(8).toDouble();
    roe = q.value(9).toDouble();
    revenue = q.value(10).toDouble();
    netProfit = q.value(11).toDouble();
    reportDate = q.value(12).toDateTime();
    
    return true;
}

bool DbManager::companyInfoExists(const QString &symbol) {
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM companies WHERE symbol=?");
    q.addBindValue(symbol);
    if (q.exec() && q.next()) {
        bool exists = q.value(0).toInt() > 0;
        
        // 如果使用连接池，释放连接
        if (m_usePool && m_pool) {
            releaseConnection();
        }
        
        return exists;
    }
    
    // 如果使用连接池，释放连接
    if (m_usePool && m_pool) {
        releaseConnection();
    }
    
    return false;
}

QStringList DbManager::getAllCompanySymbols() {
    QStringList symbols;
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return symbols;
    }
    
    QSqlQuery q(db);
    q.prepare("SELECT symbol FROM companies ORDER BY symbol");
    if (q.exec()) {
        while (q.next()) {
            symbols.append(q.value(0).toString());
        }
    }
    
    // 如果使用连接池，释放连接
    if (m_usePool && m_pool) {
        releaseConnection();
    }
    
    return symbols;
}

bool DbManager::saveUserInfo(const QString &username, const QString &realName, const QString &email,
                              const QString &phone, const QString &idCard, const QString &address,
                              const QString &status) {
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare(
        "UPDATE users SET real_name=?, email=?, phone=?, id_card=?, address=?, status=? WHERE username=?"
    );
    q.addBindValue(realName);
    q.addBindValue(email);
    q.addBindValue(phone);
    q.addBindValue(idCard);
    q.addBindValue(address);
    q.addBindValue(status);
    q.addBindValue(username);
    
    bool success = q.exec();
    if (!success) {
        qWarning() << "Save user info failed:" << q.lastError();
    }
    
    // 如果使用连接池，释放连接
    if (m_usePool && m_pool) {
        releaseConnection();
    }
    
    return success;
}

bool DbManager::loadUserInfo(const QString &username, QString &realName, QString &email,
                              QString &phone, QString &idCard, QString &address,
                              QDateTime &registerTime, QDateTime &lastLoginTime, QString &status) {
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare(
        "SELECT real_name, email, phone, id_card, address, created_at, last_login_time, status "
        "FROM users WHERE username=?"
    );
    q.addBindValue(username);
    
    if (!q.exec() || !q.next()) {
        return false;
    }
    
    realName = q.value(0).toString();
    email = q.value(1).toString();
    phone = q.value(2).toString();
    idCard = q.value(3).toString();
    address = q.value(4).toString();
    registerTime = q.value(5).toDateTime();
    lastLoginTime = q.value(6).toDateTime();
    status = q.value(7).toString();
    
    // 如果使用连接池，释放连接
    if (m_usePool && m_pool) {
        releaseConnection();
    }
    
    return true;
}

bool DbManager::updateLastLoginTime(const QString &username) {
    QSqlDatabase db = database();
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare("UPDATE users SET last_login_time=CURRENT_TIMESTAMP WHERE username=?");
    q.addBindValue(username);
    
    bool success = q.exec();
    if (!success) {
        qWarning() << "Update last login time failed:" << q.lastError();
    }
    
    // 如果使用连接池，释放连接
    if (m_usePool && m_pool) {
        releaseConnection();
    }
    
    return success;
}


