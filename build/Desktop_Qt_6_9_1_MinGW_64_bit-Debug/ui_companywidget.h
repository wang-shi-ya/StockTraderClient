/********************************************************************************
** Form generated from reading UI file 'companywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPANYWIDGET_H
#define UI_COMPANYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CompanyWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *controlLayout;
    QLabel *labelSymbol;
    QComboBox *symbolCombo;
    QPushButton *refreshButton;
    QPushButton *reloadButton;
    QSpacerItem *horizontalSpacer;
    QTabWidget *tabWidget;
    QWidget *basicInfoTab;
    QVBoxLayout *basicInfoLayout;
    QTableWidget *basicInfoTable;
    QWidget *financialTab;
    QVBoxLayout *financialLayout;
    QTableWidget *financialTable;
    QWidget *announcementTab;
    QVBoxLayout *announcementLayout;
    QTextEdit *announcementText;

    void setupUi(QWidget *CompanyWidget)
    {
        if (CompanyWidget->objectName().isEmpty())
            CompanyWidget->setObjectName("CompanyWidget");
        CompanyWidget->resize(800, 600);
        verticalLayout = new QVBoxLayout(CompanyWidget);
        verticalLayout->setObjectName("verticalLayout");
        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName("controlLayout");
        labelSymbol = new QLabel(CompanyWidget);
        labelSymbol->setObjectName("labelSymbol");

        controlLayout->addWidget(labelSymbol);

        symbolCombo = new QComboBox(CompanyWidget);
        symbolCombo->setObjectName("symbolCombo");

        controlLayout->addWidget(symbolCombo);

        refreshButton = new QPushButton(CompanyWidget);
        refreshButton->setObjectName("refreshButton");

        controlLayout->addWidget(refreshButton);

        reloadButton = new QPushButton(CompanyWidget);
        reloadButton->setObjectName("reloadButton");

        controlLayout->addWidget(reloadButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(controlLayout);

        tabWidget = new QTabWidget(CompanyWidget);
        tabWidget->setObjectName("tabWidget");
        basicInfoTab = new QWidget();
        basicInfoTab->setObjectName("basicInfoTab");
        basicInfoLayout = new QVBoxLayout(basicInfoTab);
        basicInfoLayout->setObjectName("basicInfoLayout");
        basicInfoTable = new QTableWidget(basicInfoTab);
        basicInfoTable->setObjectName("basicInfoTable");
        basicInfoTable->setAlternatingRowColors(true);
        basicInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        basicInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        basicInfoLayout->addWidget(basicInfoTable);

        tabWidget->addTab(basicInfoTab, QString());
        financialTab = new QWidget();
        financialTab->setObjectName("financialTab");
        financialLayout = new QVBoxLayout(financialTab);
        financialLayout->setObjectName("financialLayout");
        financialTable = new QTableWidget(financialTab);
        financialTable->setObjectName("financialTable");
        financialTable->setAlternatingRowColors(true);
        financialTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        financialTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        financialLayout->addWidget(financialTable);

        tabWidget->addTab(financialTab, QString());
        announcementTab = new QWidget();
        announcementTab->setObjectName("announcementTab");
        announcementLayout = new QVBoxLayout(announcementTab);
        announcementLayout->setObjectName("announcementLayout");
        announcementText = new QTextEdit(announcementTab);
        announcementText->setObjectName("announcementText");
        announcementText->setReadOnly(true);

        announcementLayout->addWidget(announcementText);

        tabWidget->addTab(announcementTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(CompanyWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CompanyWidget);
    } // setupUi

    void retranslateUi(QWidget *CompanyWidget)
    {
        labelSymbol->setText(QCoreApplication::translate("CompanyWidget", "\350\202\241\347\245\250\344\273\243\347\240\201:", nullptr));
        refreshButton->setText(QCoreApplication::translate("CompanyWidget", "\345\210\267\346\226\260", nullptr));
        reloadButton->setText(QCoreApplication::translate("CompanyWidget", "\351\207\215\346\226\260\345\212\240\350\275\275\345\210\227\350\241\250", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(basicInfoTab), QCoreApplication::translate("CompanyWidget", "\345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(financialTab), QCoreApplication::translate("CompanyWidget", "\350\264\242\345\212\241\346\225\260\346\215\256", nullptr));
        announcementText->setPlainText(QCoreApplication::translate("CompanyWidget", "\346\232\202\346\227\240\345\205\254\345\221\212\344\277\241\346\201\257", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(announcementTab), QCoreApplication::translate("CompanyWidget", "\345\205\254\345\217\270\345\205\254\345\221\212", nullptr));
        (void)CompanyWidget;
    } // retranslateUi

};

namespace Ui {
    class CompanyWidget: public Ui_CompanyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPANYWIDGET_H
