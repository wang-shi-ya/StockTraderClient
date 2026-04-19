#include "quotewidget.h"
#include "ui_quotewidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QPalette>

QuoteWidget::QuoteWidget(DataService *dataService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuoteWidget)
    , m_dataService(dataService)
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
    
    // 设置定时器
    connect(m_refreshTimer, &QTimer::timeout, this, &QuoteWidget::refreshData);
    m_refreshTimer->start(5000); // 每5秒刷新一次显示
    
    // 启动实时行情
    m_dataService->startRealTimeQuotes();
}

QuoteWidget::~QuoteWidget() {
    m_dataService->stopRealTimeQuotes();
    delete ui;
}


void QuoteWidget::setupQuoteTable() {
    m_quoteTable->setColumnCount(12);
    
    QStringList headers = {
        "代码", "名称", "现价", "涨跌", "涨跌幅", "开盘", "最高", "最低", 
        "昨收", "成交量", "成交额", "更新时间"
    };
    m_quoteTable->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    m_quoteTable->setAlternatingRowColors(true);
    m_quoteTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_quoteTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_quoteTable->horizontalHeader()->setStretchLastSection(true);
    m_quoteTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    
    // 设置列宽
    m_quoteTable->setColumnWidth(0, 80);  // 代码
    m_quoteTable->setColumnWidth(1, 100); // 名称
    m_quoteTable->setColumnWidth(2, 80);  // 现价
    m_quoteTable->setColumnWidth(3, 80);  // 涨跌
    m_quoteTable->setColumnWidth(4, 80);  // 涨跌幅
}

void QuoteWidget::setupIndexTable() {
    m_indexTable->setColumnCount(7);
    
    QStringList headers = {"指数代码", "指数名称", "当前值", "涨跌", "涨跌幅", "成交量", "更新时间"};
    m_indexTable->setHorizontalHeaderLabels(headers);
    
    m_indexTable->setAlternatingRowColors(true);
    m_indexTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_indexTable->horizontalHeader()->setStretchLastSection(true);
    m_indexTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void QuoteWidget::setupSectorTable() {
    m_sectorTable->setColumnCount(7);
    
    QStringList headers = {"板块代码", "板块名称", "涨跌幅", "成交额", "股票数", "上涨", "下跌"};
    m_sectorTable->setHorizontalHeaderLabels(headers);
    
    m_sectorTable->setAlternatingRowColors(true);
    m_sectorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sectorTable->horizontalHeader()->setStretchLastSection(true);
    m_sectorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void QuoteWidget::onAddSymbol() {
    QString symbol = m_symbolInput->text().trimmed();
    if (symbol.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入股票代码");
        return;
    }
    
    if (!m_watchedSymbols.contains(symbol)) {
        m_watchedSymbols.append(symbol);
        m_dataService->subscribeQuote(symbol);
        m_symbolInput->clear();
        updateQuoteTable();
        emit symbolListChanged(); // 发出信号通知股票列表已改变
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
        updateQuoteTable();
        emit symbolListChanged(); // 发出信号通知股票列表已改变
    } else {
        QMessageBox::warning(this, "警告", "请选择要移除的股票");
    }
}

void QuoteWidget::onQuoteUpdated(const QuoteData &quote) {
    // 更新对应股票的行情数据
    for (int i = 0; i < m_quotes.size(); ++i) {
        if (m_quotes[i].symbol == quote.symbol) {
            m_quotes[i] = quote;
            break;
        }
    }
    updateQuoteTable();
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
    m_quoteTable->setRowCount(m_watchedSymbols.size());
    
    for (int i = 0; i < m_watchedSymbols.size(); ++i) {
        QString symbol = m_watchedSymbols[i];
        QuoteData quote = m_dataService->getQuote(symbol);
        
        m_quoteTable->setItem(i, 0, new QTableWidgetItem(quote.symbol));
        m_quoteTable->setItem(i, 1, new QTableWidgetItem(quote.name.isEmpty() ? symbol : quote.name));
        m_quoteTable->setItem(i, 2, new QTableWidgetItem(formatPrice(quote.currentPrice)));
        m_quoteTable->setItem(i, 3, new QTableWidgetItem(formatPrice(quote.change)));
        m_quoteTable->setItem(i, 4, new QTableWidgetItem(formatPercent(quote.changePercent)));
        m_quoteTable->setItem(i, 5, new QTableWidgetItem(formatPrice(quote.openPrice)));
        m_quoteTable->setItem(i, 6, new QTableWidgetItem(formatPrice(quote.highPrice)));
        m_quoteTable->setItem(i, 7, new QTableWidgetItem(formatPrice(quote.lowPrice)));
        m_quoteTable->setItem(i, 8, new QTableWidgetItem(formatPrice(quote.prevClose)));
        m_quoteTable->setItem(i, 9, new QTableWidgetItem(formatVolume(quote.volume)));
        m_quoteTable->setItem(i, 10, new QTableWidgetItem(formatVolume(quote.turnover)));
        m_quoteTable->setItem(i, 11, new QTableWidgetItem(quote.updateTime.toString("hh:mm:ss")));
        
        // 设置涨跌颜色
        QColor color = quote.change >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
        m_quoteTable->item(i, 2)->setForeground(color);
        m_quoteTable->item(i, 3)->setForeground(color);
        m_quoteTable->item(i, 4)->setForeground(color);
    }
}

void QuoteWidget::updateIndexTable() {
    m_indexTable->setRowCount(m_indices.size());
    
    for (int i = 0; i < m_indices.size(); ++i) {
        const IndexData &index = m_indices[i];
        
        m_indexTable->setItem(i, 0, new QTableWidgetItem(index.code));
        m_indexTable->setItem(i, 1, new QTableWidgetItem(index.name));
        m_indexTable->setItem(i, 2, new QTableWidgetItem(QString::number(index.currentValue, 'f', 2)));
        m_indexTable->setItem(i, 3, new QTableWidgetItem(QString::number(index.change, 'f', 2)));
        m_indexTable->setItem(i, 4, new QTableWidgetItem(formatPercent(index.changePercent)));
        m_indexTable->setItem(i, 5, new QTableWidgetItem(formatVolume(index.volume)));
        m_indexTable->setItem(i, 6, new QTableWidgetItem(index.updateTime.toString("hh:mm:ss")));
        
        // 设置涨跌颜色
        QColor color = index.change >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
        m_indexTable->item(i, 2)->setForeground(color);
        m_indexTable->item(i, 3)->setForeground(color);
        m_indexTable->item(i, 4)->setForeground(color);
    }
}

void QuoteWidget::updateSectorTable() {
    m_sectorTable->setRowCount(m_sectors.size());
    
    for (int i = 0; i < m_sectors.size(); ++i) {
        const SectorData &sector = m_sectors[i];
        
        m_sectorTable->setItem(i, 0, new QTableWidgetItem(sector.code));
        m_sectorTable->setItem(i, 1, new QTableWidgetItem(sector.name));
        m_sectorTable->setItem(i, 2, new QTableWidgetItem(formatPercent(sector.changePercent)));
        m_sectorTable->setItem(i, 3, new QTableWidgetItem(formatVolume(sector.turnover)));
        m_sectorTable->setItem(i, 4, new QTableWidgetItem(QString::number(sector.stockCount)));
        m_sectorTable->setItem(i, 5, new QTableWidgetItem(QString::number(sector.riseCount)));
        m_sectorTable->setItem(i, 6, new QTableWidgetItem(QString::number(sector.fallCount)));
        
        // 设置涨跌颜色
        QColor color = sector.changePercent >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
        m_sectorTable->item(i, 2)->setForeground(color);
    }
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
    return QString::number(percent, 'f', 2) + "%";
}
