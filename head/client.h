#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QSqlDatabase>
#include <QTimer>
#include "dbmanager.h"

struct OrderInfo {
	QString orderId;
	QString symbol;
	QString side; // Buy or Sell
	int quantity{0};
	double price{0.0};
	QString status; // New, Filled, Canceled, Rejected
	QDateTime createdAt; // 订单创建时间
};

struct PositionInfo {
	QString symbol;
	long long quantity{0};
	double averagePrice{0.0};
	double currentPrice{0.0}; // 当前市价
	double profitLoss{0.0}; // 浮动盈亏
	double profitLossPercent{0.0}; // 盈亏比例
};

class Client : public QObject {
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
	Q_INVOKABLE void loadOrderHistory(); // 加载历史订单
	Q_INVOKABLE void logout();

	const QVector<OrderInfo> &orders() const { return m_orders; }
	bool isLoggedIn() const { return m_loggedIn; }
	QString currentUser() const { return m_currentUser; }
	DbManager* dbManager() { return &m_db; }

signals:
	void loginResult(bool success, const QString &message);
	void registerResult(bool success, const QString &message);
	void quoteUpdated(const QString &symbol, double lastPrice);
	void orderPlaced(const OrderInfo &order);
	void orderUpdated(const OrderInfo &order);
	void orderHistoryLoaded(const QVector<OrderInfo> &orders);
	void orderRejected(const QString &reason); // 订单被拒绝
	void positionsLoaded(const QVector<PositionInfo> &positions);
	void loggedOut(const QString &message);
	void accountFrozen();

private:
	QString generateOrderId() const;
	void checkAccountStatus(); // 检查账号状态
	QVector<OrderInfo> m_orders;
	bool m_loggedIn{false};
    QString m_currentUser;
    DbManager m_db;
    QTimer *m_statusCheckTimer; // 定期检查账号状态的定时器
};


