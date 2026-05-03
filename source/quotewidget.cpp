#include "quotewidget.h"
#include "ui_quotewidget.h"
#include "themehelper.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QPalette>
#include <QCompleter>
#include <QStringListModel>

QuoteWidget::QuoteWidget(DataService *dataService, DbManager *dbManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuoteWidget)
    , m_dataService(dataService)
    , m_dbManager(dbManager)
    , m_refreshTimer(new QTimer(this))
{
    ui->setupUi(this);

    // 获取UI组件引用
    m_quoteTable = ui->quoteTable;
    m_indexTable = ui->indexTable;
    m_sectorTable = ui->sectorTable;
    m_symbolInput = ui->symbolInput;
    m_addButton = ui->addButton;
    m_removeButton = ui->removeButton;
    m_refreshButton = ui->refreshButton;

    // 设置表格
    setupQuoteTable();
    setupIndexTable();
    setupSectorTable();

    // 连接数据服务信号
    connect(m_dataService, &DataService::quoteUpdated, this, &QuoteWidget::onQuoteUpdated);
    connect(m_dataService, &DataService::quoteListUpdated, this, &QuoteWidget::onQuoteListUpdated);
    connect(m_dataService, &DataService::indexDataUpdated, this, &QuoteWidget::onIndexDataUpdated);
    connect(m_dataService, &DataService::sectorDataUpdated, this, &QuoteWidget::onSectorDataUpdated);

    // 连接UI信号
    connect(m_addButton, &QPushButton::clicked, this, &QuoteWidget::onAddSymbol);
    connect(m_removeButton, &QPushButton::clicked, this, &QuoteWidget::onRemoveSymbol);
    connect(m_refreshButton, &QPushButton::clicked, this, &QuoteWidget::refreshData);

    // 当异步加载完成后更新下拉框（股票名称届时可用）
    connect(m_dataService, &DataService::companyListUpdated, this, [this](const QStringList &) {
        rebuildSymbolCombo();
    });

    // 设置定时器
    connect(m_refreshTimer, &QTimer::timeout, this, &QuoteWidget::refreshData);
    m_refreshTimer->start(5000); // 每5秒刷新一次显示

    // 行情更新节流：合并短时间内的多次更新为一次 table 刷新
    m_quoteThrottleTimer = new QTimer(this);
    m_quoteThrottleTimer->setSingleShot(true);
    m_quoteThrottleTimer->setInterval(100);
    connect(m_quoteThrottleTimer, &QTimer::timeout, this, &QuoteWidget::updateQuoteTable);

    // 连接DbManager的异步自选股加载信号
    if (m_dbManager) {
        connect(m_dbManager, &DbManager::watchedSymbolsLoaded, this, &QuoteWidget::onWatchedSymbolsLoaded);
    }

    // 启动实时行情
    m_dataService->startRealTimeQuotes();

    // 设置可编辑和 QCompleter 实现模糊搜索
    m_symbolInput->setEditable(true);
    m_symbolInput->setInsertPolicy(QComboBox::NoInsert);
    QCompleter *completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    m_symbolInput->setCompleter(completer);

    // 初始填充（名称可能尚未加载，待 companyListUpdated 后 rebuild）
    rebuildSymbolCombo();

    // 加载数据库中的股票列表
    loadStockListFromDB();

    // 主题切换
    connect(ThemeHelper::instance(), &ThemeHelper::themeChanged, this, &QuoteWidget::refreshTheme);
}

QuoteWidget::~QuoteWidget() {
    m_dataService->stopRealTimeQuotes();
    delete ui;
}

void QuoteWidget::setCurrentUsername(const QString &username) {
    m_currentUsername = username;
    if (!username.isEmpty()) {
        loadWatchedSymbolsFromDB();
    } else {
        clearWatchedSymbols();
    }
}

void QuoteWidget::clearWatchedSymbols() {
    for (const QString &symbol : m_watchedSymbols) {
        m_dataService->unsubscribeQuote(symbol);
    }
    m_watchedSymbols.clear();
    updateQuoteTable();
    emit symbolListChanged();
}

void QuoteWidget::loadWatchedSymbolsFromDB() {
    if (m_dbManager && !m_currentUsername.isEmpty()) {
        m_dbManager->loadUserWatchedSymbols(m_currentUsername);
    }
}

void QuoteWidget::onWatchedSymbolsLoaded(const QString &username, const QStringList &symbols) {
    if (username != m_currentUsername) return;
    for (const QString &symbol : symbols) {
        if (!m_watchedSymbols.contains(symbol)) {
            m_watchedSymbols.append(symbol);
            m_dataService->subscribeQuote(symbol);
        }
    }
    updateQuoteTable();
    emit symbolListChanged();
}

void QuoteWidget::addSymbolToDB(const QString &symbol) {
    if (m_dbManager && !m_currentUsername.isEmpty()) {
        m_dbManager->saveUserWatchedSymbol(m_currentUsername, symbol);
    }
}

void QuoteWidget::removeSymbolFromDB(const QString &symbol) {
    if (m_dbManager && !m_currentUsername.isEmpty()) {
        m_dbManager->removeUserWatchedSymbol(m_currentUsername, symbol);
    }
}

void QuoteWidget::loadStockListFromDB() {
    if (m_dbManager) {
        m_allStockSymbols = m_dbManager->getAllStockSymbols();
        rebuildSymbolCombo();
    }
}

void QuoteWidget::rebuildSymbolCombo() {
    if (!m_symbolInput) return;

    // 使用来自 DataService 的完整列表（如果可用），否则沿用 m_allStockSymbols
    QStringList symbols = m_dataService->getAllStockSymbols();
    if (symbols.isEmpty())
        symbols = m_allStockSymbols;
    if (symbols.isEmpty()) {
        // 最终回退
        symbols = {
            "000001", "000002", "000333", "000338", "000651", "000858", "000895",
            "600000", "600009", "600016", "600028", "600030", "600031", "600036", "600050",
            "600104", "600276", "600309", "600519", "600585", "600690", "600887", "600900",
            "601006", "601012", "601018", "601088", "601166", "601186", "601229", "601288",
            "601318", "601328", "601398", "601628", "601668", "601688", "601766", "601818",
            "601857", "601888", "601989", "603259", "603288", "603501", "603799", "603986"
        };
    }
    m_allStockSymbols = symbols;

    QString currentText = m_symbolInput->currentText();
    // 提取纯股票代码
    int parenIdx = currentText.indexOf('(');
    QString currentCode = parenIdx > 0 ? currentText.left(parenIdx).trimmed() : currentText.trimmed();

    m_symbolInput->blockSignals(true);
    m_symbolInput->clear();

    QStringList displayItems;
    for (const QString &symbol : symbols) {
        QString name = m_dataService->getStockName(symbol);
        QString displayText = name.isEmpty() ? symbol : (symbol + " (" + name + ")");
        m_symbolInput->addItem(displayText);
        displayItems.append(displayText);
    }

    // 更新 completer 的模型
    QCompleter *c = m_symbolInput->completer();
    if (c) {
        QStringListModel *model = new QStringListModel(displayItems, c);
        c->setModel(model);
    }

    // 恢复之前的文本（如果有）
    if (!currentCode.isEmpty()) {
        m_symbolInput->setEditText(currentText);
    }

    m_symbolInput->blockSignals(false);
}


void QuoteWidget::setupQuoteTable() {
    m_quoteTable->setColumnCount(12);

    QStringList headers = {
        "代码", "名称", "现价", "涨跌", "涨跌幅", "开盘", "最高", "最低",
        "昨收", "成交量", "成交额", "更新时间"
    };
    m_quoteTable->setHorizontalHeaderLabels(headers);

    m_quoteTable->setAlternatingRowColors(true);
    m_quoteTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_quoteTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_quoteTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_quoteTable->verticalHeader()->setVisible(false);
    m_quoteTable->verticalHeader()->setDefaultSectionSize(36);
    m_quoteTable->horizontalHeader()->setFixedHeight(44);
    m_quoteTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_quoteTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void QuoteWidget::setupIndexTable() {
    m_indexTable->setColumnCount(7);

    QStringList headers = {"代码", "指数名称", "当前值", "涨跌", "涨跌幅", "成交量", "更新时间"};
    m_indexTable->setHorizontalHeaderLabels(headers);

    m_indexTable->setAlternatingRowColors(true);
    m_indexTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_indexTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_indexTable->verticalHeader()->setVisible(false);
    m_indexTable->verticalHeader()->setDefaultSectionSize(36);
    m_indexTable->horizontalHeader()->setFixedHeight(44);
    m_indexTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_indexTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void QuoteWidget::setupSectorTable() {
    m_sectorTable->setColumnCount(7);

    QStringList headers = {"板块代码", "板块名称", "涨跌幅", "成交额", "上涨", "下跌", "股票总数"};
    m_sectorTable->setHorizontalHeaderLabels(headers);

    m_sectorTable->setAlternatingRowColors(true);
    m_sectorTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_sectorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sectorTable->verticalHeader()->setVisible(false);
    m_sectorTable->verticalHeader()->setDefaultSectionSize(36);
    m_sectorTable->horizontalHeader()->setFixedHeight(44);
    m_sectorTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_sectorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void QuoteWidget::onAddSymbol() {
    if (m_currentUsername.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录后再添加股票到监控列表");
        return;
    }

    if (!m_symbolInput) {
        return;
    }

    QString displayText = m_symbolInput->currentText().trimmed();
    if (displayText.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入股票代码");
        return;
    }

    // 提取纯股票代码
    QString symbol = displayText;
    int parenthesisIndex = displayText.indexOf('(');
    if (parenthesisIndex > 0) {
        symbol = displayText.left(parenthesisIndex).trimmed();
    }

    if (!m_watchedSymbols.contains(symbol)) {
        m_watchedSymbols.append(symbol);
        m_dataService->subscribeQuote(symbol);
        addSymbolToDB(symbol);
        QLineEdit *lineEdit = m_symbolInput->lineEdit();
        if (lineEdit) {
            lineEdit->clear();
        }
        updateQuoteTable();
        emit symbolListChanged();
    } else {
        QMessageBox::information(this, "提示", "该股票已在监控列表中");
    }
}

void QuoteWidget::onRemoveSymbol() {
    int currentRow = m_quoteTable->currentRow();
    if (currentRow >= 0 && currentRow < m_watchedSymbols.size()) {
        QString symbol = m_watchedSymbols[currentRow];
        m_watchedSymbols.removeAt(currentRow);
        m_dataService->unsubscribeQuote(symbol);
        removeSymbolFromDB(symbol);
        updateQuoteTable();
        emit symbolListChanged();
    } else {
        QMessageBox::warning(this, "警告", "请选择要移除的股票");
    }
}

void QuoteWidget::onQuoteUpdated(const QuoteData &quote) {
    for (int i = 0; i < m_quotes.size(); ++i) {
        if (m_quotes[i].symbol == quote.symbol) {
            m_quotes[i] = quote;
            break;
        }
    }
    // Throttle: coalesce rapid quote updates into a single table refresh
    m_quoteThrottleTimer->start();
}

void QuoteWidget::onQuoteListUpdated(const QVector<QuoteData> &quotes) {
    m_quotes = quotes;
    updateQuoteTable();
}

void QuoteWidget::onIndexDataUpdated(const QVector<IndexData> &indices) {
    m_indices = indices;
    updateIndexTable();
}

void QuoteWidget::onSectorDataUpdated(const QVector<SectorData> &sectors) {
    m_sectors = sectors;
    updateSectorTable();
}

void QuoteWidget::refreshData() {
    // 刷新指数和板块数据
    m_dataService->requestIndexData();
    m_dataService->requestSectorData();
}

void QuoteWidget::updateQuoteTable() {
    const int count = m_watchedSymbols.size();
    m_quoteTable->setUpdatesEnabled(false);
    m_quoteTable->setRowCount(count);

    for (int i = 0; i < count; ++i) {
        const QString &symbol = m_watchedSymbols[i];
        QuoteData quote = m_dataService->getQuote(symbol);
        QColor trendColor = quote.change >= 0 ? QColor("#ef5350") : QColor("#26a69a");
        QString stockName = m_dataService->getStockName(symbol);

        // In-place update: reuse existing items, only create if missing
        auto setItem = [&](int col, const QString &text, QColor color = QColor(), Qt::Alignment align = Qt::AlignCenter) {
            QTableWidgetItem *item = m_quoteTable->item(i, col);
            if (!item) {
                item = new QTableWidgetItem();
                m_quoteTable->setItem(i, col, item);
            }
            item->setText(text);
            item->setTextAlignment(align);
            item->setForeground(color.isValid() ? QBrush(color) : QBrush());
        };

        setItem(0, symbol, QColor(), Qt::AlignCenter);
        setItem(1, stockName.isEmpty() ? symbol : stockName, QColor(), Qt::AlignCenter);
        setItem(2, formatPrice(quote.currentPrice), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(3, formatSigned(quote.change), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(4, formatSignedPercent(quote.changePercent), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(5, formatPrice(quote.openPrice), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(6, formatPrice(quote.highPrice), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(7, formatPrice(quote.lowPrice), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(8, formatPrice(quote.prevClose), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(9, formatVolume(quote.volume), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(10, formatVolume(quote.turnover), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(11, quote.updateTime.toString("hh:mm:ss"), QColor(), Qt::AlignCenter);
    }
    m_quoteTable->setUpdatesEnabled(true);
    m_quoteTable->resizeColumnsToContents();
}

void QuoteWidget::updateIndexTable() {
    const int count = m_indices.size();
    m_indexTable->setUpdatesEnabled(false);
    m_indexTable->setRowCount(count);

    for (int i = 0; i < count; ++i) {
        const IndexData &index = m_indices[i];
        QColor trendColor = index.change >= 0 ? QColor("#ef5350") : QColor("#26a69a");

        auto setItem = [&](int col, const QString &text, QColor color = QColor(), Qt::Alignment align = Qt::AlignCenter) {
            QTableWidgetItem *item = m_indexTable->item(i, col);
            if (!item) {
                item = new QTableWidgetItem();
                m_indexTable->setItem(i, col, item);
            }
            item->setText(text);
            item->setTextAlignment(align);
            item->setForeground(color.isValid() ? QBrush(color) : QBrush());
        };

        setItem(0, index.code, QColor(), Qt::AlignCenter);
        setItem(1, index.name, QColor(), Qt::AlignCenter);
        setItem(2, QString::number(index.currentValue, 'f', 2), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(3, formatSigned(index.change), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(4, formatSignedPercent(index.changePercent), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(5, formatVolume(index.volume), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(6, index.updateTime.toString("hh:mm:ss"), QColor(), Qt::AlignCenter);
    }
    m_indexTable->setUpdatesEnabled(true);
    m_indexTable->resizeColumnsToContents();
}

void QuoteWidget::updateSectorTable() {
    const int count = m_sectors.size();
    m_sectorTable->setUpdatesEnabled(false);
    m_sectorTable->setRowCount(count);

    for (int i = 0; i < count; ++i) {
        const SectorData &sector = m_sectors[i];
        QColor trendColor = sector.changePercent >= 0 ? QColor("#ef5350") : QColor("#26a69a");

        auto setItem = [&](int col, const QString &text, QColor color = QColor(), Qt::Alignment align = Qt::AlignCenter) {
            QTableWidgetItem *item = m_sectorTable->item(i, col);
            if (!item) {
                item = new QTableWidgetItem();
                m_sectorTable->setItem(i, col, item);
            }
            item->setText(text);
            item->setTextAlignment(align);
            item->setForeground(color.isValid() ? QBrush(color) : QBrush());
        };

        setItem(0, sector.code, QColor(), Qt::AlignCenter);
        setItem(1, sector.name, QColor(), Qt::AlignCenter);
        setItem(2, formatSignedPercent(sector.changePercent), trendColor, Qt::AlignRight | Qt::AlignVCenter);
        setItem(3, formatVolume(sector.turnover), QColor(), Qt::AlignRight | Qt::AlignVCenter);
        setItem(4, QString::number(sector.riseCount), QColor("#ef5350"), Qt::AlignRight | Qt::AlignVCenter);
        setItem(5, QString::number(sector.fallCount), QColor("#26a69a"), Qt::AlignRight | Qt::AlignVCenter);
        setItem(6, QString::number(sector.stockCount), QColor(), Qt::AlignRight | Qt::AlignVCenter);
    }
    m_sectorTable->setUpdatesEnabled(true);
    m_sectorTable->resizeColumnsToContents();
}

QString QuoteWidget::formatPrice(double price) const {
    return QString::number(price, 'f', 2);
}

QString QuoteWidget::formatVolume(long long volume) const {
    if (volume >= 100000000) {
        return QString::number(volume / 100000000.0, 'f', 2) + "亿";
    } else if (volume >= 10000) {
        return QString::number(volume / 10000.0, 'f', 2) + "万";
    } else {
        return QString::number(volume);
    }
}

QString QuoteWidget::formatPercent(double percent) const {
    return (percent >= 0 ? "+" : "") + QString::number(percent, 'f', 2) + "%";
}

QString QuoteWidget::formatSigned(double value) const {
    return (value >= 0 ? "+" : "") + QString::number(value, 'f', 2);
}

QString QuoteWidget::formatSignedPercent(double percent) const {
    return (percent >= 0 ? "+" : "") + QString::number(percent, 'f', 2) + "%";
}

QStringList QuoteWidget::getWatchedSymbols() const {
    return m_watchedSymbols.toList();
}

void QuoteWidget::refreshTheme() {
    bool dark = ThemeHelper::isDarkMode();
    QString titleColor = dark ? "#e0e0e0" : "#333";
    ui->indexTitleLabel->setStyleSheet(
        QString("font-weight: bold; font-size: 14px; color: %1; padding: 4px 0;").arg(titleColor));
    ui->sectorTitleLabel->setStyleSheet(
        QString("font-weight: bold; font-size: 14px; color: %1; padding: 4px 0;").arg(titleColor));
}
