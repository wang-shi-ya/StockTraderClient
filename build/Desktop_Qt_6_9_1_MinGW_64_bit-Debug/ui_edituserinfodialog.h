/********************************************************************************
** Form generated from reading UI file 'edituserinfodialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITUSERINFODIALOG_H
#define UI_EDITUSERINFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditUserInfoDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *realNameLabel;
    QLineEdit *realNameEdit;
    QLabel *emailLabel;
    QLineEdit *emailEdit;
    QLabel *phoneLabel;
    QLineEdit *phoneEdit;
    QLabel *idCardLabel;
    QLineEdit *idCardEdit;
    QLabel *addressLabel;
    QLineEdit *addressEdit;
    QLabel *registerTimeLabel;
    QLabel *registerTimeValueLabel;
    QLabel *lastLoginTimeLabel;
    QLabel *lastLoginTimeValueLabel;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *EditUserInfoDialog)
    {
        if (EditUserInfoDialog->objectName().isEmpty())
            EditUserInfoDialog->setObjectName("EditUserInfoDialog");
        EditUserInfoDialog->resize(500, 400);
        verticalLayout = new QVBoxLayout(EditUserInfoDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        usernameLabel = new QLabel(EditUserInfoDialog);
        usernameLabel->setObjectName("usernameLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, usernameLabel);

        usernameEdit = new QLineEdit(EditUserInfoDialog);
        usernameEdit->setObjectName("usernameEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, usernameEdit);

        realNameLabel = new QLabel(EditUserInfoDialog);
        realNameLabel->setObjectName("realNameLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, realNameLabel);

        realNameEdit = new QLineEdit(EditUserInfoDialog);
        realNameEdit->setObjectName("realNameEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, realNameEdit);

        emailLabel = new QLabel(EditUserInfoDialog);
        emailLabel->setObjectName("emailLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, emailLabel);

        emailEdit = new QLineEdit(EditUserInfoDialog);
        emailEdit->setObjectName("emailEdit");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, emailEdit);

        phoneLabel = new QLabel(EditUserInfoDialog);
        phoneLabel->setObjectName("phoneLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, phoneLabel);

        phoneEdit = new QLineEdit(EditUserInfoDialog);
        phoneEdit->setObjectName("phoneEdit");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, phoneEdit);

        idCardLabel = new QLabel(EditUserInfoDialog);
        idCardLabel->setObjectName("idCardLabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, idCardLabel);

        idCardEdit = new QLineEdit(EditUserInfoDialog);
        idCardEdit->setObjectName("idCardEdit");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, idCardEdit);

        addressLabel = new QLabel(EditUserInfoDialog);
        addressLabel->setObjectName("addressLabel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, addressLabel);

        addressEdit = new QLineEdit(EditUserInfoDialog);
        addressEdit->setObjectName("addressEdit");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, addressEdit);

        registerTimeLabel = new QLabel(EditUserInfoDialog);
        registerTimeLabel->setObjectName("registerTimeLabel");

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, registerTimeLabel);

        registerTimeValueLabel = new QLabel(EditUserInfoDialog);
        registerTimeValueLabel->setObjectName("registerTimeValueLabel");

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, registerTimeValueLabel);

        lastLoginTimeLabel = new QLabel(EditUserInfoDialog);
        lastLoginTimeLabel->setObjectName("lastLoginTimeLabel");

        formLayout->setWidget(7, QFormLayout::ItemRole::LabelRole, lastLoginTimeLabel);

        lastLoginTimeValueLabel = new QLabel(EditUserInfoDialog);
        lastLoginTimeValueLabel->setObjectName("lastLoginTimeValueLabel");

        formLayout->setWidget(7, QFormLayout::ItemRole::FieldRole, lastLoginTimeValueLabel);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        saveButton = new QPushButton(EditUserInfoDialog);
        saveButton->setObjectName("saveButton");

        buttonLayout->addWidget(saveButton);

        cancelButton = new QPushButton(EditUserInfoDialog);
        cancelButton->setObjectName("cancelButton");

        buttonLayout->addWidget(cancelButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(EditUserInfoDialog);

        QMetaObject::connectSlotsByName(EditUserInfoDialog);
    } // setupUi

    void retranslateUi(QDialog *EditUserInfoDialog)
    {
        EditUserInfoDialog->setWindowTitle(QCoreApplication::translate("EditUserInfoDialog", "\347\274\226\350\276\221\344\270\252\344\272\272\344\277\241\346\201\257", nullptr));
        usernameLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        usernameEdit->setText(QString());
        realNameLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\347\234\237\345\256\236\345\247\223\345\220\215\357\274\232", nullptr));
        emailLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        phoneLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\346\211\213\346\234\272\345\217\267\357\274\232", nullptr));
        idCardLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\350\272\253\344\273\275\350\257\201\345\217\267\357\274\232", nullptr));
        addressLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\345\234\260\345\235\200\357\274\232", nullptr));
        registerTimeLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\346\263\250\345\206\214\346\227\266\351\227\264\357\274\232", nullptr));
        registerTimeValueLabel->setText(QString());
        lastLoginTimeLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\346\234\200\345\220\216\347\231\273\345\275\225\357\274\232", nullptr));
        lastLoginTimeValueLabel->setText(QString());
        saveButton->setText(QCoreApplication::translate("EditUserInfoDialog", "\344\277\235\345\255\230", nullptr));
        cancelButton->setText(QCoreApplication::translate("EditUserInfoDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditUserInfoDialog: public Ui_EditUserInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITUSERINFODIALOG_H
