/********************************************************************************
** Form generated from reading UI file 'quotewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUOTEWIDGET_H
#define UI_QUOTEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QuoteWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *quoteTab;
    QVBoxLayout *quoteLayout;
    QHBoxLayout *controlLayout;
    QLabel *labelSymbol;
    QLineEdit *symbolInput;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *refreshButton;
    QSpacerItem *horizontalSpacer;
    QTableWidget *quoteTable;
    QWidget *indexTab;
    QVBoxLayout *indexLayout;
    QTableWidget *indexTable;
    QWidget *sectorTab;
    QVBoxLayout *sectorLayout;
    QTableWidget *sectorTable;

    void setupUi(QWidget *QuoteWidget)
    {
        if (QuoteWidget->objectName().isEmpty())
            QuoteWidget->setObjectName("QuoteWidget");
        QuoteWidget->resize(800, 600);
        verticalLayout = new QVBoxLayout(QuoteWidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(QuoteWidget);
        tabWidget->setObjectName("tabWidget");
        quoteTab = new QWidget();
        quoteTab->setObjectName("quoteTab");
        quoteLayout = new QVBoxLayout(quoteTab);
        quoteLayout->setObjectName("quoteLayout");
        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName("controlLayout");
        labelSymbol = new QLabel(quoteTab);
        labelSymbol->setObjectName("labelSymbol");

        controlLayout->addWidget(labelSymbol);

        symbolInput = new QLineEdit(quoteTab);
        symbolInput->setObjectName("symbolInput");

        controlLayout->addWidget(symbolInput);

        addButton = new QPushButton(quoteTab);
        addButton->setObjectName("addButton");

        controlLayout->addWidget(addButton);

        removeButton = new QPushButton(quoteTab);
        removeButton->setObjectName("removeButton");

        controlLayout->addWidget(removeButton);

        refreshButton = new QPushButton(quoteTab);
        refreshButton->setObjectName("refreshButton");

        controlLayout->addWidget(refreshButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(horizontalSpacer);


        quoteLayout->addLayout(controlLayout);

        quoteTable = new QTableWidget(quoteTab);
        quoteTable->setObjectName("quoteTable");
        quoteTable->setAlternatingRowColors(true);
        quoteTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        quoteTable->setSelectionMode(QAbstractItemView::SingleSelection);

        quoteLayout->addWidget(quoteTable);

        tabWidget->addTab(quoteTab, QString());
        indexTab = new QWidget();
        indexTab->setObjectName("indexTab");
        indexLayout = new QVBoxLayout(indexTab);
        indexLayout->setObjectName("indexLayout");
        indexTable = new QTableWidget(indexTab);
        indexTable->setObjectName("indexTable");
        indexTable->setAlternatingRowColors(true);
        indexTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        indexLayout->addWidget(indexTable);

        tabWidget->addTab(indexTab, QString());
        sectorTab = new QWidget();
        sectorTab->setObjectName("sectorTab");
        sectorLayout = new QVBoxLayout(sectorTab);
        sectorLayout->setObjectName("sectorLayout");
        sectorTable = new QTableWidget(sectorTab);
        sectorTable->setObjectName("sectorTable");
        sectorTable->setAlternatingRowColors(true);
        sectorTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        sectorLayout->addWidget(sectorTable);

        tabWidget->addTab(sectorTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(QuoteWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QuoteWidget);
    } // setupUi

    void retranslateUi(QWidget *QuoteWidget)
    {
        labelSymbol->setText(QCoreApplication::translate("QuoteWidget", "\350\202\241\347\245\250\344\273\243\347\240\201:", nullptr));
        symbolInput->setPlaceholderText(QCoreApplication::translate("QuoteWidget", "\350\276\223\345\205\245\350\202\241\347\245\250\344\273\243\347\240\201\357\274\214\345\246\202\357\274\232000001", nullptr));
        addButton->setText(QCoreApplication::translate("QuoteWidget", "\346\267\273\345\212\240", nullptr));
        removeButton->setText(QCoreApplication::translate("QuoteWidget", "\347\247\273\351\231\244", nullptr));
        refreshButton->setText(QCoreApplication::translate("QuoteWidget", "\345\210\267\346\226\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(quoteTab), QCoreApplication::translate("QuoteWidget", "\345\256\236\346\227\266\350\241\214\346\203\205", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(indexTab), QCoreApplication::translate("QuoteWidget", "\345\270\202\345\234\272\346\214\207\346\225\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(sectorTab), QCoreApplication::translate("QuoteWidget", "\346\235\277\345\235\227\350\241\214\346\203\205", nullptr));
        (void)QuoteWidget;
    } // retranslateUi

};

namespace Ui {
    class QuoteWidget: public Ui_QuoteWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUOTEWIDGET_H
