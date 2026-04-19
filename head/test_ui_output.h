/********************************************************************************
** Form generated from reading UI file 'edituserinfodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TEST_UI_OUTPUT_H
#define TEST_UI_OUTPUT_H

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
    QLabel *usernameValueLabel;
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
            EditUserInfoDialog->setObjectName(QString::fromUtf8("EditUserInfoDialog"));
        EditUserInfoDialog->resize(500, 400);
        verticalLayout = new QVBoxLayout(EditUserInfoDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        usernameLabel = new QLabel(EditUserInfoDialog);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, usernameLabel);

        usernameValueLabel = new QLabel(EditUserInfoDialog);
        usernameValueLabel->setObjectName(QString::fromUtf8("usernameValueLabel"));

        formLayout->setWidget(0, QFormLayout::FieldRole, usernameValueLabel);

        realNameLabel = new QLabel(EditUserInfoDialog);
        realNameLabel->setObjectName(QString::fromUtf8("realNameLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, realNameLabel);

        realNameEdit = new QLineEdit(EditUserInfoDialog);
        realNameEdit->setObjectName(QString::fromUtf8("realNameEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, realNameEdit);

        emailLabel = new QLabel(EditUserInfoDialog);
        emailLabel->setObjectName(QString::fromUtf8("emailLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, emailLabel);

        emailEdit = new QLineEdit(EditUserInfoDialog);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, emailEdit);

        phoneLabel = new QLabel(EditUserInfoDialog);
        phoneLabel->setObjectName(QString::fromUtf8("phoneLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, phoneLabel);

        phoneEdit = new QLineEdit(EditUserInfoDialog);
        phoneEdit->setObjectName(QString::fromUtf8("phoneEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, phoneEdit);

        idCardLabel = new QLabel(EditUserInfoDialog);
        idCardLabel->setObjectName(QString::fromUtf8("idCardLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, idCardLabel);

        idCardEdit = new QLineEdit(EditUserInfoDialog);
        idCardEdit->setObjectName(QString::fromUtf8("idCardEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, idCardEdit);

        addressLabel = new QLabel(EditUserInfoDialog);
        addressLabel->setObjectName(QString::fromUtf8("addressLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, addressLabel);

        addressEdit = new QLineEdit(EditUserInfoDialog);
        addressEdit->setObjectName(QString::fromUtf8("addressEdit"));

        formLayout->setWidget(5, QFormLayout::FieldRole, addressEdit);

        registerTimeLabel = new QLabel(EditUserInfoDialog);
        registerTimeLabel->setObjectName(QString::fromUtf8("registerTimeLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, registerTimeLabel);

        registerTimeValueLabel = new QLabel(EditUserInfoDialog);
        registerTimeValueLabel->setObjectName(QString::fromUtf8("registerTimeValueLabel"));

        formLayout->setWidget(6, QFormLayout::FieldRole, registerTimeValueLabel);

        lastLoginTimeLabel = new QLabel(EditUserInfoDialog);
        lastLoginTimeLabel->setObjectName(QString::fromUtf8("lastLoginTimeLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, lastLoginTimeLabel);

        lastLoginTimeValueLabel = new QLabel(EditUserInfoDialog);
        lastLoginTimeValueLabel->setObjectName(QString::fromUtf8("lastLoginTimeValueLabel"));

        formLayout->setWidget(7, QFormLayout::FieldRole, lastLoginTimeValueLabel);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        saveButton = new QPushButton(EditUserInfoDialog);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        buttonLayout->addWidget(saveButton);

        cancelButton = new QPushButton(EditUserInfoDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttonLayout->addWidget(cancelButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(EditUserInfoDialog);

        QMetaObject::connectSlotsByName(EditUserInfoDialog);
    } // setupUi

    void retranslateUi(QDialog *EditUserInfoDialog)
    {
        EditUserInfoDialog->setWindowTitle(QCoreApplication::translate("EditUserInfoDialog", "\347\274\226\350\276\221\344\270\252\344\272\272\344\277\241\346\201\257", nullptr));
        usernameLabel->setText(QCoreApplication::translate("EditUserInfoDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        usernameValueLabel->setText(QString());
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

#endif // TEST_UI_OUTPUT_H
