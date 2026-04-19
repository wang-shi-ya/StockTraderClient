#pragma once

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QTextEdit>
#include "userinfo.h"
#include "userservice.h"
#include "quotedata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserInfoWidget; }
QT_END_NAMESPACE

class Client; // 前向声明

class UserInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit UserInfoWidget(UserService *userService, Client *client = nullptr, QWidget *parent = nullptr);
	~UserInfoWidget();

    void setCurrentUser(const QString &username);
    void refreshData();

signals:
    void logoutRequested(); // 请求退出登录的信号

private slots:
    void onUserInfoLoaded(const UserInfo &userInfo);
    void onAccountInfoLoaded(const AccountInfo &accountInfo);
    void onCashFlowRecordsLoaded(const QVector<CashFlowRecord> &records);
    void onTradingStatsLoaded(const TradingStats &stats);
    void onRefreshClicked();
    void onEditPersonalInfoClicked(); // 编辑个人信息
    void onAuthButtonClicked();
    void handleLoggedOut(const QString &message);

private:
    void updateAuthButton();
    void clearUserDataViews();
    void setupPersonalInfoTab();
    void setupAssetTab();
    void setupTradingTab();
    void setupRecordsTab();
    void updatePersonalInfo(const UserInfo &userInfo);
    void updateAssetInfo(const AccountInfo &accountInfo);
    void updateTradingStats(const TradingStats &stats);
    void updateCashFlowRecords(const QVector<CashFlowRecord> &records);
    QString formatMoney(double amount) const;
    QString formatPercent(double percent) const;

private:
    Ui::UserInfoWidget *ui;
    UserService *m_userService;
    Client *m_client;
    QString m_currentUser;
    
    // UI组件
    QTabWidget *m_tabWidget;
    QPushButton *m_refreshButton;
    QPushButton *m_editPersonalInfoButton; // 编辑个人信息按钮
    QLabel *m_userNameLabel;
    
    // 个人信息标签页
    QWidget *m_personalTab;
    QTableWidget *m_personalTable;
    
    // 资产信息标签页
    QWidget *m_assetTab;
    QLabel *m_totalAssetsLabel;
    QLabel *m_availableCashLabel;
    QLabel *m_frozenCashLabel;
    QLabel *m_marketValueLabel;
    QLabel *m_profitLossLabel;
    QProgressBar *m_profitLossBar;
    QTableWidget *m_assetTable;
    
    // 交易统计标签页
    QWidget *m_tradingTab;
    QTableWidget *m_tradingStatsTable;
    QLabel *m_winRateLabel;
    QProgressBar *m_winRateBar;
    
    // 资金流水标签页
    QWidget *m_recordsTab;
    QTableWidget *m_cashFlowTable;
};
