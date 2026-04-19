/****************************************************************************
** Meta object code from reading C++ file 'userservice.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../head/userservice.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userservice.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11UserServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto UserService::qt_create_metaobjectdata<qt_meta_tag_ZN11UserServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UserService",
        "userInfoLoaded",
        "",
        "UserInfo",
        "userInfo",
        "userInfoUpdated",
        "accountInfoLoaded",
        "AccountInfo",
        "accountInfo",
        "accountInfoUpdated",
        "cashFlowRecordsLoaded",
        "QList<CashFlowRecord>",
        "records",
        "tradingStatsLoaded",
        "TradingStats",
        "stats",
        "assetOverviewUpdated",
        "account",
        "QList<PositionInfo>",
        "positions",
        "recentRecords",
        "loadUserInfo",
        "username",
        "updateUserInfo",
        "saveUserInfoToDB",
        "getUserInfo",
        "loadAccountInfo",
        "updateAccountInfo",
        "availableCash",
        "getAccountInfo",
        "calculateMarketValue",
        "loadCashFlowRecords",
        "limit",
        "addCashFlowRecord",
        "CashFlowRecord",
        "record",
        "getCashFlowRecords",
        "loadTradingStats",
        "updateTradingStats",
        "OrderInfo",
        "order",
        "getTradingStats",
        "loadAssetOverview",
        "refreshAssetData",
        "clearAllCaches"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'userInfoLoaded'
        QtMocHelpers::SignalData<void(const UserInfo &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'userInfoUpdated'
        QtMocHelpers::SignalData<void(const UserInfo &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'accountInfoLoaded'
        QtMocHelpers::SignalData<void(const AccountInfo &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'accountInfoUpdated'
        QtMocHelpers::SignalData<void(const AccountInfo &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'cashFlowRecordsLoaded'
        QtMocHelpers::SignalData<void(const QVector<CashFlowRecord> &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Signal 'tradingStatsLoaded'
        QtMocHelpers::SignalData<void(const TradingStats &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Signal 'assetOverviewUpdated'
        QtMocHelpers::SignalData<void(const AccountInfo &, const QVector<PositionInfo> &, const QVector<CashFlowRecord> &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 17 }, { 0x80000000 | 18, 19 }, { 0x80000000 | 11, 20 },
        }}),
        // Method 'loadUserInfo'
        QtMocHelpers::MethodData<void(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'updateUserInfo'
        QtMocHelpers::MethodData<void(const UserInfo &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'saveUserInfoToDB'
        QtMocHelpers::MethodData<bool(const UserInfo &)>(24, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'getUserInfo'
        QtMocHelpers::MethodData<UserInfo(const QString &) const>(25, 2, QMC::AccessPublic, 0x80000000 | 3, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'loadAccountInfo'
        QtMocHelpers::MethodData<void(const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'updateAccountInfo'
        QtMocHelpers::MethodData<void(const QString &, double)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::Double, 28 },
        }}),
        // Method 'getAccountInfo'
        QtMocHelpers::MethodData<AccountInfo(const QString &) const>(29, 2, QMC::AccessPublic, 0x80000000 | 7, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'calculateMarketValue'
        QtMocHelpers::MethodData<void(const QString &, const QVector<PositionInfo> &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { 0x80000000 | 18, 19 },
        }}),
        // Method 'loadCashFlowRecords'
        QtMocHelpers::MethodData<void(const QString &, int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::Int, 32 },
        }}),
        // Method 'loadCashFlowRecords'
        QtMocHelpers::MethodData<void(const QString &)>(31, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'addCashFlowRecord'
        QtMocHelpers::MethodData<void(const CashFlowRecord &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 34, 35 },
        }}),
        // Method 'getCashFlowRecords'
        QtMocHelpers::MethodData<QVector<CashFlowRecord>(const QString &) const>(36, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'loadTradingStats'
        QtMocHelpers::MethodData<void(const QString &)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'updateTradingStats'
        QtMocHelpers::MethodData<void(const QString &, const OrderInfo &)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { 0x80000000 | 39, 40 },
        }}),
        // Method 'getTradingStats'
        QtMocHelpers::MethodData<TradingStats(const QString &) const>(41, 2, QMC::AccessPublic, 0x80000000 | 14, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'loadAssetOverview'
        QtMocHelpers::MethodData<void(const QString &)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'refreshAssetData'
        QtMocHelpers::MethodData<void(const QString &)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Method 'clearAllCaches'
        QtMocHelpers::MethodData<void()>(44, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<UserService, qt_meta_tag_ZN11UserServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject UserService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11UserServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11UserServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11UserServiceE_t>.metaTypes,
    nullptr
} };

void UserService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UserService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->userInfoLoaded((*reinterpret_cast< std::add_pointer_t<UserInfo>>(_a[1]))); break;
        case 1: _t->userInfoUpdated((*reinterpret_cast< std::add_pointer_t<UserInfo>>(_a[1]))); break;
        case 2: _t->accountInfoLoaded((*reinterpret_cast< std::add_pointer_t<AccountInfo>>(_a[1]))); break;
        case 3: _t->accountInfoUpdated((*reinterpret_cast< std::add_pointer_t<AccountInfo>>(_a[1]))); break;
        case 4: _t->cashFlowRecordsLoaded((*reinterpret_cast< std::add_pointer_t<QList<CashFlowRecord>>>(_a[1]))); break;
        case 5: _t->tradingStatsLoaded((*reinterpret_cast< std::add_pointer_t<TradingStats>>(_a[1]))); break;
        case 6: _t->assetOverviewUpdated((*reinterpret_cast< std::add_pointer_t<AccountInfo>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<PositionInfo>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QList<CashFlowRecord>>>(_a[3]))); break;
        case 7: _t->loadUserInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->updateUserInfo((*reinterpret_cast< std::add_pointer_t<UserInfo>>(_a[1]))); break;
        case 9: { bool _r = _t->saveUserInfoToDB((*reinterpret_cast< std::add_pointer_t<UserInfo>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { UserInfo _r = _t->getUserInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< UserInfo*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->loadAccountInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->updateAccountInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 13: { AccountInfo _r = _t->getAccountInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< AccountInfo*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->calculateMarketValue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<PositionInfo>>>(_a[2]))); break;
        case 15: _t->loadCashFlowRecords((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 16: _t->loadCashFlowRecords((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->addCashFlowRecord((*reinterpret_cast< std::add_pointer_t<CashFlowRecord>>(_a[1]))); break;
        case 18: { QList<CashFlowRecord> _r = _t->getCashFlowRecords((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<CashFlowRecord>*>(_a[0]) = std::move(_r); }  break;
        case 19: _t->loadTradingStats((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->updateTradingStats((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<OrderInfo>>(_a[2]))); break;
        case 21: { TradingStats _r = _t->getTradingStats((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< TradingStats*>(_a[0]) = std::move(_r); }  break;
        case 22: _t->loadAssetOverview((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->refreshAssetData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->clearAllCaches(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< UserInfo >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< UserInfo >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< AccountInfo >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< AccountInfo >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<CashFlowRecord> >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< TradingStats >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< AccountInfo >(); break;
            case 2:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<CashFlowRecord> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< UserInfo >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< UserInfo >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< CashFlowRecord >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const UserInfo & )>(_a, &UserService::userInfoLoaded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const UserInfo & )>(_a, &UserService::userInfoUpdated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const AccountInfo & )>(_a, &UserService::accountInfoLoaded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const AccountInfo & )>(_a, &UserService::accountInfoUpdated, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const QVector<CashFlowRecord> & )>(_a, &UserService::cashFlowRecordsLoaded, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const TradingStats & )>(_a, &UserService::tradingStatsLoaded, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserService::*)(const AccountInfo & , const QVector<PositionInfo> & , const QVector<CashFlowRecord> & )>(_a, &UserService::assetOverviewUpdated, 6))
            return;
    }
}

const QMetaObject *UserService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11UserServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UserService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void UserService::userInfoLoaded(const UserInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void UserService::userInfoUpdated(const UserInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void UserService::accountInfoLoaded(const AccountInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void UserService::accountInfoUpdated(const AccountInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void UserService::cashFlowRecordsLoaded(const QVector<CashFlowRecord> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void UserService::tradingStatsLoaded(const TradingStats & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void UserService::assetOverviewUpdated(const AccountInfo & _t1, const QVector<PositionInfo> & _t2, const QVector<CashFlowRecord> & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
