#pragma once

#include <QDialog>
#include <QRegularExpressionValidator>
#include <QIcon>
class QLineEdit;
class QAction;

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class Client;

class LoginDialog : public QDialog {
	Q_OBJECT
public:
	explicit LoginDialog(Client *client, QWidget *parent = nullptr);
	~LoginDialog() override;

signals:
	void loginSucceeded();

private slots:
	void onDoLogin();
	void onDoRegister();
	void handleLoginResult(bool ok, const QString &message);
	void handleRegisterResult(bool ok, const QString &message);
	void refreshCaptcha();

private:
	void setupConnections();
	void setupInputValidation();
	void setupPasswordToggles();
	void setupPasswordToggle(QLineEdit *lineEdit);
	static QIcon createEyeIcon(bool open);
	void generateCaptcha();
	bool validateCaptcha(const QString &input);
	bool eventFilter(QObject *obj, QEvent *event) override;
	void refreshTheme();

	QString m_captchaCode; // 当前验证码
	QRegularExpressionValidator *m_emailValidator; // 邮箱验证器
	QRegularExpressionValidator *m_phoneValidator; // 手机号验证器
	QRegularExpressionValidator *m_idCardValidator; // 身份证号验证器

	Ui::LoginDialog *ui;
	Client *m_client;
};


