#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include <QJsonObject>

class DbManager : public QObject
{
    Q_OBJECT

public:
    explicit DbManager(QObject *parent = nullptr);
    ~DbManager() override = default;

    void initialize();
    bool isInitialized() const { return m_initialized; }

    // Sync cache-based getters — safe to call anytime after initialize() completes
    QString getStockName(const QString &symbol) const;
    QStringList getAllStockSymbols() const;
    QStringList getAllCompanySymbols() const;
    bool companyInfoExists(const QString &symbol) const;

    // Async HTTP methods — results delivered via signals
    void loadCompanyInfo(const QString &symbol);
    void saveCompanyInfo(const QString &symbol, const QString &name, const QString &industry,
                         const QString &sector, double totalShares, double circulatingShares,
                         double marketCap, double circulatingCap, double pe, double pb,
                         double roe, double revenue, double netProfit, const QDateTime &reportDate);
    void getCompanyAnnouncements(const QString &symbol);

    void loadUserInfo(const QString &username);
    void saveUserInfo(const QString &username, const QString &realName, const QString &email,
                      const QString &phone, const QString &idCard, const QString &address,
                      const QString &status = "正常");
    void updateLastLoginTime(const QString &username);

    // Watchlist
    void loadUserWatchedSymbols(const QString &username);
    void saveUserWatchedSymbol(const QString &username, const QString &symbol);
    void removeUserWatchedSymbol(const QString &username, const QString &symbol);
    void clearUserWatchedSymbols(const QString &username);

    // System log (fire-and-forget)
    void addSystemLog(const QString &operatorName, const QString &operationType, const QString &ipAddress = "");

signals:
    void initialized(bool success);
    void errorOccurred(const QString &message);

    // Company info
    void companyInfoLoaded(const QString &symbol, const QString &name, const QString &industry,
                           const QString &sector, double totalShares, double circulatingShares,
                           double marketCap, double circulatingCap, double pe, double pb,
                           double roe, double revenue, double netProfit, const QDateTime &reportDate);
    void companyInfoSaved(bool success);
    void announcementsLoaded(const QString &symbol, const QString &text);

    // User info
    void userInfoLoaded(const QString &username, const QString &realName, const QString &email,
                        const QString &phone, const QString &idCard, const QString &address,
                        const QDateTime &registerTime, const QDateTime &lastLoginTime, const QString &status);

    // Watchlist
    void watchedSymbolsLoaded(const QString &username, const QStringList &symbols);

private:
    void preloadCache();
    void parseAndEmitCompanyInfo(const QString &symbol, const QJsonObject &data);

    QMap<QString, QString> m_stockNames;
    QStringList m_allSymbols;
    bool m_initialized = false;
};
