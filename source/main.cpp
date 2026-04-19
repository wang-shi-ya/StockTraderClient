#include <QApplication>
#include "mainwindow.h"
#include "client.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Client client;
	// 启动即进入主界面（默认 K 线图），无需先登录；登录从菜单「账户 → 登录/注册」进行
	MainWindow w(&client);
	w.show();
	return a.exec();
}


