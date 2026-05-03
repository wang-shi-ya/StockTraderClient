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
        "companyAnnouncementsReceived",
        "announcements",
        "indexDataUpdated",
        "QList<IndexData>",
        "indices",
        "sectorDataUpdated",
        "QList<SectorData>",
        "sectors",
        "newsListReceived",
        "newsList",
        "total",
        "stockNewsReceived",
        "news",
        "newsDetailReceived",
        "id",
        "detail",
        "newsRefreshFinished",
        "pollQuotes",
        "pollIndices",
        "pollSectors",
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
        "getSectorList",
        "getAllStockSymbols",
        "getStockName",
        "requestCompanyAnnouncements",
        "requestNewsList",
        "category",
        "page",
        "size",
        "keyword",
        "requestStockNews",
        "requestNewsDetail",
        "requestNewsRefresh"
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
        // Signal 'companyAnnouncementsReceived'
        QtMocHelpers::SignalData<void(const QString &, const QJsonArray &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QJsonArray, 24 },
        }}),
        // Signal 'indexDataUpdated'
        QtMocHelpers::SignalData<void(const QVector<IndexData> &)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 26, 27 },
        }}),
        // Signal 'sectorDataUpdated'
        QtMocHelpers::SignalData<void(const QVector<SectorData> &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 29, 30 },
        }}),
        // Signal 'newsListReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &, int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 32 }, { QMetaType::Int, 33 },
        }}),
        // Signal 'stockNewsReceived'
        QtMocHelpers::SignalData<void(const QString &, const QJsonArray &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QJsonArray, 35 },
        }}),
        // Signal 'newsDetailReceived'
        QtMocHelpers::SignalData<void(int, const QJsonObject &)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 37 }, { QMetaType::QJsonObject, 38 },
        }}),
        // Signal 'newsRefreshFinished'
        QtMocHelpers::SignalData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pollQuotes'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pollIndices'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pollSectors'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'startRealTimeQuotes'
        QtMocHelpers::MethodData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stopRealTimeQuotes'
        QtMocHelpers::MethodData<void()>(44, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'subscribeQuote'
        QtMocHelpers::MethodData<void(const QString &)>(45, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'unsubscribeQuote'
        QtMocHelpers::MethodData<void(const QString &)>(46, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'getQuoteList'
        QtMocHelpers::MethodData<QVector<QuoteData>() const>(47, 2, QMC::AccessPublic, 0x80000000 | 6),
        // Method 'getQuote'
        QtMocHelpers::MethodData<QuoteData(const QString &) const>(48, 2, QMC::AccessPublic, 0x80000000 | 3, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestKLineData'
        QtMocHelpers::MethodData<void(const QString &, const QString &, int)>(49, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { QMetaType::Int, 50 },
        }}),
        // Method 'requestKLineData'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(49, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Method 'requestTradeDetails'
        QtMocHelpers::MethodData<void(const QString &, const QDateTime &, const QDateTime &)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QDateTime, 52 }, { QMetaType::QDateTime, 53 },
        }}),
        // Method 'requestCompanyInfo'
        QtMocHelpers::MethodData<void(const QString &)>(54, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestFinancialData'
        QtMocHelpers::MethodData<void(const QString &)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'loadCompanyList'
        QtMocHelpers::MethodData<void()>(56, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestIndexData'
        QtMocHelpers::MethodData<void()>(57, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestSectorData'
        QtMocHelpers::MethodData<void()>(58, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'getIndexList'
        QtMocHelpers::MethodData<QVector<IndexData>() const>(59, 2, QMC::AccessPublic, 0x80000000 | 26),
        // Method 'getSectorList'
        QtMocHelpers::MethodData<QVector<SectorData>() const>(60, 2, QMC::AccessPublic, 0x80000000 | 29),
        // Method 'getAllStockSymbols'
        QtMocHelpers::MethodData<QStringList() const>(61, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'getStockName'
        QtMocHelpers::MethodData<QString(const QString &) const>(62, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestCompanyAnnouncements'
        QtMocHelpers::MethodData<void(const QString &)>(63, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestNewsList'
        QtMocHelpers::MethodData<void(const QString &, int, int, const QString &)>(64, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 65 }, { QMetaType::Int, 66 }, { QMetaType::Int, 67 }, { QMetaType::QString, 68 },
        }}),
        // Method 'requestNewsList'
        QtMocHelpers::MethodData<void(const QString &, int, int)>(64, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 65 }, { QMetaType::Int, 66 }, { QMetaType::Int, 67 },
        }}),
        // Method 'requestNewsList'
        QtMocHelpers::MethodData<void(const QString &, int)>(64, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 65 }, { QMetaType::Int, 66 },
        }}),
        // Method 'requestNewsList'
        QtMocHelpers::MethodData<void(const QString &)>(64, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 65 },
        }}),
        // Method 'requestNewsList'
        QtMocHelpers::MethodData<void()>(64, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Method 'requestStockNews'
        QtMocHelpers::MethodData<void(const QString &)>(69, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'requestNewsDetail'
        QtMocHelpers::MethodData<void(int)>(70, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 37 },
        }}),
        // Method 'requestNewsRefresh'
        QtMocHelpers::MethodData<void()>(71, 2, QMC::AccessPublic, QMetaType::Void),
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
        case 7: _t->companyAnnouncementsReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[2]))); break;
        case 8: _t->indexDataUpdated((*reinterpret_cast< std::add_pointer_t<QList<IndexData>>>(_a[1]))); break;
        case 9: _t->sectorDataUpdated((*reinterpret_cast< std::add_pointer_t<QList<SectorData>>>(_a[1]))); break;
        case 10: _t->newsListReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 11: _t->stockNewsReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[2]))); break;
        case 12: _t->newsDetailReceived((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[2]))); break;
        case 13: _t->newsRefreshFinished(); break;
        case 14: _t->pollQuotes(); break;
        case 15: _t->pollIndices(); break;
        case 16: _t->pollSectors(); break;
        case 17: _t->startRealTimeQuotes(); break;
        case 18: _t->stopRealTimeQuotes(); break;
        case 19: _t->subscribeQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->unsubscribeQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: { QList<QuoteData> _r = _t->getQuoteList();
            if (_a[0]) *reinterpret_cast< QList<QuoteData>*>(_a[0]) = std::move(_r); }  break;
        case 22: { QuoteData _r = _t->getQuote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QuoteData*>(_a[0]) = std::move(_r); }  break;
        case 23: _t->requestKLineData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 24: _t->requestKLineData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 25: _t->requestTradeDetails((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QDateTime>>(_a[3]))); break;
        case 26: _t->requestCompanyInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->requestFinancialData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 28: _t->loadCompanyList(); break;
        case 29: _t->requestIndexData(); break;
        case 30: _t->requestSectorData(); break;
        case 31: { QList<IndexData> _r = _t->getIndexList();
            if (_a[0]) *reinterpret_cast< QList<IndexData>*>(_a[0]) = std::move(_r); }  break;
        case 32: { QList<SectorData> _r = _t->getSectorList();
            if (_a[0]) *reinterpret_cast< QList<SectorData>*>(_a[0]) = std::move(_r); }  break;
        case 33: { QStringList _r = _t->getAllStockSymbols();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 34: { QString _r = _t->getStockName((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 35: _t->requestCompanyAnnouncements((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 36: _t->requestNewsList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 37: _t->requestNewsList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 38: _t->requestNewsList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 39: _t->requestNewsList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 40: _t->requestNewsList(); break;
        case 41: _t->requestStockNews((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 42: _t->requestNewsDetail((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 43: _t->requestNewsRefresh(); break;
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
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<IndexData> >(); break;
            }
            break;
        case 9:
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
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QString & , const QJsonArray & )>(_a, &DataService::companyAnnouncementsReceived, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QVector<IndexData> & )>(_a, &DataService::indexDataUpdated, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QVector<SectorData> & )>(_a, &DataService::sectorDataUpdated, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QJsonArray & , int )>(_a, &DataService::newsListReceived, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(const QString & , const QJsonArray & )>(_a, &DataService::stockNewsReceived, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)(int , const QJsonObject & )>(_a, &DataService::newsDetailReceived, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (DataService::*)()>(_a, &DataService::newsRefreshFinished, 13))
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
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
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
void DataService::companyAnnouncementsReceived(const QString & _t1, const QJsonArray & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void DataService::indexDataUpdated(const QVector<IndexData> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void DataService::sectorDataUpdated(const QVector<SectorData> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void DataService::newsListReceived(const QJsonArray & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}

// SIGNAL 11
void DataService::stockNewsReceived(const QString & _t1, const QJsonArray & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2);
}

// SIGNAL 12
void DataService::newsDetailReceived(int _t1, const QJsonObject & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2);
}

// SIGNAL 13
void DataService::newsRefreshFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}
QT_WARNING_POP
