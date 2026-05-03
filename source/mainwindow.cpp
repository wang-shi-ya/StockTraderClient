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
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QCompleter>
#include <QtCore>
#include "rippleoverlay.h"

MainWindow::MainWindow(Client *client, QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), m_client(client) {
	ui->setupUi(this);
	setWindowIcon(QIcon(":/icons/app.ico"));

	// 初始化主题并应用全局样式
	ThemeHelper::instance();
	qApp->setStyleSheet(ThemeHelper::currentStyleSheet());

	// Clear widget-level stylesheets so global QSS takes effect
	ui->quoteWidget->setStyleSheet(QString());
	ui->orderWidget->setStyleSheet(QString());
	ui->ordersWidget->setStyleSheet(QString());
	ui->positionsWidget->setStyleSheet(QString());
	ui->statusbar->setStyleSheet(QString());
	ui->btnPlace->setStyleSheet(QString());
	ui->labelOrdersTitle->setStyleSheet(QString());
	ui->labelPositionsTitle->setStyleSheet(QString());
	ui->labelStatusFilter->setStyleSheet(QString());

	// 创建数据服务
	m_dataService = new DataService(this);
	m_userService = new UserService(this);
	
	// ---- 交易记录表格 ----
	ui->tableOrders->setColumnCount(7);
	QStringList headers;
	headers << "订单号" << "代码" << "方向" << "数量" << "价格" << "状态" << "创建时间";
	ui->tableOrders->setHorizontalHeaderLabels(headers);
	ui->tableOrders->setAlternatingRowColors(true);
	ui->tableOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableOrders->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableOrders->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableOrders->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->tableOrders->verticalHeader()->setVisible(false);
	ui->tableOrders->verticalHeader()->setDefaultSectionSize(36);
	ui->tableOrders->horizontalHeader()->setFixedHeight(44);
	ui->tableOrders->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	// ---- 持仓表格 ----
	ui->tablePositions->setColumnCount(6);
	QStringList posHeaders;
	posHeaders << "代码" << "持仓数量" << "持仓均价" << "当前市价" << "浮动盈亏" << "盈亏比例";
	ui->tablePositions->setHorizontalHeaderLabels(posHeaders);
	ui->tablePositions->setAlternatingRowColors(true);
	ui->tablePositions->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tablePositions->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tablePositions->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tablePositions->verticalHeader()->setVisible(false);
	ui->tablePositions->verticalHeader()->setDefaultSectionSize(36);
	ui->tablePositions->horizontalHeader()->setFixedHeight(44);
	ui->tablePositions->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	// 设置新的标签页
	setupTabs();
	refreshThemeSpecificStyles();  // 初始化主题样式
	setupConnections();
	
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
	connect(ui->tableOrders, &QTableWidget::customContextMenuRequested,
	        this, &MainWindow::onOrderContextMenu);
	connect(ui->comboStatusFilter, &QComboBox::currentTextChanged,
	        this, &MainWindow::onStatusFilterChanged);
	connect(ui->comboSymbol, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
	        this, &MainWindow::onSymbolChanged);

	connect(m_client, &Client::quoteUpdated, this, &MainWindow::handleQuoteUpdated);
	connect(m_client, &Client::orderPlaced, this, &MainWindow::handleOrderPlaced);
	connect(m_client, &Client::orderUpdated, this, &MainWindow::handleOrderUpdated);
	connect(m_client, &Client::orderFilled, this, &MainWindow::handleOrderUpdated);
	connect(m_client, &Client::orderHistoryLoaded, this, &MainWindow::handleOrderHistoryLoaded);
	connect(m_client, &Client::orderRejected, this, &MainWindow::handleOrderRejected);
	connect(m_client, &Client::positionsLoaded, this, &MainWindow::handlePositionsLoaded);
	connect(m_client, &Client::loggedOut, this, &MainWindow::handleLoggedOut);
	connect(m_client, &Client::accountFrozen, this, &MainWindow::handleAccountFrozen);
	
	// 连接数据服务信号以更新股票列表
	connect(m_dataService, &DataService::quoteListUpdated, this, &MainWindow::updateSymbolCombo);
	// DataService 轮询行情时同步更新交易栏最新价
	connect(m_dataService, &DataService::quoteUpdated, this, &MainWindow::onDataServiceQuoteUpdated);
}

void MainWindow::clearSessionTradingUI() {
	ui->tableOrders->setRowCount(0);
	ui->tablePositions->setRowCount(0);
	ui->labelLast->setText(QString::fromUtf8("最新: -"));
	QString accentColor = ThemeHelper::isDarkMode() ? "#64B5F6" : "#1E88E5";
	ui->labelLast->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(accentColor));
}

void MainWindow::onQuery() {
	QString symbol = ui->comboSymbol->currentData().toString();
	if (symbol.isEmpty())
		symbol = ui->comboSymbol->currentText();
	m_client->queryQuote(symbol);
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

void MainWindow::onOrderContextMenu(const QPoint &pos) {
	int row = ui->tableOrders->rowAt(pos.y());
	if (row < 0) return;

	ui->tableOrders->selectRow(row);
	const QString orderId = ui->tableOrders->item(row, 0)->text();
	if (orderId.isEmpty()) return;
	const QString status = ui->tableOrders->item(row, 5)->text();

	if (status != "New") return;

	QMenu menu(this);
	QAction *fillAction = menu.addAction(QString::fromUtf8("成交"));
	QAction *cancelAction = menu.addAction(QString::fromUtf8("撤单"));

	QAction *chosen = menu.exec(ui->tableOrders->viewport()->mapToGlobal(pos));
	if (chosen == fillAction) {
		m_client->fillOrder(orderId);
	} else if (chosen == cancelAction) {
		m_client->cancelOrder(orderId);
	}
}

void MainWindow::onStatusFilterChanged(const QString &text) {
	for (int r = 0; r < ui->tableOrders->rowCount(); ++r) {
		QTableWidgetItem *item = ui->tableOrders->item(r, 5);
		if (!item) continue;
		QString status = item->text();
		bool show = (text == QString::fromUtf8("全部")) || (status == text);
		ui->tableOrders->setRowHidden(r, !show);
	}
}

void MainWindow::onThemeToggled() {
	triggerRippleEffect();
}

void MainWindow::triggerRippleEffect() {
	if (m_rippleOverlay) return;

	// Capture the current window (old theme)
	QPixmap screenshot = grab();

	QPoint btnCenter = m_themeBtn->mapToGlobal(
		QPoint(m_themeBtn->width() / 2, m_themeBtn->height() / 2));

	// Toggle theme immediately — new theme appears underneath the overlay
	ThemeHelper::instance()->toggle();
	bool nowDark = ThemeHelper::isDarkMode();
	m_themeBtn->setDark(nowDark);
	refreshThemeSpecificStyles();

	// Overlay shows old screenshot with expanding hole revealing new theme
	m_rippleOverlay = new RippleOverlay(this);

	connect(m_rippleOverlay, &RippleOverlay::finished, this, [this]() {
		m_rippleOverlay = nullptr;
	});

	m_rippleOverlay->animate(btnCenter, screenshot);
}

void MainWindow::onFillOrder() {
	// 获取当前选中的行
	int row = ui->tableOrders->currentRow();
	if (row < 0) return;
	const QString orderId = ui->tableOrders->item(row, 0)->text();
	m_client->fillOrder(orderId);
}

void MainWindow::handleQuoteUpdated(const QString &symbol, double lastPrice) {
	QuoteData quote = m_dataService->getQuote(symbol);
	double change = 0.0, changePercent = 0.0;

	if (quote.currentPrice > 0 && quote.symbol == symbol) {
		change = quote.change;
		changePercent = quote.changePercent;
	} else if (lastPrice > 0) {
		// DataService 暂未有数据，仅显示价格
		QString accentColor = ThemeHelper::isDarkMode() ? "#64B5F6" : "#1E88E5";
		ui->labelLast->setText(QString::fromUtf8("最新: %1").arg(lastPrice, 0, 'f', 3));
		ui->labelLast->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(accentColor));
		return;
	} else {
		QString accentColor = ThemeHelper::isDarkMode() ? "#64B5F6" : "#1E88E5";
		ui->labelLast->setText(QString::fromUtf8("最新: -"));
		ui->labelLast->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(accentColor));
		return;
	}

	QColor color = change >= 0 ? QColor("#ef5350") : QColor("#26a69a");
	QString sign = change >= 0 ? "+" : "";
	ui->labelLast->setText(QString::fromUtf8("最新: %1  %2%3  %4%5%")
		.arg(lastPrice > 0 ? lastPrice : quote.currentPrice, 0, 'f', 3)
		.arg(sign).arg(change, 0, 'f', 2)
		.arg(sign).arg(changePercent, 0, 'f', 2));
	ui->labelLast->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(color.name()));
}

void MainWindow::handleOrderPlaced(const OrderInfo &order) { 
	appendOrUpdateOrderRow(order);
	QMessageBox::information(this, QString::fromUtf8("下单成功"), 
	                        QString::fromUtf8("订单已提交，订单号：%1").arg(order.orderId));
}

void MainWindow::handleOrderUpdated(const OrderInfo &order) {
	appendOrUpdateOrderRow(order);
	onStatusFilterChanged(ui->comboStatusFilter->currentText());
}

void MainWindow::handleOrderHistoryLoaded(const QVector<OrderInfo> &orders) {
	ui->tableOrders->setRowCount(0);
	for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
		appendOrUpdateOrderRow(*it);
	}
	ui->tableOrders->resizeColumnsToContents();
	onStatusFilterChanged(ui->comboStatusFilter->currentText());
}

void MainWindow::handleOrderRejected(const QString &reason) {
	QMessageBox::warning(this, QString::fromUtf8("下单失败"), reason);
}

void MainWindow::appendOrUpdateOrderRow(const OrderInfo &order) {
	int foundRow = -1;
	for (int r = 0; r < ui->tableOrders->rowCount(); ++r) {
		if (ui->tableOrders->item(r, 0) && ui->tableOrders->item(r, 0)->text() == order.orderId) {
			foundRow = r;
			break;
		}
	}
	if (foundRow == -1) {
		foundRow = 0;
		ui->tableOrders->insertRow(foundRow);
	}

	auto setItem = [&](int col, const QString &text, QColor color = QColor(), Qt::Alignment alignment = Qt::AlignCenter) {
		QTableWidgetItem *item = ui->tableOrders->item(foundRow, col);
		if (!item) {
			item = new QTableWidgetItem();
			ui->tableOrders->setItem(foundRow, col, item);
		}
		item->setText(text);
		item->setTextAlignment(alignment);
		if (color.isValid()) {
			item->setForeground(color);
		}
	};

	setItem(0, order.orderId, QColor(), Qt::AlignCenter);
	setItem(1, order.symbol, QColor(), Qt::AlignCenter);

	QColor sideColor = (order.side == "买入" || order.side == "Buy") ? QColor("#ef5350") : QColor("#26a69a");
	setItem(2, order.side, sideColor, Qt::AlignCenter);

	setItem(3, QString::number(order.quantity), QColor(), Qt::AlignRight | Qt::AlignVCenter);
	setItem(4, QString::number(order.price, 'f', 3), QColor(), Qt::AlignRight | Qt::AlignVCenter);

	QString statusText = order.status;
	QColor statusColor;
	if (statusText == "Filled") {
		statusColor = QColor("#42a5f5");
	} else if (statusText == "Canceled") {
		statusColor = QColor("#9e9e9e");
	} else if (statusText == "New") {
		statusColor = QColor("#ff9800");
	} else {
		statusColor = QColor("#757575");
	}
	if (statusText.isEmpty())
		statusText = QString::fromUtf8("未知");
	setItem(5, statusText, statusColor, Qt::AlignCenter);

	QString timeStr = order.createdAt.isValid() ?
	                  order.createdAt.toString("yyyy-MM-dd hh:mm:ss") :
	                  QString::fromUtf8("未知");
	setItem(6, timeStr, QColor(), Qt::AlignCenter);
}

void MainWindow::handlePositionsLoaded(const QVector<PositionInfo> &positions) {
	renderPositions(positions);

	// 更新用户信息
	if (m_client && m_client->isLoggedIn()) {
		QString currentUser = m_client->currentUser();
		if (!currentUser.isEmpty()) {
			m_userService->calculateMarketValue(currentUser, positions);
			m_userInfoWidget->setCurrentUser(currentUser);
		}
	}
}

void MainWindow::renderPositions(const QVector<PositionInfo> &positions) {
	ui->tablePositions->setUpdatesEnabled(false);
	ui->tablePositions->setRowCount(positions.size());
	int row = 0;
	for (const auto &p : positions) {
		double currentPrice = p.currentPrice;
		if (currentPrice <= 0) {
			QuoteData quote = m_dataService->getQuote(p.symbol);
			currentPrice = quote.currentPrice;
		}

		double profitLoss = (currentPrice - p.averagePrice) * p.quantity;
		double profitLossPercent = p.averagePrice > 0 ?
		                          ((currentPrice - p.averagePrice) / p.averagePrice) * 100.0 : 0.0;

		auto setItem = [&](int col, const QString &text, QColor color = QColor(), Qt::Alignment alignment = Qt::AlignCenter) {
			QTableWidgetItem *item = new QTableWidgetItem(text);
			item->setTextAlignment(alignment);
			if (color.isValid()) {
				item->setForeground(color);
			}
			ui->tablePositions->setItem(row, col, item);
		};

		setItem(0, p.symbol, QColor(), Qt::AlignCenter);
		setItem(1, QString::number(p.quantity), QColor(), Qt::AlignRight | Qt::AlignVCenter);
		setItem(2, QString::number(p.averagePrice, 'f', 3), QColor(), Qt::AlignRight | Qt::AlignVCenter);
		setItem(3, currentPrice > 0 ? QString::number(currentPrice, 'f', 3) : QString::fromUtf8("--"),
		        QColor(), Qt::AlignRight | Qt::AlignVCenter);

		QColor profitColor = profitLoss >= 0 ? QColor("#ef5350") : QColor("#26a69a");
		QString profitStr = profitLoss >= 0 ?
		                    QString("+%1").arg(profitLoss, 0, 'f', 2) :
		                    QString::number(profitLoss, 'f', 2);
		setItem(4, profitStr, profitColor, Qt::AlignRight | Qt::AlignVCenter);

		QString percentStr = profitLossPercent >= 0 ?
		                     QString("+%1%").arg(profitLossPercent, 0, 'f', 1) :
		                     QString("%1%").arg(profitLossPercent, 0, 'f', 1);
		setItem(5, percentStr, profitColor, Qt::AlignRight | Qt::AlignVCenter);

		row++;
	}
	ui->tablePositions->setUpdatesEnabled(true);
	ui->tablePositions->resizeColumnsToContents();
}

void MainWindow::setupTabs() {
	// 创建主标签页
	m_mainTabs = new QTabWidget();

	// 创建各个功能组件
	m_quoteWidget = new QuoteWidget(m_dataService, m_client->dbManager());
	m_klineWidget = new KLineWidget(m_dataService, this);
	m_companyWidget = new CompanyWidget(m_dataService);
	m_newsWidget = new NewsWidget(m_dataService);
	m_userInfoWidget = new UserInfoWidget(m_userService, m_client);

	// 连接QuoteWidget的信号以更新股票选择列表
	connect(m_quoteWidget, &QuoteWidget::symbolListChanged, this, &MainWindow::updateSymbolCombo);

	// 登录成功后加载用户监控的股票
	connect(m_client, &Client::loginResult, this, [this](bool ok, const QString &) {
		if (ok) {
			m_quoteWidget->setCurrentUsername(m_client->currentUser());
		}
	});

	// 退出登录后清除监控列表
	connect(m_client, &Client::loggedOut, this, [this](const QString &) {
		m_quoteWidget->setCurrentUsername(QString());
	});

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
	m_mainTabs->addTab(m_newsWidget, "新闻资讯");
	m_mainTabs->addTab(m_userInfoWidget, "个人中心");

	// 设置中央部件
	setCentralWidget(m_mainTabs);

	// 主题切换按钮（放在标签栏最右侧）
	m_themeBtn = new ThemeToggleButton(this);
	connect(m_themeBtn, &ThemeToggleButton::toggled, this, &MainWindow::onThemeToggled);
	QWidget *cornerContainer = new QWidget();
	QHBoxLayout *cornerLayout = new QHBoxLayout(cornerContainer);
	cornerLayout->setContentsMargins(0, 0, 8, 0);
	cornerLayout->addWidget(m_themeBtn);
	m_mainTabs->setCornerWidget(cornerContainer, Qt::TopRightCorner);

	// 初始化股票选择列表
	updateSymbolCombo();

	// 如果已登录，加载用户监控的股票
	if (m_client->isLoggedIn()) {
		m_quoteWidget->setCurrentUsername(m_client->currentUser());
	}

	// 启动后默认显示 K 线图（不依赖是否已登录）
	m_mainTabs->setCurrentWidget(m_klineWidget);
}

void MainWindow::updateSymbolCombo() {
	// 获取当前选中的股票代码
	QString currentSymbol = ui->comboSymbol->currentText();
	
	// 清空并重新填充股票列表
	ui->comboSymbol->clear();
	
	// 从QuoteWidget获取用户添加的股票列表
	QStringList watchedSymbols = m_quoteWidget->getWatchedSymbols();
	
	if (watchedSymbols.isEmpty()) {
		// 如果没有添加的股票，添加提示项
		ui->comboSymbol->addItem("请先在实时行情中添加股票");
		ui->comboSymbol->setEnabled(false);
		ui->comboSymbol->setEditable(false);
	} else {
		ui->comboSymbol->setEnabled(true);
		ui->comboSymbol->setEditable(true);
		
		// 构建股票列表
		QStringList stockList;
		for (const QString &symbol : watchedSymbols) {
			// 从数据服务获取股票名称
			QString stockName = m_dataService->getStockName(symbol);
			QString displayText = QString("%1 (%2)").arg(symbol).arg(stockName.isEmpty() ? "未知" : stockName);
			ui->comboSymbol->addItem(displayText, symbol);
			stockList.append(displayText);
		}
		
		// 添加QCompleter实现模糊搜索
		QCompleter *completer = new QCompleter(stockList, this);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setFilterMode(Qt::MatchContains); // 模糊匹配
		ui->comboSymbol->setCompleter(completer);
		
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
		QuoteData quote = m_dataService->getQuote(symbol);
		if (quote.currentPrice > 0) {
			ui->spinPrice->setValue(quote.currentPrice);

			double change = quote.change;
			double changePercent = quote.changePercent;
			QColor color = change >= 0 ? QColor("#ef5350") : QColor("#26a69a");
			QString sign = change >= 0 ? "+" : "";
			ui->labelLast->setText(QString::fromUtf8("最新: %1  %2%3  %4%5%")
				.arg(quote.currentPrice, 0, 'f', 3)
				.arg(sign).arg(change, 0, 'f', 2)
				.arg(sign).arg(changePercent, 0, 'f', 2));
			ui->labelLast->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(color.name()));
		}
	}
}

void MainWindow::onDataServiceQuoteUpdated(const QuoteData &quote) {
	QString selectedSymbol = ui->comboSymbol->currentData().toString();
	if (quote.symbol != selectedSymbol || quote.currentPrice <= 0)
		return;

	double change = quote.change;
	double changePercent = quote.changePercent;
	QColor color = change >= 0 ? QColor("#ef5350") : QColor("#26a69a");
	QString sign = change >= 0 ? "+" : "";
	ui->labelLast->setText(QString::fromUtf8("最新: %1  %2%3  %4%5%")
		.arg(quote.currentPrice, 0, 'f', 3)
		.arg(sign).arg(change, 0, 'f', 2)
		.arg(sign).arg(changePercent, 0, 'f', 2));
	ui->labelLast->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(color.name()));
}

void MainWindow::handleLoggedOut(const QString &message) {
	// 账号冻结时 handleAccountFrozen 已弹窗，此处不再重复提示
	if (!message.contains(QString::fromUtf8("冻结"))) {
		QMessageBox::information(this, QString::fromUtf8("退出登录"), message);
	}
	clearSessionTradingUI();
	if (m_userInfoWidget)
		m_userInfoWidget->setCurrentUser(QString());
	if (m_quoteWidget)
		m_quoteWidget->setCurrentUsername(QString());
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

void MainWindow::refreshThemeSpecificStyles() {
	// Cards, statusbar, button, and most labels are handled by global QSS.
	// Only update widgets whose style depends on dynamic content.
	bool dark = ThemeHelper::isDarkMode();
	QString accentColor = dark ? "#64B5F6" : "#1E88E5";

	QString currentText = ui->labelLast->text();
	if (currentText == QString::fromUtf8("最新: -")) {
		ui->labelLast->setStyleSheet(
			QString("font-size: 16px; font-weight: bold; color: %1;").arg(accentColor));
	}
}


