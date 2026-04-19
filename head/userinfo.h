#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include "quotedata.h"

// 用户基本信息
struct UserInfo {
    QString username;           // 用户名
    QString email;             // 邮箱
    QString phone;             // 电话
    QString realName;          // 真实姓名
    QString idCard;            // 身份证号
    QString address;           // 地址
    QDateTime registerTime;    // 注册时间
    QDateTime lastLoginTime;   // 最后登录时间
    QString status;            // 账户状态
};

// 账户资产信息
struct AccountInfo {
    QString username;          // 用户名
    double totalAssets;        // 总资产
    double availableCash;      // 可用资金
    double frozenCash;         // 冻结资金
    double marketValue;        // 持仓市值
    double profitLoss;         // 浮动盈亏
    double profitLossPercent;  // 盈亏比例
    QDateTime updateTime;      // 更新时间
};

// 资金流水记录
struct CashFlowRecord {
    QString recordId;          // 记录ID
    QString username;          // 用户名
    QString type;              // 类型（充值、提现、买入、卖出、分红等）
    double amount;             // 金额
    double balance;            // 余额
    QString description;       // 描述
    QDateTime createTime;      // 创建时间
    QString status;            // 状态
};

// 交易统计
struct TradingStats {
    QString username;          // 用户名
    int totalTrades;           // 总交易次数
    int buyTrades;             // 买入次数
    int sellTrades;            // 卖出次数
    double totalVolume;        // 总交易量
    double totalAmount;        // 总交易金额
    double totalProfit;        // 总盈亏
    double winRate;            // 胜率
    QDateTime lastTradeTime;   // 最后交易时间
};

Q_DECLARE_METATYPE(UserInfo)
Q_DECLARE_METATYPE(AccountInfo)
Q_DECLARE_METATYPE(CashFlowRecord)
Q_DECLARE_METATYPE(TradingStats)
