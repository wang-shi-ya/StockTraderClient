#include "companywidget.h"
#include "ui_companywidget.h"
#include <QHeaderView>
#include <QDebug>

CompanyWidget::CompanyWidget(DataService *dataService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompanyWidget)
    , m_dataService(dataService)
    , m_currentSymbol("")
{
    ui->setupUi(this);
    
    // 获取UI组件引用
    m_symbolCombo = ui->symbolCombo;
    m_refreshButton = ui->refreshButton;
    m_tabWidget = ui->tabWidget;
    m_basicInfoTab = ui->basicInfoTab;
    m_basicInfoTable = ui->basicInfoTable;
    m_financialTab = ui->financialTab;
    m_financialTable = ui->financialTable;
    m_announcementTab = ui->announcementTab;
    m_announcementText = ui->announcementText;
    
    // 设置表格
    setupBasicInfoTab();
    setupFinancialTab();
    setupAnnouncementTab();
    
    // 连接数据服务信号
    connect(m_dataService, &DataService::companyInfoReceived, this, &CompanyWidget::onCompanyInfoReceived);
    connect(m_dataService, &DataService::financialDataReceived, this, &CompanyWidget::onFinancialDataReceived);
    connect(m_dataService, &DataService::companyListUpdated, this, &CompanyWidget::onCompanyListUpdated);
    
    // 连接UI信号
    connect(m_symbolCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, &CompanyWidget::onSymbolChanged);
    connect(m_refreshButton, &QPushButton::clicked, this, &CompanyWidget::requestData);
    connect(ui->reloadButton, &QPushButton::clicked, this, &CompanyWidget::loadCompanyListFromDatabase);
    
    // 从数据库加载公司列表
    loadCompanyListFromDatabase();
}

CompanyWidget::~CompanyWidget() {
    delete ui;
}

void CompanyWidget::setSymbol(const QString &symbol) {
    m_currentSymbol = symbol;
    m_symbolCombo->setCurrentText(symbol);
    requestData();
}


void CompanyWidget::setupBasicInfoTab() {
    m_basicInfoTable->setColumnCount(2);
    m_basicInfoTable->setHorizontalHeaderLabels({"项目", "数值"});
    m_basicInfoTable->setAlternatingRowColors(true);
    m_basicInfoTable->horizontalHeader()->setStretchLastSection(true);
    m_basicInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_basicInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CompanyWidget::setupFinancialTab() {
    m_financialTable->setColumnCount(2);
    m_financialTable->setHorizontalHeaderLabels({"财务指标", "数值"});
    m_financialTable->setAlternatingRowColors(true);
    m_financialTable->horizontalHeader()->setStretchLastSection(true);
    m_financialTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_financialTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CompanyWidget::setupAnnouncementTab() {
    m_announcementText->setReadOnly(true);
    m_announcementText->setPlainText("暂无公告信息");
}

void CompanyWidget::onSymbolChanged() {
    m_currentSymbol = m_symbolCombo->currentText();
    requestData();
}

void CompanyWidget::onCompanyInfoReceived(const CompanyInfo &info) {
    if (info.symbol == m_currentSymbol) {
        m_companyInfo = info;
        updateBasicInfo(info);
    }
}

void CompanyWidget::onFinancialDataReceived(const QString &symbol, const QMap<QString, QVariant> &data) {
    if (symbol == m_currentSymbol) {
        m_financialData = data;
        updateFinancialData(data);
    }
}

void CompanyWidget::updateBasicInfo(const CompanyInfo &info) {
    m_basicInfoTable->setRowCount(10);
    
    QStringList labels = {
        "股票代码", "公司名称", "所属行业", "所属板块", 
        "总股本", "流通股本", "总市值", "流通市值", 
        "市盈率", "市净率"
    };
    
    QStringList values = {
        info.symbol,
        info.name,
        info.industry,
        info.sector,
        QString::number(info.totalShares / 10000, 'f', 2) + "万股",
        QString::number(info.circulatingShares / 10000, 'f', 2) + "万股",
        QString::number(info.marketCap / 100000000, 'f', 2) + "亿元",
        QString::number(info.circulatingCap / 100000000, 'f', 2) + "亿元",
        QString::number(info.pe, 'f', 2),
        QString::number(info.pb, 'f', 2)
    };
    
    for (int i = 0; i < labels.size(); ++i) {
        m_basicInfoTable->setItem(i, 0, new QTableWidgetItem(labels[i]));
        m_basicInfoTable->setItem(i, 1, new QTableWidgetItem(values[i]));
    }
}

void CompanyWidget::updateFinancialData(const QMap<QString, QVariant> &data) {
    m_financialTable->setRowCount(data.size());
    
    QStringList labels = {
        "营业收入", "净利润", "总资产", "净资产", 
        "净资产收益率", "市盈率", "市净率"
    };
    
    QStringList keys = {
        "revenue", "netProfit", "totalAssets", "netAssets",
        "roe", "pe", "pb"
    };
    
    int row = 0;
    for (int i = 0; i < labels.size() && i < keys.size(); ++i) {
        if (data.contains(keys[i])) {
            m_financialTable->setItem(row, 0, new QTableWidgetItem(labels[i]));
            
            QVariant value = data[keys[i]];
            QString valueStr;
            if (keys[i] == "roe" || keys[i] == "pe" || keys[i] == "pb") {
                valueStr = QString::number(value.toDouble(), 'f', 2);
                if (keys[i] == "roe") valueStr += "%";
            } else {
                valueStr = QString::number(value.toDouble() / 10000, 'f', 2) + "万元";
            }
            
            m_financialTable->setItem(row, 1, new QTableWidgetItem(valueStr));
            row++;
        }
    }
}

void CompanyWidget::requestData() {
    if (!m_currentSymbol.isEmpty()) {
        m_dataService->requestCompanyInfo(m_currentSymbol);
        m_dataService->requestFinancialData(m_currentSymbol);
    }
}

void CompanyWidget::loadCompanyListFromDatabase() {
    m_dataService->loadCompanyList();
}

void CompanyWidget::onCompanyListUpdated(const QStringList &symbols) {
    QString currentSelection = m_symbolCombo->currentText();
    
    m_symbolCombo->clear();
    
    if (symbols.isEmpty()) {
        m_symbolCombo->addItem("数据库中没有公司信息");
        m_symbolCombo->setEnabled(false);
        m_currentSymbol = "";
    } else {
        m_symbolCombo->setEnabled(true);
        m_symbolCombo->addItems(symbols);
        
        // 尝试恢复之前的选择
        if (!currentSelection.isEmpty()) {
            int index = m_symbolCombo->findText(currentSelection);
            if (index >= 0) {
                m_symbolCombo->setCurrentIndex(index);
                m_currentSymbol = currentSelection;
            } else {
                // 选择第一个
                m_symbolCombo->setCurrentIndex(0);
                m_currentSymbol = symbols.first();
                requestData();
            }
        } else {
            // 首次加载，选择第一个
            if (!symbols.isEmpty()) {
                m_symbolCombo->setCurrentIndex(0);
                m_currentSymbol = symbols.first();
                requestData();
            }
        }
    }
    
    qDebug() << "Company list updated, count:" << symbols.size();
}
