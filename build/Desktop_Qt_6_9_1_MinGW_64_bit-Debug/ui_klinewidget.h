/********************************************************************************
** Form generated from reading UI file 'klinewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KLINEWIDGET_H
#define UI_KLINEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KLineWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *controlLayout;
    QLabel *labelSymbol;
    QComboBox *symbolCombo;
    QLabel *labelPeriod;
    QComboBox *periodCombo;
    QPushButton *refreshButton;
    QSpacerItem *horizontalSpacer;
    QLabel *infoLabel;
    QWidget *chartWidget;

    void setupUi(QWidget *KLineWidget)
    {
        if (KLineWidget->objectName().isEmpty())
            KLineWidget->setObjectName("KLineWidget");
        KLineWidget->resize(800, 600);
        verticalLayout = new QVBoxLayout(KLineWidget);
        verticalLayout->setObjectName("verticalLayout");
        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName("controlLayout");
        labelSymbol = new QLabel(KLineWidget);
        labelSymbol->setObjectName("labelSymbol");

        controlLayout->addWidget(labelSymbol);

        symbolCombo = new QComboBox(KLineWidget);
        symbolCombo->setObjectName("symbolCombo");

        controlLayout->addWidget(symbolCombo);

        labelPeriod = new QLabel(KLineWidget);
        labelPeriod->setObjectName("labelPeriod");

        controlLayout->addWidget(labelPeriod);

        periodCombo = new QComboBox(KLineWidget);
        periodCombo->setObjectName("periodCombo");

        controlLayout->addWidget(periodCombo);

        refreshButton = new QPushButton(KLineWidget);
        refreshButton->setObjectName("refreshButton");

        controlLayout->addWidget(refreshButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(horizontalSpacer);

        infoLabel = new QLabel(KLineWidget);
        infoLabel->setObjectName("infoLabel");

        controlLayout->addWidget(infoLabel);


        verticalLayout->addLayout(controlLayout);

        chartWidget = new QWidget(KLineWidget);
        chartWidget->setObjectName("chartWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(chartWidget->sizePolicy().hasHeightForWidth());
        chartWidget->setSizePolicy(sizePolicy);
        chartWidget->setMinimumSize(QSize(400, 300));

        verticalLayout->addWidget(chartWidget);


        retranslateUi(KLineWidget);

        QMetaObject::connectSlotsByName(KLineWidget);
    } // setupUi

    void retranslateUi(QWidget *KLineWidget)
    {
        labelSymbol->setText(QCoreApplication::translate("KLineWidget", "\350\202\241\347\245\250\344\273\243\347\240\201:", nullptr));
        labelPeriod->setText(QCoreApplication::translate("KLineWidget", "\345\221\250\346\234\237:", nullptr));
        refreshButton->setText(QCoreApplication::translate("KLineWidget", "\345\210\267\346\226\260", nullptr));
        infoLabel->setText(QCoreApplication::translate("KLineWidget", "K\347\272\277\345\233\276", nullptr));
        (void)KLineWidget;
    } // retranslateUi

};

namespace Ui {
    class KLineWidget: public Ui_KLineWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KLINEWIDGET_H
