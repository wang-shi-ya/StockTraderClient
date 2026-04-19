#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include "dbconnectionpool.h"

class DbManager : public QObject {
	Q_OBJECT
public:
	explicit DbManager(QObject *parent = nullptr);
	~DbManager() override;

	bool initialize();
	QSqlDatabase database();
	bool ensureConnection();
	void releaseConnection(); // 释放连接（连接池模式）
	void setUsePool(bool usePool) { m_usePool = usePool; } // 设置是否使用连接池

	// schema
	bool ensureSchema();

	// Company info methods
	bool saveCompanyInfo(const QString &symbol, const QString &name, const QString &industry, 
	                     const QString &sector, double totalShares, double circulatingShares,
	                     double marketCap, double circulatingCap, double pe, double pb, 
	                     double roe, double revenue, double netProfit, const QDateTime &reportDate);
	bool loadCompanyInfo(const QString &symbol, QString &name, QString &industry, QString &sector,
	                     double &totalShares, double &circulatingShares, double &marketCap,
	                     double &circulatingCap, double &pe, double &pb, double &roe,
	                     double &revenue, double &netProfit, QDateTime &reportDate);
	bool companyInfoExists(const QString &symbol);
	QStringList getAllCompanySymbols();

	// User info methods
	bool saveUserInfo(const QString &username, const QString &realName, const QString &email,
	                  const QString &phone, const QString &idCard, const QString &address,
	                  const QString &status = "正常");
	bool loadUserInfo(const QString &username, QString &realName, QString &email,
	                  QString &phone, QString &idCard, QString &address,
	                  QDateTime &registerTime, QDateTime &lastLoginTime, QString &status);
	bool updateLastLoginTime(const QString &username);

private:
	QSqlDatabase m_db;
	DbConnectionPool *m_pool;
	bool m_usePool;
};


