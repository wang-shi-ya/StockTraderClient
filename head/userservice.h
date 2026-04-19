#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include "userinfo.h"
#include "quotedata.h"
#include "client.h"
#include "dbmanager.h"

class UserService : public QObject {
    Q_OBJECT

public:
    explicit UserService(DbManager *dbManager, QObject *parent = nullptr);
    ~UserService();

    // 用户信息管理
    Q_INVOKABLE void loadUserInfo(const QString &username);
    Q_INVOKABLE void updateUserInfo(const UserInfo &userInfo);
    Q_INVOKABLE bool saveUserInfoToDB(const UserInfo &userInfo); // 保存到数据库
    Q_INVOKABLE UserInfo getUserInfo(const QString &username) const;

    // 账户资产管理
    Q_INVOKABLE void loadAccountInfo(const QString &username);
    Q_INVOKABLE void updateAccountInfo(const QString &username, double availableCash);
    Q_INVOKABLE AccountInfo getAccountInfo(const QString &username) const;
    Q_INVOKABLE void calculateMarketValue(const QString &username, const QVector<PositionInfo> &positions);

    // 资金流水管理
    Q_INVOKABLE void loadCashFlowRecords(const QString &username, int limit = 50);
    Q_INVOKABLE void addCashFlowRecord(const CashFlowRecord &record);
    Q_INVOKABLE QVector<CashFlowRecord> getCashFlowRecords(const QString &username) const;

    // 交易统计
    Q_INVOKABLE void loadTradingStats(const QString &username);
    Q_INVOKABLE void updateTradingStats(const QString &username, const OrderInfo &order);
    Q_INVOKABLE TradingStats getTradingStats(const QString &username) const;

    // 资产概览
    Q_INVOKABLE void loadAssetOverview(const QString &username);
    Q_INVOKABLE void refreshAssetData(const QString &username);

    /// 退出登录时清空内存缓存，避免再次登录前界面仍显示旧数据
    Q_INVOKABLE void clearAllCaches();

signals:
    // 用户信息信号
    void userInfoLoaded(const UserInfo &userInfo);
    void userInfoUpdated(const UserInfo &userInfo);

    // 账户资产信号
    void accountInfoLoaded(const AccountInfo &accountInfo);
    void accountInfoUpdated(const AccountInfo &accountInfo);

    // 资金流水信号
    void cashFlowRecordsLoaded(const QVector<CashFlowRecord> &records);

    // 交易统计信号
    void tradingStatsLoaded(const TradingStats &stats);

    // 资产概览信号
    void assetOverviewUpdated(const AccountInfo &account, const QVector<PositionInfo> &positions, 
                             const QVector<CashFlowRecord> &recentRecords);

private:
    // 模拟数据生成
    UserInfo generateRandomUserInfo(const QString &username) const;
    UserInfo loadUserInfoFromDB(const QString &username) const;
    AccountInfo generateRandomAccountInfo(const QString &username) const;
    CashFlowRecord generateRandomCashFlowRecord(const QString &username) const;
    TradingStats generateRandomTradingStats(const QString &username) const;

    // 数据存储
    DbManager *m_dbManager;
    QMap<QString, UserInfo> m_userInfo;
    QMap<QString, AccountInfo> m_accountInfo;
    QMap<QString, QVector<CashFlowRecord>> m_cashFlowRecords;
    QMap<QString, TradingStats> m_tradingStats;
};
