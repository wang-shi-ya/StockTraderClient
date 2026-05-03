#include "newswidget.h"
#include "ui_newswidget.h"
#include "themehelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QSplitter>

// 时间字符串转简短显示（"2026-05-03 15:00:00" → "05-03 15:00"）
static QString shortTime(const QString &timeStr) {
    if (timeStr.length() >= 16)
        return timeStr.mid(5, 11);  // "MM-DD HH:mm"
    return timeStr;
}

NewsWidget::NewsWidget(DataService *dataService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewsWidget)
    , m_dataService(dataService)
    , m_currentSymbol("")
    , m_currentPage(1)
    , m_pageSize(20)
    , m_totalCount(0)
    , m_currentCategory("all")
    , m_currentKeyword("")
{
    ui->setupUi(this);

    m_searchEdit = ui->searchEdit;
    m_categoryCombo = ui->categoryCombo;
    m_refreshBtn = ui->refreshBtn;
    m_searchBtn = ui->searchBtn;
    m_newsListWidget = ui->newsListWidget;
    m_detailTextEdit = ui->detailTextEdit;
    m_titleLabel = ui->titleLabel;
    m_sourceLabel = ui->sourceLabel;
    m_timeLabel = ui->timeLabel;

    m_categoryCombo->addItem("全部", "all");
    m_categoryCombo->addItem("宏观", "macro");
    m_categoryCombo->addItem("行业", "industry");
    m_categoryCombo->addItem("个股", "stock");

    setupUI();

    connect(m_dataService, &DataService::newsListReceived, this, &NewsWidget::onNewsListReceived);
    connect(m_dataService, &DataService::stockNewsReceived, this, &NewsWidget::onStockNewsReceived);
    connect(m_dataService, &DataService::newsDetailReceived, this, &NewsWidget::onNewsDetailReceived);
    connect(m_dataService, &DataService::newsRefreshFinished, this, [this]() {
        loadNewsList(m_currentCategory);
    });

    connect(m_categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NewsWidget::onCategoryChanged);
    connect(m_refreshBtn, &QPushButton::clicked, this, &NewsWidget::onRefreshClicked);
    connect(m_searchBtn, &QPushButton::clicked, this, &NewsWidget::onSearchClicked);
    connect(m_newsListWidget, &QListWidget::itemClicked, this, &NewsWidget::onNewsItemClicked);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &NewsWidget::onSearchClicked);

    connect(ThemeHelper::instance(), &ThemeHelper::themeChanged, this, &NewsWidget::refreshTheme);

    loadNewsList();
}

NewsWidget::~NewsWidget() {
    delete ui;
}

void NewsWidget::setupUI() {
    m_newsListWidget->setAlternatingRowColors(true);
    m_newsListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_newsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_newsListWidget->setWordWrap(true);
    m_newsListWidget->setIconSize(QSize(0, 0));
    m_newsListWidget->setSpacing(0);
    m_newsListWidget->setUniformItemSizes(false);  // 允许每条不同高度

    m_detailTextEdit->setReadOnly(true);

    // 分页控件
    m_prevPageBtn = new QPushButton("上一页", this);
    m_nextPageBtn = new QPushButton("下一页", this);
    m_pageInfoLabel = new QLabel("第 1 页", this);
    m_loadingLabel = new QLabel(this);

    m_prevPageBtn->setEnabled(false);
    m_nextPageBtn->setEnabled(false);

    QHBoxLayout *pagLayout = qobject_cast<QHBoxLayout*>(ui->paginationBar->layout());
    if (pagLayout) {
        pagLayout->addWidget(m_prevPageBtn);
        pagLayout->addWidget(m_pageInfoLabel);
        pagLayout->addWidget(m_nextPageBtn);
        pagLayout->addStretch();
        pagLayout->addWidget(m_loadingLabel);
    }

    connect(m_prevPageBtn, &QPushButton::clicked, this, &NewsWidget::onPrevPageClicked);
    connect(m_nextPageBtn, &QPushButton::clicked, this, &NewsWidget::onNextPageClicked);

    ui->splitter->setStretchFactor(0, 38);
    ui->splitter->setStretchFactor(1, 62);
    ui->splitter->setSizes({380, 580});

    clearDetail();
    refreshTheme();
}

void NewsWidget::setSymbol(const QString &symbol) {
    m_currentSymbol = symbol;
    if (!symbol.isEmpty()) {
        setLoading(true);
        m_dataService->requestStockNews(symbol);
    }
}

void NewsWidget::loadNewsList(const QString &category) {
    m_currentCategory = category;
    setLoading(true);
    m_dataService->requestNewsList(category, m_currentPage, m_pageSize, m_currentKeyword);
}

void NewsWidget::onCategoryChanged(int index) {
    QString category = m_categoryCombo->itemData(index).toString();
    m_currentPage = 1;
    m_currentKeyword = "";
    m_searchEdit->clear();
    loadNewsList(category);
}

void NewsWidget::onRefreshClicked() {
    m_currentPage = 1;
    setLoading(true);
    m_dataService->requestNewsRefresh();
}

void NewsWidget::onSearchClicked() {
    QString keyword = m_searchEdit->text().trimmed();
    m_currentKeyword = keyword;
    m_currentPage = 1;
    loadNewsList(m_currentCategory);
}

void NewsWidget::onNewsItemClicked(QListWidgetItem *item) {
    int row = m_newsListWidget->row(item);
    if (row < 0 || row >= m_newsMeta.size()) return;

    QJsonObject meta = m_newsMeta[row].toObject();
    int id = meta["id"].toInt();

    if (meta.contains("content") && !meta["content"].toString().isEmpty()) {
        displayNewsDetail(meta);
    } else {
        setLoading(true);
        m_dataService->requestNewsDetail(id);
    }
}

// ── 构建列表项，设置合理的行高 ──
static void buildListItem(QListWidgetItem *item, const QString &title,
                          const QString &timeStr, const QString &source,
                          const QString &category) {
    QString sub = QString("%1  |  %2  |  %3")
        .arg(shortTime(timeStr), source, category);

    item->setText(title + "\n" + sub);

    // 计算合适的高度：两行文字 + 上下内边距
    QFontMetrics fm(item->font());
    int titleLines = qMax(1, fm.horizontalAdvance(title) / 280 + 1); // 约 280px 一行
    int h = titleLines * fm.height() + fm.height() + 24; // 标题 + 副标题 + padding
    item->setSizeHint(QSize(0, qMax(56, h)));
}

void NewsWidget::onNewsListReceived(const QJsonArray &newsList, int total) {
    setLoading(false);
    m_newsMeta = newsList;
    m_totalCount = total;
    m_newsListWidget->clear();

    if (newsList.isEmpty()) {
        showEmptyState("暂无新闻数据");
        m_prevPageBtn->setEnabled(false);
        m_nextPageBtn->setEnabled(false);
        updatePageInfo();
        return;
    }

    for (const QJsonValue &v : newsList) {
        QJsonObject news = v.toObject();
        QJsonObject meta;
        meta["id"] = news["id"].toInt();
        meta["title"] = news["title"].toString();
        meta["source"] = news["source"].toString();
        meta["time"] = news["time"].toString();
        meta["category"] = news["category"].toString();
        meta["content"] = news["content"].toString();

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, meta);
        buildListItem(item, meta["title"].toString(), meta["time"].toString(),
                      meta["source"].toString(), meta["category"].toString());
        m_newsListWidget->addItem(item);
    }

    m_prevPageBtn->setEnabled(m_currentPage > 1);
    m_nextPageBtn->setEnabled((m_currentPage * m_pageSize) < m_totalCount);
    updatePageInfo();
}

void NewsWidget::onStockNewsReceived(const QString &symbol, const QJsonArray &news) {
    setLoading(false);
    if (symbol != m_currentSymbol) return;

    m_newsMeta = news;
    m_totalCount = news.size();
    m_newsListWidget->clear();

    if (news.isEmpty()) {
        showEmptyState(QString("暂无 %1 相关新闻").arg(symbol));
        return;
    }

    for (const QJsonValue &v : news) {
        QJsonObject newsItem = v.toObject();
        QJsonObject meta;
        meta["id"] = newsItem["id"].toInt();
        meta["title"] = newsItem["title"].toString();
        meta["source"] = newsItem["source"].toString();
        meta["time"] = newsItem["time"].toString();
        meta["category"] = newsItem["category"].toString();
        meta["content"] = newsItem["content"].toString();

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, meta);
        buildListItem(item, meta["title"].toString(), meta["time"].toString(),
                      meta["source"].toString(), meta["category"].toString());
        m_newsListWidget->addItem(item);
    }

    m_prevPageBtn->setEnabled(false);
    m_nextPageBtn->setEnabled(false);
    m_pageInfoLabel->setText(QString("共 %1 条").arg(news.size()));
}

void NewsWidget::onNewsDetailReceived(int id, const QJsonObject &detail) {
    setLoading(false);
    if (detail.isEmpty()) {
        showEmptyState("新闻详情加载失败");
        return;
    }
    displayNewsDetail(detail);

    for (int i = 0; i < m_newsMeta.size(); ++i) {
        QJsonObject obj = m_newsMeta[i].toObject();
        if (obj["id"].toInt() == id) {
            m_newsMeta[i] = detail;
            break;
        }
    }
}

void NewsWidget::displayNewsDetail(const QJsonObject &news) {
    QString title = news["title"].toString();
    QString source = news["source"].toString();
    QString time = news["time"].toString();
    QString content = news["content"].toString();
    QString category = news["category"].toString();

    if (title.isEmpty()) {
        m_titleLabel->setText("加载失败");
        return;
    }

    bool dark = ThemeHelper::isDarkMode();
    QString accentColor = dark ? "#64B5F6" : "#1E88E5";

    ui->detailHeader->setText("新闻详情");
    m_titleLabel->setText(title);
    m_titleLabel->setStyleSheet(
        QString("font-size: 15px; font-weight: bold; color: %1;").arg(accentColor));

    m_sourceLabel->setText(QString("来源: %1").arg(source));
    m_timeLabel->setText(category.isEmpty()
        ? QString("时间: %1").arg(time)
        : QString("时间: %1  |  分类: %2").arg(time, category));

    m_detailTextEdit->setPlainText(content);
}

void NewsWidget::clearDetail() {
    m_titleLabel->setText("请选择一条新闻查看详情");
    m_titleLabel->setStyleSheet("");
    ui->detailHeader->setText("新闻详情");
    m_sourceLabel->setText("");
    m_timeLabel->setText("");
    m_detailTextEdit->clear();
}

void NewsWidget::updatePageInfo() {
    if (m_totalCount > 0)
        m_pageInfoLabel->setText(QString("第 %1 页 / 共 %2 条").arg(m_currentPage).arg(m_totalCount));
    else
        m_pageInfoLabel->setText(QString("第 %1 页").arg(m_currentPage));
}

void NewsWidget::onPrevPageClicked() {
    if (m_currentPage > 1) {
        m_currentPage--;
        loadNewsList(m_currentCategory);
    }
}

void NewsWidget::onNextPageClicked() {
    if ((m_currentPage * m_pageSize) < m_totalCount) {
        m_currentPage++;
        loadNewsList(m_currentCategory);
    }
}

void NewsWidget::refreshTheme() {
    bool dark = ThemeHelper::isDarkMode();

    QString accentColor = dark ? "#64B5F6" : "#1E88E5";
    QString panelBg     = dark ? "#181825" : "#ffffff";
    QString borderColor = dark ? "#2a2a3a" : "#e0e0e0";
    QString textColor   = dark ? "#c8c8d0" : "#333";
    QString mutedColor  = dark ? "#888" : "#999";
    QString listBg      = dark ? "#0f0f1a" : "#fafafa";
    QString detailBg    = dark ? "#1a1a2e" : "#ffffff";
    QString hoverBg     = dark ? "#1e1e35" : "#f0f4ff";
    QString selectBg    = dark ? "#132744" : "#E3F2FD";
    QString subColor    = dark ? "#999" : "#888";
    QString titleColor  = dark ? "#e0e0e0" : "#222";
    QString emptyColor  = dark ? "#777" : "#aaa";

    // 分隔线
    if (ui->divider1)
        ui->divider1->setStyleSheet(QString("background-color: %1;").arg(borderColor));

    // 面板
    ui->listPanel->setStyleSheet(
        QString("QWidget#listPanel { background-color: %1; border-radius: 4px; }").arg(listBg));
    ui->listHeader->setStyleSheet(
        QString("font-size: 13px; font-weight: bold; color: %1; padding: 4px 0;").arg(mutedColor));
    ui->paginationBar->setStyleSheet(
        QString("QWidget#paginationBar { background-color: %1; }").arg(listBg));

    ui->detailPanel->setStyleSheet(
        QString("QWidget#detailPanel { background-color: %1; border-radius: 4px; }").arg(detailBg));
    ui->detailHeader->setStyleSheet(
        QString("font-size: 13px; font-weight: bold; color: %1; padding: 4px 0;").arg(mutedColor));
    ui->detailSeparator->setStyleSheet(
        QString("background-color: %1;").arg(borderColor));

    // 标签
    m_pageInfoLabel->setStyleSheet(QString("color: %1; font-size: 12px;").arg(mutedColor));
    m_loadingLabel->setStyleSheet(
        QString("color: %1; font-size: 12px; font-style: italic;").arg(accentColor));
    m_sourceLabel->setStyleSheet(QString("color: %1; font-size: 12px;").arg(mutedColor));
    m_timeLabel->setStyleSheet(QString("color: %1; font-size: 12px;").arg(mutedColor));
    ui->labelCategory->setStyleSheet(QString("color: %1; font-size: 13px;").arg(textColor));

    // ── 新闻列表（关键：QListWidget::item 设置 color 和 min-height）──
    m_newsListWidget->setStyleSheet(
        QString(
            "QListWidget {"
            "  background-color: %1;"
            "  border: none;"
            "  outline: none;"
            "}"
            "QListWidget::item {"
            "  padding: 12px 14px;"
            "  min-height: 60px;"
            "  border-bottom: 1px solid %2;"
            "  color: %3;"
            "}"
            "QListWidget::item:hover {"
            "  background-color: %4;"
            "}"
            "QListWidget::item:selected {"
            "  background-color: %5;"
            "  color: %6;"
            "}"
        ).arg(listBg, borderColor, titleColor,
              hoverBg, selectBg, accentColor));

    // 详情文本框
    m_detailTextEdit->setStyleSheet(
        QString(
            "QTextEdit {"
            "  background-color: %1;"
            "  color: %2;"
            "  border: none;"
            "  font-size: 14px;"
            "}"
        ).arg(detailBg, textColor));

    // 刷新已有列表项的空状态颜色
    if (m_newsListWidget->count() == 1 && m_newsListWidget->item(0)
        && !(m_newsListWidget->item(0)->flags() & Qt::ItemIsSelectable)) {
        m_newsListWidget->item(0)->setForeground(QColor(emptyColor));
    }
}

void NewsWidget::setLoading(bool loading) {
    if (loading) {
        m_loadingLabel->setText("加载中...");
        m_loadingLabel->show();
    } else {
        m_loadingLabel->hide();
    }
}

void NewsWidget::showEmptyState(const QString &msg) {
    m_newsListWidget->clear();
    QListWidgetItem *item = new QListWidgetItem(msg);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    QFont f = item->font();
    f.setItalic(true);
    item->setFont(f);
    item->setForeground(QColor(ThemeHelper::isDarkMode() ? "#777" : "#aaa"));
    m_newsListWidget->addItem(item);
    clearDetail();
}
