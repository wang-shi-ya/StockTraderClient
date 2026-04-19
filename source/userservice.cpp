#include "userservice.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QDateTime>
#include <QTime>

UserService::UserService(DbManager *dbManager, QObject *parent)
    : QObject(parent)
    , m_dbManager(dbManager)
{
}

UserService::~UserService() {
}

void UserService::clearAllCaches() {
    m_userInfo.clear();
    m_accountInfo.clear();
    m_cashFlowRecords.clear();
    m_tradingStats.clear();
}

void UserService::loadUserInfo(const QString &username) {
    if (!m_userInfo.contains(username)) {
        // 尝试从数据库加载
        UserInfo info = loadUserInfoFromDB(username);
        // 如果数据库中没有找到记录（username为空），或者username不匹配，生成默认值
        if (info.username.isEmpty() || info.username != username) {
            // 如果数据库中没有，使用默认值，但至少设置username
            info = generateRandomUserInfo(username);
        } else {
            // 从数据库加载成功，但确保username正确设置
            info.username = username;
        }
        m_userInfo[username] = info;
    } else {
        // 如果已经缓存，确保username是最新的
        m_userInfo[username].username = username;
    }
    emit userInfoLoaded(m_userInfo[username]);
    qDebug() << "User info loaded for:" << username << "username:" << m_userInfo[username].username;
}

void UserService::updateUserInfo(const UserInfo &userInfo) {
    m_userInfo[userInfo.username] = userInfo;
    emit userInfoUpdated(userInfo);
    qDebug() << "User info updated for:" << userInfo.username;
}

bool UserService::saveUserInfoToDB(const UserInfo &userInfo) {
    if (!m_dbManager) {
        qWarning() << "DbManager is null, cannot save user info";
        return false;
    }
    
    // 保存到数据库
    bool success = m_dbManager->saveUserInfo(
        userInfo.username,
        userInfo.realName,
        userInfo.email,
        userInfo.phone,
        userInfo.idCard,
        userInfo.address,
        userInfo.status.isEmpty() ? "正常" : userInfo.status
    );
    
    if (success) {
        // 更新内存中的数据
        m_userInfo[userInfo.username] = userInfo;
        emit userInfoUpdated(userInfo);
        qDebug() << "User info saved to DB for:" << userInfo.username;
    } else {
        qWarning() << "Failed to save user info to DB for:" << userInfo.username;
    }
    
    return success;
}

UserInfo UserService::getUserInfo(const QString &username) const {
    return m_userInfo.value(username, UserInfo());
}

void UserService::loadAccountInfo(const QString &username) {
    if (!m_accountInfo.contains(username)) {
        m_accountInfo[username] = generateRandomAccountInfo(username);
    }
    emit accountInfoLoaded(m_accountInfo[username]);
    qDebug() << "Account info loaded for:" << username;
}

void UserService::updateAccountInfo(const QString &username, double availableCash) {
    if (m_accountInfo.contains(username)) {
        m_accountInfo[username].availableCash = availableCash;
        m_accountInfo[username].totalAssets = availableCash + m_accountInfo[username].frozenCash + m_accountInfo[username].marketValue;
        m_accountInfo[username].updateTime = QDateTime::currentDateTime();
        emit accountInfoUpdated(m_accountInfo[username]);
    }
}

AccountInfo UserService::getAccountInfo(const QString &username) const {
    return m_accountInfo.value(username, AccountInfo());
}

void UserService::calculateMarketValue(const QString &username, const QVector<PositionInfo> &positions) {
    if (!m_accountInfo.contains(username)) {
        return;
    }

    double marketValue = 0.0;
    for (const PositionInfo &position : positions) {
        // 这里应该使用实时价格，暂时使用平均价格
        marketValue += position.quantity * position.averagePrice;
    }

    m_accountInfo[username].marketValue = marketValue;
    m_accountInfo[username].totalAssets = m_accountInfo[username].availableCash + 
                                        m_accountInfo[username].frozenCash + marketValue;
    m_accountInfo[username].updateTime = QDateTime::currentDateTime();
    
    emit accountInfoUpdated(m_accountInfo[username]);
}

void UserService::loadCashFlowRecords(const QString &username, int limit) {
    if (!m_cashFlowRecords.contains(username)) {
        QVector<CashFlowRecord> records;
        for (int i = 0; i < limit; ++i) {
            records.append(generateRandomCashFlowRecord(username));
        }
        m_cashFlowRecords[username] = records;
    }
    emit cashFlowRecordsLoaded(m_cashFlowRecords[username]);
    qDebug() << "Cash flow records loaded for:" << username << "count:" << m_cashFlowRecords[username].size();
}

void UserService::addCashFlowRecord(const CashFlowRecord &record) {
    m_cashFlowRecords[record.username].prepend(record);
    // 保持最近50条记录
    if (m_cashFlowRecords[record.username].size() > 50) {
        m_cashFlowRecords[record.username].resize(50);
    }
    emit cashFlowRecordsLoaded(m_cashFlowRecords[record.username]);
}

QVector<CashFlowRecord> UserService::getCashFlowRecords(const QString &username) const {
    return m_cashFlowRecords.value(username, QVector<CashFlowRecord>());
}

void UserService::loadTradingStats(const QString &username) {
    if (!m_tradingStats.contains(username)) {
        m_tradingStats[username] = generateRandomTradingStats(username);
    }
    emit tradingStatsLoaded(m_tradingStats[username]);
    qDebug() << "Trading stats loaded for:" << username;
}

void UserService::updateTradingStats(const QString &username, const OrderInfo &order) {
    if (!m_tradingStats.contains(username)) {
        m_tradingStats[username] = generateRandomTradingStats(username);
    }

    TradingStats &stats = m_tradingStats[username];
    stats.totalTrades++;
    if (order.side == "买入" || order.side == "Buy") {
        stats.buyTrades++;
    } else {
        stats.sellTrades++;
    }
    stats.totalVolume += order.quantity;
    stats.totalAmount += order.quantity * order.price;
    stats.lastTradeTime = QDateTime::currentDateTime();
    
    // 计算胜率（简化计算）
    stats.winRate = QRandomGenerator::global()->bounded(40, 80);
    
    emit tradingStatsLoaded(stats);
}

TradingStats UserService::getTradingStats(const QString &username) const {
    return m_tradingStats.value(username, TradingStats());
}

void UserService::loadAssetOverview(const QString &username) {
    loadUserInfo(username);
    loadAccountInfo(username);
    loadCashFlowRecords(username, 10);
    loadTradingStats(username);
}

void UserService::refreshAssetData(const QString &username) {
    loadAccountInfo(username);
    loadCashFlowRecords(username, 10);
    loadTradingStats(username);
}

UserInfo UserService::generateRandomUserInfo(const QString &username) const {
    UserInfo info;
    info.username = username;
    info.email = QString("%1@example.com").arg(username);
    info.phone = QString("1%1").arg(QRandomGenerator::global()->bounded(100000000, 999999999));
    info.realName = QString("用户%1").arg(QRandomGenerator::global()->bounded(1000, 9999));
    info.idCard = QString("%1%2%3").arg(QRandomGenerator::global()->bounded(110000, 659999))
                                   .arg(QRandomGenerator::global()->bounded(19000101, 20231231))
                                   .arg(QRandomGenerator::global()->bounded(1000, 9999));
    info.address = QString("北京市朝阳区%1号").arg(QRandomGenerator::global()->bounded(1, 999));
    info.registerTime = QDateTime::currentDateTime().addDays(-QRandomGenerator::global()->bounded(1, 365));
    info.lastLoginTime = QDateTime::currentDateTime().addSecs(-QRandomGenerator::global()->bounded(60, 3600));
    info.status = "正常";
    return info;
}

AccountInfo UserService::generateRandomAccountInfo(const QString &username) const {
    AccountInfo info;
    info.username = username;
    info.availableCash = QRandomGenerator::global()->bounded(10000, 1000000);
    info.frozenCash = QRandomGenerator::global()->bounded(0, 50000);
    info.marketValue = QRandomGenerator::global()->bounded(50000, 500000);
    info.totalAssets = info.availableCash + info.frozenCash + info.marketValue;
    info.profitLoss = QRandomGenerator::global()->bounded(-50000, 100000);
    info.profitLossPercent = info.profitLoss / (info.totalAssets - info.profitLoss) * 100;
    info.updateTime = QDateTime::currentDateTime();
    return info;
}

CashFlowRecord UserService::generateRandomCashFlowRecord(const QString &username) const {
    CashFlowRecord record;
    record.recordId = QString("CF%1").arg(QRandomGenerator::global()->bounded(100000, 999999));
    record.username = username;
    
    QStringList types = {"充值", "提现", "买入", "卖出", "分红", "手续费", "利息"};
    record.type = types[QRandomGenerator::global()->bounded(types.size())];
    
    record.amount = QRandomGenerator::global()->bounded(-50000, 100000);
    record.balance = QRandomGenerator::global()->bounded(10000, 1000000);
    record.description = QString("%1操作").arg(record.type);
    record.createTime = QDateTime::currentDateTime().addDays(-QRandomGenerator::global()->bounded(0, 30));
    record.status = "已完成";
    
    return record;
}

UserInfo UserService::loadUserInfoFromDB(const QString &username) const {
    UserInfo info;
    info.username = username; // 至少设置用户名
    
    if (!m_dbManager) {
        qDebug() << "DbManager is null, cannot load user info";
        return info;
    }
    
    QString realName, email, phone, idCard, address, status;
    QDateTime registerTime, lastLoginTime;
    
    bool success = m_dbManager->loadUserInfo(username, realName, email, phone, idCard, 
                                             address, registerTime, lastLoginTime, status);
    if (success) {
        // 从数据库加载成功，填充信息
        info.username = username;
        info.realName = realName;
        info.email = email;
        info.phone = phone;
        info.idCard = idCard;
        info.address = address;
        info.registerTime = registerTime;
        info.lastLoginTime = lastLoginTime;
        info.status = status.isEmpty() ? "正常" : status;
        qDebug() << "User info loaded from DB for:" << username << "realName:" << realName;
    } else {
        qDebug() << "Failed to load user info from DB for:" << username;
        // 即使加载失败，也返回带有username的info，这样至少用户名可以显示
    }
    
    return info;
}

TradingStats UserService::generateRandomTradingStats(const QString &username) const {
    TradingStats stats;
    stats.username = username;
    stats.totalTrades = QRandomGenerator::global()->bounded(10, 1000);
    stats.buyTrades = stats.totalTrades * QRandomGenerator::global()->bounded(40, 60) / 100;
    stats.sellTrades = stats.totalTrades - stats.buyTrades;
    stats.totalVolume = QRandomGenerator::global()->bounded(10000, 1000000);
    stats.totalAmount = QRandomGenerator::global()->bounded(100000, 10000000);
    stats.totalProfit = QRandomGenerator::global()->bounded(-100000, 200000);
    stats.winRate = QRandomGenerator::global()->bounded(40, 80);
    stats.lastTradeTime = QDateTime::currentDateTime().addDays(-QRandomGenerator::global()->bounded(0, 7));
    return stats;
}
