/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabLogin;
    QGridLayout *gridLayoutLogin;
    QLabel *labelUserLogin;
    QLineEdit *lineUserLogin;
    QLabel *labelPwdLogin;
    QLineEdit *linePwdLogin;
    QLabel *labelCaptchaLogin;
    QHBoxLayout *captchaLayout;
    QLineEdit *lineCaptchaLogin;
    QLabel *labelCaptchaCode;
    QPushButton *btnLogin;
    QWidget *tabRegister;
    QGridLayout *gridLayoutRegister;
    QLabel *labelUserReg;
    QLineEdit *lineUserReg;
    QLabel *labelPwdReg;
    QLineEdit *linePwdReg;
    QLabel *labelRealNameReg;
    QLineEdit *lineRealNameReg;
    QLabel *labelEmailReg;
    QLineEdit *lineEmailReg;
    QLabel *labelPhoneReg;
    QLineEdit *linePhoneReg;
    QLabel *labelIdCardReg;
    QLineEdit *lineIdCardReg;
    QLabel *labelAddressReg;
    QLineEdit *lineAddressReg;
    QPushButton *btnRegister;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(345, 228);
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(LoginDialog);
        tabWidget->setObjectName("tabWidget");
        tabLogin = new QWidget();
        tabLogin->setObjectName("tabLogin");
        gridLayoutLogin = new QGridLayout(tabLogin);
        gridLayoutLogin->setObjectName("gridLayoutLogin");
        labelUserLogin = new QLabel(tabLogin);
        labelUserLogin->setObjectName("labelUserLogin");

        gridLayoutLogin->addWidget(labelUserLogin, 0, 0, 1, 1);

        lineUserLogin = new QLineEdit(tabLogin);
        lineUserLogin->setObjectName("lineUserLogin");

        gridLayoutLogin->addWidget(lineUserLogin, 0, 1, 1, 1);

        labelPwdLogin = new QLabel(tabLogin);
        labelPwdLogin->setObjectName("labelPwdLogin");

        gridLayoutLogin->addWidget(labelPwdLogin, 1, 0, 1, 1);

        linePwdLogin = new QLineEdit(tabLogin);
        linePwdLogin->setObjectName("linePwdLogin");
        linePwdLogin->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayoutLogin->addWidget(linePwdLogin, 1, 1, 1, 1);

        labelCaptchaLogin = new QLabel(tabLogin);
        labelCaptchaLogin->setObjectName("labelCaptchaLogin");

        gridLayoutLogin->addWidget(labelCaptchaLogin, 2, 0, 1, 1);

        captchaLayout = new QHBoxLayout();
        captchaLayout->setObjectName("captchaLayout");
        lineCaptchaLogin = new QLineEdit(tabLogin);
        lineCaptchaLogin->setObjectName("lineCaptchaLogin");
        lineCaptchaLogin->setMaximumSize(QSize(100, 16777215));

        captchaLayout->addWidget(lineCaptchaLogin);

        labelCaptchaCode = new QLabel(tabLogin);
        labelCaptchaCode->setObjectName("labelCaptchaCode");
        labelCaptchaCode->setMinimumSize(QSize(80, 30));
        labelCaptchaCode->setMaximumSize(QSize(80, 30));
        labelCaptchaCode->setAlignment(Qt::AlignmentFlag::AlignCenter);

        captchaLayout->addWidget(labelCaptchaCode);


        gridLayoutLogin->addLayout(captchaLayout, 2, 1, 1, 1);

        btnLogin = new QPushButton(tabLogin);
        btnLogin->setObjectName("btnLogin");

        gridLayoutLogin->addWidget(btnLogin, 3, 0, 1, 2);

        tabWidget->addTab(tabLogin, QString());
        tabRegister = new QWidget();
        tabRegister->setObjectName("tabRegister");
        gridLayoutRegister = new QGridLayout(tabRegister);
        gridLayoutRegister->setObjectName("gridLayoutRegister");
        labelUserReg = new QLabel(tabRegister);
        labelUserReg->setObjectName("labelUserReg");

        gridLayoutRegister->addWidget(labelUserReg, 0, 0, 1, 1);

        lineUserReg = new QLineEdit(tabRegister);
        lineUserReg->setObjectName("lineUserReg");

        gridLayoutRegister->addWidget(lineUserReg, 0, 1, 1, 1);

        labelPwdReg = new QLabel(tabRegister);
        labelPwdReg->setObjectName("labelPwdReg");

        gridLayoutRegister->addWidget(labelPwdReg, 1, 0, 1, 1);

        linePwdReg = new QLineEdit(tabRegister);
        linePwdReg->setObjectName("linePwdReg");
        linePwdReg->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayoutRegister->addWidget(linePwdReg, 1, 1, 1, 1);

        labelRealNameReg = new QLabel(tabRegister);
        labelRealNameReg->setObjectName("labelRealNameReg");

        gridLayoutRegister->addWidget(labelRealNameReg, 2, 0, 1, 1);

        lineRealNameReg = new QLineEdit(tabRegister);
        lineRealNameReg->setObjectName("lineRealNameReg");

        gridLayoutRegister->addWidget(lineRealNameReg, 2, 1, 1, 1);

        labelEmailReg = new QLabel(tabRegister);
        labelEmailReg->setObjectName("labelEmailReg");

        gridLayoutRegister->addWidget(labelEmailReg, 3, 0, 1, 1);

        lineEmailReg = new QLineEdit(tabRegister);
        lineEmailReg->setObjectName("lineEmailReg");

        gridLayoutRegister->addWidget(lineEmailReg, 3, 1, 1, 1);

        labelPhoneReg = new QLabel(tabRegister);
        labelPhoneReg->setObjectName("labelPhoneReg");

        gridLayoutRegister->addWidget(labelPhoneReg, 4, 0, 1, 1);

        linePhoneReg = new QLineEdit(tabRegister);
        linePhoneReg->setObjectName("linePhoneReg");

        gridLayoutRegister->addWidget(linePhoneReg, 4, 1, 1, 1);

        labelIdCardReg = new QLabel(tabRegister);
        labelIdCardReg->setObjectName("labelIdCardReg");

        gridLayoutRegister->addWidget(labelIdCardReg, 5, 0, 1, 1);

        lineIdCardReg = new QLineEdit(tabRegister);
        lineIdCardReg->setObjectName("lineIdCardReg");

        gridLayoutRegister->addWidget(lineIdCardReg, 5, 1, 1, 1);

        labelAddressReg = new QLabel(tabRegister);
        labelAddressReg->setObjectName("labelAddressReg");

        gridLayoutRegister->addWidget(labelAddressReg, 6, 0, 1, 1);

        lineAddressReg = new QLineEdit(tabRegister);
        lineAddressReg->setObjectName("lineAddressReg");

        gridLayoutRegister->addWidget(lineAddressReg, 6, 1, 1, 1);

        btnRegister = new QPushButton(tabRegister);
        btnRegister->setObjectName("btnRegister");

        gridLayoutRegister->addWidget(btnRegister, 7, 0, 1, 2);

        tabWidget->addTab(tabRegister, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(LoginDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Login / Register", nullptr));
        labelUserLogin->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        labelPwdLogin->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        labelCaptchaLogin->setText(QCoreApplication::translate("LoginDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        labelCaptchaCode->setStyleSheet(QCoreApplication::translate("LoginDialog", "background-color: #f0f0f0; border: 1px solid #ccc; font-size: 18px; font-weight: bold; color: #333;", nullptr));
        btnLogin->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLogin), QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        labelUserReg->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        labelPwdReg->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        labelRealNameReg->setText(QCoreApplication::translate("LoginDialog", "\347\234\237\345\256\236\345\247\223\345\220\215\357\274\232", nullptr));
        labelEmailReg->setText(QCoreApplication::translate("LoginDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        labelPhoneReg->setText(QCoreApplication::translate("LoginDialog", "\346\211\213\346\234\272\345\217\267\357\274\232", nullptr));
        labelIdCardReg->setText(QCoreApplication::translate("LoginDialog", "\350\272\253\344\273\275\350\257\201\345\217\267\357\274\232", nullptr));
        labelAddressReg->setText(QCoreApplication::translate("LoginDialog", "\345\234\260\345\235\200\357\274\232", nullptr));
        btnRegister->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRegister), QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
