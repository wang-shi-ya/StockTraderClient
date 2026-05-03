#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include "userinfo.h"
#include "quotedata.h"
#include "client.h"

class UserService : public QObject
{
    Q_OBJECT

public:
    explicit UserService(QObject *parent = nullptr);
    ~UserService();

    Q_INVOKABLE void loadUserInfo(const QString &username);
    Q_INVOKABLE void updateUserInfo(const UserInfo &userInfo);
    Q_INVOKABLE bool saveUserInfoToDB(const UserInfo &userInfo);
    Q_INVOKABLE UserInfo getUserInfo(const QString &username) const;
    Q_INVOKABLE void loadAccountInfo(const QString &username);
    Q_INVOKABLE void updateAccountInfo(const QString &username, double availableCash);
    Q_INVOKABLE AccountInfo getAccountInfo(const QString &username) const;
    Q_INVOKABLE void calculateMarketValue(const QString &username, const QVector<PositionInfo> &positions);
    Q_INVOKABLE void loadCashFlowRecords(const QString &username, int limit = 50);
    Q_INVOKABLE void addCashFlowRecord(const CashFlowRecord &record);
    Q_INVOKABLE QVector<CashFlowRecord> getCashFlowRecords(const QString &username) const;
    Q_INVOKABLE void loadTradingStats(const QString &username);
    Q_INVOKABLE void updateTradingStats(const QString &username, const OrderInfo &order);
    Q_INVOKABLE TradingStats getTradingStats(const QString &username) const;
    Q_INVOKABLE void loadAssetOverview(const QString &username);
    Q_INVOKABLE void deposit(double amount, const QString &description);
    Q_INVOKABLE void withdraw(double amount, const QString &description);
    Q_INVOKABLE void loadTransactions(const QString &username, int page = 1, int pageSize = 20);
    Q_INVOKABLE void refreshAssetData(const QString &username);
    Q_INVOKABLE void changePassword(const QString &oldPassword, const QString &newPassword);
    Q_INVOKABLE void clearAllCaches();

signals:
    void passwordChangeSuccess(const QString &message);
    void passwordChangeFailed(const QString &message);
    void userInfoLoaded(const UserInfo &userInfo);
    void userInfoUpdated(const UserInfo &userInfo);
    void accountInfoLoaded(const AccountInfo &accountInfo);
    void accountInfoUpdated(const AccountInfo &accountInfo);
    void cashFlowRecordsLoaded(const QVector<CashFlowRecord> &records);
    void tradingStatsLoaded(const TradingStats &stats);
    void depositSuccess(const QString &message);
    void depositFailed(const QString &message);
    void withdrawSuccess(const QString &message);
    void withdrawFailed(const QString &message);
    void transactionsLoaded(const QVector<CashFlowRecord> &records);
    void assetOverviewUpdated(const AccountInfo &account, const QVector<PositionInfo> &positions,
                              const QVector<CashFlowRecord> &recentRecords);

private:
    static UserInfo parseUserInfoJson(const QString &username, const QJsonObject &json);
    static AccountInfo parseAccountJson(const QString &username, const QJsonObject &json);
    static CashFlowRecord parseCashFlowJson(const QString &username, const QJsonObject &json);
    static TradingStats parseTradingStatsJson(const QString &username, const QJsonObject &json);

    QMap<QString, UserInfo> m_userInfo;
    QMap<QString, AccountInfo> m_accountInfo;
    QMap<QString, QVector<CashFlowRecord>> m_cashFlowRecords;
    QMap<QString, TradingStats> m_tradingStats;
};
