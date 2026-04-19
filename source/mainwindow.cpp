#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QColor>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>

MainWindow::MainWindow(Client *client, QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), m_client(client) {
	ui->setupUi(this);
	
	// 创建数据服务，传递数据库管理器
	m_dataService = new DataService(m_client->dbManager(), this);
	m_userService = new UserService(m_client->dbManager(), this);
	
	// 设置原有表格
	ui->tableOrders->setColumnCount(7);
	QStringList headers;
	headers << "订单号" << "代码" << "方向" << "数量" << "价格" << "状态" << "创建时间";
	ui->tableOrders->setHorizontalHeaderLabels(headers);
	ui->tableOrders->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableOrders->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableOrders->setColumnWidth(0, 100);
	ui->tableOrders->setColumnWidth(6, 150);

	// positions table
	ui->tablePositions->setColumnCount(6);
	QStringList posHeaders;
	posHeaders << "代码" << "持仓数量" << "持仓均价" << "当前市价" << "浮动盈亏" << "盈亏比例";
	ui->tablePositions->setHorizontalHeaderLabels(posHeaders);

	// 设置新的标签页
	setupTabs();
	setupConnections();
	setupAccountMenu();
	
	// 如果已登录，立即加载用户信息
	if (m_client && m_client->isLoggedIn()) {
		QString currentUser = m_client->currentUser();
		if (!currentUser.isEmpty()) {
			m_userInfoWidget->setCurrentUser(currentUser);
		}
	}
}

MainWindow::~MainWindow() { 
	delete ui; 
}

void MainWindow::setupConnections() {
	connect(ui->btnQuery, &QPushButton::clicked, this, &MainWindow::onQuery);
	connect(ui->btnPlace, &QPushButton::clicked, this, &MainWindow::onPlaceOrder);
	connect(ui->tableOrders, &QTableWidget::cellDoubleClicked, this, &MainWindow::onOrderDoubleClicked);
	connect(ui->comboSymbol, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
	        this, &MainWindow::onSymbolChanged);

	connect(m_client, &Client::quoteUpdated, this, &MainWindow::handleQuoteUpdated);
	connect(m_client, &Client::orderPlaced, this, &MainWindow::handleOrderPlaced);
	connect(m_client, &Client::orderUpdated, this, &MainWindow::handleOrderUpdated);
	connect(m_client, &Client::orderHistoryLoaded, this, &MainWindow::handleOrderHistoryLoaded);
	connect(m_client, &Client::orderRejected, this, &MainWindow::handleOrderRejected);
	connect(m_client, &Client::positionsLoaded, this, &MainWindow::handlePositionsLoaded);
	connect(m_client, &Client::loggedOut, this, &MainWindow::handleLoggedOut);
	connect(m_client, &Client::accountFrozen, this, &MainWindow::handleAccountFrozen);
	
	// 连接数据服务信号以更新股票列表
	connect(m_dataService, &DataService::quoteListUpdated, this, &MainWindow::updateSymbolCombo);
}

void MainWindow::clearSessionTradingUI() {
	ui->tableOrders->setRowCount(0);
	ui->tablePositions->setRowCount(0);
	ui->labelLast->setText(QString::fromUtf8("最新: -"));
}

void MainWindow::setupAccountMenu() {
	QMenu *accountMenu = menuBar()->addMenu(QString::fromUtf8("账户"));
	QAction *loginAction = accountMenu->addAction(QString::fromUtf8("登录/注册..."));
	connect(loginAction, &QAction::triggered, this, [this]() {
		LoginDialog dlg(m_client, this);
		if (dlg.exec() == QDialog::Accepted && m_client && m_client->isLoggedIn()) {
			const QString user = m_client->currentUser();
			if (!user.isEmpty())
				m_userInfoWidget->setCurrentUser(user);
		}
	});
}

void MainWindow::onQuery() {
	m_client->queryQuote(ui->comboSymbol->currentText());
}

void MainWindow::onPlaceOrder() {
	const QString symbol = ui->comboSymbol->currentData().toString(); // 获取实际代码
	if (symbol.isEmpty()) {
		QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("请先选择要交易的股票代码"));
		return;
	}
	
	const QString side = ui->comboSide->currentText();
	const int qty = ui->spinQty->value();
	const double price = ui->spinPrice->value();
	
	// 显示交易确认对话框
	QString confirmMsg = QString::fromUtf8("确认下单\n\n")
	                    + QString::fromUtf8("股票代码: %1\n").arg(symbol)
	                    + QString::fromUtf8("交易方向: %1\n").arg(side)
	                    + QString::fromUtf8("数量: %1\n").arg(qty)
	                    + QString::fromUtf8("价格: %1\n").arg(price, 0, 'f', 3)
	                    + QString::fromUtf8("金额: %1").arg(qty * price, 0, 'f', 2);
	
	int ret = QMessageBox::question(this, QString::fromUtf8("确认下单"), confirmMsg,
	                                QMessageBox::Yes | QMessageBox::No,
	                                QMessageBox::No);
	
	if (ret == QMessageBox::Yes) {
		m_client->placeOrder(symbol, side, qty, price);
	}
}

void MainWindow::onOrderDoubleClicked(int row, int /*column*/) {
	if (row < 0) return;
	const QString orderId = ui->tableOrders->item(row, 0)->text();
	m_client->cancelOrder(orderId);
}

void MainWindow::handleQuoteUpdated(const QString &symbol, double lastPrice) {
	ui->labelLast->setText(QString::fromUtf8("最新: %1 %2").arg(symbol).arg(lastPrice, 0, 'f', 3));
}

void MainWindow::handleOrderPlaced(const OrderInfo &order) { 
	appendOrUpdateOrderRow(order);
	QMessageBox::information(this, QString::fromUtf8("下单成功"), 
	                        QString::fromUtf8("订单已提交，订单号：%1").arg(order.orderId));
}

void MainWindow::handleOrderUpdated(const OrderInfo &order) { 
	appendOrUpdateOrderRow(order);
}

void MainWindow::handleOrderHistoryLoaded(const QVector<OrderInfo> &orders) {
	ui->tableOrders->setRowCount(0);
	for (const auto &order : orders) {
		appendOrUpdateOrderRow(order);
	}
}

void MainWindow::handleOrderRejected(const QString &reason) {
	QMessageBox::warning(this, QString::fromUtf8("下单失败"), reason);
}

void MainWindow::appendOrUpdateOrderRow(const OrderInfo &order) {
	// search by orderId
	int foundRow = -1;
	for (int r = 0; r < ui->tableOrders->rowCount(); ++r) {
		if (ui->tableOrders->item(r, 0) && ui->tableOrders->item(r, 0)->text() == order.orderId) {
			foundRow = r;
			break;
		}
	}
	if (foundRow == -1) {
		foundRow = ui->tableOrders->rowCount();
		ui->tableOrders->insertRow(foundRow);
	}
	auto setItem = [&](int col, const QString &text, QColor color = QColor()) {
		QTableWidgetItem *item = ui->tableOrders->item(foundRow, col);
		if (!item) {
			item = new QTableWidgetItem();
			ui->tableOrders->setItem(foundRow, col, item);
		}
		item->setText(text);
		if (color.isValid()) {
			item->setForeground(color);
		}
	};
	setItem(0, order.orderId);
	setItem(1, order.symbol);
	
	// 设置方向颜色
	QColor sideColor = (order.side == "买入" || order.side == "Buy") ? QColor(255, 0, 0) : QColor(0, 255, 0);
	setItem(2, order.side, sideColor);
	
	setItem(3, QString::number(order.quantity));
	setItem(4, QString::number(order.price, 'f', 3));
	
	// 设置状态颜色
	QColor statusColor;
	if (order.status == QString::fromUtf8("Filled") || order.status == QString::fromUtf8("已成交")) {
		statusColor = QColor(0, 0, 255);
	} else if (order.status == QString::fromUtf8("Canceled") || order.status == QString::fromUtf8("已取消")) {
		statusColor = QColor(128, 128, 128);
	} else if (order.status == QString::fromUtf8("Rejected") || order.status == QString::fromUtf8("已拒绝")) {
		statusColor = QColor(255, 0, 0);
	}
	setItem(5, order.status, statusColor);
	
	// 显示创建时间
	QString timeStr = order.createdAt.isValid() ? 
	                  order.createdAt.toString("yyyy-MM-dd hh:mm:ss") : 
	                  QString::fromUtf8("未知");
	setItem(6, timeStr);
}

void MainWindow::handlePositionsLoaded(const QVector<PositionInfo> &positions) {
	renderPositions(positions);
	
	// 更新用户信息
	if (m_client && m_client->isLoggedIn()) {
		QString currentUser = m_client->currentUser();
		if (!currentUser.isEmpty()) {
			m_userInfoWidget->setCurrentUser(currentUser);
		}
	}
}

void MainWindow::renderPositions(const QVector<PositionInfo> &positions) {
	ui->tablePositions->setRowCount(0);
	int row = 0;
	for (const auto &p : positions) {
		ui->tablePositions->insertRow(row);
		
		// 获取当前市价（从数据服务）
		double currentPrice = p.currentPrice;
		if (currentPrice <= 0) {
			// 尝试从数据服务获取
			QuoteData quote = m_dataService->getQuote(p.symbol);
			currentPrice = quote.currentPrice;
		}
		
		// 计算盈亏
		double profitLoss = (currentPrice - p.averagePrice) * p.quantity;
		double profitLossPercent = p.averagePrice > 0 ? 
		                          ((currentPrice - p.averagePrice) / p.averagePrice) * 100.0 : 0.0;
		
		auto setItem = [&](int col, const QString &text, QColor color = QColor()) {
			QTableWidgetItem *item = new QTableWidgetItem(text);
			if (color.isValid()) {
				item->setForeground(color);
			}
			ui->tablePositions->setItem(row, col, item);
		};
		
		setItem(0, p.symbol);
		setItem(1, QString::number(p.quantity));
		setItem(2, QString::number(p.averagePrice, 'f', 3));
		setItem(3, currentPrice > 0 ? QString::number(currentPrice, 'f', 3) : QString::fromUtf8("--"));
		
		// 盈亏颜色
		QColor profitColor = profitLoss >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
		QString profitStr = QString::number(profitLoss, 'f', 2);
		setItem(4, profitStr, profitColor);
		
		QString percentStr = QString("%1%").arg(profitLossPercent, 0, 'f', 2);
		setItem(5, percentStr, profitColor);
		
		row++;
	}
}

void MainWindow::setupTabs() {
	// 创建主标签页
	m_mainTabs = new QTabWidget();
	
	// 创建各个功能组件
	m_quoteWidget = new QuoteWidget(m_dataService);
	m_klineWidget = new KLineWidget(m_dataService);
	m_companyWidget = new CompanyWidget(m_dataService);
	m_userInfoWidget = new UserInfoWidget(m_userService, m_client);
	
	// 连接QuoteWidget的信号以更新股票选择列表
	connect(m_quoteWidget, &QuoteWidget::symbolListChanged, this, &MainWindow::updateSymbolCombo);
	
	// 退出登录后切回 K 线（提示已由 Client::loggedOut → handleLoggedOut 或 UserInfoWidget 处理）
	connect(m_userInfoWidget, &UserInfoWidget::logoutRequested, this, [this]() {
		if (m_mainTabs && m_klineWidget)
			m_mainTabs->setCurrentWidget(m_klineWidget);
		show();
	});
	
	// 添加标签页
	m_mainTabs->addTab(ui->centralwidget, "交易");
	m_mainTabs->addTab(m_quoteWidget, "实时行情");
	m_mainTabs->addTab(m_klineWidget, "K线图");
	m_mainTabs->addTab(m_companyWidget, "公司信息");
	m_mainTabs->addTab(m_userInfoWidget, "个人中心");
	
	// 设置中央部件
	setCentralWidget(m_mainTabs);
	
	// 初始化股票选择列表
	updateSymbolCombo();
	
	// 添加一些预置的股票代码供用户选择
	QStringList defaultSymbols = {"000001", "000002", "600000", "600036", "000858", "600519"};
	for (const QString &symbol : defaultSymbols) {
		m_dataService->subscribeQuote(symbol);
	}

	// 启动后默认显示 K 线图（不依赖是否已登录）
	m_mainTabs->setCurrentWidget(m_klineWidget);
}

void MainWindow::updateSymbolCombo() {
	// 获取当前选中的股票代码
	QString currentSymbol = ui->comboSymbol->currentText();
	
	// 清空并重新填充股票列表
	ui->comboSymbol->clear();
	
	// 从数据服务获取已订阅的股票列表
	QVector<QuoteData> quotes = m_dataService->getQuoteList();
	
	if (quotes.isEmpty()) {
		// 如果没有订阅的股票，添加提示项
		ui->comboSymbol->addItem("请先在实时行情中添加股票");
		ui->comboSymbol->setEnabled(false);
	} else {
		ui->comboSymbol->setEnabled(true);
		for (const QuoteData &quote : quotes) {
			QString displayText = QString("%1 (%2)").arg(quote.symbol).arg(quote.name.isEmpty() ? "未知" : quote.name);
			ui->comboSymbol->addItem(displayText, quote.symbol);
		}
		
		// 尝试恢复之前选中的股票
		if (!currentSymbol.isEmpty()) {
			int index = ui->comboSymbol->findData(currentSymbol);
			if (index >= 0) {
				ui->comboSymbol->setCurrentIndex(index);
			} else {
				ui->comboSymbol->setCurrentIndex(0);
			}
		}
	}
}

void MainWindow::onSymbolChanged() {
	QString symbol = ui->comboSymbol->currentData().toString();
	if (!symbol.isEmpty()) {
		// 更新价格输入框为当前股票的最新价格
		QuoteData quote = m_dataService->getQuote(symbol);
		if (quote.currentPrice > 0) {
			ui->spinPrice->setValue(quote.currentPrice);
		}
	}
}

void MainWindow::handleLoggedOut(const QString &message) {
	// 账号冻结时 handleAccountFrozen 已弹窗，此处不再重复提示
	if (!message.contains(QString::fromUtf8("冻结"))) {
		QMessageBox::information(this, QString::fromUtf8("退出登录"), message);
	}
	clearSessionTradingUI();
	if (m_userInfoWidget)
		m_userInfoWidget->setCurrentUser(QString());
	if (m_mainTabs && m_klineWidget)
		m_mainTabs->setCurrentWidget(m_klineWidget);
	show();
}

void MainWindow::handleAccountFrozen() {
	// 注意：Client::checkAccountStatus() 已经停止了定时器并清除了登录状态
	// 这里只需要显示消息并隐藏窗口
	// 使用静态变量避免重复弹窗
	static qint64 lastFrozenTime = 0;
	qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
	
	// 如果1秒内已经显示过冻结消息，就不再显示
	if (currentTime - lastFrozenTime < 1000) {
		if (m_mainTabs && m_klineWidget)
			m_mainTabs->setCurrentWidget(m_klineWidget);
		show();
		return;
	}
	
	lastFrozenTime = currentTime;
	QMessageBox::warning(this, QString::fromUtf8("账号被冻结"), 
	                     QString::fromUtf8("您的账号已被冻结，系统已自动退出登录。"));
	if (m_mainTabs && m_klineWidget)
		m_mainTabs->setCurrentWidget(m_klineWidget);
	show();
}


