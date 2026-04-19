#include "edituserinfodialog.h"
#include "ui_edituserinfodialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

EditUserInfoDialog::EditUserInfoDialog(const UserInfo &userInfo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditUserInfoDialog)
    , m_userInfo(userInfo)
    , m_emailValidator(nullptr)
    , m_phoneValidator(nullptr)
    , m_idCardValidator(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("编辑个人信息"));
    
    setupUI();
    setupValidation();
    
    // 连接保存按钮
    connect(ui->saveButton, &QPushButton::clicked, this, &EditUserInfoDialog::onSaveClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // 连接输入验证
    connect(ui->emailEdit, &QLineEdit::textChanged, this, &EditUserInfoDialog::validateInput);
    connect(ui->phoneEdit, &QLineEdit::textChanged, this, &EditUserInfoDialog::validateInput);
    connect(ui->idCardEdit, &QLineEdit::textChanged, this, &EditUserInfoDialog::validateInput);
}

EditUserInfoDialog::~EditUserInfoDialog() {
    delete ui;
}

void EditUserInfoDialog::setupUI() {
    // 填充现有数据
    ui->usernameEdit->setText(m_userInfo.username);
    ui->realNameEdit->setText(m_userInfo.realName);
    ui->emailEdit->setText(m_userInfo.email);
    ui->phoneEdit->setText(m_userInfo.phone);
    ui->idCardEdit->setText(m_userInfo.idCard);
    ui->addressEdit->setText(m_userInfo.address);
    
    // 注册时间和最后登录时间不可编辑，只显示
    ui->registerTimeValueLabel->setText(
        m_userInfo.registerTime.isValid() ? 
        m_userInfo.registerTime.toString("yyyy-MM-dd hh:mm:ss") : 
        QString::fromUtf8("未知")
    );
    ui->lastLoginTimeValueLabel->setText(
        m_userInfo.lastLoginTime.isValid() ? 
        m_userInfo.lastLoginTime.toString("yyyy-MM-dd hh:mm:ss") : 
        QString::fromUtf8("未知")
    );
}

void EditUserInfoDialog::setupValidation() {
    // 邮箱验证
    QRegularExpression emailRegExp("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    m_emailValidator = new QRegularExpressionValidator(emailRegExp, this);
    ui->emailEdit->setValidator(m_emailValidator);
    
    // 手机号验证（11位数字，以1开头）
    QRegularExpression phoneRegExp("^1[3-9]\\d{9}$");
    m_phoneValidator = new QRegularExpressionValidator(phoneRegExp, this);
    ui->phoneEdit->setValidator(m_phoneValidator);
    
    // 身份证号验证（15位或18位）
    QRegularExpression idCardRegExp("^(\\d{15}|\\d{17}[\\dXx])$");
    m_idCardValidator = new QRegularExpressionValidator(idCardRegExp, this);
    ui->idCardEdit->setValidator(m_idCardValidator);
}

UserInfo EditUserInfoDialog::getUserInfo() const {
    return m_userInfo;
}

void EditUserInfoDialog::onSaveClicked() {
    // 验证必填字段
    if (ui->usernameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("请输入用户名"));
        ui->usernameEdit->setFocus();
        return;
    }
    
    if (ui->realNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("请输入真实姓名"));
        ui->realNameEdit->setFocus();
        return;
    }
    
    if (ui->emailEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("请输入邮箱"));
        ui->emailEdit->setFocus();
        return;
    }
    
    // 验证邮箱格式
    if (!validateEmail(ui->emailEdit->text().trimmed())) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("邮箱格式不正确"));
        ui->emailEdit->setFocus();
        return;
    }
    
    if (ui->phoneEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("请输入手机号"));
        ui->phoneEdit->setFocus();
        return;
    }
    
    // 验证手机号格式
    if (!validatePhone(ui->phoneEdit->text().trimmed())) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("手机号格式不正确（应为11位数字，以1开头）"));
        ui->phoneEdit->setFocus();
        return;
    }
    
    if (ui->idCardEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("请输入身份证号"));
        ui->idCardEdit->setFocus();
        return;
    }
    
    // 验证身份证号格式
    if (!validateIdCard(ui->idCardEdit->text().trimmed())) {
        QMessageBox::warning(this, QString::fromUtf8("输入错误"), QString::fromUtf8("身份证号格式不正确（应为15位或18位）"));
        ui->idCardEdit->setFocus();
        return;
    }
    
    // 更新用户信息
    m_userInfo.username = ui->usernameEdit->text().trimmed();
    m_userInfo.realName = ui->realNameEdit->text().trimmed();
    m_userInfo.email = ui->emailEdit->text().trimmed();
    m_userInfo.phone = ui->phoneEdit->text().trimmed();
    m_userInfo.idCard = ui->idCardEdit->text().trimmed();
    m_userInfo.address = ui->addressEdit->text().trimmed();
    
    accept();
}

void EditUserInfoDialog::validateInput() {
    // 实时验证输入格式
    QLineEdit *sender = qobject_cast<QLineEdit*>(this->sender());
    if (!sender) return;
    
    QString text = sender->text().trimmed();
    
    if (sender == ui->emailEdit && !text.isEmpty()) {
        if (!validateEmail(text)) {
            sender->setStyleSheet("border: 1px solid red;");
        } else {
            sender->setStyleSheet("");
        }
    } else if (sender == ui->phoneEdit && !text.isEmpty()) {
        if (!validatePhone(text)) {
            sender->setStyleSheet("border: 1px solid red;");
        } else {
            sender->setStyleSheet("");
        }
    } else if (sender == ui->idCardEdit && !text.isEmpty()) {
        if (!validateIdCard(text)) {
            sender->setStyleSheet("border: 1px solid red;");
        } else {
            sender->setStyleSheet("");
        }
    }
}

bool EditUserInfoDialog::validateEmail(const QString &email) {
    QRegularExpression emailRegExp("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = emailRegExp.match(email);
    return match.hasMatch();
}

bool EditUserInfoDialog::validatePhone(const QString &phone) {
    QRegularExpression phoneRegExp("^1[3-9]\\d{9}$");
    QRegularExpressionMatch match = phoneRegExp.match(phone);
    return match.hasMatch();
}

bool EditUserInfoDialog::validateIdCard(const QString &idCard) {
    QRegularExpression idCardRegExp("^(\\d{15}|\\d{17}[\\dXx])$");
    QRegularExpressionMatch match = idCardRegExp.match(idCard);
    return match.hasMatch();
}

