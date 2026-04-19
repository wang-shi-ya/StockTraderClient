#include "client.h"
#include <QRandomGenerator>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QCryptographicHash>
#include <QUuid>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

Client::Client(QObject *parent) : QObject(parent) {
	bool dbOk = m_db.initialize();
	if (!dbOk) {
		qCritical() << "Database initialization failed! Please check MySQL connection.";
	}
	
	// 创建定时器，定期检查账号状态
	m_statusCheckTimer = new QTimer(this);
	connect(m_statusCheckTimer, &QTimer::timeout, this, &Client::checkAccountStatus);
	m_statusCheckTimer->setInterval(30000); // 每30秒检查一次
}

void Client::login(const QString &user, const QString &password) {
	// Check database connection first
	QSqlDatabase db = m_db.database();
	if (!db.isOpen()) {
		m_db.releaseConnection();
		emit loginResult(false, QString::fromUtf8("数据库连接失败，请检查MySQL服务是否运行"));
		return;
	}
	
	// Check user in DB, including status
    QSqlQuery q(db);
    q.prepare("SELECT password_hash, COALESCE(salt, ''), COALESCE(status, '正常') FROM users WHERE username=?");
	q.addBindValue(user.trimmed());
	bool ok = q.exec() && q.next();
	
	if (!ok && q.lastError().isValid()) {
		// SQL error occurred
		qWarning() << "Login query error:" << q.lastError();
		m_db.releaseConnection();
		emit loginResult(false, QString::fromUtf8("数据库查询错误：") + q.lastError().text());
		return;
	}
	
	if (!ok) {
		// User not found
		m_db.releaseConnection();
		emit loginResult(false, QString::fromUtf8("用户名或密码错误，请先注册"));
		return;
	}
	
	// Check account status first
	QString status = q.value(2).toString();
	if (status == "冻结" || status == "已冻结" || status.toLower() == "frozen") {
		m_db.releaseConnection();
		emit loginResult(false, QString::fromUtf8("账号已被冻结，无法登录"));
		return;
	}
	
	// Verify password
	bool passOk = false;
    const QByteArray storedHash = q.value(0).toByteArray();
    const QByteArray salt = q.value(1).toByteArray();
    const QByteArray candidate = QCryptographicHash::hash(password.toUtf8() + salt, QCryptographicHash::Sha256).toHex();
    passOk = (storedHash == candidate);
	
	if (!passOk) {
		m_db.releaseConnection();
		emit loginResult(false, QString::fromUtf8("用户名或密码错误"));
		return;
	}
	
	m_loggedIn = true;
	m_currentUser = user.trimmed();
	
	// 释放第一个连接（用户验证已完成）
	m_db.releaseConnection();
	
	// 更新最后登录时间（该方法内部会获取和释放连接）
	m_db.updateLastLoginTime(m_currentUser);
	
	// Load positions after login
	QSqlDatabase db2 = m_db.database();
	if (!db2.isOpen()) {
		m_db.releaseConnection();
		emit loginResult(false, QString::fromUtf8("数据库连接失败"));
		return;
	}
	
	QSqlQuery qp(db2);
	qp.prepare("SELECT symbol, quantity, avg_price FROM positions WHERE username=?");
	qp.addBindValue(m_currentUser);
	QVector<PositionInfo> ps;
	if (qp.exec()) {
		while (qp.next()) {
			PositionInfo p;
			p.symbol = qp.value(0).toString();
			p.quantity = qp.value(1).toLongLong();
			p.averagePrice = qp.value(2).toDouble();
			p.currentPrice = 0.0; // 将在UI中更新
			p.profitLoss = 0.0;
			p.profitLossPercent = 0.0;
			ps.push_back(p);
		}
	}
	m_db.releaseConnection();
	
	emit loginResult(true, QString::fromUtf8("登录成功"));
	emit positionsLoaded(ps);
	
	// 加载历史订单
	loadOrderHistory();
	
	// 启动状态检查定时器
	if (m_statusCheckTimer) {
		m_statusCheckTimer->start();
	}
}

void Client::registerUser(const QString &user, const QString &password,
                          const QString &realName, const QString &email,
                          const QString &phone, const QString &idCard,
                          const QString &address) {
	const QString u = user.trimmed();
	if (u.size() < 3 || password.size() < 6) {
		emit registerResult(false, QString::fromUtf8("注册失败：用户名>=3且密码>=6"));
		return;
	}
	
	// 验证必填信息
	if (realName.trimmed().isEmpty()) {
		emit registerResult(false, QString::fromUtf8("注册失败：请输入真实姓名"));
		return;
	}
	
	if (email.trimmed().isEmpty()) {
		emit registerResult(false, QString::fromUtf8("注册失败：请输入邮箱"));
		return;
	}
	
	if (phone.trimmed().isEmpty()) {
		emit registerResult(false, QString::fromUtf8("注册失败：请输入手机号"));
		return;
	}
	
	if (idCard.trimmed().isEmpty()) {
		emit registerResult(false, QString::fromUtf8("注册失败：请输入身份证号"));
		return;
	}
	
	// Check database connection first
	QSqlDatabase db = m_db.database();
	if (!db.isOpen()) {
		m_db.releaseConnection();
		emit registerResult(false, QString::fromUtf8("数据库连接失败，请检查MySQL服务是否运行"));
		return;
	}
	
    const QByteArray salt = QUuid::createUuid().toString(QUuid::WithoutBraces).toUtf8();
    const QByteArray hash = QCryptographicHash::hash(password.toUtf8() + salt, QCryptographicHash::Sha256).toHex();
    QSqlQuery q(db);
    q.prepare("INSERT INTO users(username, password_hash, salt) VALUES(?, ?, ?)");
    q.addBindValue(u);
    q.addBindValue(QString::fromUtf8(hash));
    q.addBindValue(QString::fromUtf8(salt));
    const bool ok = q.exec();
	
	QString msg;
	if (ok) {
		msg = QString::fromUtf8("注册成功");
		// 保存用户个人信息（该方法内部会释放连接）
		m_db.saveUserInfo(u, realName.trimmed(), email.trimmed(), phone.trimmed(), 
		                  idCard.trimmed(), address.trimmed(), "正常");
	} else {
		m_db.releaseConnection();
		QString error = q.lastError().text();
		if (error.contains("Duplicate entry") || error.contains("UNIQUE")) {
			msg = QString::fromUtf8("注册失败：用户名已存在");
		} else {
			msg = QString::fromUtf8("注册失败：") + error;
			qWarning() << "Register error:" << q.lastError();
		}
	}
	emit registerResult(ok, msg);
}

void Client::queryQuote(const QString &symbol) {
	if (symbol.trimmed().isEmpty()) {
		emit quoteUpdated(symbol, 0.0);
		return;
	}
	// Mock random last price around 10~100
    double price = 10.0 + QRandomGenerator::global()->generateDouble() * (100.0 - 10.0);
	emit quoteUpdated(symbol.toUpper(), price);
}

void Client::placeOrder(const QString &symbol, const QString &side, int quantity, double price) {
	// 基本验证
	if (!m_loggedIn) {
		emit orderRejected(QString::fromUtf8("请先登录"));
		return;
	}
	
	if (symbol.trimmed().isEmpty()) {
		emit orderRejected(QString::fromUtf8("股票代码不能为空"));
		return;
	}
	
	if (quantity <= 0) {
		emit orderRejected(QString::fromUtf8("数量必须大于0"));
		return;
	}
	
	if (price <= 0) {
		emit orderRejected(QString::fromUtf8("价格必须大于0"));
		return;
	}
	
	QSqlDatabase db = m_db.database();
	if (!db.isOpen()) {
		m_db.releaseConnection();
		emit orderRejected(QString::fromUtf8("数据库连接失败"));
		return;
	}
	
	// 卖出时检查持仓是否足够
	bool isSell = (side.compare("卖出", Qt::CaseInsensitive) == 0 || side.compare("Sell", Qt::CaseInsensitive) == 0);
	if (isSell) {
		QSqlQuery qp(db);
		qp.prepare("SELECT quantity FROM positions WHERE username=? AND symbol=?");
		qp.addBindValue(m_currentUser);
		qp.addBindValue(symbol.toUpper());
		long long currentQty = 0;
		if (qp.exec() && qp.next()) {
			currentQty = qp.value(0).toLongLong();
		}
		
		if (currentQty < quantity) {
			m_db.releaseConnection();
			emit orderRejected(QString::fromUtf8("持仓不足，当前持仓：%1，卖出数量：%2").arg(currentQty).arg(quantity));
			return;
		}
	}
	
	OrderInfo info;
	info.orderId = generateOrderId();
	info.symbol = symbol.toUpper();
	info.side = side;
	info.quantity = quantity;
	info.price = price;
	info.status = QString::fromUtf8("New");
	info.createdAt = QDateTime::currentDateTime();
	m_orders.push_back(info);
	
	// 保存订单到数据库
	QSqlQuery q(db);
	q.prepare("INSERT INTO orders(order_id, username, symbol, side, quantity, price, status, created_at) VALUES(?,?,?,?,?,?,?,?)");
	q.addBindValue(info.orderId);
	q.addBindValue(m_currentUser);
	q.addBindValue(info.symbol);
	q.addBindValue(info.side);
	q.addBindValue(info.quantity);
	q.addBindValue(info.price);
	q.addBindValue(info.status);
	q.addBindValue(info.createdAt);
	
	if (!q.exec()) {
		m_db.releaseConnection();
		qWarning() << "Failed to insert order:" << q.lastError();
		emit orderRejected(QString::fromUtf8("订单提交失败：") + q.lastError().text());
		return;
	}
	
	emit orderPlaced(info);
	
	// 立即成交（演示模式）
	// 更新订单状态为已成交
	info.status = QString::fromUtf8("Filled");
	QSqlQuery qu(db);
	qu.prepare("UPDATE orders SET status=? WHERE order_id=?");
	qu.addBindValue(info.status);
	qu.addBindValue(info.orderId);
	qu.exec();
	
	// 插入成交记录
	QSqlQuery qt(db);
	qt.prepare("INSERT INTO trades(order_id, username, symbol, side, quantity, price) VALUES(?,?,?,?,?,?)");
	qt.addBindValue(info.orderId);
	qt.addBindValue(m_currentUser);
	qt.addBindValue(info.symbol);
	qt.addBindValue(info.side);
	qt.addBindValue(info.quantity);
	qt.addBindValue(info.price);
	qt.exec();

	// 更新持仓
	if (isSell) {
		// sell: reduce qty
		QSqlQuery qp(db);
		qp.prepare("SELECT quantity, avg_price FROM positions WHERE username=? AND symbol=?");
		qp.addBindValue(m_currentUser);
		qp.addBindValue(info.symbol);
		long long oldQty = 0; double oldAvg = 0.0;
		if (qp.exec() && qp.next()) { 
			oldQty = qp.value(0).toLongLong(); 
			oldAvg = qp.value(1).toDouble(); 
		}
		const long long newQty = qMax<qint64>(0, oldQty - info.quantity);
		QSqlQuery qup(db);
		if (newQty == 0) {
			// 全部卖出，删除持仓记录
			qup.prepare("DELETE FROM positions WHERE username=? AND symbol=?");
			qup.addBindValue(m_currentUser);
			qup.addBindValue(info.symbol);
		} else {
			qup.prepare("UPDATE positions SET quantity=?, avg_price=? WHERE username=? AND symbol=?");
			qup.addBindValue(newQty);
			qup.addBindValue(oldAvg);
			qup.addBindValue(m_currentUser);
			qup.addBindValue(info.symbol);
		}
		qup.exec();
	} else {
		// buy: new_qty = qty +, new_avg = (old_qty*old_avg + qty*price)/new_qty
		QSqlQuery qp(db);
		qp.prepare("SELECT quantity, avg_price FROM positions WHERE username=? AND symbol=?");
		qp.addBindValue(m_currentUser);
		qp.addBindValue(info.symbol);
		long long oldQty = 0; double oldAvg = 0.0;
		if (qp.exec() && qp.next()) { 
			oldQty = qp.value(0).toLongLong(); 
			oldAvg = qp.value(1).toDouble(); 
		}
		const long long newQty = oldQty + info.quantity;
		const double newAvg = newQty > 0 ? (oldQty * oldAvg + info.quantity * info.price) / double(newQty) : 0.0;
		QSqlQuery qup(db);
		if (oldQty == 0) {
			qup.prepare("INSERT INTO positions(username, symbol, quantity, avg_price) VALUES(?,?,?,?)");
			qup.addBindValue(m_currentUser); 
			qup.addBindValue(info.symbol); 
			qup.addBindValue(newQty); 
			qup.addBindValue(newAvg);
		} else {
			qup.prepare("UPDATE positions SET quantity=?, avg_price=? WHERE username=? AND symbol=?");
			qup.addBindValue(newQty); 
			qup.addBindValue(newAvg); 
			qup.addBindValue(m_currentUser); 
			qup.addBindValue(info.symbol);
		}
		qup.exec();
	}

	// 更新内存中的订单状态
	for (OrderInfo &o : m_orders) {
		if (o.orderId == info.orderId) {
			o.status = info.status;
			break;
		}
	}
	
	emit orderUpdated(info);

	// 重新加载持仓
	QSqlQuery qr(db);
	qr.prepare("SELECT symbol, quantity, avg_price FROM positions WHERE username=?");
	qr.addBindValue(m_currentUser);
	QVector<PositionInfo> ps;
	if (qr.exec()) {
		while (qr.next()) {
			PositionInfo p; 
			p.symbol = qr.value(0).toString(); 
			p.quantity = qr.value(1).toLongLong(); 
			p.averagePrice = qr.value(2).toDouble();
			p.currentPrice = 0.0; // 将在UI中更新
			p.profitLoss = 0.0;
			p.profitLossPercent = 0.0;
			ps.push_back(p);
		}
	}
	m_db.releaseConnection();
	emit positionsLoaded(ps);
}

void Client::cancelOrder(const QString &orderId) {
	for (OrderInfo &o : m_orders) {
		if (o.orderId == orderId) {
			o.status = QString::fromUtf8("Canceled");
			emit orderUpdated(o);
			break;
		}
	}
	// persist
	QSqlDatabase db = m_db.database();
	if (db.isOpen()) {
		QSqlQuery q(db);
		q.prepare("UPDATE orders SET status='Canceled' WHERE order_id=?");
		q.addBindValue(orderId);
		q.exec();
		m_db.releaseConnection();
	}
}

QString Client::generateOrderId() const {
	const quint32 r = QRandomGenerator::global()->generate();
	return QString::number(r, 16).toUpper();
}

void Client::logout() {
	if (!m_loggedIn) {
		return;
	}
	
	// 停止状态检查定时器
	if (m_statusCheckTimer) {
		m_statusCheckTimer->stop();
	}
	
	// 清除登录状态
	m_loggedIn = false;
	QString username = m_currentUser;
	m_currentUser.clear();
	m_orders.clear();
	
	emit loggedOut(QString::fromUtf8("已退出登录"));
	qDebug() << "User logged out:" << username;
}

void Client::loadOrderHistory() {
	if (!m_loggedIn || m_currentUser.isEmpty()) {
		return;
	}
	
	QSqlDatabase db = m_db.database();
	if (!db.isOpen()) {
		m_db.releaseConnection();
		return;
	}
	
	QSqlQuery q(db);
	q.prepare("SELECT order_id, symbol, side, quantity, price, status, created_at FROM orders WHERE username=? ORDER BY created_at DESC LIMIT 100");
	q.addBindValue(m_currentUser);
	
	QVector<OrderInfo> orders;
	if (q.exec()) {
		while (q.next()) {
			OrderInfo order;
			order.orderId = q.value(0).toString();
			order.symbol = q.value(1).toString();
			order.side = q.value(2).toString();
			order.quantity = q.value(3).toInt();
			order.price = q.value(4).toDouble();
			order.status = q.value(5).toString();
			order.createdAt = q.value(6).toDateTime();
			orders.push_back(order);
		}
	}
	
	m_orders = orders;
	m_db.releaseConnection();
	emit orderHistoryLoaded(orders);
}

void Client::checkAccountStatus() {
	if (!m_loggedIn || m_currentUser.isEmpty()) {
		return;
	}
	
	// 检查数据库连接
	QSqlDatabase db = m_db.database();
	if (!db.isOpen()) {
		m_db.releaseConnection();
		return;
	}
	
	// 查询用户状态
	QSqlQuery q(db);
	q.prepare("SELECT COALESCE(status, '正常') FROM users WHERE username=?");
	q.addBindValue(m_currentUser);
	
	if (q.exec() && q.next()) {
		QString status = q.value(0).toString();
		
		// 如果账号被冻结，自动退出登录
		if (status == "冻结" || status == "已冻结" || status.toLower() == "frozen") {
			qWarning() << "Account frozen, logging out:" << m_currentUser;
			
			// 停止定时器
			if (m_statusCheckTimer) {
				m_statusCheckTimer->stop();
			}
			
			// 清除登录状态
			QString username = m_currentUser;
			m_loggedIn = false;
			m_currentUser.clear();
			m_orders.clear();
			
			// 发出信号
			emit accountFrozen();
			emit loggedOut(QString::fromUtf8("账号已被冻结，已自动退出登录"));
		}
	}
	m_db.releaseConnection();
}


