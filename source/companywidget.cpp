#include "companywidget.h"
#include "ui_companywidget.h"
#include "themehelper.h"
#include <QHeaderView>
#include <QDebug>
#include <QCompleter>

CompanyWidget::CompanyWidget(DataService *dataService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompanyWidget)
    , m_dataService(dataService)
    , m_currentSymbol("")
    , m_basicInfoLoaded(false)
    , m_financialDataLoaded(false)
    , m_announcementsLoaded(false)
    , m_networkManager(new QNetworkAccessManager(this))
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

    // 加载状态标签
    m_loadingLabel = new QLabel(this);
    m_loadingLabel->setStyleSheet(
        QString("color: %1; font-size: 12px; margin-left: 8px;")
            .arg(ThemeHelper::isDarkMode() ? "#64B5F6" : "#1a73e8"));
    m_loadingLabel->hide();
    ui->controlLayout->addWidget(m_loadingLabel);
    
    // 连接信号槽
    connect(m_dataService, &DataService::companyInfoReceived, this, &CompanyWidget::onCompanyInfoReceived);
    connect(m_dataService, &DataService::financialDataReceived, this, &CompanyWidget::onFinancialDataReceived);
    connect(m_dataService, &DataService::companyAnnouncementsReceived, this, &CompanyWidget::onCompanyAnnouncementsReceived);
    connect(m_dataService, &DataService::companyListUpdated, this, &CompanyWidget::onCompanyListUpdated);
    
    // 连接UI信号
    connect(m_symbolCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, &CompanyWidget::onSymbolChanged);
    connect(m_refreshButton, &QPushButton::clicked, this, &CompanyWidget::forceRefreshCurrentTab);
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &CompanyWidget::onTabChanged);
    
    // 从数据库加载公司列表
    loadCompanyListFromDatabase();

    connect(ThemeHelper::instance(), &ThemeHelper::themeChanged, this, &CompanyWidget::refreshTheme);
}

CompanyWidget::~CompanyWidget() {
    delete ui;
}

void CompanyWidget::setSymbol(const QString &symbol) {
    m_currentSymbol = symbol;
    
    // 重置加载状态
    m_basicInfoLoaded = false;
    m_financialDataLoaded = false;
    m_announcementsLoaded = false;
    
    // 找到对应的显示文本（包含公司名称）
    bool found = false;
    for (int i = 0; i < m_symbolCombo->count(); ++i) {
        QString itemText = m_symbolCombo->itemText(i);
        if (itemText.startsWith(symbol + " ") || itemText == symbol) {
            m_symbolCombo->setCurrentIndex(i);
            found = true;
            break;
        }
    }
    
    if (!found) {
        m_symbolCombo->setCurrentText(symbol);
    }
    
    // 只加载当前显示的标签页数据
    requestData(m_tabWidget->currentIndex());
}


void CompanyWidget::setupBasicInfoTab() {
    m_basicInfoTable->setColumnCount(2);
    m_basicInfoTable->setHorizontalHeaderLabels({"项目", "数值"});
    m_basicInfoTable->setAlternatingRowColors(true);
    m_basicInfoTable->horizontalHeader()->setStretchLastSection(true);
    m_basicInfoTable->verticalHeader()->setVisible(false);
    m_basicInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_basicInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CompanyWidget::setupFinancialTab() {
    m_financialTable->setColumnCount(2);
    m_financialTable->setHorizontalHeaderLabels({"财务指标", "数值"});
    m_financialTable->setAlternatingRowColors(true);
    m_financialTable->horizontalHeader()->setStretchLastSection(true);
    m_financialTable->verticalHeader()->setVisible(false);
    m_financialTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_financialTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CompanyWidget::setupAnnouncementTab() {
    // 将公告页固定为可滚动容器，避免反复删除/重建 layout 和控件
    m_announcementScroll = new QScrollArea(m_announcementTab);
    m_announcementScroll->setWidgetResizable(true);
    m_announcementScroll->setFrameShape(QFrame::NoFrame);
    m_announcementScroll->setObjectName("announcementScroll");

    m_announcementContainer = new QWidget();
    m_announcementContainer->setObjectName("announcementContainer");
    m_announcementContainerLayout = new QVBoxLayout(m_announcementContainer);
    m_announcementContainerLayout->setSpacing(10);
    m_announcementContainerLayout->setContentsMargins(10, 10, 10, 10);

    // 先从原布局中移除，再重新挂到容器上（避免清理旧布局时被 deleteLater 误删）
    if (QLayout *oldLayout = m_announcementTab->layout())
        oldLayout->removeWidget(m_announcementText);
    m_announcementText->setParent(m_announcementContainer);
    m_announcementText->setReadOnly(true);
    m_announcementText->setPlainText("暂无公告信息");
    m_announcementText->show();
    m_announcementContainerLayout->addWidget(m_announcementText);
    m_announcementContainerLayout->addStretch();

    m_announcementScroll->setWidget(m_announcementContainer);

    // 用滚动区域替换 announcementTab 原有内容
    if (QLayout *layout = m_announcementTab->layout()) {
        QLayoutItem *item = nullptr;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        layout->addWidget(m_announcementScroll);
    } else {
        auto *newLayout = new QVBoxLayout(m_announcementTab);
        newLayout->addWidget(m_announcementScroll);
        m_announcementTab->setLayout(newLayout);
    }
}

void CompanyWidget::onSymbolChanged() {
    // 提取纯股票代码
    QString displayText = m_symbolCombo->currentText();
    QString symbol = displayText;
    int parenthesisIndex = displayText.indexOf('(');
    if (parenthesisIndex > 0) {
        symbol = displayText.left(parenthesisIndex).trimmed();
    }
    
    m_currentSymbol = symbol;
    
    // 重置加载状态
    m_basicInfoLoaded = false;
    m_financialDataLoaded = false;
    m_announcementsLoaded = false;
    
    // 只加载当前显示的标签页数据
    requestData(m_tabWidget->currentIndex());
}

void CompanyWidget::onTabChanged(int index) {
    // 切换标签页时重置对应标志，确保加载最新数据
    switch (index) {
        case 0: m_basicInfoLoaded = false; break;
        case 1: m_financialDataLoaded = false; break;
        case 2: m_announcementsLoaded = false; break;
    }
    setLoading(true);
    requestData(index);
}

void CompanyWidget::onCompanyInfoReceived(const CompanyInfo &info) {
    if (info.symbol == m_currentSymbol) {
        m_companyInfo = info;
        if (info.name.isEmpty()) {
            // 请求失败
            m_basicInfoTable->setRowCount(1);
            m_basicInfoTable->setItem(0, 0, new QTableWidgetItem(QString::fromUtf8("提示")));
            m_basicInfoTable->setItem(0, 1, new QTableWidgetItem(QString::fromUtf8("数据加载失败，请检查网络后刷新重试")));
        } else {
            updateBasicInfo(info);
        }
        setLoading(false);
    }
}

void CompanyWidget::onFinancialDataReceived(const QString &symbol, const QMap<QString, QVariant> &data) {
    if (symbol == m_currentSymbol) {
        m_financialData = data;
        if (data.isEmpty()) {
            m_financialTable->setRowCount(1);
            m_financialTable->setItem(0, 0, new QTableWidgetItem(QString::fromUtf8("提示")));
            m_financialTable->setItem(0, 1, new QTableWidgetItem(QString::fromUtf8("数据加载失败，请检查网络后刷新重试")));
        } else {
            updateFinancialData(data);
        }
        setLoading(false);
    }
}

void CompanyWidget::updateBasicInfo(const CompanyInfo &info) {
    m_basicInfoTable->setUpdatesEnabled(false);
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
    m_basicInfoTable->setUpdatesEnabled(true);
}

void CompanyWidget::updateFinancialData(const QMap<QString, QVariant> &data) {
    m_financialTable->setUpdatesEnabled(false);
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
    m_financialTable->setUpdatesEnabled(true);
}

void CompanyWidget::forceRefreshCurrentTab() {
    // 重置所有标签页的加载状态，强制重新获取数据
    m_basicInfoLoaded = false;
    m_financialDataLoaded = false;
    m_announcementsLoaded = false;
    setLoading(true);
    requestData(m_tabWidget->currentIndex());
}

void CompanyWidget::setLoading(bool loading) {
    if (loading) {
        m_loadingLabel->setText(QString::fromUtf8("加载中..."));
        m_loadingLabel->show();
    } else {
        m_loadingLabel->hide();
    }
}

void CompanyWidget::requestData(int tabIndex) {
    if (m_currentSymbol.isEmpty()) {
        return;
    }
    
    // 如果 tabIndex 为 -1，加载当前显示的标签页
    if (tabIndex == -1) {
        tabIndex = m_tabWidget->currentIndex();
    }
    
    switch (tabIndex) {
        case 0: // 基本信息标签页
            if (!m_basicInfoLoaded) {
                m_dataService->requestCompanyInfo(m_currentSymbol);
                m_basicInfoLoaded = true;
            }
            break;
        case 1: // 财务数据标签页
            if (!m_financialDataLoaded) {
                m_dataService->requestFinancialData(m_currentSymbol);
                m_financialDataLoaded = true;
            }
            break;
        case 2: // 公告信息标签页
            if (!m_announcementsLoaded) {
                m_dataService->requestCompanyAnnouncements(m_currentSymbol);
                m_announcementsLoaded = true;
            }
            break;
    }
}

void CompanyWidget::onCompanyAnnouncementsReceived(const QString &symbol, const QJsonArray &announcements) {
    if (symbol != m_currentSymbol) return;

    m_announcementsLoaded = true;
    setLoading(false);

    if (!m_announcementContainerLayout) {
        setupAnnouncementTab();
    }

    // 清理容器但保留 m_announcementText
    auto clearContainer = [this]() {
        if (!m_announcementContainerLayout) return;
        QLayoutItem *item = nullptr;
        while ((item = m_announcementContainerLayout->takeAt(0)) != nullptr) {
            QWidget *w = item->widget();
            if (w && w == m_announcementText) {
                delete item;
                continue;
            }
            if (w) w->deleteLater();
            delete item;
        }
    };

    if (announcements.isEmpty()) {
        clearContainer();
        m_announcementText->setPlainText("暂无公告信息");
        m_announcementText->show();
        m_announcementContainerLayout->addWidget(m_announcementText);
        m_announcementContainerLayout->addStretch();
        return;
    }

    clearContainer();
    m_announcementText->hide();

    for (const QJsonValue &v : announcements) {
        QJsonObject a = v.toObject();

        // 提取字段
        QString companyName = a["companyName"].toString();
        QString title = a["title"].toString();
        QString content = a["content"].toString();
        QString publishDate = a["publishDate"].toString();
        QJsonArray images = a["images"].toArray();

        // 创建公告卡片（背景/边框由全局主题 QSS 通过 objectName 控制）
        QWidget *card = new QWidget();
        card->setObjectName("announcementCard");
        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(6);
        cardLayout->setContentsMargins(12, 10, 12, 10);

        // 标题（颜色由全局 QSS 自动适配主题）
        QLabel *titleLabel = new QLabel(QString::fromUtf8("【%1】%2").arg(companyName, title));
        QFont titleFont = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);
        titleLabel->setWordWrap(true);
        cardLayout->addWidget(titleLabel);

        // 日期
        QLabel *dateLabel = new QLabel(publishDate.left(10));
        QFont dateFont = dateLabel->font();
        dateFont.setPointSize(9);
        dateLabel->setFont(dateFont);
        cardLayout->addWidget(dateLabel);

        // 正文
        QLabel *contentLabel = new QLabel(content);
        contentLabel->setWordWrap(true);
        cardLayout->addWidget(contentLabel);

        // 图片
        if (!images.isEmpty()) {
            for (const QJsonValue &imgVal : images) {
                QJsonObject img = imgVal.toObject();
                int imageId = img["id"].toInt();

                QLabel *imageLabel = new QLabel();
                imageLabel->setAlignment(Qt::AlignCenter);
                imageLabel->setMaximumHeight(300);
                imageLabel->setScaledContents(false);
                imageLabel->setText(QString::fromUtf8("加载中..."));
                imageLabel->setStyleSheet("color: #aaa; font-size: 12px; border: 1px dashed #ddd; border-radius: 4px;");
                cardLayout->addWidget(imageLabel);

                // 异步加载图片
                fetchAnnouncementImage(imageId, imageLabel);
            }
        }

        m_announcementContainerLayout->addWidget(card);
    }

    m_announcementContainerLayout->addStretch();
}

void CompanyWidget::fetchAnnouncementImage(int imageId, QLabel *targetLabel) {
    QUrl url{QString("http://localhost:3000/api/company/announcements/%1/image").arg(imageId)};
    QNetworkRequest req{url};
    QNetworkReply *reply = m_networkManager->get(req);

    connect(reply, &QNetworkReply::finished, this, [targetLabel, reply]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            targetLabel->setText(QString::fromUtf8("图片加载失败"));
            targetLabel->setStyleSheet("color: #c00; font-size: 12px;");
            return;
        }
        QByteArray data = reply->readAll();
        QPixmap pixmap;
        if (pixmap.loadFromData(data)) {
            QSize maxSize(400, 300);
            QPixmap scaled = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            targetLabel->setPixmap(scaled);
            targetLabel->setStyleSheet("");
        } else {
            targetLabel->setText(QString::fromUtf8("图片格式不支持"));
            targetLabel->setStyleSheet("color: #c00; font-size: 12px;");
        }
    });
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
        m_symbolCombo->setEditable(false);
        m_currentSymbol = "";
    } else {
        m_symbolCombo->setEnabled(true);
        m_symbolCombo->setEditable(true);
        
        // 构建股票列表
        QStringList stockList;
        for (const QString &symbol : symbols) {
            QString name = m_dataService->getStockName(symbol);
            if (!name.isEmpty()) {
                QString displayText = symbol + " (" + name + ")";
                m_symbolCombo->addItem(displayText);
                stockList.append(displayText);
            } else {
                m_symbolCombo->addItem(symbol);
                stockList.append(symbol);
            }
        }
        
        // 添加QCompleter实现模糊搜索
        QCompleter *completer = new QCompleter(stockList, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains); // 模糊匹配
        m_symbolCombo->setCompleter(completer);
        
        // 尝试恢复之前的选择
        if (!currentSelection.isEmpty()) {
            // 提取纯股票代码
            QString pureSymbol = currentSelection;
            int parenthesisIndex = currentSelection.indexOf('(');
            if (parenthesisIndex > 0) {
                pureSymbol = currentSelection.left(parenthesisIndex).trimmed();
            }
            
            // 查找对应的显示文本
            int index = -1;
            for (int i = 0; i < m_symbolCombo->count(); ++i) {
                QString itemText = m_symbolCombo->itemText(i);
                if (itemText.startsWith(pureSymbol + " ") || itemText == pureSymbol) {
                    index = i;
                    break;
                }
            }
            
            if (index >= 0) {
                m_symbolCombo->setCurrentIndex(index);
                m_currentSymbol = pureSymbol;
                // 只加载当前显示的标签页数据
                requestData(m_tabWidget->currentIndex());
            } else {
                // 选择第一个
                m_symbolCombo->setCurrentIndex(0);
                // 提取第一个项目的纯股票代码
                QString firstItem = m_symbolCombo->itemText(0);
                int firstParenthesisIndex = firstItem.indexOf('(');
                if (firstParenthesisIndex > 0) {
                    m_currentSymbol = firstItem.left(firstParenthesisIndex).trimmed();
                } else {
                    m_currentSymbol = firstItem;
                }
                // 只加载当前显示的标签页数据
                requestData(m_tabWidget->currentIndex());
            }
        } else {
            // 首次加载，选择第一个
            if (!symbols.isEmpty()) {
                m_symbolCombo->setCurrentIndex(0);
                // 提取第一个项目的纯股票代码
                QString firstItem = m_symbolCombo->itemText(0);
                int firstParenthesisIndex = firstItem.indexOf('(');
                if (firstParenthesisIndex > 0) {
                    m_currentSymbol = firstItem.left(firstParenthesisIndex).trimmed();
                } else {
                    m_currentSymbol = firstItem;
                }
                // 只加载当前显示的标签页数据
                requestData(m_tabWidget->currentIndex());
            }
        }
    }
    
    qDebug() << "Company list updated, count:" << symbols.size();
}

void CompanyWidget::refreshTheme() {
    bool dark = ThemeHelper::isDarkMode();
    m_loadingLabel->setStyleSheet(
        QString("color: %1; font-size: 12px; margin-left: 8px;")
            .arg(dark ? "#64B5F6" : "#1a73e8"));
}

