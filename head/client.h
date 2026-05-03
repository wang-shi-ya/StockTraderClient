#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QTimer>
#include "dbmanager.h"

struct OrderInfo {
    QString orderId;
    QString symbol;
    QString side;
    int quantity{0};
    double price{0.0};
    QString status;
    QDateTime createdAt;
};

struct PositionInfo {
    QString symbol;
    long long quantity{0};
    double averagePrice{0.0};
    double currentPrice{0.0};
    double profitLoss{0.0};
    double profitLossPercent{0.0};
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    Q_INVOKABLE void login(const QString &user, const QString &password);
    Q_INVOKABLE void registerUser(const QString &user, const QString &password,
                                  const QString &realName = "", const QString &email = "",
                                  const QString &phone = "", const QString &idCard = "",
                                  const QString &address = "");
    Q_INVOKABLE void queryQuote(const QString &symbol);
    Q_INVOKABLE void placeOrder(const QString &symbol, const QString &side, int quantity, double price);
    Q_INVOKABLE void cancelOrder(const QString &orderId);
    Q_INVOKABLE void fillOrder(const QString &orderId);
    Q_INVOKABLE void loadOrderHistory();
    Q_INVOKABLE void logout();

    const QVector<OrderInfo> &orders() const { return m_orders; }
    bool isLoggedIn() const { return m_loggedIn; }
    QString currentUser() const { return m_currentUser; }
    DbManager *dbManager() { return &m_db; }

signals:
    void loginResult(bool success, const QString &message);
    void registerResult(bool success, const QString &message);
    void quoteUpdated(const QString &symbol, double lastPrice);
    void orderPlaced(const OrderInfo &order);
    void orderUpdated(const OrderInfo &order);
    void orderFilled(const OrderInfo &order);
    void orderHistoryLoaded(const QVector<OrderInfo> &orders);
    void orderRejected(const QString &reason);
    void positionsLoaded(const QVector<PositionInfo> &positions);
    void loggedOut(const QString &message);
    void accountFrozen();

private:
    void checkAccountStatus();
    void loadPositions();
    static OrderInfo parseOrderJson(const QJsonObject &json);
    static PositionInfo parsePositionJson(const QJsonObject &json);

    QVector<OrderInfo> m_orders;
    bool m_loggedIn{false};
    QString m_currentUser;
    DbManager m_db;
    QTimer *m_statusCheckTimer;
};
