#include "klinewidget.h"
#include "ui_klinewidget.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

KLineWidget::KLineWidget(DataService *dataService, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KLineWidget)
    , m_dataService(dataService)
    , m_currentSymbol("000001")
    , m_currentPeriod("1d")
    , m_chartWidth(800)
    , m_chartHeight(400)
    , m_volumeHeight(100)
    , m_marginLeft(60)
    , m_marginRight(20)
    , m_marginTop(20)
    , m_marginBottom(40)
    , m_minPrice(0)
    , m_maxPrice(100)
    , m_maxVolume(0)
    , m_showCrosshair(false)
    , m_selectedIndex(-1)
    , m_visibleBars(50)
    , m_scrollOffset(0)
    , m_zoomFactor(1.0)
    , m_updateTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // 获取UI组件引用
    m_symbolCombo = ui->symbolCombo;
    m_periodCombo = ui->periodCombo;
    m_refreshButton = ui->refreshButton;
    m_infoLabel = ui->infoLabel;
    
    // 设置chartWidget占满剩余空间
    if (ui->chartWidget) {
        ui->chartWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->chartWidget->setMouseTracking(true);
    }
    
    // 设置图表widget为自定义绘制区域
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // 初始化组合框
    m_symbolCombo->addItems({"000001", "000002", "600000", "600036", "000858", "600519"});
    m_symbolCombo->setCurrentText(m_currentSymbol);
    m_periodCombo->addItems({"1m", "5m", "15m", "30m", "1h", "1d", "1w"});
    m_periodCombo->setCurrentText(m_currentPeriod);
    
    // 连接数据服务信号
    connect(m_dataService, &DataService::kLineDataReceived, this, &KLineWidget::onKLineDataReceived);
    connect(m_dataService, &DataService::quoteUpdated, this, &KLineWidget::onQuoteUpdated);
    
    // 连接UI信号
    connect(m_symbolCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, [this](const QString &symbol) {
                setSymbol(symbol);
            });
    connect(m_periodCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, &KLineWidget::onPeriodChanged);
    connect(m_refreshButton, &QPushButton::clicked, this, [this]() {
        m_dataService->requestKLineData(m_currentSymbol, m_currentPeriod, 100);
    });
    
    // 设置定时器
    connect(m_updateTimer, &QTimer::timeout, this, &KLineWidget::updateChart);
    m_updateTimer->start(1000); // 每秒更新一次
    
    // 请求初始数据
    m_dataService->requestKLineData(m_currentSymbol, m_currentPeriod, 100);
}

KLineWidget::~KLineWidget() {
    m_updateTimer->stop();
    delete ui;
}

void KLineWidget::setSymbol(const QString &symbol) {
    m_currentSymbol = symbol;
    m_symbolCombo->setCurrentText(symbol);
    m_dataService->requestKLineData(m_currentSymbol, m_currentPeriod, 100);
}


void KLineWidget::onPeriodChanged() {
    m_currentPeriod = m_periodCombo->currentText();
    m_dataService->requestKLineData(m_currentSymbol, m_currentPeriod, 100);
}

void KLineWidget::onKLineDataReceived(const QString &symbol, const QString &period, const QVector<KLineData> &data) {
    if (symbol == m_currentSymbol && period == m_currentPeriod) {
        m_klineData = data;
        updateChart();
    }
}

void KLineWidget::onQuoteUpdated(const QuoteData &quote) {
    if (quote.symbol == m_currentSymbol) {
        // 更新最新价格信息
        updateChart();
    }
}

void KLineWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    
    QRect chartRect;
    if (ui && ui->chartWidget) {
        // 获取chartWidget的实际几何区域
        chartRect = ui->chartWidget->geometry();
        // 确保chartWidget占满剩余空间（在布局中应该已经自动处理）
        if (chartRect.isEmpty()) {
            // 如果geometry还没有设置，使用整个widget区域减去控制栏
            int controlHeight = 60; // 控制栏大致高度
            chartRect = QRect(0, controlHeight, width(), height() - controlHeight);
        }
    } else {
        // 如果没有chartWidget，使用整个widget区域减去控制栏
        int controlHeight = 60;
        chartRect = QRect(0, controlHeight, width(), height() - controlHeight);
    }
    
    // 更新图表尺寸
    m_chartWidth = chartRect.width();
    m_chartHeight = chartRect.height() - m_volumeHeight;
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 设置背景
    painter.fillRect(chartRect, QColor(20, 20, 20));
    
    if (m_klineData.isEmpty()) {
        painter.setPen(QColor(255, 255, 255));
        painter.drawText(chartRect, Qt::AlignCenter, "暂无数据");
        return;
    }
    
    // 直接绘制，不使用viewport转换
    // 绘制K线图
    drawKLineChart(painter, chartRect);
    
    // 绘制成交量图
    drawVolumeChart(painter, chartRect);
    
    // 绘制坐标轴
    drawPriceAxis(painter, chartRect);
    drawTimeAxis(painter, chartRect);
    
    // 绘制十字线
    if (m_showCrosshair) {
        drawCrosshair(painter, chartRect);
    }
}

void KLineWidget::mouseMoveEvent(QMouseEvent *event) {
    m_mousePos = event->pos();
    m_showCrosshair = true;
    
    // 获取图表区域
    QRect chartRect;
    if (ui && ui->chartWidget) {
        chartRect = ui->chartWidget->geometry();
    } else {
        chartRect = rect();
    }
    
    // 计算选中的K线（相对于图表区域）
    QPoint localPos = m_mousePos;
    if (chartRect.contains(localPos)) {
        int localX = localPos.x() - chartRect.left();
        if (localX >= m_marginLeft && localX < m_chartWidth - m_marginRight) {
            int barWidth = (m_chartWidth - m_marginLeft - m_marginRight) / m_visibleBars;
            if (barWidth > 0) {
                int localIndex = (localX - m_marginLeft) / barWidth;
                int startIndex = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
                int barIndex = startIndex + localIndex;
                
                if (barIndex >= 0 && barIndex < m_klineData.size()) {
                    m_selectedIndex = barIndex;
                    const KLineData &data = m_klineData[barIndex];
                    
                    QString info = QString("时间: %1 | 开: %2 | 高: %3 | 低: %4 | 收: %5 | 量: %6")
                                  .arg(data.dateTime.toString("yyyy-MM-dd hh:mm"))
                                  .arg(formatPrice(data.open))
                                  .arg(formatPrice(data.high))
                                  .arg(formatPrice(data.low))
                                  .arg(formatPrice(data.close))
                                  .arg(formatVolume(data.volume));
                    m_infoLabel->setText(info);
                }
            }
        }
    }
    
    update();
}

void KLineWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 可以添加点击事件处理
    }
}

void KLineWidget::wheelEvent(QWheelEvent *event) {
    // 缩放功能
    if (event->angleDelta().y() > 0) {
        m_zoomFactor *= 1.1;
        m_visibleBars = qMax(10, static_cast<int>(m_visibleBars / 1.1));
    } else {
        m_zoomFactor /= 1.1;
        m_visibleBars = qMin(200, static_cast<int>(m_visibleBars * 1.1));
    }
    
    updateChart();
    update();
}

void KLineWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    if (ui && ui->chartWidget) {
        // 确保chartWidget占满剩余空间
        QRect chartRect = ui->chartWidget->geometry();
        m_chartWidth = chartRect.width();
        m_chartHeight = chartRect.height() - m_volumeHeight;
    } else {
        // 如果没有chartWidget，使用整个widget区域减去控制栏
        int controlHeight = 60; // 控制栏大致高度
        m_chartWidth = width();
        m_chartHeight = height() - m_volumeHeight - controlHeight;
    }
    updateChart();
    update(); // 触发重绘
}

void KLineWidget::drawKLineChart(QPainter &painter, const QRect &chartRect) {
    if (m_klineData.isEmpty()) return;
    
    int barWidth = (m_chartWidth - m_marginLeft - m_marginRight) / m_visibleBars;
    if (barWidth <= 0) return;
    
    int startIndex = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
    int endIndex = qMin(m_klineData.size(), startIndex + m_visibleBars);
    
    for (int i = startIndex; i < endIndex; ++i) {
        const KLineData &data = m_klineData[i];
        int x = chartRect.left() + m_marginLeft + (i - startIndex) * barWidth + barWidth / 2;
        
        // 计算价格位置（相对于chartRect）
        int openY = chartRect.top() + getPositionAtPrice(data.open);
        int closeY = chartRect.top() + getPositionAtPrice(data.close);
        int highY = chartRect.top() + getPositionAtPrice(data.high);
        int lowY = chartRect.top() + getPositionAtPrice(data.low);
        
        // 设置颜色
        QColor color = data.close >= data.open ? QColor(255, 0, 0) : QColor(0, 255, 0);
        painter.setPen(QPen(color, 1));
        
        // 绘制影线
        painter.drawLine(x, highY, x, lowY);
        
        // 绘制实体
        int bodyHeight = qAbs(closeY - openY);
        if (bodyHeight < 2) bodyHeight = 2; // 最小高度
        
        QRect bodyRect(x - barWidth/2 + 1, qMin(openY, closeY), barWidth - 2, bodyHeight);
        painter.fillRect(bodyRect, color);
    }
}

void KLineWidget::drawVolumeChart(QPainter &painter, const QRect &chartRect) {
    if (m_klineData.isEmpty() || m_maxVolume <= 0) return;
    
    int barWidth = (m_chartWidth - m_marginLeft - m_marginRight) / m_visibleBars;
    if (barWidth <= 0) return;
    
    int startIndex = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
    int endIndex = qMin(m_klineData.size(), startIndex + m_visibleBars);
    
    int volumeY = chartRect.top() + m_chartHeight + m_marginTop;
    
    for (int i = startIndex; i < endIndex; ++i) {
        const KLineData &data = m_klineData[i];
        int x = chartRect.left() + m_marginLeft + (i - startIndex) * barWidth + barWidth / 2;
        
        // 计算成交量高度
        int volumeHeight = static_cast<int>((double)data.volume / m_maxVolume * m_volumeHeight);
        int y = volumeY + m_volumeHeight - volumeHeight;
        
        // 设置颜色
        QColor color = data.close >= data.open ? QColor(255, 100, 100) : QColor(100, 255, 100);
        painter.fillRect(x - barWidth/2 + 1, y, barWidth - 2, volumeHeight, color);
    }
}

void KLineWidget::drawCrosshair(QPainter &painter, const QRect &chartRect) {
    painter.setPen(QPen(QColor(255, 255, 255), 1, Qt::DashLine));
    
    // 确保鼠标位置在图表区域内
    QPoint localPos = m_mousePos;
    if (!chartRect.contains(localPos)) {
        return;
    }
    
    // 垂直线
    painter.drawLine(localPos.x(), chartRect.top() + m_marginTop, 
                     localPos.x(), chartRect.top() + m_chartHeight + m_marginTop);
    
    // 水平线
    painter.drawLine(chartRect.left() + m_marginLeft, localPos.y(), 
                     chartRect.left() + m_chartWidth - m_marginRight, localPos.y());
}

void KLineWidget::drawPriceAxis(QPainter &painter, const QRect &chartRect) {
    painter.setPen(QColor(255, 255, 255));
    
    // 绘制价格刻度
    int steps = 10;
    if (m_maxPrice <= m_minPrice) return;
    
    double priceStep = (m_maxPrice - m_minPrice) / steps;
    
    for (int i = 0; i <= steps; ++i) {
        double price = m_minPrice + i * priceStep;
        int y = chartRect.top() + getPositionAtPrice(price);
        
        painter.drawLine(chartRect.left() + m_marginLeft - 5, y, 
                         chartRect.left() + m_marginLeft, y);
        painter.drawText(chartRect.left(), y - 10, m_marginLeft - 10, 20, 
                        Qt::AlignRight | Qt::AlignVCenter, formatPrice(price));
    }
}

void KLineWidget::drawTimeAxis(QPainter &painter, const QRect &chartRect) {
    painter.setPen(QColor(255, 255, 255));
    
    if (m_klineData.isEmpty()) return;
    
    // 绘制时间刻度
    int steps = 5;
    int barWidth = (m_chartWidth - m_marginLeft - m_marginRight) / m_visibleBars;
    if (barWidth <= 0) return;
    
    int startIndex = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
    int endIndex = qMin(m_klineData.size(), startIndex + m_visibleBars);
    int visibleCount = endIndex - startIndex;
    
    if (visibleCount <= 0) return;
    
    int timeStep = visibleCount / steps;
    if (timeStep <= 0) timeStep = 1;
    
    for (int i = 0; i <= steps; ++i) {
        int localIndex = i * timeStep;
        int dataIndex = startIndex + localIndex;
        
        if (dataIndex >= 0 && dataIndex < m_klineData.size()) {
            int x = chartRect.left() + m_marginLeft + localIndex * barWidth + barWidth / 2;
            int y = chartRect.top() + m_chartHeight + m_marginTop;
            
            painter.drawLine(x, y, x, y + 5);
            painter.drawText(x - 50, y + 10, 100, 20, Qt::AlignCenter, 
                           m_klineData[dataIndex].dateTime.toString("MM-dd"));
        }
    }
}

void KLineWidget::updateChart() {
    if (m_klineData.isEmpty()) {
        m_minPrice = 0;
        m_maxPrice = 100;
        m_maxVolume = 0;
        update();
        return;
    }
    
    // 计算价格范围
    m_minPrice = m_klineData[0].low;
    m_maxPrice = m_klineData[0].high;
    m_maxVolume = m_klineData[0].volume;
    
    for (const KLineData &data : m_klineData) {
        m_minPrice = qMin(m_minPrice, data.low);
        m_maxPrice = qMax(m_maxPrice, data.high);
        m_maxVolume = qMax(m_maxVolume, data.volume);
    }
    
    // 添加一些边距
    double priceRange = m_maxPrice - m_minPrice;
    if (priceRange <= 0) {
        // 如果价格范围无效，使用默认值
        m_minPrice = m_maxPrice * 0.9;
        m_maxPrice = m_maxPrice * 1.1;
        if (m_maxPrice <= 0) {
            m_minPrice = 0;
            m_maxPrice = 100;
        }
    } else {
        m_minPrice -= priceRange * 0.1;
        m_maxPrice += priceRange * 0.1;
    }
    
    if (m_maxVolume <= 0) {
        m_maxVolume = 1; // 避免除零错误
    }
    
    m_startTime = m_klineData.first().dateTime;
    m_endTime = m_klineData.last().dateTime;
    
    update();
}

QString KLineWidget::formatPrice(double price) const {
    return QString::number(price, 'f', 2);
}

QString KLineWidget::formatVolume(long long volume) const {
    if (volume >= 100000000) {
        return QString::number(volume / 100000000.0, 'f', 2) + "亿";
    } else if (volume >= 10000) {
        return QString::number(volume / 10000.0, 'f', 2) + "万";
    } else {
        return QString::number(volume);
    }
}

QDateTime KLineWidget::getTimeAtPosition(int x) const {
    Q_UNUSED(x)
    // 根据x坐标计算时间
    return QDateTime::currentDateTime();
}

double KLineWidget::getPriceAtPosition(int y) const {
    // 根据y坐标计算价格
    double priceRange = m_maxPrice - m_minPrice;
    double ratio = (double)(m_chartHeight + m_marginTop - y) / m_chartHeight;
    return m_minPrice + ratio * priceRange;
}

int KLineWidget::getPositionAtTime(const QDateTime &time) const {
    Q_UNUSED(time)
    // 根据时间计算x坐标
    return 0;
}

int KLineWidget::getPositionAtPrice(double price) const {
    // 根据价格计算y坐标
    double priceRange = m_maxPrice - m_minPrice;
    double ratio = (price - m_minPrice) / priceRange;
    return m_chartHeight + m_marginTop - static_cast<int>(ratio * m_chartHeight);
}
