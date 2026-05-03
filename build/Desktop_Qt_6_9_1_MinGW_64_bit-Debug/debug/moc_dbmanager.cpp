/****************************************************************************
** Meta object code from reading C++ file 'dbmanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../head/dbmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dbmanager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9DbManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto DbManager::qt_create_metaobjectdata<qt_meta_tag_ZN9DbManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DbManager",
        "initialized",
        "",
        "success",
        "errorOccurred",
        "message",
        "companyInfoLoaded",
        "symbol",
        "name",
        "industry",
        "sector",
        "totalShares",
        "circulatingShares",
        "marketCap",
        "circulatingCap",
        "pe",
        "pb",
        "roe",
        "revenue",
        "netProfit",
        "reportDate",
        "companyInfoSaved",
        "announcementsLoaded",
        "text",
        "userInfoLoaded",
        "username",
        "realName",
        "email",
        "phone",
        "idCard",
        "address",
        "registerTime",
        "lastLoginTime",
        "status",
        "watchedSymbolsLoaded",
        "symbols"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'initialized'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'companyInfoLoaded'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &, const QString &, double, double, double, double, double, double, double, double, double, const QDateTime &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
            { QMetaType::Double, 11 }, { QMetaType::Double, 12 }, { QMetaType::Double, 13 }, { QMetaType::Double, 14 },
            { QMetaType::Double, 15 }, { QMetaType::Double, 16 }, { QMetaType::Double, 17 }, { QMetaType::Double, 18 },
            { QMetaType::Double, 19 }, { QMetaType::QDateTime, 20 },
        }}),
        // Signal 'companyInfoSaved'
        QtMocHelpers::SignalData<void(bool)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'announcementsLoaded'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 23 },
        }}),
        // Signal 'userInfoLoaded'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, const QDateTime &, const QDateTime &, const QString &)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 25 }, { QMetaType::QString, 26 }, { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
            { QMetaType::QString, 29 }, { QMetaType::QString, 30 }, { QMetaType::QDateTime, 31 }, { QMetaType::QDateTime, 32 },
            { QMetaType::QString, 33 },
        }}),
        // Signal 'watchedSymbolsLoaded'
        QtMocHelpers::SignalData<void(const QString &, const QStringList &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 25 }, { QMetaType::QStringList, 35 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DbManager, qt_meta_tag_ZN9DbManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DbManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DbManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DbManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9DbManagerE_t>.metaTypes,
    nullptr
} };

void DbManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DbManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->initialized((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->companyInfoLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[9])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[10])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[11])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[12])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[13])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[14]))); break;
        case 3: _t->companyInfoSaved((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->announcementsLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->userInfoLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[9]))); break;
        case 6: _t->watchedSymbolsLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(bool )>(_a, &DbManager::initialized, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(const QString & )>(_a, &DbManager::errorOccurred, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(const QString & , const QString & , const QString & , const QString & , double , double , double , double , double , double , double , double , double , const QDateTime & )>(_a, &DbManager::companyInfoLoaded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(bool )>(_a, &DbManager::companyInfoSaved, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(const QString & , const QString & )>(_a, &DbManager::announcementsLoaded, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(const QString & , const QString & , const QString & , const QString & , const QString & , const QString & , const QDateTime & , const QDateTime & , const QString & )>(_a, &DbManager::userInfoLoaded, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (DbManager::*)(const QString & , const QStringList & )>(_a, &DbManager::watchedSymbolsLoaded, 6))
            return;
    }
}

const QMetaObject *DbManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DbManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DbManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DbManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DbManager::initialized(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DbManager::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void DbManager::companyInfoLoaded(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, double _t5, double _t6, double _t7, double _t8, double _t9, double _t10, double _t11, double _t12, double _t13, const QDateTime & _t14)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11, _t12, _t13, _t14);
}

// SIGNAL 3
void DbManager::companyInfoSaved(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void DbManager::announcementsLoaded(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void DbManager::userInfoLoaded(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, const QString & _t6, const QDateTime & _t7, const QDateTime & _t8, const QString & _t9)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
}

// SIGNAL 6
void DbManager::watchedSymbolsLoaded(const QString & _t1, const QStringList & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}
QT_WARNING_POP
