#include "dbmanager.h"
#include "httpmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

DbManager::DbManager(QObject *parent)
    : QObject(parent)
{
}

void DbManager::initialize()
{
    QNetworkReply *reply = HttpManager::instance().get("/api/stocks");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            qWarning() << "DbManager: failed to load stock list:" << HttpManager::errorMessage(resp);
            m_initialized = true; // Still mark initialized, cache will be empty but app can work
            emit initialized(false);
            return;
        }
        QJsonArray arr = HttpManager::dataArray(resp);
        m_stockNames.clear();
        m_allSymbols.clear();
        for (const QJsonValue &v : arr) {
            QJsonObject obj = v.toObject();
            QString symbol = obj["symbol"].toString();
            QString name = obj["name"].toString();
            m_stockNames[symbol] = name;
            m_allSymbols.append(symbol);
        }
        m_initialized = true;
        qDebug() << "DbManager: loaded" << m_allSymbols.size() << "stocks";
        emit initialized(true);
    });
}

QString DbManager::getStockName(const QString &symbol) const
{
    if (m_stockNames.contains(symbol))
        return m_stockNames[symbol];
    // Try without suffix
    for (auto it = m_stockNames.begin(); it != m_stockNames.end(); ++it) {
        if (it.key().startsWith(symbol))
            return it.value();
    }
    return QString();
}

QStringList DbManager::getAllStockSymbols() const
{
    return m_allSymbols;
}

QStringList DbManager::getAllCompanySymbols() const
{
    return m_allSymbols;
}

bool DbManager::companyInfoExists(const QString &symbol) const
{
    return m_stockNames.contains(symbol)
        || std::any_of(m_stockNames.begin(), m_stockNames.end(),
                       [&](const QString &k) { return k.startsWith(symbol); });
}

void DbManager::loadCompanyInfo(const QString &symbol)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/company/" + symbol);
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit errorOccurred(HttpManager::errorMessage(resp));
            return;
        }
        QJsonObject data = HttpManager::dataObject(resp);
        parseAndEmitCompanyInfo(symbol, data);
    });
}

void DbManager::saveCompanyInfo(const QString &symbol, const QString &name, const QString &industry,
                                 const QString &sector, double totalShares, double circulatingShares,
                                 double marketCap, double circulatingCap, double pe, double pb,
                                 double roe, double revenue, double netProfit, const QDateTime &reportDate)
{
    QJsonObject body;
    body["symbol"] = symbol;
    body["name"] = name;
    body["industry"] = industry;
    body["sector"] = sector;
    body["total_shares"] = totalShares;
    body["circulating_shares"] = circulatingShares;
    body["market_cap"] = marketCap;
    body["circulating_cap"] = circulatingCap;
    body["pe"] = pe;
    body["pb"] = pb;
    body["roe"] = roe;
    body["revenue"] = revenue;
    body["net_profit"] = netProfit;
    body["report_date"] = reportDate.toString(Qt::ISODate);

    QNetworkReply *reply = HttpManager::instance().put("/api/admin/companies/0", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        emit companyInfoSaved(HttpManager::isSuccess(resp));
    });
}

void DbManager::getCompanyAnnouncements(const QString &symbol)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/company/" + symbol + "/announcements");
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit errorOccurred(HttpManager::errorMessage(resp));
            emit announcementsLoaded(symbol, QString());
            return;
        }
        QJsonArray arr = HttpManager::dataArray(resp);
        QString text;
        QString companyName = getStockName(symbol);
        if (companyName.isEmpty()) companyName = symbol;
        for (const QJsonValue &v : arr) {
            QJsonObject a = v.toObject();
            QString title = a["title"].toString();
            QString content = a["content"].toString();
            QString date = a["publishDate"].toString();
            QString images = a["images"].toString();
            text += QString::fromUtf8("【%1】%2\n\n%3\n%4\n").arg(companyName, title, content, date);
            if (!images.isEmpty())
                text += QString::fromUtf8("[IMAGE]%1[/IMAGE]\n").arg(images);
            text += "\n";
        }
        emit announcementsLoaded(symbol, text);
    });
}

void DbManager::loadUserInfo(const QString &username)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/user/info");
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit userInfoLoaded(username, QString(), QString(), QString(),
                                QString(), QString(),
                                QDateTime(), QDateTime(), QString());
            return;
        }
        QJsonObject d = HttpManager::dataObject(resp);
        emit userInfoLoaded(
            username,
            d["realName"].toString(),
            d["email"].toString(),
            d["phone"].toString(),
            d["idCard"].toString(),
            d["address"].toString(),
            QDateTime::fromString(d["registerTime"].toString(), Qt::ISODate),
            QDateTime::fromString(d["lastLoginTime"].toString(), Qt::ISODate),
            d["status"].toString("正常")
        );
    });
}

void DbManager::saveUserInfo(const QString &username, const QString &realName, const QString &email,
                              const QString &phone, const QString &idCard, const QString &address,
                              const QString &status)
{
    Q_UNUSED(username)
    QJsonObject body;
    body["realName"] = realName;
    body["email"] = email;
    body["phone"] = phone;
    body["idCard"] = idCard;
    body["address"] = address;
    body["status"] = status;

    QNetworkReply *reply = HttpManager::instance().put("/api/user/info", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        // Fire and forget — no signal needed
    });
}

void DbManager::updateLastLoginTime(const QString &username)
{
    Q_UNUSED(username)
    // Server handles this automatically on login — no-op on client side
}

void DbManager::loadUserWatchedSymbols(const QString &username)
{
    QNetworkReply *reply = HttpManager::instance().get("/api/user/watchlist");
    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) {
            emit watchedSymbolsLoaded(username, QStringList());
            return;
        }
        QJsonArray arr = HttpManager::dataArray(resp);
        QStringList symbols;
        for (const QJsonValue &v : arr)
            symbols.append(v.toString());
        emit watchedSymbolsLoaded(username, symbols);
    });
}

void DbManager::saveUserWatchedSymbol(const QString &username, const QString &symbol)
{
    Q_UNUSED(username)
    QNetworkReply *reply = HttpManager::instance().post("/api/user/watchlist/" + symbol);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void DbManager::removeUserWatchedSymbol(const QString &username, const QString &symbol)
{
    Q_UNUSED(username)
    QNetworkReply *reply = HttpManager::instance().del("/api/user/watchlist/" + symbol);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void DbManager::clearUserWatchedSymbols(const QString &username)
{
    // No bulk clear endpoint — we'd need to add one. For now, load and delete individually.
    // Actually, just skip for now. The QuoteWidget can remove individually.
    Q_UNUSED(username)
}

void DbManager::addSystemLog(const QString &operatorName, const QString &operationType, const QString &ipAddress)
{
    QJsonObject body;
    body["operator"] = operatorName;
    body["operation_type"] = operationType;
    body["ip_address"] = ipAddress;

    QNetworkReply *reply = HttpManager::instance().post("/api/admin/system-logs", body);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void DbManager::parseAndEmitCompanyInfo(const QString &symbol, const QJsonObject &data)
{
    emit companyInfoLoaded(
        symbol,
        data["name"].toString(),
        data["industry"].toString(),
        data["sector"].toString(),
        data["totalShares"].toDouble(),
        data["circulatingShares"].toDouble(),
        data["marketCap"].toDouble(),
        data["circulatingCap"].toDouble(),
        data["pe"].toDouble(),
        data["pb"].toDouble(),
        data["roe"].toDouble(),
        data["revenue"].toDouble(),
        data["netProfit"].toDouble(),
        QDateTime::fromString(data["reportDate"].toString(), Qt::ISODate)
    );
}
