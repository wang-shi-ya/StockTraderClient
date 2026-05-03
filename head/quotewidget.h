#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTimer>
#include "quotedata.h"
#include "dataservice.h"
#include "dbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QuoteWidget; }
QT_END_NAMESPACE

class QuoteWidget : public QWidget {
    Q_OBJECT

public:
    explicit QuoteWidget(DataService *dataService, DbManager *dbManager, QWidget *parent = nullptr);
    ~QuoteWidget();
    QStringList getWatchedSymbols() const;
    void setCurrentUsername(const QString &username);
    void clearWatchedSymbols();
    void refreshTheme();

signals:
    void symbolListChanged();

private slots:
    void onAddSymbol();
    void onRemoveSymbol();
    void onQuoteUpdated(const QuoteData &quote);
    void onQuoteListUpdated(const QVector<QuoteData> &quotes);
    void onIndexDataUpdated(const QVector<IndexData> &indices);
    void onSectorDataUpdated(const QVector<SectorData> &sectors);
    void refreshData();
    void onWatchedSymbolsLoaded(const QString &username, const QStringList &symbols);

private:
    void setupQuoteTable();
    void setupIndexTable();
    void setupSectorTable();
    void updateQuoteTable();
    void updateIndexTable();
    void updateSectorTable();
    QString formatPrice(double price) const;
    QString formatVolume(long long volume) const;
    QString formatPercent(double percent) const;
    QString formatSigned(double value) const;
    QString formatSignedPercent(double percent) const;
    void loadWatchedSymbolsFromDB();
    void addSymbolToDB(const QString &symbol);
    void removeSymbolFromDB(const QString &symbol);
    void loadStockListFromDB();
    void rebuildSymbolCombo();

private:
    Ui::QuoteWidget *ui;
    DataService *m_dataService;
    DbManager *m_dbManager;
    QString m_currentUsername;

    // 行情表格
    QTableWidget *m_quoteTable;
    QVector<QString> m_watchedSymbols;

    // 指数表格
    QTableWidget *m_indexTable;

    // 板块表格
    QTableWidget *m_sectorTable;

    // 控制组件
    QComboBox *m_symbolInput;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_refreshButton;
    QComboBox *m_periodCombo;

    // 数据存储
    QVector<QuoteData> m_quotes;
    QVector<IndexData> m_indices;
    QVector<SectorData> m_sectors;

    // 定时器
    QTimer *m_refreshTimer;
    QTimer *m_quoteThrottleTimer;

    // 股票列表
    QStringList m_allStockSymbols;
};
