#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QMap>
#include <QString>
#include "quotedata.h"

class DataService : public QObject
{
    Q_OBJECT

public:
    explicit DataService(QObject *parent = nullptr);
    ~DataService();

    Q_INVOKABLE void startRealTimeQuotes();
    Q_INVOKABLE void stopRealTimeQuotes();
    Q_INVOKABLE void subscribeQuote(const QString &symbol);
    Q_INVOKABLE void unsubscribeQuote(const QString &symbol);
    Q_INVOKABLE QVector<QuoteData> getQuoteList() const;
    Q_INVOKABLE QuoteData getQuote(const QString &symbol) const;

    Q_INVOKABLE void requestKLineData(const QString &symbol, const QString &period, int count = 100);
    Q_INVOKABLE void requestTradeDetails(const QString &symbol, const QDateTime &startTime, const QDateTime &endTime);
    Q_INVOKABLE void requestCompanyInfo(const QString &symbol);
    Q_INVOKABLE void requestFinancialData(const QString &symbol);
    Q_INVOKABLE void loadCompanyList();
    Q_INVOKABLE void requestIndexData();
    Q_INVOKABLE void requestSectorData();
    Q_INVOKABLE QVector<IndexData> getIndexList() const;
    Q_INVOKABLE QVector<SectorData> getSectorList() const;
    Q_INVOKABLE QStringList getAllStockSymbols() const;
    Q_INVOKABLE QString getStockName(const QString &symbol) const;
    Q_INVOKABLE void requestCompanyAnnouncements(const QString &symbol);
    Q_INVOKABLE void requestNewsList(const QString &category = "all", int page = 1, int size = 20, const QString &keyword = "");
    Q_INVOKABLE void requestStockNews(const QString &symbol);
    Q_INVOKABLE void requestNewsDetail(int id);
    Q_INVOKABLE void requestNewsRefresh();

signals:
    void quoteUpdated(const QuoteData &quote);
    void quoteListUpdated(const QVector<QuoteData> &quotes);
    void kLineDataReceived(const QString &symbol, const QString &period, const QVector<KLineData> &data);
    void tradeDetailsReceived(const QString &symbol, const QVector<TradeDetail> &details);
    void companyInfoReceived(const CompanyInfo &info);
    void financialDataReceived(const QString &symbol, const QMap<QString, QVariant> &data);
    void companyListUpdated(const QStringList &symbols);
    void companyAnnouncementsReceived(const QString &symbol, const QJsonArray &announcements);
    void indexDataUpdated(const QVector<IndexData> &indices);
    void sectorDataUpdated(const QVector<SectorData> &sectors);
    void newsListReceived(const QJsonArray &newsList, int total);
    void stockNewsReceived(const QString &symbol, const QJsonArray &news);
    void newsDetailReceived(int id, const QJsonObject &detail);
    void newsRefreshFinished();

private slots:
    void pollQuotes();
    void pollIndices();
    void pollSectors();

private:
    void requestQuoteHttp(const QString &symbol);

    // Parsing helpers
    static QuoteData parseQuoteJson(const QJsonObject &json);
    static KLineData parseKLineJson(const QJsonObject &json);
    static CompanyInfo parseCompanyJson(const QJsonObject &json);
    static IndexData parseIndexJson(const QJsonObject &json);
    static SectorData parseSectorJson(const QJsonObject &json);

    QMap<QString, QuoteData> m_quotes;
    QMap<QString, QVector<KLineData>> m_klineData;
    QMap<QString, QVector<TradeDetail>> m_tradeDetails;
    QMap<QString, CompanyInfo> m_companyInfo;
    QVector<IndexData> m_indices;
    QVector<SectorData> m_sectors;
    QVector<QString> m_subscribedSymbols;
    QMap<QString, QString> m_stockNames;
    QStringList m_allSymbols;

    QTimer *m_quoteTimer;
    QTimer *m_indexTimer;
    QTimer *m_sectorTimer;
    bool m_running = false;
};
