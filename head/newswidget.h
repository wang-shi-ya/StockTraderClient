#pragma once

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include "dataservice.h"
#include <QJsonArray>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class NewsWidget; }
QT_END_NAMESPACE

class NewsWidget : public QWidget {
    Q_OBJECT

public:
    explicit NewsWidget(DataService *dataService, QWidget *parent = nullptr);
    ~NewsWidget();

    void setSymbol(const QString &symbol);
    void refreshTheme();

private slots:
    void onCategoryChanged(int index);
    void onRefreshClicked();
    void onSearchClicked();
    void onNewsItemClicked(QListWidgetItem *item);
    void onNewsListReceived(const QJsonArray &newsList, int total);
    void onStockNewsReceived(const QString &symbol, const QJsonArray &news);
    void onNewsDetailReceived(int id, const QJsonObject &detail);
    void onPrevPageClicked();
    void onNextPageClicked();

private:
    void setupUI();
    void loadNewsList(const QString &category = "all");
    void displayNewsDetail(const QJsonObject &news);
    void clearDetail();
    void updatePageInfo();
    void setLoading(bool loading);
    void showEmptyState(const QString &msg);

    Ui::NewsWidget *ui;
    DataService *m_dataService;
    QString m_currentSymbol;
    QJsonArray m_newsMeta;        // id/title/source/time only — no content
    int m_currentPage;
    int m_pageSize;
    int m_totalCount;
    QString m_currentCategory;
    QString m_currentKeyword;

    QLineEdit *m_searchEdit;
    QComboBox *m_categoryCombo;
    QPushButton *m_refreshBtn;
    QPushButton *m_searchBtn;
    QListWidget *m_newsListWidget;
    QTextEdit *m_detailTextEdit;
    QLabel *m_titleLabel;
    QLabel *m_sourceLabel;
    QLabel *m_timeLabel;
    QLabel *m_pageInfoLabel;
    QPushButton *m_prevPageBtn;
    QPushButton *m_nextPageBtn;
    QLabel *m_loadingLabel;
};
