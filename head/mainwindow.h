#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QTabWidget>
#include "client.h"
#include "dataservice.h"
#include "quotedata.h"
#include "quotewidget.h"
#include "klinewidget.h"
#include "companywidget.h"
#include "userservice.h"
#include "userinfowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
    explicit MainWindow(Client *client, QWidget *parent = nullptr);
	~MainWindow() override;

private slots:
	void onQuery();
	void onPlaceOrder();
	void onOrderDoubleClicked(int row, int column);

	void handleQuoteUpdated(const QString &symbol, double lastPrice);
	void handleOrderPlaced(const OrderInfo &order);
	void handleOrderUpdated(const OrderInfo &order);
	void handleOrderHistoryLoaded(const QVector<OrderInfo> &orders);
	void handleOrderRejected(const QString &reason);
	void handlePositionsLoaded(const QVector<PositionInfo> &positions);
	void handleLoggedOut(const QString &message);
	void handleAccountFrozen();

private:
    void setupConnections();
	void setupAccountMenu();
	void clearSessionTradingUI();
	void appendOrUpdateOrderRow(const OrderInfo &order);
	void renderPositions(const QVector<PositionInfo> &positions);
    void setupTabs();
    void updateSymbolCombo();
    void onSymbolChanged();

	Ui::MainWindow *ui;
	QPointer<Client> m_client;
    DataService *m_dataService;
    UserService *m_userService;
    QTabWidget *m_mainTabs;
    QuoteWidget *m_quoteWidget;
    KLineWidget *m_klineWidget;
    CompanyWidget *m_companyWidget;
    UserInfoWidget *m_userInfoWidget;
};


