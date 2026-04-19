#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include "quotedata.h"
#include "dataservice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KLineWidget; }
QT_END_NAMESPACE

class KLineWidget : public QWidget {
    Q_OBJECT

public:
    explicit KLineWidget(DataService *dataService, QWidget *parent = nullptr);
    ~KLineWidget();

    void setSymbol(const QString &symbol);

private slots:
    void onPeriodChanged();
    void onKLineDataReceived(const QString &symbol, const QString &period, const QVector<KLineData> &data);
    void onQuoteUpdated(const QuoteData &quote);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    void drawKLineChart(QPainter &painter, const QRect &chartRect);
    void drawVolumeChart(QPainter &painter, const QRect &chartRect);
    void drawCrosshair(QPainter &painter, const QRect &chartRect);
    void drawPriceAxis(QPainter &painter, const QRect &chartRect);
    void drawTimeAxis(QPainter &painter, const QRect &chartRect);
    void updateChart();
    QString formatPrice(double price) const;
    QString formatVolume(long long volume) const;
    QDateTime getTimeAtPosition(int x) const;
    double getPriceAtPosition(int y) const;
    int getPositionAtTime(const QDateTime &time) const;
    int getPositionAtPrice(double price) const;

private:
    Ui::KLineWidget *ui;
    DataService *m_dataService;
    QString m_currentSymbol;
    QString m_currentPeriod;
    QVector<KLineData> m_klineData;
    
    // UI组件
    QComboBox *m_symbolCombo;
    QComboBox *m_periodCombo;
    QPushButton *m_refreshButton;
    QLabel *m_infoLabel;
    
    // 图表参数
    int m_chartWidth;
    int m_chartHeight;
    int m_volumeHeight;
    int m_marginLeft;
    int m_marginRight;
    int m_marginTop;
    int m_marginBottom;
    
    // 数据范围
    double m_minPrice;
    double m_maxPrice;
    long long m_maxVolume;
    QDateTime m_startTime;
    QDateTime m_endTime;
    
    // 鼠标交互
    QPoint m_mousePos;
    bool m_showCrosshair;
    int m_selectedIndex;
    
    // 缩放和平移
    int m_visibleBars;
    int m_scrollOffset;
    double m_zoomFactor;
    
    // 定时器
    QTimer *m_updateTimer;
};
