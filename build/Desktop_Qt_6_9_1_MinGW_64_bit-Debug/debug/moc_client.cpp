/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../head/client.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN6ClientE_t {};
} // unnamed namespace

template <> constexpr inline auto Client::qt_create_metaobjectdata<qt_meta_tag_ZN6ClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Client",
        "loginResult",
        "",
        "success",
        "message",
        "registerResult",
        "quoteUpdated",
        "symbol",
        "lastPrice",
        "orderPlaced",
        "OrderInfo",
        "order",
        "orderUpdated",
        "orderHistoryLoaded",
        "QList<OrderInfo>",
        "orders",
        "orderRejected",
        "reason",
        "positionsLoaded",
        "QList<PositionInfo>",
        "positions",
        "loggedOut",
        "accountFrozen",
        "login",
        "user",
        "password",
        "registerUser",
        "realName",
        "email",
        "phone",
        "idCard",
        "address",
        "queryQuote",
        "placeOrder",
        "side",
        "quantity",
        "price",
        "cancelOrder",
        "orderId",
        "loadOrderHistory",
        "logout"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'loginResult'
        QtMocHelpers::SignalData<void(bool, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 }, { QMetaType::QString, 4 },
        }}),
        // Signal 'registerResult'
        QtMocHelpers::SignalData<void(bool, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 }, { QMetaType::QString, 4 },
        }}),
        // Signal 'quoteUpdated'
        QtMocHelpers::SignalData<void(const QString &, double)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::Double, 8 },
        }}),
        // Signal 'orderPlaced'
        QtMocHelpers::SignalData<void(const OrderInfo &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Signal 'orderUpdated'
        QtMocHelpers::SignalData<void(const OrderInfo &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Signal 'orderHistoryLoaded'
        QtMocHelpers::SignalData<void(const QVector<OrderInfo> &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Signal 'orderRejected'
        QtMocHelpers::SignalData<void(const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 },
        }}),
        // Signal 'positionsLoaded'
        QtMocHelpers::SignalData<void(const QVector<PositionInfo> &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Signal 'loggedOut'
        QtMocHelpers::SignalData<void(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'accountFrozen'
        QtMocHelpers::SignalData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'login'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 }, { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
            { QMetaType::QString, 29 }, { QMetaType::QString, 30 }, { QMetaType::QString, 31 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &)>(26, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 }, { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
            { QMetaType::QString, 29 }, { QMetaType::QString, 30 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, const QString &, const QString &)>(26, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 }, { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
            { QMetaType::QString, 29 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, const QString &)>(26, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 }, { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(26, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 }, { QMetaType::QString, 27 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(26, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 },
        }}),
        // Method 'queryQuote'
        QtMocHelpers::MethodData<void(const QString &)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Method 'placeOrder'
        QtMocHelpers::MethodData<void(const QString &, const QString &, int, double)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 34 }, { QMetaType::Int, 35 }, { QMetaType::Double, 36 },
        }}),
        // Method 'cancelOrder'
        QtMocHelpers::MethodData<void(const QString &)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 38 },
        }}),
        // Method 'loadOrderHistory'
        QtMocHelpers::MethodData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'logout'
        QtMocHelpers::MethodData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Client, qt_meta_tag_ZN6ClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Client::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6ClientE_t>.metaTypes,
    nullptr
} };

void Client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Client *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->loginResult((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->registerResult((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->quoteUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 3: _t->orderPlaced((*reinterpret_cast< std::add_pointer_t<OrderInfo>>(_a[1]))); break;
        case 4: _t->orderUpdated((*reinterpret_cast< std::add_pointer_t<OrderInfo>>(_a[1]))); break;
        case 5: _t->orderHistoryLoaded((*reinterpret_cast< std::add_pointer_t<QList<OrderInfo>>>(_a[1]))); break;
        case 6: _t->orderRejected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->positionsLoaded((*reinterpret_cast< std::add_pointer_t<QList<PositionInfo>>>(_a[1]))); break;
        case 8: _t->loggedOut((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->accountFrozen(); break;
        case 10: _t->login((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[7]))); break;
        case 12: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[6]))); break;
        case 13: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5]))); break;
        case 14: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 15: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 16: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 17: _t->queryQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->placeOrder((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 19: _t->cancelOrder((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->loadOrderHistory(); break;
        case 21: _t->logout(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Client::*)(bool , const QString & )>(_a, &Client::loginResult, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(bool , const QString & )>(_a, &Client::registerResult, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const QString & , double )>(_a, &Client::quoteUpdated, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const OrderInfo & )>(_a, &Client::orderPlaced, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const OrderInfo & )>(_a, &Client::orderUpdated, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const QVector<OrderInfo> & )>(_a, &Client::orderHistoryLoaded, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const QString & )>(_a, &Client::orderRejected, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const QVector<PositionInfo> & )>(_a, &Client::positionsLoaded, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)(const QString & )>(_a, &Client::loggedOut, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Client::*)()>(_a, &Client::accountFrozen, 9))
            return;
    }
}

const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void Client::loginResult(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void Client::registerResult(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void Client::quoteUpdated(const QString & _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void Client::orderPlaced(const OrderInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Client::orderUpdated(const OrderInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Client::orderHistoryLoaded(const QVector<OrderInfo> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void Client::orderRejected(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Client::positionsLoaded(const QVector<PositionInfo> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Client::loggedOut(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void Client::accountFrozen()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
