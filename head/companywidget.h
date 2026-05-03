#pragma once

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QScrollArea>
#include "quotedata.h"
#include "dataservice.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class CompanyWidget; }
QT_END_NAMESPACE

class CompanyWidget : public QWidget {
    Q_OBJECT

public:
    explicit CompanyWidget(DataService *dataService, QWidget *parent = nullptr);
    ~CompanyWidget();

    void setSymbol(const QString &symbol);
    void refreshTheme();

private slots:
    void onSymbolChanged();
    void onTabChanged(int index);
    void onCompanyInfoReceived(const CompanyInfo &info);
    void onFinancialDataReceived(const QString &symbol, const QMap<QString, QVariant> &data);
    void onCompanyAnnouncementsReceived(const QString &symbol, const QJsonArray &announcements);
    void onCompanyListUpdated(const QStringList &symbols);

private:
    void setupUI();
    void setupBasicInfoTab();
    void setupFinancialTab();
    void setupAnnouncementTab();
    void updateBasicInfo(const CompanyInfo &info);
    void updateFinancialData(const QMap<QString, QVariant> &data);
    void requestData(int tabIndex = -1);
    void forceRefreshCurrentTab();
    void setLoading(bool loading);
    void loadCompanyListFromDatabase();
    void fetchAnnouncementImage(int imageId, QLabel *targetLabel);

private:
    Ui::CompanyWidget *ui;
    DataService *m_dataService;
    QString m_currentSymbol;
    CompanyInfo m_companyInfo;
    QMap<QString, QVariant> m_financialData;
    
    // 标签页加载状态
    bool m_basicInfoLoaded;
    bool m_financialDataLoaded;
    bool m_announcementsLoaded;

    // UI组件
    QComboBox *m_symbolCombo;
    QPushButton *m_refreshButton;
    QTabWidget *m_tabWidget;

    // 基本信息标签页
    QWidget *m_basicInfoTab;
    QTableWidget *m_basicInfoTable;

    // 财务数据标签页
    QWidget *m_financialTab;
    QTableWidget *m_financialTable;

    // 公告信息标签页
    QWidget *m_announcementTab;
    QTextEdit *m_announcementText;
    QScrollArea *m_announcementScroll;
    QWidget *m_announcementContainer;
    QVBoxLayout *m_announcementContainerLayout;

    QLabel *m_loadingLabel;
    QNetworkAccessManager *m_networkManager;
};
