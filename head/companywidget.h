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
#include "quotedata.h"
#include "dataservice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CompanyWidget; }
QT_END_NAMESPACE

class CompanyWidget : public QWidget {
    Q_OBJECT

public:
    explicit CompanyWidget(DataService *dataService, QWidget *parent = nullptr);
    ~CompanyWidget();

    void setSymbol(const QString &symbol);

private slots:
    void onSymbolChanged();
    void onCompanyInfoReceived(const CompanyInfo &info);
    void onFinancialDataReceived(const QString &symbol, const QMap<QString, QVariant> &data);
    void onCompanyListUpdated(const QStringList &symbols);

private:
    void setupUI();
    void setupBasicInfoTab();
    void setupFinancialTab();
    void setupAnnouncementTab();
    void updateBasicInfo(const CompanyInfo &info);
    void updateFinancialData(const QMap<QString, QVariant> &data);
    void requestData();
    void loadCompanyListFromDatabase();

private:
    Ui::CompanyWidget *ui;
    DataService *m_dataService;
    QString m_currentSymbol;
    CompanyInfo m_companyInfo;
    QMap<QString, QVariant> m_financialData;
    
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
};
