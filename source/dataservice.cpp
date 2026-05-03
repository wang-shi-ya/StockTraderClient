#include "dataservice.h"
#include "httpmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>
#include <QDateTime>

DataService::DataService(QObject *parent)
    : QObject(parent)
    , m_quoteTimer(new QTimer(this))
    , m_indexTimer(new QTimer(this))
    , m_sectorTimer(new QTimer(this))
{
    connect(m_quoteTimer, &QTimer::timeout, this, &DataService::pollQuotes);
    connect(m_indexTimer, &QTimer::timeout, this, &DataService::pollIndices);
    connect(m_sectorTimer, &QTimer::timeout, this, &DataService::pollSectors);
}

DataService::~DataService()
{
    stopRealTimeQuotes();
}

void DataService::startRealTimeQuotes()
{
    if (m_running) return;

    // Preload stock list
    QNetworkReply *reply = HttpManager::instance().get("/api/stocks");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            QJsonArray arr = HttpManager::dataArray(resp);
            m_stockNames.clear();
            m_allSymbols.clear();
            for (const QJsonValue &v : arr) {
                QJsonObject obj = v.toObject();
                m_stockNames[obj["symbol"].toString()] = obj["name"].toString();
                m_allSymbols.append(obj["symbol"].toString());
            }
            emit companyListUpdated(m_allSymbols);
        }
    });

    // Load initial index/sector data
    pollIndices();
    pollSectors();

    m_quoteTimer->start(1000);
    m_indexTimer->start(5000);
    m_sectorTimer->start(5000);
    m_running = true;
    qDebug() << "DataService: real-time quotes started (HTTP polling)";
}

void DataService::stopRealTimeQuotes()
{
    if (!m_running) return;
    m_quoteTimer->stop();
    m_indexTimer->stop();
    m_sectorTimer->stop();
    m_running = false;
    qDebug() << "DataService: real-time quotes stopped";
}

void DataService::subscribeQuote(const QString &symbol)
{
    QString sym = symbol.toUpper();
    if (!m_subscribedSymbols.contains(sym)) {
        m_subscribedSymbols.append(sym);
        requestQuoteHttp(sym);
    }
}

void DataService::unsubscribeQuote(const QString &symbol)
{
    m_subscribedSymbols.removeAll(symbol.toUpper());
    m_quotes.remove(symbol.toUpper());
}

QVector<QuoteData> DataService::getQuoteList() const
{
    QVector<QuoteData> quotes;
    for (const QString &sym : m_subscribedSymbols) {
        if (m_quotes.contains(sym))
            quotes.append(m_quotes[sym]);
    }
    return quotes;
}

QuoteData DataService::getQuote(const QString &symbol) const
{
    return m_quotes.value(symbol.toUpper(), QuoteData());
}

QStringList DataService::getAllStockSymbols() const
{
    return m_allSymbols;
}

QString DataService::getStockName(const QString &symbol) const
{
    if (m_stockNames.contains(symbol))
        return m_stockNames[symbol];
    for (auto it = m_stockNames.begin(); it != m_stockNames.end(); ++it) {
        if (it.key().startsWith(symbol))
            return it.value();
    }
    return QString();
}

QVector<IndexData> DataService::getIndexList() const
{
    return m_indices;
}

QVector<SectorData> DataService::getSectorList() const
{
    return m_sectors;
}

// ---- HTTP-based async data requests ----

void DataService::requestQuoteHttp(const QString &symbol)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/stocks/" + symbol + "/quote");
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) return;
        QJsonObject data = HttpManager::dataObject(resp);
        QuoteData quote = parseQuoteJson(data);
        m_quotes[symbol] = quote;
        emit quoteUpdated(quote);
    });
}

void DataService::requestKLineData(const QString &symbol, const QString &period, int count)
{
    QString path = QString("/api/stocks/%1/kline?period=%2&count=%3")
                       .arg(symbol, period).arg(count);
    QNetworkReply *reply = HttpManager::instance().get(path);
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol, period]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) return;
        QJsonArray arr = HttpManager::dataArray(resp);
        QVector<KLineData> klines;
        for (const QJsonValue &v : arr) {
            klines.append(parseKLineJson(v.toObject()));
        }
        m_klineData[symbol + "_" + period] = klines;
        emit kLineDataReceived(symbol, period, klines);
    });
}

void DataService::requestTradeDetails(const QString &symbol, const QDateTime &startTime, const QDateTime &endTime)
{
    Q_UNUSED(symbol)
    Q_UNUSED(startTime)
    Q_UNUSED(endTime)
    // Trade details endpoint not available yet — return empty
    emit tradeDetailsReceived(symbol, QVector<TradeDetail>());
}

void DataService::requestCompanyInfo(const QString &symbol)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/company/" + symbol);
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            qWarning() << "Company info request failed:" << HttpManager::errorMessage(resp);
            CompanyInfo emptyInfo;
            emptyInfo.symbol = symbol;
            emit companyInfoReceived(emptyInfo);
            return;
        }
        QJsonObject data = HttpManager::dataObject(resp);
        CompanyInfo info = parseCompanyJson(data);
        m_companyInfo[symbol] = info;
        emit companyInfoReceived(info);
    });
}

void DataService::requestFinancialData(const QString &symbol)
{
    // Reuse cached company info if already fetched (same endpoint as requestCompanyInfo)
    if (m_companyInfo.contains(symbol)) {
        const CompanyInfo &info = m_companyInfo[symbol];
        QMap<QString, QVariant> data;
        data["revenue"] = info.revenue;
        data["netProfit"] = info.netProfit;
        data["totalAssets"] = info.totalShares * 30; // rough estimate
        data["netAssets"] = info.circulatingShares * 20;
        data["roe"] = info.roe;
        data["pe"] = info.pe;
        data["pb"] = info.pb;
        emit financialDataReceived(symbol, data);
        return;
    }

    // Fallback HTTP request if company info not yet cached
    QNetworkReply *reply = HttpManager::instance().get("/api/company/" + symbol);
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        QMap<QString, QVariant> data;
        if (HttpManager::isSuccess(resp)) {
            QJsonObject obj = HttpManager::dataObject(resp);
            data["revenue"] = obj["revenue"].toDouble();
            data["netProfit"] = obj["netProfit"].toDouble();
            data["totalAssets"] = obj["totalShares"].toDouble() * 30; // rough estimate
            data["netAssets"] = obj["circulatingShares"].toDouble() * 20;
            data["roe"] = obj["roe"].toDouble();
            data["pe"] = obj["pe"].toDouble();
            data["pb"] = obj["pb"].toDouble();
        }
        emit financialDataReceived(symbol, data);
    });
}

void DataService::loadCompanyList()
{
    QNetworkReply *reply = HttpManager::instance().get("/api/stocks");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) return;
        QJsonArray arr = HttpManager::dataArray(resp);
        QStringList symbols;
        for (const QJsonValue &v : arr) {
            QJsonObject obj = v.toObject();
            symbols.append(obj["symbol"].toString());
            m_stockNames[obj["symbol"].toString()] = obj["name"].toString();
        }
        m_allSymbols = symbols;
        emit companyListUpdated(symbols);
    });
}

void DataService::requestCompanyAnnouncements(const QString &symbol) {
    QNetworkReply *reply = HttpManager::instance().get("/api/company/" + symbol + "/announcements");
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit companyAnnouncementsReceived(symbol, QJsonArray());
            return;
        }
        QJsonArray arr = HttpManager::dataArray(resp);
        emit companyAnnouncementsReceived(symbol, arr);
    });
}

void DataService::requestNewsList(const QString &category, int page, int size, const QString &keyword) {
    QString path = QString("/api/news?category=%1&page=%2&size=%3").arg(category).arg(page).arg(size);
    if (!keyword.isEmpty()) {
        path += QString("&keyword=%1").arg(keyword);
    }
    QNetworkReply *reply = HttpManager::instance().get(path);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit newsListReceived(QJsonArray(), 0);
            return;
        }
        QJsonObject data = HttpManager::dataObject(resp);
        QJsonArray arr = data["list"].toArray();
        int total = data["total"].toInt();
        emit newsListReceived(arr, total);
    });
}

void DataService::requestStockNews(const QString &symbol) {
    QString path = QString("/api/news/stock/%1").arg(symbol);
    QNetworkReply *reply = HttpManager::instance().get(path);
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit stockNewsReceived(symbol, QJsonArray());
            return;
        }
        QJsonArray arr = HttpManager::dataArray(resp);
        emit stockNewsReceived(symbol, arr);
    });
}

void DataService::requestNewsDetail(int id) {
    QString path = QString("/api/news/%1").arg(id);
    QNetworkReply *reply = HttpManager::instance().get(path);
    connect(reply, &QNetworkReply::finished, this, [this, reply, id]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit newsDetailReceived(id, QJsonObject());
            return;
        }
        QJsonObject obj = HttpManager::dataObject(resp);
        emit newsDetailReceived(id, obj);
    });
}

void DataService::requestNewsRefresh() {
    QNetworkReply *reply = HttpManager::instance().post("/api/news/refresh");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        emit newsRefreshFinished();
    });
}

void DataService::requestIndexData()
{
    QNetworkReply *reply = HttpManager::instance().get("/api/indices");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) return;
        QJsonArray arr = HttpManager::dataArray(resp);
        m_indices.clear();
        for (const QJsonValue &v : arr)
            m_indices.append(parseIndexJson(v.toObject()));
        emit indexDataUpdated(m_indices);
    });
}

void DataService::requestSectorData()
{
    QNetworkReply *reply = HttpManager::instance().get("/api/sectors");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) return;
        QJsonArray arr = HttpManager::dataArray(resp);
        m_sectors.clear();
        for (const QJsonValue &v : arr)
            m_sectors.append(parseSectorJson(v.toObject()));
        emit sectorDataUpdated(m_sectors);
    });
}

// ---- Polling slots ----

void DataService::pollQuotes()
{
    for (const QString &symbol : m_subscribedSymbols)
        requestQuoteHttp(symbol);
}

void DataService::pollIndices()
{
    requestIndexData();
}

void DataService::pollSectors()
{
    requestSectorData();
}

// ---- JSON parsers ----

QuoteData DataService::parseQuoteJson(const QJsonObject &json)
{
    QuoteData q;
    q.symbol = json["symbol"].toString();
    q.currentPrice = json["currentPrice"].toDouble();
    q.openPrice = json["openPrice"].toDouble();
    q.highPrice = json["highPrice"].toDouble();
    q.lowPrice = json["lowPrice"].toDouble();
    q.prevClose = json["prevClose"].toDouble();
    q.change = json["change"].toDouble();
    q.changePercent = json["changePercent"].toDouble();
    q.volume = static_cast<long long>(json["volume"].toDouble());
    q.turnover = json["turnover"].toDouble();
    q.pe = json["pe"].toDouble();
    q.pb = json["pb"].toDouble();
    q.updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::ISODate);
    if (!q.updateTime.isValid()) q.updateTime = QDateTime::currentDateTime();

    QJsonArray bids = json["bidPrices"].toArray();
    QJsonArray bidVols = json["bidVolumes"].toArray();
    QJsonArray asks = json["askPrices"].toArray();
    QJsonArray askVols = json["askVolumes"].toArray();
    for (const QJsonValue &v : bids) q.bidPrices.append(v.toDouble());
    for (const QJsonValue &v : bidVols) q.bidVolumes.append(static_cast<long long>(v.toDouble()));
    for (const QJsonValue &v : asks) q.askPrices.append(v.toDouble());
    for (const QJsonValue &v : askVols) q.askVolumes.append(static_cast<long long>(v.toDouble()));

    return q;
}

KLineData DataService::parseKLineJson(const QJsonObject &json)
{
    KLineData k;
    k.dateTime = QDateTime::fromString(json["dateTime"].toString(), Qt::ISODate);
    k.open = json["open"].toDouble();
    k.high = json["high"].toDouble();
    k.low = json["low"].toDouble();
    k.close = json["close"].toDouble();
    k.volume = static_cast<long long>(json["volume"].toDouble());
    k.turnover = json["turnover"].toDouble();
    k.period = json["period"].toString();
    return k;
}

CompanyInfo DataService::parseCompanyJson(const QJsonObject &json)
{
    CompanyInfo info;
    info.symbol = json["symbol"].toString();
    info.name = json["name"].toString();
    info.industry = json["industry"].toString();
    info.sector = json["sector"].toString();
    info.totalShares = json["totalShares"].toDouble();
    info.circulatingShares = json["circulatingShares"].toDouble();
    info.marketCap = json["marketCap"].toDouble();
    info.circulatingCap = json["circulatingCap"].toDouble();
    info.pe = json["pe"].toDouble();
    info.pb = json["pb"].toDouble();
    info.roe = json["roe"].toDouble();
    info.revenue = json["revenue"].toDouble();
    info.netProfit = json["netProfit"].toDouble();
    info.reportDate = QDateTime::fromString(json["reportDate"].toString(), Qt::ISODate);
    return info;
}

IndexData DataService::parseIndexJson(const QJsonObject &json)
{
    IndexData idx;
    idx.code = json["code"].toString();
    idx.name = json["name"].toString();
    idx.currentValue = json["currentValue"].toDouble();
    idx.change = json["change"].toDouble();
    idx.changePercent = json["changePercent"].toDouble();
    idx.volume = static_cast<long long>(json["volume"].toDouble());
    idx.turnover = json["turnover"].toDouble();
    idx.updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::ISODate);
    if (!idx.updateTime.isValid()) idx.updateTime = QDateTime::currentDateTime();
    return idx;
}

SectorData DataService::parseSectorJson(const QJsonObject &json)
{
    SectorData s;
    s.code = json["code"].toString();
    s.name = json["name"].toString();
    s.changePercent = json["changePercent"].toDouble();
    s.turnover = json["turnover"].toDouble();
    s.stockCount = json["stockCount"].toInt();
    s.riseCount = json["riseCount"].toInt();
    s.fallCount = json["fallCount"].toInt();
    s.updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::ISODate);
    if (!s.updateTime.isValid()) s.updateTime = QDateTime::currentDateTime();
    return s;
}
