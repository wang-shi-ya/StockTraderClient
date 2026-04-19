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
    QHBoxLayout *quoteLayout;
    QLabel *labelSymbol;
    QComboBox *comboSymbol;
    QPushButton *btnQuery;
    QLabel *labelLast;
    QHBoxLayout *orderLayout;
    QLabel *labelSide;
    QComboBox *comboSide;
    QLabel *labelQty;
    QSpinBox *spinQty;
    QLabel *labelPrice;
    QDoubleSpinBox *spinPrice;
    QPushButton *btnPlace;
    QTableWidget *tableOrders;
    QTableWidget *tablePositions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        quoteLayout = new QHBoxLayout();
        quoteLayout->setObjectName("quoteLayout");
        labelSymbol = new QLabel(centralwidget);
        labelSymbol->setObjectName("labelSymbol");

        quoteLayout->addWidget(labelSymbol);

        comboSymbol = new QComboBox(centralwidget);
        comboSymbol->setObjectName("comboSymbol");
        comboSymbol->setEditable(false);

        quoteLayout->addWidget(comboSymbol);

        btnQuery = new QPushButton(centralwidget);
        btnQuery->setObjectName("btnQuery");

        quoteLayout->addWidget(btnQuery);

        labelLast = new QLabel(centralwidget);
        labelLast->setObjectName("labelLast");

        quoteLayout->addWidget(labelLast);


        verticalLayout->addLayout(quoteLayout);

        orderLayout = new QHBoxLayout();
        orderLayout->setObjectName("orderLayout");
        labelSide = new QLabel(centralwidget);
        labelSide->setObjectName("labelSide");

        orderLayout->addWidget(labelSide);

        comboSide = new QComboBox(centralwidget);
        comboSide->addItem(QString());
        comboSide->addItem(QString());
        comboSide->setObjectName("comboSide");

        orderLayout->addWidget(comboSide);

        labelQty = new QLabel(centralwidget);
        labelQty->setObjectName("labelQty");

        orderLayout->addWidget(labelQty);

        spinQty = new QSpinBox(centralwidget);
        spinQty->setObjectName("spinQty");
        spinQty->setMinimum(1);
        spinQty->setMaximum(1000000);
        spinQty->setValue(100);

        orderLayout->addWidget(spinQty);

        labelPrice = new QLabel(centralwidget);
        labelPrice->setObjectName("labelPrice");

        orderLayout->addWidget(labelPrice);

        spinPrice = new QDoubleSpinBox(centralwidget);
        spinPrice->setObjectName("spinPrice");
        spinPrice->setDecimals(3);
        spinPrice->setMinimum(0.000000000000000);
        spinPrice->setMaximum(1000000.000000000000000);
        spinPrice->setValue(10.000000000000000);

        orderLayout->addWidget(spinPrice);

        btnPlace = new QPushButton(centralwidget);
        btnPlace->setObjectName("btnPlace");

        orderLayout->addWidget(btnPlace);


        verticalLayout->addLayout(orderLayout);

        tableOrders = new QTableWidget(centralwidget);
        if (tableOrders->columnCount() < 6)
            tableOrders->setColumnCount(6);
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
        tableOrders->setObjectName("tableOrders");

        verticalLayout->addWidget(tableOrders);

        tablePositions = new QTableWidget(centralwidget);
        if (tablePositions->columnCount() < 3)
            tablePositions->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tablePositions->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        tablePositions->setObjectName("tablePositions");

        verticalLayout->addWidget(tablePositions);

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
        labelSymbol->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\350\202\241\347\245\250", nullptr));
        btnQuery->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\350\241\214\346\203\205", nullptr));
        labelLast->setText(QCoreApplication::translate("MainWindow", "\346\234\200\346\226\260: -", nullptr));
        labelSide->setText(QCoreApplication::translate("MainWindow", "\346\226\271\345\220\221", nullptr));
        comboSide->setItemText(0, QCoreApplication::translate("MainWindow", "\344\271\260\345\205\245", nullptr));
        comboSide->setItemText(1, QCoreApplication::translate("MainWindow", "\345\215\226\345\207\272", nullptr));

        labelQty->setText(QCoreApplication::translate("MainWindow", "\346\225\260\351\207\217", nullptr));
        labelPrice->setText(QCoreApplication::translate("MainWindow", "\344\273\267\346\240\274", nullptr));
        btnPlace->setText(QCoreApplication::translate("MainWindow", "\344\270\213\345\215\225", nullptr));
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
        QTableWidgetItem *___qtablewidgetitem6 = tablePositions->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "\344\273\243\347\240\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tablePositions->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "\346\214\201\344\273\223\346\225\260\351\207\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tablePositions->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "\346\214\201\344\273\223\345\235\207\344\273\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
