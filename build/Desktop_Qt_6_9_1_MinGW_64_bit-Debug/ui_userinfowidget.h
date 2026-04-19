/********************************************************************************
** Form generated from reading UI file 'userinfowidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINFOWIDGET_H
#define UI_USERINFOWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserInfoWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *controlLayout;
    QLabel *userNameLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *refreshButton;
    QPushButton *logoutButton;
    QTabWidget *tabWidget;
    QWidget *personalTab;
    QVBoxLayout *personalLayout;
    QTableWidget *personalTable;
    QWidget *assetTab;
    QVBoxLayout *assetLayout;
    QGroupBox *overviewGroup;
    QGridLayout *overviewLayout;
    QLabel *labelTotalAssets;
    QLabel *totalAssetsLabel;
    QLabel *labelAvailableCash;
    QLabel *availableCashLabel;
    QLabel *labelFrozenCash;
    QLabel *frozenCashLabel;
    QLabel *labelMarketValue;
    QLabel *marketValueLabel;
    QLabel *labelProfitLoss;
    QLabel *profitLossLabel;
    QProgressBar *profitLossBar;
    QTableWidget *assetTable;
    QWidget *tradingTab;
    QVBoxLayout *tradingLayout;
    QGroupBox *statsGroup;
    QGridLayout *statsLayout;
    QLabel *labelWinRate;
    QLabel *winRateLabel;
    QProgressBar *winRateBar;
    QTableWidget *tradingStatsTable;
    QWidget *recordsTab;
    QVBoxLayout *recordsLayout;
    QTableWidget *cashFlowTable;

    void setupUi(QWidget *UserInfoWidget)
    {
        if (UserInfoWidget->objectName().isEmpty())
            UserInfoWidget->setObjectName("UserInfoWidget");
        UserInfoWidget->resize(800, 600);
        verticalLayout = new QVBoxLayout(UserInfoWidget);
        verticalLayout->setObjectName("verticalLayout");
        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName("controlLayout");
        userNameLabel = new QLabel(UserInfoWidget);
        userNameLabel->setObjectName("userNameLabel");

        controlLayout->addWidget(userNameLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(horizontalSpacer);

        refreshButton = new QPushButton(UserInfoWidget);
        refreshButton->setObjectName("refreshButton");

        controlLayout->addWidget(refreshButton);

        logoutButton = new QPushButton(UserInfoWidget);
        logoutButton->setObjectName("logoutButton");

        controlLayout->addWidget(logoutButton);


        verticalLayout->addLayout(controlLayout);

        tabWidget = new QTabWidget(UserInfoWidget);
        tabWidget->setObjectName("tabWidget");
        personalTab = new QWidget();
        personalTab->setObjectName("personalTab");
        personalLayout = new QVBoxLayout(personalTab);
        personalLayout->setObjectName("personalLayout");
        personalTable = new QTableWidget(personalTab);
        personalTable->setObjectName("personalTable");
        personalTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        personalTable->setAlternatingRowColors(true);
        personalTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        personalLayout->addWidget(personalTable);

        tabWidget->addTab(personalTab, QString());
        assetTab = new QWidget();
        assetTab->setObjectName("assetTab");
        assetLayout = new QVBoxLayout(assetTab);
        assetLayout->setObjectName("assetLayout");
        overviewGroup = new QGroupBox(assetTab);
        overviewGroup->setObjectName("overviewGroup");
        overviewLayout = new QGridLayout(overviewGroup);
        overviewLayout->setObjectName("overviewLayout");
        labelTotalAssets = new QLabel(overviewGroup);
        labelTotalAssets->setObjectName("labelTotalAssets");

        overviewLayout->addWidget(labelTotalAssets, 0, 0, 1, 1);

        totalAssetsLabel = new QLabel(overviewGroup);
        totalAssetsLabel->setObjectName("totalAssetsLabel");

        overviewLayout->addWidget(totalAssetsLabel, 0, 1, 1, 1);

        labelAvailableCash = new QLabel(overviewGroup);
        labelAvailableCash->setObjectName("labelAvailableCash");

        overviewLayout->addWidget(labelAvailableCash, 1, 0, 1, 1);

        availableCashLabel = new QLabel(overviewGroup);
        availableCashLabel->setObjectName("availableCashLabel");

        overviewLayout->addWidget(availableCashLabel, 1, 1, 1, 1);

        labelFrozenCash = new QLabel(overviewGroup);
        labelFrozenCash->setObjectName("labelFrozenCash");

        overviewLayout->addWidget(labelFrozenCash, 2, 0, 1, 1);

        frozenCashLabel = new QLabel(overviewGroup);
        frozenCashLabel->setObjectName("frozenCashLabel");

        overviewLayout->addWidget(frozenCashLabel, 2, 1, 1, 1);

        labelMarketValue = new QLabel(overviewGroup);
        labelMarketValue->setObjectName("labelMarketValue");

        overviewLayout->addWidget(labelMarketValue, 3, 0, 1, 1);

        marketValueLabel = new QLabel(overviewGroup);
        marketValueLabel->setObjectName("marketValueLabel");

        overviewLayout->addWidget(marketValueLabel, 3, 1, 1, 1);

        labelProfitLoss = new QLabel(overviewGroup);
        labelProfitLoss->setObjectName("labelProfitLoss");

        overviewLayout->addWidget(labelProfitLoss, 4, 0, 1, 1);

        profitLossLabel = new QLabel(overviewGroup);
        profitLossLabel->setObjectName("profitLossLabel");

        overviewLayout->addWidget(profitLossLabel, 4, 1, 1, 1);

        profitLossBar = new QProgressBar(overviewGroup);
        profitLossBar->setObjectName("profitLossBar");
        profitLossBar->setMinimum(-100);
        profitLossBar->setMaximum(100);
        profitLossBar->setValue(0);

        overviewLayout->addWidget(profitLossBar, 5, 0, 1, 2);


        assetLayout->addWidget(overviewGroup);

        assetTable = new QTableWidget(assetTab);
        assetTable->setObjectName("assetTable");
        assetTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        assetTable->setAlternatingRowColors(true);
        assetTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        assetLayout->addWidget(assetTable);

        tabWidget->addTab(assetTab, QString());
        tradingTab = new QWidget();
        tradingTab->setObjectName("tradingTab");
        tradingLayout = new QVBoxLayout(tradingTab);
        tradingLayout->setObjectName("tradingLayout");
        statsGroup = new QGroupBox(tradingTab);
        statsGroup->setObjectName("statsGroup");
        statsLayout = new QGridLayout(statsGroup);
        statsLayout->setObjectName("statsLayout");
        labelWinRate = new QLabel(statsGroup);
        labelWinRate->setObjectName("labelWinRate");

        statsLayout->addWidget(labelWinRate, 0, 0, 1, 1);

        winRateLabel = new QLabel(statsGroup);
        winRateLabel->setObjectName("winRateLabel");

        statsLayout->addWidget(winRateLabel, 0, 1, 1, 1);

        winRateBar = new QProgressBar(statsGroup);
        winRateBar->setObjectName("winRateBar");
        winRateBar->setMaximum(100);
        winRateBar->setValue(0);

        statsLayout->addWidget(winRateBar, 1, 0, 1, 2);


        tradingLayout->addWidget(statsGroup);

        tradingStatsTable = new QTableWidget(tradingTab);
        tradingStatsTable->setObjectName("tradingStatsTable");
        tradingStatsTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tradingStatsTable->setAlternatingRowColors(true);
        tradingStatsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        tradingLayout->addWidget(tradingStatsTable);

        tabWidget->addTab(tradingTab, QString());
        recordsTab = new QWidget();
        recordsTab->setObjectName("recordsTab");
        recordsLayout = new QVBoxLayout(recordsTab);
        recordsLayout->setObjectName("recordsLayout");
        cashFlowTable = new QTableWidget(recordsTab);
        cashFlowTable->setObjectName("cashFlowTable");
        cashFlowTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        cashFlowTable->setAlternatingRowColors(true);
        cashFlowTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        recordsLayout->addWidget(cashFlowTable);

        tabWidget->addTab(recordsTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(UserInfoWidget);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(UserInfoWidget);
    } // setupUi

    void retranslateUi(QWidget *UserInfoWidget)
    {
        userNameLabel->setText(QCoreApplication::translate("UserInfoWidget", "\345\275\223\345\211\215\347\224\250\346\210\267: \346\234\252\347\231\273\345\275\225", nullptr));
        refreshButton->setText(QCoreApplication::translate("UserInfoWidget", "\345\210\267\346\226\260\346\225\260\346\215\256", nullptr));
        logoutButton->setText(QCoreApplication::translate("UserInfoWidget", "\347\231\273\345\275\225/\346\263\250\345\206\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(personalTab), QCoreApplication::translate("UserInfoWidget", "\344\270\252\344\272\272\344\277\241\346\201\257", nullptr));
        overviewGroup->setTitle(QCoreApplication::translate("UserInfoWidget", "\350\265\204\344\272\247\346\246\202\350\247\210", nullptr));
        labelTotalAssets->setText(QCoreApplication::translate("UserInfoWidget", "\346\200\273\350\265\204\344\272\247:", nullptr));
        totalAssetsLabel->setText(QCoreApplication::translate("UserInfoWidget", "\302\2450.00", nullptr));
        labelAvailableCash->setText(QCoreApplication::translate("UserInfoWidget", "\345\217\257\347\224\250\350\265\204\351\207\221:", nullptr));
        availableCashLabel->setText(QCoreApplication::translate("UserInfoWidget", "\302\2450.00", nullptr));
        labelFrozenCash->setText(QCoreApplication::translate("UserInfoWidget", "\345\206\273\347\273\223\350\265\204\351\207\221:", nullptr));
        frozenCashLabel->setText(QCoreApplication::translate("UserInfoWidget", "\302\2450.00", nullptr));
        labelMarketValue->setText(QCoreApplication::translate("UserInfoWidget", "\346\214\201\344\273\223\345\270\202\345\200\274:", nullptr));
        marketValueLabel->setText(QCoreApplication::translate("UserInfoWidget", "\302\2450.00", nullptr));
        labelProfitLoss->setText(QCoreApplication::translate("UserInfoWidget", "\346\265\256\345\212\250\347\233\210\344\272\217:", nullptr));
        profitLossLabel->setText(QCoreApplication::translate("UserInfoWidget", "\302\2450.00 (0.00%)", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(assetTab), QCoreApplication::translate("UserInfoWidget", "\350\265\204\344\272\247\344\277\241\346\201\257", nullptr));
        statsGroup->setTitle(QCoreApplication::translate("UserInfoWidget", "\344\272\244\346\230\223\347\273\237\350\256\241", nullptr));
        labelWinRate->setText(QCoreApplication::translate("UserInfoWidget", "\350\203\234\347\216\207:", nullptr));
        winRateLabel->setText(QCoreApplication::translate("UserInfoWidget", "0%", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tradingTab), QCoreApplication::translate("UserInfoWidget", "\344\272\244\346\230\223\347\273\237\350\256\241", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(recordsTab), QCoreApplication::translate("UserInfoWidget", "\350\265\204\351\207\221\346\265\201\346\260\264", nullptr));
        (void)UserInfoWidget;
    } // retranslateUi

};

namespace Ui {
    class UserInfoWidget: public Ui_UserInfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFOWIDGET_H
