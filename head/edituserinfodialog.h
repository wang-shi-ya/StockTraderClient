#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include "userinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditUserInfoDialog; }
QT_END_NAMESPACE

class EditUserInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditUserInfoDialog(const UserInfo &userInfo, QWidget *parent = nullptr);
    ~EditUserInfoDialog() override;

    UserInfo getUserInfo() const;

private slots:
    void onSaveClicked();
    void validateInput();

private:
    void setupUI();
    void setupValidation();
    bool validateEmail(const QString &email);
    bool validatePhone(const QString &phone);
    bool validateIdCard(const QString &idCard);

    Ui::EditUserInfoDialog *ui;
    UserInfo m_userInfo;
    
    QRegularExpressionValidator *m_emailValidator;
    QRegularExpressionValidator *m_phoneValidator;
    QRegularExpressionValidator *m_idCardValidator;
};

