#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>

// 实时行情数据
struct QuoteData {
    QString symbol;           // 股票代码
    QString name;            // 股票名称
    double currentPrice;     // 当前价格
    double openPrice;        // 开盘价
    double highPrice;        // 最高价
    double lowPrice;         // 最低价
    double prevClose;        // 昨收价
    double change;           // 涨跌额
    double changePercent;    // 涨跌幅
    long long volume;        // 成交量
    double turnover;         // 成交额
    double pe;              // 市盈率
    double pb;              // 市净率
    QDateTime updateTime;    // 更新时间
    
    // 买卖盘口数据
    QVector<double> bidPrices;    // 买盘价格
    QVector<long long> bidVolumes; // 买盘数量
    QVector<double> askPrices;    // 卖盘价格
    QVector<long long> askVolumes; // 卖盘数量
};

// K线数据
struct KLineData {
    QDateTime dateTime;      // 时间
    double open;            // 开盘价
    double high;            // 最高价
    double low;             // 最低价
    double close;           // 收盘价
    long long volume;       // 成交量
    double turnover;        // 成交额
    QString period;         // 周期（1m, 5m, 15m, 30m, 1h, 1d, 1w）
};

// 历史成交明细
struct TradeDetail {
    QDateTime time;         // 成交时间
    double price;           // 成交价格
    long long volume;       // 成交数量
    QString side;           // 买卖方向
    double amount;          // 成交金额
};

// 公司基本信息
struct CompanyInfo {
    QString symbol;         // 股票代码
    QString name;           // 公司名称
    QString industry;       // 所属行业
    QString sector;         // 所属板块
    double totalShares;     // 总股本
    double circulatingShares; // 流通股本
    double marketCap;       // 总市值
    double circulatingCap;  // 流通市值
    double pe;             // 市盈率
    double pb;             // 市净率
    double roe;            // 净资产收益率
    double revenue;         // 营业收入
    double netProfit;       // 净利润
    QDateTime reportDate;   // 报告期
};

// 指数数据
struct IndexData {
    QString code;           // 指数代码
    QString name;           // 指数名称
    double currentValue;    // 当前值
    double change;          // 涨跌额
    double changePercent;   // 涨跌幅
    long long volume;       // 成交量
    double turnover;        // 成交额
    QDateTime updateTime;   // 更新时间
};

// 板块数据
struct SectorData {
    QString code;           // 板块代码
    QString name;           // 板块名称
    double changePercent;   // 涨跌幅
    double turnover;        // 成交额
    int stockCount;         // 股票数量
    int riseCount;          // 上涨数量
    int fallCount;          // 下跌数量
    QDateTime updateTime;   // 更新时间
};

Q_DECLARE_METATYPE(QuoteData)
Q_DECLARE_METATYPE(KLineData)
Q_DECLARE_METATYPE(TradeDetail)
Q_DECLARE_METATYPE(CompanyInfo)
Q_DECLARE_METATYPE(IndexData)
Q_DECLARE_METATYPE(SectorData)
