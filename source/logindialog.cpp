#include "logindialog.h"
#include "ui_logindialog.h"
#include "client.h"
#include "themehelper.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QRandomGenerator>
#include <QEvent>
#include <QMouseEvent>
#include <QAction>
#include <QPainter>

LoginDialog::LoginDialog(Client *client, QWidget *parent)
	: QDialog(parent), m_emailValidator(nullptr), m_phoneValidator(nullptr), m_idCardValidator(nullptr)
	, ui(new Ui::LoginDialog), m_client(client) {
	ui->setupUi(this);
	setWindowTitle(tr("Login / Register"));
    setWindowIcon(QIcon(":/icons/app.ico"));
    setMinimumSize(300, 250);  // 设置最小尺寸
	// 设置输入验证
	setupInputValidation();
	
	// 生成初始验证码
	generateCaptcha();
	
	// 连接验证码标签点击事件（点击可刷新）
	ui->labelCaptchaCode->installEventFilter(this);
	ui->labelCaptchaCode->setCursor(Qt::PointingHandCursor);
	ui->labelCaptchaCode->setToolTip(tr("点击刷新验证码"));

    //设置账号密码
    ui->lineUserLogin->setText("wsy");
    ui->linePwdLogin->setText("123456789");
    ui->lineCaptchaLogin->setText(m_captchaCode);
	setupPasswordToggles();
	setupConnections();

	connect(ThemeHelper::instance(), &ThemeHelper::themeChanged, this, &LoginDialog::refreshTheme);
	refreshTheme();  // 初始化主题样式
}

LoginDialog::~LoginDialog() { delete ui; }

//
void LoginDialog::setupConnections() {
	connect(ui->btnLogin, &QPushButton::clicked, this, &LoginDialog::onDoLogin);
	connect(ui->btnRegister, &QPushButton::clicked, this, &LoginDialog::onDoRegister);
	connect(m_client, &Client::loginResult, this, &LoginDialog::handleLoginResult);
	connect(m_client, &Client::registerResult, this, &LoginDialog::handleRegisterResult);
	// 刷新按钮已移除，点击验证码标签即可刷新（通过eventFilter实现）
}

void LoginDialog::setupInputValidation() {
	// 邮箱验证
	QRegularExpression emailRegExp("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
	m_emailValidator = new QRegularExpressionValidator(emailRegExp, this);
	ui->lineEmailReg->setValidator(m_emailValidator);
	
	// 手机号验证（11位数字）
	QRegularExpression phoneRegExp("^1[3-9]\\d{9}$");
	m_phoneValidator = new QRegularExpressionValidator(phoneRegExp, this);
	ui->linePhoneReg->setValidator(m_phoneValidator);
	
	// 身份证号验证（15位或18位）
	QRegularExpression idCardRegExp("^(\\d{15}|\\d{17}[\\dXx])$");
	m_idCardValidator = new QRegularExpressionValidator(idCardRegExp, this);
	ui->lineIdCardReg->setValidator(m_idCardValidator);
}

void LoginDialog::generateCaptcha() {
	// 生成4位随机数字验证码
	m_captchaCode = QString::number(QRandomGenerator::global()->bounded(1000, 10000));
	ui->labelCaptchaCode->setText(m_captchaCode);
}

void LoginDialog::refreshCaptcha() {
	generateCaptcha();
	ui->lineCaptchaLogin->clear();
}

bool LoginDialog::validateCaptcha(const QString &input) {
	return input.trimmed() == m_captchaCode;
}

void LoginDialog::onDoLogin() {
	// 验证验证码
	QString captchaInput = ui->lineCaptchaLogin->text().trimmed();
	if (!validateCaptcha(captchaInput)) {
		QMessageBox::warning(this, tr("验证失败"), tr("验证码错误，请重新输入"));
		generateCaptcha(); // 刷新验证码
		ui->lineCaptchaLogin->clear();
		return;
	}
	
	// 验证输入
	QString username = ui->lineUserLogin->text().trimmed();
	QString password = ui->linePwdLogin->text();
	
	if (username.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入用户名"));
		return;
	}
	
	if (password.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入密码"));
		return;
	}
	
	m_client->login(username, password);
}

void LoginDialog::onDoRegister() {
	// 获取输入
	QString username = ui->lineUserReg->text().trimmed();
	QString password = ui->linePwdReg->text();
	QString realName = ui->lineRealNameReg->text().trimmed();
	QString email = ui->lineEmailReg->text().trimmed();
	QString phone = ui->linePhoneReg->text().trimmed();
	QString idCard = ui->lineIdCardReg->text().trimmed();
	QString address = ui->lineAddressReg->text().trimmed();
	
	// 验证必填字段
	if (username.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入用户名"));
		return;
	}
	
	if (username.length() < 3) {
		QMessageBox::warning(this, tr("输入错误"), tr("用户名至少需要3个字符"));
		return;
	}
	
	if (password.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入密码"));
		return;
	}
	
	if (password.length() < 6) {
		QMessageBox::warning(this, tr("输入错误"), tr("密码至少需要6个字符"));
		return;
	}
	
	if (realName.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入真实姓名"));
		return;
	}
	
	if (email.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入邮箱"));
		return;
	}
	
	// 验证邮箱格式
	int pos = 0;
	if (m_emailValidator && m_emailValidator->validate(email, pos) != QValidator::Acceptable) {
		QMessageBox::warning(this, tr("输入错误"), tr("邮箱格式不正确"));
		return;
	}
	
	if (phone.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入手机号"));
		return;
	}
	
	// 验证手机号格式
	if (m_phoneValidator && m_phoneValidator->validate(phone, pos) != QValidator::Acceptable) {
		QMessageBox::warning(this, tr("输入错误"), tr("手机号格式不正确（应为11位数字，以1开头）"));
		return;
	}
	
	if (idCard.isEmpty()) {
		QMessageBox::warning(this, tr("输入错误"), tr("请输入身份证号"));
		return;
	}
	
	// 验证身份证号格式
	if (m_idCardValidator && m_idCardValidator->validate(idCard, pos) != QValidator::Acceptable) {
		QMessageBox::warning(this, tr("输入错误"), tr("身份证号格式不正确（应为15位或18位）"));
		return;
	}
	
	// 调用注册方法，传递更多信息
	m_client->registerUser(username, password, realName, email, phone, idCard, address);
}

void LoginDialog::handleLoginResult(bool ok, const QString &message) {
	if (ok) {
		emit loginSucceeded();
		accept();
	} else {
		QMessageBox::warning(this, tr("Login Failed"), message);
		// 登录失败时刷新验证码（用户名错误、密码错误等情况）
		refreshCaptcha();
	}
}

void LoginDialog::handleRegisterResult(bool ok, const QString &message) {
	QMessageBox::information(this, ok ? tr("Register Success") : tr("Register Failed"), message);
}

bool LoginDialog::eventFilter(QObject *obj, QEvent *event) {
	if (obj == ui->labelCaptchaCode && event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent && mouseEvent->button() == Qt::LeftButton) {
			refreshCaptcha();
			return true;
		}
	}
	return QDialog::eventFilter(obj, event);
}

void LoginDialog::setupPasswordToggles() {
	setupPasswordToggle(ui->linePwdLogin);
	setupPasswordToggle(ui->linePwdReg);
}

void LoginDialog::setupPasswordToggle(QLineEdit *lineEdit) {
	QAction *action = lineEdit->addAction(createEyeIcon(false), QLineEdit::TrailingPosition);
	action->setCheckable(true);
	connect(action, &QAction::toggled, this, [lineEdit, action](bool checked) {
		lineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
		action->setIcon(createEyeIcon(checked));
	});
}

QIcon LoginDialog::createEyeIcon(bool open) {
	QPixmap pix(22, 20);
	pix.fill(Qt::transparent);
	QPainter p(&pix);
	p.setRenderHint(QPainter::Antialiasing);
	if (open) {
		// 眼睛外轮廓
		p.setPen(QPen(QColor("#666"), 2));
		p.setBrush(Qt::NoBrush);
		p.drawEllipse(QPointF(11, 10), 8, 6);
		// 瞳孔
		p.setBrush(QColor("#333"));
		p.drawEllipse(QPointF(11, 10), 3, 5);
	} else {
		// 眼睛外轮廓
		p.setPen(QPen(QColor("#888"), 2));
		p.setBrush(Qt::NoBrush);
		p.drawEllipse(QPointF(11, 10), 8, 6);
		// 斜线遮住
		p.drawLine(QPointF(3, 4), QPointF(19, 16));
	}
	p.end();
	return QIcon(pix);
}

void LoginDialog::refreshTheme() {
	bool dark = ThemeHelper::isDarkMode();
	QString accent = dark ? "#64B5F6" : "#1E88E5";
	QString captchaBg = dark ? "#2a2a40" : "#f0f0f0";
	QString captchaBorder = dark ? "#444" : "#ddd";
	QString captchaColor = dark ? "#e0e0e0" : "#333";

	ui->labelTitle->setStyleSheet(
		QString("font-size: 24px; font-weight: bold; color: %1; margin-bottom: 10px;").arg(accent));
	ui->labelCaptchaCode->setStyleSheet(
		QString("background-color: %1; border: 1px solid %2; font-size: 18px; font-weight: bold; color: %3; border-radius: 4px;")
			.arg(captchaBg, captchaBorder, captchaColor));
}

