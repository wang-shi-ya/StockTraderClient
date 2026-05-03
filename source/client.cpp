#include "client.h"
#include "httpmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>
#include <QDateTime>

Client::Client(QObject *parent)
    : QObject(parent)
{
    m_db.initialize();

    m_statusCheckTimer = new QTimer(this);
    connect(m_statusCheckTimer, &QTimer::timeout, this, &Client::checkAccountStatus);
    m_statusCheckTimer->setInterval(30000);
}

// ---- Auth ----

void Client::login(const QString &user, const QString &password)
{
    QJsonObject body;
    body["username"] = user.trimmed();
    body["password"] = password;

    QNetworkReply *reply = HttpManager::instance().post("/api/auth/login", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply, user]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);

        if (!HttpManager::isSuccess(resp)) {
            emit loginResult(false, HttpManager::errorMessage(resp));
            return;
        }

        QJsonObject data = HttpManager::dataObject(resp);
        QString token = data["token"].toString();
        m_currentUser = user.trimmed();
        m_loggedIn = true;

        HttpManager::instance().setAuthToken(token);

        emit loginResult(true, QString::fromUtf8("登录成功"));

        loadPositions();
        loadOrderHistory();
        m_statusCheckTimer->start();
    });
}

void Client::registerUser(const QString &user, const QString &password,
                          const QString &realName, const QString &email,
                          const QString &phone, const QString &idCard,
                          const QString &address)
{
    QString u = user.trimmed();
    if (u.size() < 3 || password.size() < 6) {
        emit registerResult(false, QString::fromUtf8("注册失败：用户名>=3且密码>=6"));
        return;
    }

    QJsonObject body;
    body["username"] = u;
    body["password"] = password;
    body["realName"] = realName.trimmed();
    body["email"] = email.trimmed();
    body["phone"] = phone.trimmed();
    body["idCard"] = idCard.trimmed();
    body["address"] = address.trimmed();

    QNetworkReply *reply = HttpManager::instance().post("/api/auth/register", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            emit registerResult(true, QString::fromUtf8("注册成功"));
        } else {
            emit registerResult(false, HttpManager::errorMessage(resp));
        }
    });
}

// ---- Quotes ----

void Client::queryQuote(const QString &symbol)
{
    if (symbol.trimmed().isEmpty()) {
        emit quoteUpdated(symbol, 0.0);
        return;
    }

    QNetworkReply *reply = HttpManager::instance().get("/api/stocks/" + symbol.toUpper() + "/quote");
    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            double price = HttpManager::dataObject(resp)["currentPrice"].toDouble();
            emit quoteUpdated(symbol.toUpper(), price);
        } else {
            emit quoteUpdated(symbol.toUpper(), 0.0);
        }
    });
}

// ---- Orders ----

void Client::placeOrder(const QString &symbol, const QString &side, int quantity, double price)
{
    if (!m_loggedIn) {
        emit orderRejected(QString::fromUtf8("请先登录"));
        return;
    }
    if (symbol.trimmed().isEmpty()) {
        emit orderRejected(QString::fromUtf8("股票代码不能为空"));
        return;
    }
    if (quantity <= 0) {
        emit orderRejected(QString::fromUtf8("数量必须大于0"));
        return;
    }
    if (price <= 0) {
        emit orderRejected(QString::fromUtf8("价格必须大于0"));
        return;
    }

    QJsonObject body;
    body["symbol"] = symbol.toUpper();
    body["side"] = side;
    body["quantity"] = quantity;
    body["price"] = price;

    QNetworkReply *reply = HttpManager::instance().post("/api/orders", body);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);

        if (!HttpManager::isSuccess(resp)) {
            emit orderRejected(HttpManager::errorMessage(resp));
            return;
        }

        QJsonObject data = HttpManager::dataObject(resp);
        OrderInfo info = parseOrderJson(data);
        m_orders.push_back(info);
        emit orderPlaced(info);

        loadPositions();
    });
}

void Client::cancelOrder(const QString &orderId)
{
    for (OrderInfo &o : m_orders) {
        if (o.orderId == orderId) {
            o.status = QString::fromUtf8("Canceled");
            emit orderUpdated(o);
            break;
        }
    }

    QNetworkReply *reply = HttpManager::instance().del("/api/orders/" + orderId);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void Client::fillOrder(const QString &orderId)
{
    QNetworkReply *reply = HttpManager::instance().put(
        "/api/orders/" + orderId + "/fill", QJsonObject());
    connect(reply, &QNetworkReply::finished, this, [this, reply, orderId]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (HttpManager::isSuccess(resp)) {
            for (OrderInfo &o : m_orders) {
                if (o.orderId == orderId) {
                    o.status = QString::fromUtf8("Filled");
                    emit orderFilled(o);
                    break;
                }
            }
            loadPositions();
        }
    });
}

void Client::loadOrderHistory()
{
    if (!m_loggedIn || m_currentUser.isEmpty()) return;

    QNetworkReply *reply = HttpManager::instance().get("/api/orders");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);

        QVector<OrderInfo> orders;
        if (HttpManager::isSuccess(resp)) {
            QJsonArray arr = HttpManager::dataArray(resp);
            for (const QJsonValue &v : arr) {
                orders.push_back(parseOrderJson(v.toObject()));
            }
        }
        m_orders = orders;
        emit orderHistoryLoaded(orders);
    });
}

// ---- Positions ----

void Client::loadPositions()
{
    QNetworkReply *reply = HttpManager::instance().get("/api/positions");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);

        QVector<PositionInfo> positions;
        if (HttpManager::isSuccess(resp)) {
            QJsonArray arr = HttpManager::dataArray(resp);
            for (const QJsonValue &v : arr) {
                positions.push_back(parsePositionJson(v.toObject()));
            }
        }
        emit positionsLoaded(positions);
    });
}

// ---- Logout ----

void Client::logout()
{
    if (!m_loggedIn) return;

    m_statusCheckTimer->stop();

    // Fire-and-forget logout
    QNetworkReply *reply = HttpManager::instance().post("/api/auth/logout");
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });

    HttpManager::instance().clearAuthToken();
    m_loggedIn = false;
    QString user = m_currentUser;
    m_currentUser.clear();
    m_orders.clear();

    emit loggedOut(QString::fromUtf8("已退出登录"));
    qDebug() << "User logged out:" << user;
}

// ---- Account Status Check ----

void Client::checkAccountStatus()
{
    if (!m_loggedIn || m_currentUser.isEmpty()) return;

    QNetworkReply *reply = HttpManager::instance().get("/api/auth/status");
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        QJsonObject resp = HttpManager::parseResponse(reply);
        if (!HttpManager::isSuccess(resp)) return;

        QJsonObject data = HttpManager::dataObject(resp);
        if (data["frozen"].toBool()) {
            qWarning() << "Account frozen, logging out:" << m_currentUser;

            m_statusCheckTimer->stop();
            QString user = m_currentUser;
            m_loggedIn = false;
            m_currentUser.clear();
            m_orders.clear();
            HttpManager::instance().clearAuthToken();

            emit accountFrozen();
            emit loggedOut(QString::fromUtf8("账号已被冻结，已自动退出登录"));
        }
    });
}

// ---- JSON Parsers ----

OrderInfo Client::parseOrderJson(const QJsonObject &json)
{
    OrderInfo info;
    info.orderId = json["orderId"].toString();
    info.symbol = json["symbol"].toString();
    info.side = json["side"].toString();
    info.quantity = json["quantity"].toInt();
    info.price = json["price"].toDouble();
    info.status = json["status"].toString();
    info.createdAt = QDateTime::fromString(json["createdAt"].toString(), Qt::ISODate);
    if (!info.createdAt.isValid()) info.createdAt = QDateTime::currentDateTime();
    return info;
}

PositionInfo Client::parsePositionJson(const QJsonObject &json)
{
    PositionInfo p;
    p.symbol = json["symbol"].toString();
    p.quantity = static_cast<long long>(json["quantity"].toDouble());
    p.averagePrice = json["averagePrice"].toDouble();
    p.currentPrice = 0.0;
    p.profitLoss = 0.0;
    p.profitLossPercent = 0.0;
    return p;
}
