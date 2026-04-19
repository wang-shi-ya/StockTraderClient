#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QMap>
#include <QString>
#include "quotedata.h"
#include "dbmanager.h"

class DataService : public QObject {
    Q_OBJECT

public:
    explicit DataService(DbManager *dbManager = nullptr, QObject *parent = nullptr);
    ~DataService();

    // 实时行情相关
    Q_INVOKABLE void startRealTimeQuotes();
    Q_INVOKABLE void stopRealTimeQuotes();
    Q_INVOKABLE void subscribeQuote(const QString &symbol);
    Q_INVOKABLE void unsubscribeQuote(const QString &symbol);
    Q_INVOKABLE QVector<QuoteData> getQuoteList() const;
    Q_INVOKABLE QuoteData getQuote(const QString &symbol) const;

    // 历史数据相关
    Q_INVOKABLE void requestKLineData(const QString &symbol, const QString &period, int count = 100);
    Q_INVOKABLE void requestTradeDetails(const QString &symbol, const QDateTime &startTime, const QDateTime &endTime);

    // 基本面信息相关
    Q_INVOKABLE void requestCompanyInfo(const QString &symbol);
    Q_INVOKABLE void requestFinancialData(const QString &symbol);
    Q_INVOKABLE void loadCompanyList();

    // 指数和板块相关
    Q_INVOKABLE void requestIndexData();
    Q_INVOKABLE void requestSectorData();
    Q_INVOKABLE QVector<IndexData> getIndexList() const;
    Q_INVOKABLE QVector<SectorData> getSectorList() const;

signals:
    // 实时行情信号
    void quoteUpdated(const QuoteData &quote);
    void quoteListUpdated(const QVector<QuoteData> &quotes);

    // 历史数据信号
    void kLineDataReceived(const QString &symbol, const QString &period, const QVector<KLineData> &data);
    void tradeDetailsReceived(const QString &symbol, const QVector<TradeDetail> &details);

    // 基本面信息信号
    void companyInfoReceived(const CompanyInfo &info);
    void financialDataReceived(const QString &symbol, const QMap<QString, QVariant> &data);
    void companyListUpdated(const QStringList &symbols);

    // 指数和板块信号
    void indexDataUpdated(const QVector<IndexData> &indices);
    void sectorDataUpdated(const QVector<SectorData> &sectors);

private slots:
    void updateRealTimeData();

private:
    // 模拟数据生成
    QuoteData generateRandomQuote(const QString &symbol) const;
    KLineData generateRandomKLine(const QDateTime &time, const QString &period) const;
    TradeDetail generateRandomTrade(const QDateTime &time) const;
    CompanyInfo generateRandomCompanyInfo(const QString &symbol) const;
    IndexData generateRandomIndex(const QString &code) const;
    SectorData generateRandomSector(const QString &code) const;

    // 数据存储
    QMap<QString, QuoteData> m_quotes;
    QMap<QString, QVector<KLineData>> m_klineData;
    QMap<QString, QVector<TradeDetail>> m_tradeDetails;
    QMap<QString, CompanyInfo> m_companyInfo;
    QVector<IndexData> m_indices;
    QVector<SectorData> m_sectors;

    // 定时器
    QTimer *m_realTimeTimer;
    QVector<QString> m_subscribedSymbols;

    // 模拟数据基础价格
    QMap<QString, double> m_basePrices;

    // 数据库管理器
    DbManager *m_dbManager;
};
