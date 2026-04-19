#include "userinfowidget.h"
#include "ui_userinfowidget.h"
#include "edituserinfodialog.h"
#include "logindialog.h"
#include "client.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QPalette>
#include <QDebug>
#include <QHBoxLayout>
#include <QDialog>

UserInfoWidget::UserInfoWidget(UserService *userService, Client *client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInfoWidget)
    , m_userService(userService)
    , m_client(client)
    , m_currentUser("")
{
    ui->setupUi(this);
    
    // 获取UI组件引用
    m_tabWidget = ui->tabWidget;
    m_refreshButton = ui->refreshButton;
    m_userNameLabel = ui->userNameLabel;
    
    // 创建编辑个人信息按钮
    m_editPersonalInfoButton = new QPushButton(QString::fromUtf8("编辑个人信息"), this);
    m_editPersonalInfoButton->setObjectName("editPersonalInfoButton");
    
    // 将编辑按钮添加到控制布局中
    QHBoxLayout *controlLayout = qobject_cast<QHBoxLayout*>(ui->controlLayout);
    if (controlLayout) {
        controlLayout->insertWidget(1, m_editPersonalInfoButton); // 插入到刷新按钮之前
    }
    
    // 连接按钮信号
    connect(m_editPersonalInfoButton, &QPushButton::clicked, this, &UserInfoWidget::onEditPersonalInfoClicked);
    
    // 登录/注册 与 退出登录 共用同一按钮，文案随登录状态变化
    if (ui->logoutButton) {
        connect(ui->logoutButton, &QPushButton::clicked, this, &UserInfoWidget::onAuthButtonClicked);
    }
    m_personalTab = ui->personalTab;
    m_personalTable = ui->personalTable;
    m_assetTab = ui->assetTab;
    m_totalAssetsLabel = ui->totalAssetsLabel;
    m_availableCashLabel = ui->availableCashLabel;
    m_frozenCashLabel = ui->frozenCashLabel;
    m_marketValueLabel = ui->marketValueLabel;
    m_profitLossLabel = ui->profitLossLabel;
    m_profitLossBar = ui->profitLossBar;
    m_assetTable = ui->assetTable;
    m_tradingTab = ui->tradingTab;
    m_tradingStatsTable = ui->tradingStatsTable;
    m_winRateLabel = ui->winRateLabel;
    m_winRateBar = ui->winRateBar;
    m_recordsTab = ui->recordsTab;
    m_cashFlowTable = ui->cashFlowTable;
    
    // 设置表格
    setupPersonalInfoTab();
    setupAssetTab();
    setupTradingTab();
    setupRecordsTab();
    
    // 连接用户服务信号
    connect(m_userService, &UserService::userInfoLoaded, this, &UserInfoWidget::onUserInfoLoaded);
    connect(m_userService, &UserService::accountInfoLoaded, this, &UserInfoWidget::onAccountInfoLoaded);
    connect(m_userService, &UserService::cashFlowRecordsLoaded, this, &UserInfoWidget::onCashFlowRecordsLoaded);
    connect(m_userService, &UserService::tradingStatsLoaded, this, &UserInfoWidget::onTradingStatsLoaded);
    
    // 连接UI信号
    connect(m_refreshButton, &QPushButton::clicked, this, &UserInfoWidget::onRefreshClicked);
    
    // 连接Client信号（如果Client对象存在）
    if (m_client) {
        connect(m_client, &Client::loggedOut, this, &UserInfoWidget::handleLoggedOut);
        connect(m_client, &Client::loginResult, this, [this](bool ok, const QString &) {
            if (ok && m_client && m_client->isLoggedIn()) {
                const QString u = m_client->currentUser();
                if (!u.isEmpty())
                    setCurrentUser(u);
                else
                    updateAuthButton();
            } else {
                updateAuthButton();
            }
        });
        // 注意：accountFrozen 信号只由 MainWindow 处理，避免重复弹窗
    }

    updateAuthButton();
}

UserInfoWidget::~UserInfoWidget() {
    delete ui;
}

void UserInfoWidget::setCurrentUser(const QString &username) {
    if (username.isEmpty()) {
        if (m_userService)
            m_userService->clearAllCaches();
        m_currentUser.clear();
        m_userNameLabel->setText(QString::fromUtf8("当前用户: 未登录"));
        clearUserDataViews();
    } else {
        m_currentUser = username;
        m_userNameLabel->setText(QString::fromUtf8("当前用户: %1").arg(username));
        refreshData();
    }
    updateAuthButton();
}

void UserInfoWidget::clearUserDataViews() {
    m_personalTable->setRowCount(0);

    m_totalAssetsLabel->setText(QString::fromUtf8("¥0.00"));
    m_availableCashLabel->setText(QString::fromUtf8("¥0.00"));
    m_frozenCashLabel->setText(QString::fromUtf8("¥0.00"));
    m_marketValueLabel->setText(QString::fromUtf8("¥0.00"));
    m_profitLossLabel->setText(QString::fromUtf8("¥0.00 (0.00%)"));
    m_profitLossLabel->setStyleSheet(QString());
    m_profitLossBar->setValue(0);

    m_assetTable->setRowCount(0);

    m_winRateLabel->setText(QString::fromUtf8("0%"));
    m_winRateBar->setValue(0);
    m_tradingStatsTable->setRowCount(0);

    m_cashFlowTable->setRowCount(0);
}

void UserInfoWidget::updateAuthButton() {
    if (!ui->logoutButton)
        return;
    if (m_client && m_client->isLoggedIn()) {
        ui->logoutButton->setText(QString::fromUtf8("退出登录"));
    } else {
        ui->logoutButton->setText(QString::fromUtf8("登录/注册"));
    }
}

void UserInfoWidget::refreshData() {
    if (!m_currentUser.isEmpty()) {
        m_userService->loadAssetOverview(m_currentUser);
    }
}


void UserInfoWidget::setupPersonalInfoTab() {
    m_personalTable->setColumnCount(2);
    m_personalTable->setHorizontalHeaderLabels({"项目", "信息"});
    m_personalTable->setAlternatingRowColors(true);
    m_personalTable->horizontalHeader()->setStretchLastSection(true);
    m_personalTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_personalTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void UserInfoWidget::setupAssetTab() {
    // 设置样式
    m_totalAssetsLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2E8B57;");
    
    // 设置资产明细表格
    m_assetTable->setColumnCount(4);
    m_assetTable->setHorizontalHeaderLabels({"项目", "金额", "占比", "说明"});
    m_assetTable->setAlternatingRowColors(true);
    m_assetTable->horizontalHeader()->setStretchLastSection(true);
    m_assetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_assetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void UserInfoWidget::setupTradingTab() {
    // 设置样式
    m_winRateLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    
    // 交易统计表格
    m_tradingStatsTable->setColumnCount(2);
    m_tradingStatsTable->setHorizontalHeaderLabels({"统计项目", "数值"});
    m_tradingStatsTable->setAlternatingRowColors(true);
    m_tradingStatsTable->horizontalHeader()->setStretchLastSection(true);
    m_tradingStatsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tradingStatsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void UserInfoWidget::setupRecordsTab() {
    m_cashFlowTable->setColumnCount(6);
    m_cashFlowTable->setHorizontalHeaderLabels({"时间", "类型", "金额", "余额", "描述", "状态"});
    m_cashFlowTable->setAlternatingRowColors(true);
    m_cashFlowTable->horizontalHeader()->setStretchLastSection(true);
    m_cashFlowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_cashFlowTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void UserInfoWidget::onUserInfoLoaded(const UserInfo &userInfo) {
    if (m_currentUser.isEmpty())
        return;
    // 如果用户名匹配，或者userInfo的username为空但userInfo是通过loadUserInfo加载的，都更新
    if (userInfo.username == m_currentUser || 
        (userInfo.username.isEmpty() && !m_currentUser.isEmpty())) {
        qDebug() << "User info loaded signal received for:" << userInfo.username << "current user:" << m_currentUser;
        updatePersonalInfo(userInfo);
    } else {
        qDebug() << "User info username mismatch:" << userInfo.username << "vs" << m_currentUser;
    }
}

void UserInfoWidget::onAccountInfoLoaded(const AccountInfo &accountInfo) {
    if (m_currentUser.isEmpty())
        return;
    if (accountInfo.username == m_currentUser) {
        updateAssetInfo(accountInfo);
    }
}

void UserInfoWidget::onCashFlowRecordsLoaded(const QVector<CashFlowRecord> &records) {
    if (m_currentUser.isEmpty()) {
        m_cashFlowTable->setRowCount(0);
        return;
    }
    updateCashFlowRecords(records);
}

void UserInfoWidget::onTradingStatsLoaded(const TradingStats &stats) {
    if (m_currentUser.isEmpty())
        return;
    if (stats.username == m_currentUser) {
        updateTradingStats(stats);
    }
}

void UserInfoWidget::onRefreshClicked() {
    refreshData();
}

void UserInfoWidget::onEditPersonalInfoClicked() {
    if (m_currentUser.isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("错误"), QString::fromUtf8("未登录，无法编辑个人信息"));
        return;
    }
    
    // 获取当前用户信息
    UserInfo currentInfo = m_userService->getUserInfo(m_currentUser);
    if (currentInfo.username.isEmpty()) {
        // 如果内存中没有，尝试从数据库加载
        m_userService->loadUserInfo(m_currentUser);
        currentInfo = m_userService->getUserInfo(m_currentUser);
    }
    
    // 确保用户名正确设置
    currentInfo.username = m_currentUser;
    
    // 创建编辑对话框
    EditUserInfoDialog dialog(currentInfo, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取编辑后的用户信息
        UserInfo editedInfo = dialog.getUserInfo();
        
        // 保存到数据库
        bool success = m_userService->saveUserInfoToDB(editedInfo);
        if (success) {
            QMessageBox::information(this, QString::fromUtf8("成功"), QString::fromUtf8("个人信息已保存"));
            // 刷新数据
            refreshData();
        } else {
            QMessageBox::warning(this, QString::fromUtf8("失败"), QString::fromUtf8("保存个人信息失败，请重试"));
        }
    }
}

void UserInfoWidget::updatePersonalInfo(const UserInfo &userInfo) {
    m_personalTable->setRowCount(9);
    
    QStringList labels = {
        "账号", "密码", "姓名", "邮箱", "电话", 
        "身份证号", "地址", "注册时间", "最后登录"
    };
    
    // 密码用小黑点遮盖
    QString passwordDisplay = "●●●●●●";
    
    // 确保用户名不为空，如果为空则使用当前用户
    QString displayUsername = userInfo.username.isEmpty() ? m_currentUser : userInfo.username;
    
    QStringList values = {
        displayUsername.isEmpty() ? "未知" : displayUsername,
        passwordDisplay,
        userInfo.realName.isEmpty() ? "未设置" : userInfo.realName,
        userInfo.email.isEmpty() ? "未设置" : userInfo.email,
        userInfo.phone.isEmpty() ? "未设置" : userInfo.phone,
        userInfo.idCard.isEmpty() ? "未设置" : userInfo.idCard,
        userInfo.address.isEmpty() ? "未设置" : userInfo.address,
        userInfo.registerTime.isValid() ? userInfo.registerTime.toString("yyyy-MM-dd hh:mm:ss") : "未知",
        userInfo.lastLoginTime.isValid() ? userInfo.lastLoginTime.toString("yyyy-MM-dd hh:mm:ss") : "未知"
    };
    
    for (int i = 0; i < labels.size(); ++i) {
        m_personalTable->setItem(i, 0, new QTableWidgetItem(labels[i]));
        QTableWidgetItem *valueItem = new QTableWidgetItem(values[i]);
        // 如果是密码字段，设置为不可编辑
        if (labels[i] == "密码") {
            valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
        }
        m_personalTable->setItem(i, 1, valueItem);
    }
    
    qDebug() << "Updated personal info for user:" << displayUsername;
}

void UserInfoWidget::updateAssetInfo(const AccountInfo &accountInfo) {
    // 更新概览标签
    m_totalAssetsLabel->setText(formatMoney(accountInfo.totalAssets));
    m_availableCashLabel->setText(formatMoney(accountInfo.availableCash));
    m_frozenCashLabel->setText(formatMoney(accountInfo.frozenCash));
    m_marketValueLabel->setText(formatMoney(accountInfo.marketValue));
    m_profitLossLabel->setText(QString("%1 (%2)")
                              .arg(formatMoney(accountInfo.profitLoss))
                              .arg(formatPercent(accountInfo.profitLossPercent)));
    
    // 设置盈亏颜色
    QColor profitColor = accountInfo.profitLoss >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
    m_profitLossLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(profitColor.name()));
    
    // 更新盈亏进度条
    int progressValue = qBound(-100, static_cast<int>(accountInfo.profitLossPercent), 100);
    m_profitLossBar->setValue(progressValue);
    
    // 更新资产明细表格
    m_assetTable->setRowCount(4);
    
    QStringList items = {"可用资金", "冻结资金", "持仓市值", "总资产"};
    QVector<double> amounts = {
        accountInfo.availableCash,
        accountInfo.frozenCash,
        accountInfo.marketValue,
        accountInfo.totalAssets
    };
    QStringList descriptions = {
        "可用于交易的资金",
        "被冻结的资金",
        "当前持仓的市值",
        "账户总资产"
    };
    
    for (int i = 0; i < items.size(); ++i) {
        m_assetTable->setItem(i, 0, new QTableWidgetItem(items[i]));
        m_assetTable->setItem(i, 1, new QTableWidgetItem(formatMoney(amounts[i])));
        
        double percentage = accountInfo.totalAssets > 0 ? (amounts[i] / accountInfo.totalAssets * 100) : 0;
        m_assetTable->setItem(i, 2, new QTableWidgetItem(formatPercent(percentage)));
        m_assetTable->setItem(i, 3, new QTableWidgetItem(descriptions[i]));
    }
}

void UserInfoWidget::updateTradingStats(const TradingStats &stats) {
    // 更新胜率显示
    m_winRateLabel->setText(formatPercent(stats.winRate));
    m_winRateBar->setValue(static_cast<int>(stats.winRate));
    
    // 更新统计表格
    m_tradingStatsTable->setRowCount(8);
    
    QStringList labels = {
        "总交易次数", "买入次数", "卖出次数", "总交易量",
        "总交易金额", "总盈亏", "胜率", "最后交易时间"
    };
    
    QStringList values = {
        QString::number(stats.totalTrades),
        QString::number(stats.buyTrades),
        QString::number(stats.sellTrades),
        QString::number(stats.totalVolume),
        formatMoney(stats.totalAmount),
        formatMoney(stats.totalProfit),
        formatPercent(stats.winRate),
        stats.lastTradeTime.toString("yyyy-MM-dd hh:mm:ss")
    };
    
    for (int i = 0; i < labels.size(); ++i) {
        m_tradingStatsTable->setItem(i, 0, new QTableWidgetItem(labels[i]));
        m_tradingStatsTable->setItem(i, 1, new QTableWidgetItem(values[i]));
        
        // 设置盈亏颜色
        if (labels[i] == "总盈亏") {
            QColor color = stats.totalProfit >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
            m_tradingStatsTable->item(i, 1)->setForeground(color);
        }
    }
}

void UserInfoWidget::updateCashFlowRecords(const QVector<CashFlowRecord> &records) {
    m_cashFlowTable->setRowCount(records.size());
    
    for (int i = 0; i < records.size(); ++i) {
        const CashFlowRecord &record = records[i];
        
        m_cashFlowTable->setItem(i, 0, new QTableWidgetItem(record.createTime.toString("MM-dd hh:mm")));
        m_cashFlowTable->setItem(i, 1, new QTableWidgetItem(record.type));
        m_cashFlowTable->setItem(i, 2, new QTableWidgetItem(formatMoney(record.amount)));
        m_cashFlowTable->setItem(i, 3, new QTableWidgetItem(formatMoney(record.balance)));
        m_cashFlowTable->setItem(i, 4, new QTableWidgetItem(record.description));
        m_cashFlowTable->setItem(i, 5, new QTableWidgetItem(record.status));
        
        // 设置金额颜色
        QColor color = record.amount >= 0 ? QColor(255, 0, 0) : QColor(0, 255, 0);
        m_cashFlowTable->item(i, 2)->setForeground(color);
    }
}

QString UserInfoWidget::formatMoney(double amount) const {
    return QString("¥%1").arg(amount, 0, 'f', 2);
}

QString UserInfoWidget::formatPercent(double percent) const {
    return QString("%1%").arg(percent, 0, 'f', 2);
}

void UserInfoWidget::onAuthButtonClicked() {
    if (!m_client) {
        QMessageBox::warning(this, QString::fromUtf8("提示"), QString::fromUtf8("未连接客户端"));
        return;
    }
    if (m_client->isLoggedIn()) {
        const QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            QString::fromUtf8("退出登录"),
            QString::fromUtf8("确定要退出登录吗？"),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            m_client->logout();
    } else {
        LoginDialog dlg(m_client, this);
        dlg.exec(); // 登录成功时 Client::loginResult 已刷新用户与按钮文案
    }
}

void UserInfoWidget::handleLoggedOut(const QString & /*message*/) {
	// 界面与缓存清空由 MainWindow::handleLoggedOut 调用 setCurrentUser("") 统一处理
	emit logoutRequested();
}

// handleAccountFrozen 已移除，由 MainWindow 统一处理账号冻结逻辑
