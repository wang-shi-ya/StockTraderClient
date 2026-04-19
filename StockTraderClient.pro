QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = StockTraderClient
TEMPLATE = app

# 头文件目录（源码中 #include "xxx.h" 均相对于 head/）
INCLUDEPATH += $$PWD/head

# 源文件
SOURCES += \
    source/main.cpp \
    source/mainwindow.cpp \
    source/client.cpp \
    source/logindialog.cpp \
    source/dbconnectionpool.cpp \
    source/dbmanager.cpp \
    source/dataservice.cpp \
    source/quotewidget.cpp \
    source/klinewidget.cpp \
    source/companywidget.cpp \
    source/userservice.cpp \
    source/userinfowidget.cpp \
    source/edituserinfodialog.cpp

# 头文件
HEADERS += \
    head/mainwindow.h \
    head/client.h \
    head/logindialog.h \
    head/dbconnectionpool.h \
    head/dbmanager.h \
    head/quotedata.h \
    head/dataservice.h \
    head/quotewidget.h \
    head/klinewidget.h \
    head/companywidget.h \
    head/userinfo.h \
    head/userservice.h \
    head/userinfowidget.h \
    head/edituserinfodialog.h

# UI 界面文件
FORMS += \
    ui/mainwindow.ui \
    ui/logindialog.ui \
    ui/quotewidget.ui \
    ui/klinewidget.ui \
    ui/companywidget.ui \
    ui/userinfowidget.ui \
    ui/edituserinfodialog.ui

# 资源文件（可选）
RESOURCES += \
    resources.qrc

# 编译选项（可根据需要修改）
QMAKE_CXXFLAGS += -Wall
