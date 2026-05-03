#include "userservice.h"
#include "httpmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>
#include <QDateTime>

UserService::UserService(QObject *parent)
    : QObject(parent)
{
}

UserService::~UserService() = default;

void UserService::changePassword(const QString &oldPassword, const QString &newPassword)
{
    QJsonObject body;
    body["oldPassword"] = oldPassword;
    body["newPassword"] = newPassword;

    QNetworkReply *reply = HttpManager::instance().put("/api/user/change-password", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            emit passwordChangeSuccess(resp["message"].toString(QString::fromUtf8("密码修改成功")));
        } else {
            emit passwordChangeFailed(resp["message"].toString(QString::fromUtf8("密码修改失败")));
        }
    });
}

void UserService::clearAllCaches()
{
    m_userInfo.clear();
    m_accountInfo.clear();
    m_cashFlowRecords.clear();
    m_tradingStats.clear();
}

// ---- User Info ----

void UserService::loadUserInfo(const QString &username)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/user/info");
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            QJsonObject data = HttpManager::dataObject(resp);
            UserInfo info = parseUserInfoJson(username, data);
            m_userInfo[username] = info;
            emit userInfoLoaded(info);
        } else {
            UserInfo info;
            info.username = username;
            m_userInfo[username] = info;
            emit userInfoLoaded(info);
        }
    });
}

void UserService::updateUserInfo(const UserInfo &userInfo)
{
    m_userInfo[userInfo.username] = userInfo;
    emit userInfoUpdated(userInfo);
}

bool UserService::saveUserInfoToDB(const UserInfo &userInfo)
{
    QJsonObject body;
    body["realName"] = userInfo.realName;
    body["email"] = userInfo.email;
    body["phone"] = userInfo.phone;
    body["idCard"] = userInfo.idCard;
    body["address"] = userInfo.address;
    body["status"] = userInfo.status.isEmpty() ? "正常" : userInfo.status;

    QNetworkReply *reply = HttpManager::instance().put("/api/user/info", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply, userInfo]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            m_userInfo[userInfo.username] = userInfo;
            emit userInfoUpdated(userInfo);
        }
    });
    return true; // Fire-and-forget, assume success
}

UserInfo UserService::getUserInfo(const QString &username) const
{
    return m_userInfo.value(username, UserInfo());
}

// ---- Account Info ----

void UserService::loadAccountInfo(const QString &username)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/user/account");
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            QJsonObject data = HttpManager::dataObject(resp);
            AccountInfo info = parseAccountJson(username, data);
            m_accountInfo[username] = info;
            emit accountInfoLoaded(info);
        }
    });
}

void UserService::updateAccountInfo(const QString &username, double availableCash)
{
    if (m_accountInfo.contains(username)) {
        AccountInfo &info = m_accountInfo[username];
        info.availableCash = availableCash;
        info.totalAssets = availableCash + info.frozenCash + info.marketValue;
        info.updateTime = QDateTime::currentDateTime();
        emit accountInfoUpdated(info);
    }
}

AccountInfo UserService::getAccountInfo(const QString &username) const
{
    return m_accountInfo.value(username, AccountInfo());
}

void UserService::calculateMarketValue(const QString &username, const QVector<PositionInfo> &positions)
{
    if (!m_accountInfo.contains(username)) return;
    double marketValue = 0.0;
    double costBasis = 0.0;
    for (const auto &p : positions) {
        double price = p.currentPrice > 0 ? p.currentPrice : p.averagePrice;
        marketValue += p.quantity * price;
        costBasis += p.quantity * p.averagePrice;
    }
    AccountInfo &info = m_accountInfo[username];
    info.marketValue = marketValue;
    info.totalAssets = info.availableCash + info.frozenCash + marketValue;
    info.profitLoss = marketValue - costBasis;
    info.profitLossPercent = costBasis > 0 ? (info.profitLoss / costBasis) * 100.0 : 0.0;
    info.updateTime = QDateTime::currentDateTime();
    emit accountInfoUpdated(info);
}

// ---- Cash Flow ----

void UserService::loadCashFlowRecords(const QString &username, int limit)
{
    QString path = QString("/api/user/cash-flow?limit=%1").arg(limit);
    QNetworkReply *reply = HttpManager::instance().get(path);
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        QVector<CashFlowRecord> records;
        if (HttpManager::isSuccess(resp)) {
            QJsonArray arr = HttpManager::dataArray(resp);
            for (const QJsonValue &v : arr)
                records.append(parseCashFlowJson(username, v.toObject()));
        }
        m_cashFlowRecords[username] = records;
        emit cashFlowRecordsLoaded(records);
    });
}

void UserService::addCashFlowRecord(const CashFlowRecord &record)
{
    m_cashFlowRecords[record.username].prepend(record);
    if (m_cashFlowRecords[record.username].size() > 50)
        m_cashFlowRecords[record.username].resize(50);
    emit cashFlowRecordsLoaded(m_cashFlowRecords[record.username]);
}

QVector<CashFlowRecord> UserService::getCashFlowRecords(const QString &username) const
{
    return m_cashFlowRecords.value(username, QVector<CashFlowRecord>());
}

// ---- Trading Stats ----

void UserService::loadTradingStats(const QString &username)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/user/trading-stats");
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            QJsonObject data = HttpManager::dataObject(resp);
            TradingStats stats = parseTradingStatsJson(username, data);
            m_tradingStats[username] = stats;
            emit tradingStatsLoaded(stats);
        }
    });
}

void UserService::updateTradingStats(const QString &username, const OrderInfo &order)
{
    if (!m_tradingStats.contains(username))
        return;

    TradingStats &stats = m_tradingStats[username];
    stats.totalTrades++;
    if (order.side == QString::fromUtf8("买入") || order.side == "Buy")
        stats.buyTrades++;
    else
        stats.sellTrades++;
    stats.totalVolume += order.quantity;
    stats.totalAmount += order.quantity * order.price;
    stats.lastTradeTime = QDateTime::currentDateTime();
    emit tradingStatsLoaded(stats);
}

TradingStats UserService::getTradingStats(const QString &username) const
{
    return m_tradingStats.value(username, TradingStats());
}

// ---- Deposit / Withdraw ----

void UserService::deposit(double amount, const QString &description)
{
    QJsonObject body;
    body["amount"] = amount;
    body["description"] = description;

    QNetworkReply *reply = HttpManager::instance().post("/api/user/deposit", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            emit depositSuccess(resp["message"].toString(QString::fromUtf8("充值成功")));
        } else {
            emit depositFailed(resp["message"].toString(QString::fromUtf8("充值失败")));
        }
    });
}

void UserService::withdraw(double amount, const QString &description)
{
    QJsonObject body;
    body["amount"] = amount;
    body["description"] = description;

    QNetworkReply *reply = HttpManager::instance().post("/api/user/withdraw", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            emit withdrawSuccess(resp["message"].toString(QString::fromUtf8("提现成功")));
        } else {
            emit withdrawFailed(resp["message"].toString(QString::fromUtf8("提现失败")));
        }
    });
}

void UserService::loadTransactions(const QString &username, int page, int pageSize)
{
    QString path = QString("/api/user/transactions?page=%1&pageSize=%2").arg(page).arg(pageSize);
    QNetworkReply *reply = HttpManager::instance().get(path);
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        QVector<CashFlowRecord> records;
        if (HttpManager::isSuccess(resp)) {
            QJsonObject data = HttpManager::dataObject(resp);
            QJsonArray arr = data["rows"].toArray();
            for (const QJsonValue &v : arr)
                records.append(parseCashFlowJson(username, v.toObject()));
        }
        m_cashFlowRecords[username] = records;
        emit transactionsLoaded(records);
    });
}

// ---- Asset Overview ----

void UserService::loadAssetOverview(const QString &username)
{
    loadUserInfo(username);
    loadAccountInfo(username);
    loadCashFlowRecords(username, 10);
    loadTradingStats(username);
}

void UserService::refreshAssetData(const QString &username)
{
    loadAccountInfo(username);
    loadCashFlowRecords(username, 10);
    loadTradingStats(username);
}

// ---- JSON Parsers ----

UserInfo UserService::parseUserInfoJson(const QString &username, const QJsonObject &json)
{
    UserInfo info;
    info.username = username;
    info.realName = json["realName"].toString();
    info.email = json["email"].toString();
    info.phone = json["phone"].toString();
    info.idCard = json["idCard"].toString();
    info.address = json["address"].toString();
    info.registerTime = QDateTime::fromString(json["registerTime"].toString(), Qt::ISODate);
    info.lastLoginTime = QDateTime::fromString(json["lastLoginTime"].toString(), Qt::ISODate);
    info.status = json["status"].toString("正常");
    return info;
}

AccountInfo UserService::parseAccountJson(const QString &username, const QJsonObject &json)
{
    AccountInfo info;
    info.username = username;
    info.availableCash = json["availableCash"].toDouble();
    info.frozenCash = json["frozenCash"].toDouble();
    info.marketValue = json["marketValue"].toDouble();
    info.totalAssets = json["totalAssets"].toDouble();
    info.profitLoss = json["profitLoss"].toDouble();
    info.profitLossPercent = json["profitLossPercent"].toDouble();
    info.updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::ISODate);
    if (!info.updateTime.isValid()) info.updateTime = QDateTime::currentDateTime();
    return info;
}

CashFlowRecord UserService::parseCashFlowJson(const QString &username, const QJsonObject &json)
{
    CashFlowRecord record;
    record.recordId = json["recordId"].toString();
    record.username = username;
    record.type = json["type"].toString();
    record.amount = json["amount"].toDouble();
    record.balance = json["balance"].toDouble();
    record.description = json["description"].toString();
    record.createTime = QDateTime::fromString(json["createTime"].toString(), Qt::ISODate);
    record.status = json["status"].toString("已完成");
    return record;
}

TradingStats UserService::parseTradingStatsJson(const QString &username, const QJsonObject &json)
{
    TradingStats stats;
    stats.username = username;
    stats.totalTrades = json["totalTrades"].toInt();
    stats.buyTrades = json["buyTrades"].toInt();
    stats.sellTrades = json["sellTrades"].toInt();
    stats.totalVolume = json["totalVolume"].toDouble();
    stats.totalAmount = json["totalAmount"].toDouble();
    stats.totalProfit = json["totalProfit"].toDouble();
    stats.winRate = json["winRate"].toDouble();
    stats.lastTradeTime = QDateTime::fromString(json["lastTradeTime"].toString(), Qt::ISODate);
    if (!stats.lastTradeTime.isValid()) stats.lastTradeTime = QDateTime::currentDateTime();
    return stats;
}
