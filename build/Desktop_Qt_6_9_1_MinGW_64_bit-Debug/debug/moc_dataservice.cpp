/****************************************************************************
** Meta object code from reading C++ file 'dataservice.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../head/dataservice.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataservice.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11DataServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto DataService::qt_create_metaobjectdata<qt_meta_tag_ZN11DataServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DataService",
        "quoteUpdated",
        "",
        "QuoteData",
        "quote",
        "quoteListUpdated",
        "QList<QuoteData>",
        "quotes",
        "kLineDataReceived",
        "symbol",
        "period",
        "QList<KLineData>",
        "data",
        "tradeDetailsReceived",
        "QList<TradeDetail>",
        "details",
        "companyInfoReceived",
        "CompanyInfo",
        "info",
        "financialDataReceived",
        "QMap<QString,QVariant>",
        "companyListUpdated",
        "symbols",
        "indexDataUpdated",
        "QList<IndexData>",
        "indices",
        "sectorDataUpdated",
        "QList<SectorData>",
        "sectors",
        "updateRealTimeData",
        "startRealTimeQuotes",
        "stopRealTimeQuotes",
        "subscribeQuote",
        "unsubscribeQuote",
        "getQuoteList",
        "getQuote",
        "requestKLineData",
        "count",
        "requestTradeDetails",
        "startTime",
        "endTime",
        "requestCompanyInfo",
        "requestFinancialData",
        "loadCompanyList",
        "requestIndexData",
        "requestSectorData",
        "getIndexList",
        "getSectorList"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'quoteUpdated'
        QtMocHelpers::SignalData<void(const QuoteData &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'quoteListUpdated'
        QtMocHelpers::SignalData<void(const QVector<QuoteData> &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'kLineDataReceived'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QVector<KLineData> &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { 0x80000000 | 11, 12 },
        }}),
        // Signal 'tradeDetailsReceived'
        QtMocHelpers::SignalData<void(const QString &, const QVector<TradeDetail> &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { 0x80000000 | 14, 15 },
        }}),
        // Signal 'companyInfoReceived'
        QtMocHelpers::SignalData<void(const CompanyInfo &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Signal 'financialDataReceived'
        QtMocHelpers::SignalData<void(const QString &, const QMap<QString,QVariant> &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { 0x80000000 | 20, 12 },
        }}),
        // Signal 'companyListUpdated'
        QtMocHelpers::SignalData<void(const QStringList &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 22 },
        }}),
        // Signal 'indexDataUpdated'
        QtMocHelpers::SignalData<void(const QVector<IndexData> &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 25 },
        }}),
        // Signal 'sectorDataUpdated'
        QtMocHelpers::SignalData<void(const QVector<SectorData> &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 27, 28 },
        }}),
        // Slot 'updateRealTimeData'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'startRealTimeQuotes'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stopRealTimeQuotes'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'subscribeQuote'
        QtMocHelpers::MethodData<void(const QString &)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'unsubscribeQuote'
        QtMocHelpers::MethodData<void(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'getQuoteList'
        QtMocHelpers::MethodData<QVector<QuoteData>() const>(34, 2, QMC::AccessPublic, 0x80000000 | 6),
        // Method 'getQuote'
        QtMocHelpers::MethodData<QuoteData(const QString &) const>(35, 2, QMC::AccessPublic, 0x80000000 | 3, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestKLineData'
        QtMocHelpers::MethodData<void(const QString &, const QString &, int)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { QMetaType::Int, 37 },
        }}),
        // Method 'requestKLineData'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(36, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Method 'requestTradeDetails'
        QtMocHelpers::MethodData<void(const QString &, const QDateTime &, const QDateTime &)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QDateTime, 39 }, { QMetaType::QDateTime, 40 },
        }}),
        // Method 'requestCompanyInfo'
        QtMocHelpers::MethodData<void(const QString &)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestFinancialData'
        QtMocHelpers::MethodData<void(const QString &)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'loadCompanyList'
        QtMocHelpers::MethodData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestIndexData'
        QtMocHelpers::MethodData<void()>(44, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestSectorData'
        QtMocHelpers::MethodData<void()>(45, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'getIndexList'
        QtMocHelpers::MethodData<QVector<IndexData>() const>(46, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'getSectorList'
        QtMocHelpers::MethodData<QVector<SectorData>() const>(47, 2, QMC::AccessPublic, 0x80000000 | 27),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DataService, qt_meta_tag_ZN11DataServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DataService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11DataServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11DataServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11DataServiceE_t>.metaTypes,
    nullptr
} };

void DataService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DataService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->quoteUpdated((*reinterpret_cast< std::add_pointer_t<QuoteData>>(_a[1]))); break;
        case 1: _t->quoteListUpdated((*reinterpret_cast< std::add_pointer_t<QList<QuoteData>>>(_a[1]))); break;
        case 2: _t->kLineDataReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QList<KLineData>>>(_a[3]))); break;
        case 3: _t->tradeDetailsReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<TradeDetail>>>(_a[2]))); break;
        case 4: _t->companyInfoReceived((*reinterpret_cast< std::add_pointer_t<CompanyInfo>>(_a[1]))); break;
        case 5: _t->financialDataReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QMap<QString,QVariant>>>(_a[2]))); break;
        case 6: _t->companyListUpdated((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 7: _t->indexDataUpdated((*reinterpret_cast< std::add_pointer_t<QList<IndexData>>>(_a[1]))); break;
        case 8: _t->sectorDataUpdated((*reinterpret_cast< std::add_pointer_t<QList<SectorData>>>(_a[1]))); break;
        case 9: _t->updateRealTimeData(); break;
        case 10: _t->startRealTimeQuotes(); break;
        case 11: _t->stopRealTimeQuotes(); break;
        case 12: _t->subscribeQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->unsubscribeQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: { QList<QuoteData> _r = _t->getQuoteList();
            if (_a[0]) *reinterpret_cast< QList<QuoteData>*>(_a[0]) = std::move(_r); }  break;
        case 15: { QuoteData _r = _t->getQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QuoteData*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->requestKLineData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 17: _t->requestKLineData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 18: _t->requestTradeDetails((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[3]))); break;
        case 19: _t->requestCompanyInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->requestFinancialData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->loadCompanyList(); break;
        case 22: _t->requestIndexData(); break;
        case 23: _t->requestSectorData(); break;
        case 24: { QList<IndexData> _r = _t->getIndexList();
            if (_a[0]) *reinterpret_cast< QList<IndexData>*>(_a[0]) = std::move(_r); }  break;
        case 25: { QList<SectorData> _r = _t->getSectorList();
            if (_a[0]) *reinterpret_cast< QList<SectorData>*>(_a[0]) = std::move(_r); }  break;
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
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QuoteData >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QuoteData> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<KLineData> >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<TradeDetail> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< CompanyInfo >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<IndexData> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<SectorData> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QuoteData & )>(_a, &DataService::quoteUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QVector<QuoteData> & )>(_a, &DataService::quoteListUpdated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QString & , const QString & , const QVector<KLineData> & )>(_a, &DataService::kLineDataReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QString & , const QVector<TradeDetail> & )>(_a, &DataService::tradeDetailsReceived, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const CompanyInfo & )>(_a, &DataService::companyInfoReceived, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QString & , const QMap<QString,QVariant> & )>(_a, &DataService::financialDataReceived, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QStringList & )>(_a, &DataService::companyListUpdated, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QVector<IndexData> & )>(_a, &DataService::indexDataUpdated, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QVector<SectorData> & )>(_a, &DataService::sectorDataUpdated, 8))
            return;
    }
}

const QMetaObject *DataService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11DataServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void DataService::quoteUpdated(const QuoteData & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DataService::quoteListUpdated(const QVector<QuoteData> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void DataService::kLineDataReceived(const QString & _t1, const QString & _t2, const QVector<KLineData> & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}

// SIGNAL 3
void DataService::tradeDetailsReceived(const QString & _t1, const QVector<TradeDetail> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void DataService::companyInfoReceived(const CompanyInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void DataService::financialDataReceived(const QString & _t1, const QMap<QString,QVariant> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void DataService::companyListUpdated(const QStringList & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void DataService::indexDataUpdated(const QVector<IndexData> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void DataService::sectorDataUpdated(const QVector<SectorData> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}
QT_WARNING_POP
