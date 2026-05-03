/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *quoteWidget;
    QHBoxLayout *quoteLayout;
    QLabel *labelSymbol;
    QComboBox *comboSymbol;
    QPushButton *btnQuery;
    QSpacerItem *horizontalSpacer;
    QLabel *labelLast;
    QWidget *orderWidget;
    QHBoxLayout *orderLayout;
    QLabel *labelSide;
    QComboBox *comboSide;
    QLabel *labelQty;
    QSpinBox *spinQty;
    QLabel *labelPrice;
    QDoubleSpinBox *spinPrice;
    QPushButton *btnPlace;
    QWidget *ordersWidget;
    QVBoxLayout *ordersLayout;
    QLabel *labelOrdersTitle;
    QWidget *ordersFilterWidget;
    QHBoxLayout *ordersFilterLayout;
    QLabel *labelStatusFilter;
    QComboBox *comboStatusFilter;
    QSpacerItem *ordersFilterSpacer;
    QTableWidget *tableOrders;
    QSpacerItem *ordersVerticalSpacer;
    QWidget *positionsWidget;
    QVBoxLayout *positionsLayout;
    QLabel *labelPositionsTitle;
    QTableWidget *tablePositions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName("verticalLayout");
        quoteWidget = new QWidget(centralwidget);
        quoteWidget->setObjectName("quoteWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(quoteWidget->sizePolicy().hasHeightForWidth());
        quoteWidget->setSizePolicy(sizePolicy);
        quoteWidget->setMaximumSize(QSize(16777215, 50));
        quoteLayout = new QHBoxLayout(quoteWidget);
        quoteLayout->setSpacing(8);
        quoteLayout->setObjectName("quoteLayout");
        quoteLayout->setContentsMargins(0, 0, 0, 0);
        labelSymbol = new QLabel(quoteWidget);
        labelSymbol->setObjectName("labelSymbol");

        quoteLayout->addWidget(labelSymbol);

        comboSymbol = new QComboBox(quoteWidget);
        comboSymbol->setObjectName("comboSymbol");
        comboSymbol->setEditable(false);
        comboSymbol->setMinimumWidth(100);

        quoteLayout->addWidget(comboSymbol);

        btnQuery = new QPushButton(quoteWidget);
        btnQuery->setObjectName("btnQuery");

        quoteLayout->addWidget(btnQuery);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        quoteLayout->addItem(horizontalSpacer);

        labelLast = new QLabel(quoteWidget);
        labelLast->setObjectName("labelLast");

        quoteLayout->addWidget(labelLast);


        verticalLayout->addWidget(quoteWidget);

        orderWidget = new QWidget(centralwidget);
        orderWidget->setObjectName("orderWidget");
        sizePolicy.setHeightForWidth(orderWidget->sizePolicy().hasHeightForWidth());
        orderWidget->setSizePolicy(sizePolicy);
        orderWidget->setMaximumSize(QSize(16777215, 50));
        orderLayout = new QHBoxLayout(orderWidget);
        orderLayout->setSpacing(8);
        orderLayout->setObjectName("orderLayout");
        orderLayout->setContentsMargins(0, 0, 0, 0);
        labelSide = new QLabel(orderWidget);
        labelSide->setObjectName("labelSide");

        orderLayout->addWidget(labelSide);

        comboSide = new QComboBox(orderWidget);
        comboSide->addItem(QString());
        comboSide->addItem(QString());
        comboSide->setObjectName("comboSide");

        orderLayout->addWidget(comboSide);

        labelQty = new QLabel(orderWidget);
        labelQty->setObjectName("labelQty");

        orderLayout->addWidget(labelQty);

        spinQty = new QSpinBox(orderWidget);
        spinQty->setObjectName("spinQty");
        spinQty->setMinimum(1);
        spinQty->setMaximum(1000000);
        spinQty->setValue(100);
        spinQty->setMinimumWidth(80);

        orderLayout->addWidget(spinQty);

        labelPrice = new QLabel(orderWidget);
        labelPrice->setObjectName("labelPrice");

        orderLayout->addWidget(labelPrice);

        spinPrice = new QDoubleSpinBox(orderWidget);
        spinPrice->setObjectName("spinPrice");
        spinPrice->setDecimals(3);
        spinPrice->setMinimum(0.000000000000000);
        spinPrice->setMaximum(1000000.000000000000000);
        spinPrice->setValue(10.000000000000000);
        spinPrice->setMinimumWidth(90);

        orderLayout->addWidget(spinPrice);

        btnPlace = new QPushButton(orderWidget);
        btnPlace->setObjectName("btnPlace");

        orderLayout->addWidget(btnPlace);


        verticalLayout->addWidget(orderWidget);

        ordersWidget = new QWidget(centralwidget);
        ordersWidget->setObjectName("ordersWidget");
        ordersLayout = new QVBoxLayout(ordersWidget);
        ordersLayout->setSpacing(6);
        ordersLayout->setObjectName("ordersLayout");
        ordersLayout->setContentsMargins(0, 0, 0, 0);
        labelOrdersTitle = new QLabel(ordersWidget);
        labelOrdersTitle->setObjectName("labelOrdersTitle");

        ordersLayout->addWidget(labelOrdersTitle);

        ordersFilterWidget = new QWidget(ordersWidget);
        ordersFilterWidget->setObjectName("ordersFilterWidget");
        ordersFilterLayout = new QHBoxLayout(ordersFilterWidget);
        ordersFilterLayout->setSpacing(6);
        ordersFilterLayout->setObjectName("ordersFilterLayout");
        ordersFilterLayout->setContentsMargins(0, 0, 0, 0);
        labelStatusFilter = new QLabel(ordersFilterWidget);
        labelStatusFilter->setObjectName("labelStatusFilter");

        ordersFilterLayout->addWidget(labelStatusFilter);

        comboStatusFilter = new QComboBox(ordersFilterWidget);
        comboStatusFilter->addItem(QString());
        comboStatusFilter->addItem(QString());
        comboStatusFilter->addItem(QString());
        comboStatusFilter->addItem(QString());
        comboStatusFilter->setObjectName("comboStatusFilter");
        comboStatusFilter->setMinimumWidth(90);

        ordersFilterLayout->addWidget(comboStatusFilter);

        ordersFilterSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        ordersFilterLayout->addItem(ordersFilterSpacer);


        ordersLayout->addWidget(ordersFilterWidget);

        tableOrders = new QTableWidget(ordersWidget);
        if (tableOrders->columnCount() < 7)
            tableOrders->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableOrders->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableOrders->setObjectName("tableOrders");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(tableOrders->sizePolicy().hasHeightForWidth());
        tableOrders->setSizePolicy(sizePolicy1);
        tableOrders->horizontalHeader()->setVisible(true);

        ordersLayout->addWidget(tableOrders);

        ordersVerticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        ordersLayout->addItem(ordersVerticalSpacer);


        verticalLayout->addWidget(ordersWidget);

        positionsWidget = new QWidget(centralwidget);
        positionsWidget->setObjectName("positionsWidget");
        positionsLayout = new QVBoxLayout(positionsWidget);
        positionsLayout->setSpacing(6);
        positionsLayout->setObjectName("positionsLayout");
        positionsLayout->setContentsMargins(0, 0, 0, 0);
        labelPositionsTitle = new QLabel(positionsWidget);
        labelPositionsTitle->setObjectName("labelPositionsTitle");

        positionsLayout->addWidget(labelPositionsTitle);

        tablePositions = new QTableWidget(positionsWidget);
        if (tablePositions->columnCount() < 6)
            tablePositions->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(3, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(4, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(5, __qtablewidgetitem12);
        tablePositions->setObjectName("tablePositions");
        tablePositions->horizontalHeader()->setVisible(true);

        positionsLayout->addWidget(tablePositions);


        verticalLayout->addWidget(positionsWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\202\241\347\245\250\344\272\244\346\230\223\345\256\242\346\210\267\347\253\257", nullptr));
        quoteWidget->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white;\n"
"        border-radius: 6px;\n"
"        padding: 10px;", nullptr));
        labelSymbol->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\350\202\241\347\245\250", nullptr));
        btnQuery->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\350\241\214\346\203\205", nullptr));
        labelLast->setText(QCoreApplication::translate("MainWindow", "\346\234\200\346\226\260: -", nullptr));
        labelLast->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px;\n"
"            font-weight: bold;\n"
"            color: #1E88E5;", nullptr));
        orderWidget->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white;\n"
"        border-radius: 6px;\n"
"        padding: 10px;", nullptr));
        labelSide->setText(QCoreApplication::translate("MainWindow", "\346\226\271\345\220\221", nullptr));
        comboSide->setItemText(0, QCoreApplication::translate("MainWindow", "\344\271\260\345\205\245", nullptr));
        comboSide->setItemText(1, QCoreApplication::translate("MainWindow", "\345\215\226\345\207\272", nullptr));

        labelQty->setText(QCoreApplication::translate("MainWindow", "\346\225\260\351\207\217", nullptr));
        labelPrice->setText(QCoreApplication::translate("MainWindow", "\344\273\267\346\240\274", nullptr));
        btnPlace->setText(QCoreApplication::translate("MainWindow", "\344\270\213\345\215\225", nullptr));
        btnPlace->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #4CAF50;\n"
"            color: white;\n"
"            border: none;\n"
"            border-radius: 4px;\n"
"            padding: 8px 24px;\n"
"            font-size: 14px;\n"
"            font-weight: 500;", nullptr));
        ordersWidget->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white;\n"
"        border-radius: 6px;\n"
"        padding: 10px;", nullptr));
        labelOrdersTitle->setText(QCoreApplication::translate("MainWindow", "\344\272\244\346\230\223\350\256\260\345\275\225", nullptr));
        labelOrdersTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 15px; font-weight: bold; color: #333; padding: 4px 0;", nullptr));
        labelStatusFilter->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201\347\255\233\351\200\211", nullptr));
        labelStatusFilter->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 13px; color: #666;", nullptr));
        comboStatusFilter->setItemText(0, QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250", nullptr));
        comboStatusFilter->setItemText(1, QCoreApplication::translate("MainWindow", "New", nullptr));
        comboStatusFilter->setItemText(2, QCoreApplication::translate("MainWindow", "Filled", nullptr));
        comboStatusFilter->setItemText(3, QCoreApplication::translate("MainWindow", "Canceled", nullptr));

        QTableWidgetItem *___qtablewidgetitem = tableOrders->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\350\256\242\345\215\225\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableOrders->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\344\273\243\347\240\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableOrders->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\346\226\271\345\220\221", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableOrders->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "\346\225\260\351\207\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableOrders->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\344\273\267\346\240\274", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableOrders->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableOrders->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "\345\210\233\345\273\272\346\227\266\351\227\264", nullptr));
        positionsWidget->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white;\n"
"        border-radius: 6px;\n"
"        padding: 10px;", nullptr));
        labelPositionsTitle->setText(QCoreApplication::translate("MainWindow", "\346\214\201\344\273\223", nullptr));
        labelPositionsTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 15px; font-weight: bold; color: #333; padding: 4px 0;", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tablePositions->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "\344\273\243\347\240\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tablePositions->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "\346\214\201\344\273\223\346\225\260\351\207\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tablePositions->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "\346\214\201\344\273\223\345\235\207\344\273\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tablePositions->horizontalHeaderItem(3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\270\202\344\273\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tablePositions->horizontalHeaderItem(4);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "\346\265\256\345\212\250\347\233\210\344\272\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tablePositions->horizontalHeaderItem(5);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "\347\233\210\344\272\217\346\257\224\344\276\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
