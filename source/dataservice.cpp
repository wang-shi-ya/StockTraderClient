#include "dataservice.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QDateTime>
#include <QThread>

DataService::DataService(DbManager *dbManager, QObject *parent)
    : QObject(parent)
    , m_realTimeTimer(new QTimer(this))
    , m_dbManager(dbManager)
{
    // 初始化基础价格
    m_basePrices["000001"] = 15.50;  // 平安银行
    m_basePrices["000002"] = 25.30;  // 万科A
    m_basePrices["600000"] = 12.80;  // 浦发银行
    m_basePrices["600036"] = 35.20;  // 招商银行
    m_basePrices["000858"] = 18.90;  // 五粮液
    m_basePrices["600519"] = 1800.0; // 贵州茅台
    m_basePrices["000001.SZ"] = 15.50; // 平安银行
    m_basePrices["000002.SZ"] = 25.30; // 万科A
    m_basePrices["600000.SH"] = 12.80; // 浦发银行
    m_basePrices["600036.SH"] = 35.20; // 招商银行
    m_basePrices["000858.SZ"] = 18.90; // 五粮液
    m_basePrices["600519.SH"] = 1800.0; // 贵州茅台

    // 连接定时器
    connect(m_realTimeTimer, &QTimer::timeout, this, &DataService::updateRealTimeData);
}

DataService::~DataService() {
    stopRealTimeQuotes();
}

void DataService::startRealTimeQuotes() {
    if (!m_realTimeTimer->isActive()) {
        m_realTimeTimer->start(1000); // 每秒更新一次
        qDebug() << "Real-time quotes started";
    }
}

void DataService::stopRealTimeQuotes() {
    if (m_realTimeTimer->isActive()) {
        m_realTimeTimer->stop();
        qDebug() << "Real-time quotes stopped";
    }
}

void DataService::subscribeQuote(const QString &symbol) {
    if (!m_subscribedSymbols.contains(symbol)) {
        m_subscribedSymbols.append(symbol);
        // 生成初始数据
        m_quotes[symbol] = generateRandomQuote(symbol);
        emit quoteUpdated(m_quotes[symbol]);
        qDebug() << "Subscribed to quote:" << symbol;
    }
}

void DataService::unsubscribeQuote(const QString &symbol) {
    m_subscribedSymbols.removeAll(symbol);
    m_quotes.remove(symbol);
    qDebug() << "Unsubscribed from quote:" << symbol;
}

QVector<QuoteData> DataService::getQuoteList() const {
    QVector<QuoteData> quotes;
    for (const QString &symbol : m_subscribedSymbols) {
        if (m_quotes.contains(symbol)) {
            quotes.append(m_quotes[symbol]);
        }
    }
    return quotes;
}

QuoteData DataService::getQuote(const QString &symbol) const {
    return m_quotes.value(symbol, QuoteData());
}

void DataService::requestKLineData(const QString &symbol, const QString &period, int count) {
    QVector<KLineData> klineData;
    QDateTime currentTime = QDateTime::currentDateTime();
    
    // 根据周期调整时间间隔
    int intervalMinutes = 1;
    if (period == "5m") intervalMinutes = 5;
    else if (period == "15m") intervalMinutes = 15;
    else if (period == "30m") intervalMinutes = 30;
    else if (period == "1h") intervalMinutes = 60;
    else if (period == "1d") intervalMinutes = 1440; // 24小时
    else if (period == "1w") intervalMinutes = 10080; // 7天

    for (int i = count - 1; i >= 0; --i) {
        QDateTime time = currentTime.addSecs(-i * intervalMinutes * 60);
        klineData.append(generateRandomKLine(time, period));
    }
    
    m_klineData[symbol + "_" + period] = klineData;
    emit kLineDataReceived(symbol, period, klineData);
    qDebug() << "K-line data requested for" << symbol << period << "count:" << count;
}

void DataService::requestTradeDetails(const QString &symbol, const QDateTime &startTime, const QDateTime &endTime) {
    QVector<TradeDetail> details;
    QDateTime currentTime = startTime;
    
    while (currentTime <= endTime && details.size() < 1000) {
        if (QRandomGenerator::global()->bounded(100) < 30) { // 30%概率生成成交
            details.append(generateRandomTrade(currentTime));
        }
        currentTime = currentTime.addSecs(QRandomGenerator::global()->bounded(1, 60));
    }
    
    m_tradeDetails[symbol] = details;
    emit tradeDetailsReceived(symbol, details);
    qDebug() << "Trade details requested for" << symbol << "count:" << details.size();
}

void DataService::requestCompanyInfo(const QString &symbol) {
    CompanyInfo info;
    
    // 先从数据库加载
    if (m_dbManager) {
        QString name, industry, sector;
        double totalShares = 0, circulatingShares = 0, marketCap = 0, circulatingCap = 0;
        double pe = 0, pb = 0, roe = 0, revenue = 0, netProfit = 0;
        QDateTime reportDate;
        
        bool loaded = m_dbManager->loadCompanyInfo(symbol, name, industry, sector,
                                                    totalShares, circulatingShares, marketCap,
                                                    circulatingCap, pe, pb, roe,
                                                    revenue, netProfit, reportDate);
        
        if (loaded) {
            // 从数据库加载成功
            info.symbol = symbol;
            info.name = name;
            info.industry = industry;
            info.sector = sector;
            info.totalShares = totalShares;
            info.circulatingShares = circulatingShares;
            info.marketCap = marketCap;
            info.circulatingCap = circulatingCap;
            info.pe = pe;
            info.pb = pb;
            info.roe = roe;
            info.revenue = revenue;
            info.netProfit = netProfit;
            info.reportDate = reportDate;
            qDebug() << "Company info loaded from database for" << symbol;
        } else {
            // 数据库中没有，生成模拟数据并保存
            info = generateRandomCompanyInfo(symbol);
            if (m_dbManager->saveCompanyInfo(symbol, info.name, info.industry, info.sector,
                                              info.totalShares, info.circulatingShares,
                                              info.marketCap, info.circulatingCap,
                                              info.pe, info.pb, info.roe,
                                              info.revenue, info.netProfit, info.reportDate)) {
                qDebug() << "Company info saved to database for" << symbol;
            } else {
                qWarning() << "Failed to save company info to database for" << symbol;
            }
        }
    } else {
        // 没有数据库管理器，直接生成模拟数据
        info = generateRandomCompanyInfo(symbol);
    }
    
    m_companyInfo[symbol] = info;
    emit companyInfoReceived(info);
    qDebug() << "Company info requested for" << symbol;
}

void DataService::requestFinancialData(const QString &symbol) {
    QMap<QString, QVariant> data;
    data["revenue"] = QRandomGenerator::global()->bounded(1000000, 10000000);
    data["netProfit"] = QRandomGenerator::global()->bounded(100000, 1000000);
    data["totalAssets"] = QRandomGenerator::global()->bounded(5000000, 50000000);
    data["netAssets"] = QRandomGenerator::global()->bounded(2000000, 20000000);
    data["roe"] = QRandomGenerator::global()->bounded(5, 25) / 100.0;
    data["pe"] = QRandomGenerator::global()->bounded(10, 50);
    data["pb"] = QRandomGenerator::global()->bounded(1, 10);
    
    emit financialDataReceived(symbol, data);
    qDebug() << "Financial data requested for" << symbol;
}

void DataService::loadCompanyList() {
    QStringList symbols;
    if (m_dbManager) {
        symbols = m_dbManager->getAllCompanySymbols();
    }
    emit companyListUpdated(symbols);
    qDebug() << "Company list loaded, count:" << symbols.size();
}

void DataService::requestIndexData() {
    QVector<IndexData> indices;
    
    QStringList indexCodes = {"000001", "399001", "399006", "000016", "000300"};
    QStringList indexNames = {"上证指数", "深证成指", "创业板指", "上证50", "沪深300"};
    
    for (int i = 0; i < indexCodes.size(); ++i) {
        IndexData index = generateRandomIndex(indexCodes[i]);
        index.name = indexNames[i];
        indices.append(index);
    }
    
    m_indices = indices;
    emit indexDataUpdated(indices);
    qDebug() << "Index data requested, count:" << indices.size();
}

void DataService::requestSectorData() {
    QVector<SectorData> sectors;
    
    QStringList sectorCodes = {"BK0001", "BK0002", "BK0003", "BK0004", "BK0005"};
    QStringList sectorNames = {"银行", "房地产", "医药生物", "电子", "食品饮料"};
    
    for (int i = 0; i < sectorCodes.size(); ++i) {
        SectorData sector = generateRandomSector(sectorCodes[i]);
        sector.name = sectorNames[i];
        sectors.append(sector);
    }
    
    m_sectors = sectors;
    emit sectorDataUpdated(sectors);
    qDebug() << "Sector data requested, count:" << sectors.size();
}

QVector<IndexData> DataService::getIndexList() const {
    return m_indices;
}

QVector<SectorData> DataService::getSectorList() const {
    return m_sectors;
}

void DataService::updateRealTimeData() {
    for (const QString &symbol : m_subscribedSymbols) {
        m_quotes[symbol] = generateRandomQuote(symbol);
        emit quoteUpdated(m_quotes[symbol]);
    }
    
    // 更新指数和板块数据
    requestIndexData();
    requestSectorData();
}

QuoteData DataService::generateRandomQuote(const QString &symbol) const {
    QuoteData quote;
    quote.symbol = symbol;
    
    // 获取基础价格
    double basePrice = m_basePrices.value(symbol, 20.0);
    
    // 生成随机价格变化 (-5% 到 +5%)
    double changePercent = QRandomGenerator::global()->bounded(-500, 501) / 10000.0;
    quote.currentPrice = basePrice * (1 + changePercent);
    quote.change = quote.currentPrice - basePrice;
    quote.changePercent = changePercent * 100;
    
    // 生成其他价格数据
    quote.openPrice = basePrice * (1 + QRandomGenerator::global()->bounded(-200, 201) / 10000.0);
    quote.highPrice = qMax(quote.currentPrice, quote.openPrice) * (1 + QRandomGenerator::global()->bounded(0, 101) / 10000.0);
    quote.lowPrice = qMin(quote.currentPrice, quote.openPrice) * (1 - QRandomGenerator::global()->bounded(0, 101) / 10000.0);
    quote.prevClose = basePrice;
    
    // 生成成交数据
    quote.volume = QRandomGenerator::global()->bounded(1000000, 10000000);
    quote.turnover = quote.volume * quote.currentPrice;
    
    // 生成估值数据
    quote.pe = QRandomGenerator::global()->bounded(10, 50);
    quote.pb = QRandomGenerator::global()->bounded(1, 10);
    
    quote.updateTime = QDateTime::currentDateTime();
    
    // 生成买卖盘口数据
    quote.bidPrices.resize(5);
    quote.bidVolumes.resize(5);
    quote.askPrices.resize(5);
    quote.askVolumes.resize(5);
    
    for (int i = 0; i < 5; ++i) {
        quote.bidPrices[i] = quote.currentPrice - (i + 1) * 0.01;
        quote.bidVolumes[i] = QRandomGenerator::global()->bounded(100, 10000);
        quote.askPrices[i] = quote.currentPrice + (i + 1) * 0.01;
        quote.askVolumes[i] = QRandomGenerator::global()->bounded(100, 10000);
    }
    
    return quote;
}

KLineData DataService::generateRandomKLine(const QDateTime &time, const QString &period) const {
    KLineData kline;
    kline.dateTime = time;
    
    double basePrice = 20.0 + QRandomGenerator::global()->bounded(0, 1000) / 100.0;
    kline.open = basePrice;
    kline.high = basePrice * (1 + QRandomGenerator::global()->bounded(0, 500) / 10000.0);
    kline.low = basePrice * (1 - QRandomGenerator::global()->bounded(0, 500) / 10000.0);
    kline.close = basePrice * (1 + QRandomGenerator::global()->bounded(-500, 501) / 10000.0);
    kline.volume = QRandomGenerator::global()->bounded(100000, 1000000);
    kline.turnover = kline.volume * (kline.high + kline.low) / 2;
    kline.period = period;
    
    return kline;
}

TradeDetail DataService::generateRandomTrade(const QDateTime &time) const {
    TradeDetail trade;
    trade.time = time;
    trade.price = 20.0 + QRandomGenerator::global()->bounded(0, 1000) / 100.0;
    trade.volume = QRandomGenerator::global()->bounded(100, 10000);
    trade.side = QRandomGenerator::global()->bounded(2) ? "买入" : "卖出";
    trade.amount = trade.price * trade.volume;
    
    return trade;
}

CompanyInfo DataService::generateRandomCompanyInfo(const QString &symbol) const {
    CompanyInfo info;
    info.symbol = symbol;
    info.name = QString("公司%1").arg(symbol);
    info.industry = "金融";
    info.sector = "银行";
    info.totalShares = QRandomGenerator::global()->bounded(1000000, 10000000);
    info.circulatingShares = info.totalShares * 0.8;
    info.marketCap = info.totalShares * 20.0;
    info.circulatingCap = info.circulatingShares * 20.0;
    info.pe = QRandomGenerator::global()->bounded(10, 30);
    info.pb = QRandomGenerator::global()->bounded(1, 5);
    info.roe = QRandomGenerator::global()->bounded(5, 20) / 100.0;
    info.revenue = QRandomGenerator::global()->bounded(1000000, 10000000);
    info.netProfit = QRandomGenerator::global()->bounded(100000, 1000000);
    info.reportDate = QDateTime::currentDateTime().addDays(-30);
    
    return info;
}

IndexData DataService::generateRandomIndex(const QString &code) const {
    IndexData index;
    index.code = code;
    index.name = "指数";
    index.currentValue = 3000 + QRandomGenerator::global()->bounded(0, 1000);
    index.change = QRandomGenerator::global()->bounded(-100, 101);
    index.changePercent = index.change / (index.currentValue - index.change) * 100;
    index.volume = QRandomGenerator::global()->bounded(10000000, 100000000);
    index.turnover = index.volume * index.currentValue;
    index.updateTime = QDateTime::currentDateTime();
    
    return index;
}

SectorData DataService::generateRandomSector(const QString &code) const {
    SectorData sector;
    sector.code = code;
    sector.name = "板块";
    sector.changePercent = QRandomGenerator::global()->bounded(-500, 501) / 100.0;
    sector.turnover = QRandomGenerator::global()->bounded(100000000, 1000000000);
    sector.stockCount = QRandomGenerator::global()->bounded(50, 200);
    sector.riseCount = sector.stockCount * (50 + QRandomGenerator::global()->bounded(-20, 21)) / 100;
    sector.fallCount = sector.stockCount - sector.riseCount;
    sector.updateTime = QDateTime::currentDateTime();
    
    return sector;
}
