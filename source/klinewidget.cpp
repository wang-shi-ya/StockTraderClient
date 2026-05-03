#include "klinewidget.h"
#include "ui_klinewidget.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QCompleter>

// Pre-allocated chart colors (theme-independent)
static const QColor s_riseColor("#ef5350");
static const QColor s_fallColor("#26a69a");
static const QColor s_riseBorder("#d32f2f");
static const QColor s_fallBorder("#1b8a75");

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

    // 初始化周期组合框
    m_periodCombo->addItems({"1m", "5m", "15m", "30m", "1h", "1d", "1w"});
    m_periodCombo->setCurrentText(m_currentPeriod);

    // 从数据库获取股票代码列表并添加公司名称
    QStringList stockSymbols = m_dataService->getAllStockSymbols();
    if (stockSymbols.isEmpty()) {
        // 后备方案：使用默认股票代码
        stockSymbols = {
            "000001", "000002", "000333", "000338", "000651", "000858", "000895",
            "600000", "600009", "600016", "600028", "600030", "600031", "600036", "600050",
            "600104", "600276", "600309", "600519", "600585", "600690", "600887", "600900",
            "601006", "601012", "601018", "601088", "601166", "601186", "601229", "601288",
            "601318", "601328", "601398", "601628", "601668", "601688", "601766", "601818",
            "601857", "601888", "601989", "603259", "603288", "603501", "603799", "603986"
        };
    }
    
    // 为每个股票代码添加公司名称
    for (const QString &symbol : stockSymbols) {
        QString name = m_dataService->getStockName(symbol);
        if (!name.isEmpty()) {
            m_symbolCombo->addItem(symbol + " (" + name + ")");
        } else {
            m_symbolCombo->addItem(symbol);
        }
    }

    // 设置默认股票代码
    bool found = false;
    for (int i = 0; i < m_symbolCombo->count(); ++i) {
        QString itemText = m_symbolCombo->itemText(i);
        if (itemText.startsWith(m_currentSymbol + " ") || itemText == m_currentSymbol) {
            m_symbolCombo->setCurrentIndex(i);
            found = true;
            break;
        }
    }
    
    if (!found) {
        m_symbolCombo->setCurrentText(m_currentSymbol);
    }

    // 连接数据服务信号
    connect(m_dataService, &DataService::kLineDataReceived, this, &KLineWidget::onKLineDataReceived);
    connect(m_dataService, &DataService::quoteUpdated, this, &KLineWidget::onQuoteUpdated);
    connect(m_dataService, &DataService::companyListUpdated, this, &KLineWidget::onStockListUpdated);

    // 连接UI信号
    connect(m_symbolCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, [this](const QString &displayText) {
                // 提取纯股票代码
                QString pureSymbol = displayText;
                int parenthesisIndex = displayText.indexOf('(');
                if (parenthesisIndex > 0) {
                    pureSymbol = displayText.left(parenthesisIndex).trimmed();
                }
                setSymbol(pureSymbol);
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
    // 提取纯股票代码（去掉公司名称部分）
    QString pureSymbol = symbol;
    int parenthesisIndex = symbol.indexOf('(');
    if (parenthesisIndex > 0) {
        pureSymbol = symbol.left(parenthesisIndex).trimmed();
    }
    
    m_currentSymbol = pureSymbol;
    
    // 找到对应的显示文本（包含公司名称）
    bool found = false;
    for (int i = 0; i < m_symbolCombo->count(); ++i) {
        QString itemText = m_symbolCombo->itemText(i);
        if (itemText.startsWith(pureSymbol + " ") || itemText == pureSymbol) {
            m_symbolCombo->setCurrentIndex(i);
            found = true;
            break;
        }
    }
    
    if (!found) {
        m_symbolCombo->setCurrentText(symbol);
    }
    
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

void KLineWidget::onStockListUpdated(const QStringList &symbols) {
    Q_UNUSED(symbols)
    // 保存当前选中
    QString currentText = m_symbolCombo->currentText();
    // 提取纯代码
    int parenIdx = currentText.indexOf('(');
    QString currentCode = parenIdx > 0 ? currentText.left(parenIdx).trimmed() : m_currentSymbol;

    m_symbolCombo->blockSignals(true);
    m_symbolCombo->clear();

    QStringList allSymbols = m_dataService->getAllStockSymbols();
    for (const QString &symbol : allSymbols) {
        QString name = m_dataService->getStockName(symbol);
        if (!name.isEmpty())
            m_symbolCombo->addItem(symbol + " (" + name + ")");
        else
            m_symbolCombo->addItem(symbol);
    }

    // 恢复选中
    bool found = false;
    for (int i = 0; i < m_symbolCombo->count(); ++i) {
        QString itemText = m_symbolCombo->itemText(i);
        if (itemText.startsWith(currentCode + " ") || itemText == currentCode) {
            m_symbolCombo->setCurrentIndex(i);
            found = true;
            break;
        }
    }
    if (!found && m_symbolCombo->count() > 0)
        m_symbolCombo->setCurrentIndex(0);

    m_symbolCombo->blockSignals(false);
}

void KLineWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QRect chartRect;
    if (ui && ui->chartWidget) {
        chartRect = ui->chartWidget->geometry();
        if (chartRect.isEmpty()) {
            int controlHeight = 60;
            chartRect = QRect(0, controlHeight, width(), height() - controlHeight);
        }
    } else {
        int controlHeight = 60;
        chartRect = QRect(0, controlHeight, width(), height() - controlHeight);
    }

    m_chartWidth = chartRect.width();
    m_chartHeight = chartRect.height() - m_volumeHeight;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景
    painter.fillRect(chartRect, QColor("#1a1a2e"));

    if (m_klineData.isEmpty()) {
        QFont f = painter.font();
        f.setPixelSize(16);
        painter.setFont(f);
        painter.setPen(QColor("#888"));
        painter.drawText(chartRect, Qt::AlignCenter, "请选择股票查看K线图");
        return;
    }

    // 绘制网格
    drawGridLines(painter, chartRect);

    // 绘制均线
    drawMALines(painter, chartRect);

    // K线
    drawKLineChart(painter, chartRect);

    // 分隔线
    drawSeparator(painter, chartRect);

    // 成交量
    drawVolumeChart(painter, chartRect);

    // 坐标轴（画在K线之上，确保刻度可见）
    drawPriceAxis(painter, chartRect);
    drawTimeAxis(painter, chartRect);

    // 当前价格虚线
    QuoteData quote = m_dataService->getQuote(m_currentSymbol);
    if (quote.currentPrice > 0 && quote.currentPrice >= m_minPrice && quote.currentPrice <= m_maxPrice) {
        int priceY = chartRect.top() + getPositionAtPrice(quote.currentPrice);
        QPen pricePen(QColor("#ef5350"), 1, Qt::DotLine);
        pricePen.setDashPattern({3, 4});
        painter.setPen(pricePen);
        painter.drawLine(chartRect.left() + m_marginLeft, priceY,
                         chartRect.left() + m_chartWidth - m_marginRight, priceY);
        // 右侧价格标签
        QFont f = painter.font();
        f.setPixelSize(10);
        painter.setFont(f);
        painter.setPen(QColor("#ef5350"));
        QString priceText = QString::number(quote.currentPrice, 'f', 2);
        QRect priceLabelRect(chartRect.left() + m_chartWidth - m_marginRight + 2, priceY - 8,
                             m_marginRight - 4, 16);
        painter.drawText(priceLabelRect, Qt::AlignLeft | Qt::AlignVCenter, priceText);
    }

    // 十字线
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

                    double chg = data.close - data.open;
                    double chgPct = data.open > 0 ? (chg / data.open) * 100 : 0;
                    QString info = QString("时间: %1 | 开: %2 | 高: %3 | 低: %4 | 收: %5 | 幅: %6% | 量: %7")
                                  .arg(data.dateTime.toString("yyyy-MM-dd hh:mm"))
                                  .arg(formatPrice(data.open))
                                  .arg(formatPrice(data.high))
                                  .arg(formatPrice(data.low))
                                  .arg(formatPrice(data.close))
                                  .arg(QString::number(chgPct, 'f', 2))
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

        int openY = chartRect.top() + getPositionAtPrice(data.open);
        int closeY = chartRect.top() + getPositionAtPrice(data.close);
        int highY = chartRect.top() + getPositionAtPrice(data.high);
        int lowY = chartRect.top() + getPositionAtPrice(data.low);

        bool isRise = data.close >= data.open;
        QColor bodyColor = isRise ? s_riseColor : s_fallColor;
        QColor borderColor = isRise ? s_riseBorder : s_fallBorder;

        // 选中高亮
        if (i == m_selectedIndex) {
            bodyColor = bodyColor.lighter(130);
            borderColor = borderColor.lighter(130);
        }

        // 影线
        painter.setPen(QPen(borderColor, 1));
        painter.drawLine(x, highY, x, lowY);

        // 实体
        int bodyHeight = qAbs(closeY - openY);
        if (bodyHeight < 1) bodyHeight = 1;

        QRect bodyRect(x - barWidth / 2 + 1, qMin(openY, closeY), barWidth - 2, bodyHeight);
        painter.fillRect(bodyRect, bodyColor);
        painter.setPen(QPen(borderColor, 1));
        painter.drawRect(bodyRect);
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

        int volH = static_cast<int>((double)data.volume / m_maxVolume * m_volumeHeight);
        int y = volumeY + m_volumeHeight - volH;

        QColor color = data.close >= data.open
            ? QColor("#ef5350") : QColor("#26a69a");
        color.setAlpha(i == m_selectedIndex ? 200 : 120);
        painter.fillRect(x - barWidth / 2 + 1, y, barWidth - 2, volH, color);
    }
}

void KLineWidget::drawCrosshair(QPainter &painter, const QRect &chartRect) {
    QPoint localPos = m_mousePos;
    if (!chartRect.contains(localPos)) return;

    // 十字线
    QPen crossPen(QColor("#888"), 1, Qt::DashLine);
    crossPen.setDashPattern({4, 4});
    painter.setPen(crossPen);
    painter.drawLine(localPos.x(), chartRect.top() + m_marginTop,
                     localPos.x(), chartRect.top() + m_chartHeight + m_volumeHeight + m_marginTop);
    painter.drawLine(chartRect.left() + m_marginLeft, localPos.y(),
                     chartRect.left() + m_chartWidth - m_marginRight, localPos.y());

    // 价格标签
    double price = getPriceAtPosition(localPos.y() - chartRect.top());
    QFont f = painter.font();
    f.setPixelSize(10);
    painter.setFont(f);
    QString priceText = QString::number(price, 'f', 2);
    QFontMetrics fm(painter.font());
    int textW = fm.horizontalAdvance(priceText) + 4;
    int textH = fm.height();

    int labelX = chartRect.left() + m_chartWidth - m_marginRight - textW;
    int labelY = localPos.y() - textH / 2;
    if (labelY < chartRect.top()) labelY = chartRect.top();
    if (labelY + textH > chartRect.top() + m_chartHeight + m_volumeHeight + m_marginTop)
        labelY = chartRect.top() + m_chartHeight + m_volumeHeight + m_marginTop - textH;

    painter.fillRect(labelX - 2, labelY, textW + 4, textH, QColor("#333"));
    painter.setPen(QColor("#fff"));
    painter.drawText(labelX, labelY, textW, textH, Qt::AlignCenter, priceText);
}

void KLineWidget::drawPriceAxis(QPainter &painter, const QRect &chartRect) {
    if (m_maxPrice <= m_minPrice) return;

    QFont f = painter.font();
    f.setPixelSize(10);
    painter.setFont(f);

    int steps = 8;
    double priceStep = (m_maxPrice - m_minPrice) / steps;

    for (int i = 0; i <= steps; ++i) {
        double price = m_minPrice + i * priceStep;
        int y = chartRect.top() + getPositionAtPrice(price);

        QString text = QString::number(price, 'f', 2);
        QFontMetrics fm(painter.font());
        int textW = fm.horizontalAdvance(text);
        int textH = fm.height();

        // 标签背景
        QRect labelBg(chartRect.left() + m_marginLeft - textW - 10, y - textH / 2 - 1,
                      textW + 6, textH + 2);
        painter.fillRect(labelBg, QColor(26, 26, 46, 220));
        painter.setPen(QColor("#aaa"));
        painter.drawText(chartRect.left() + m_marginLeft - 6, y - textH / 2,
                         textW, textH, Qt::AlignRight | Qt::AlignVCenter, text);
    }

    // 价格轴竖线
    painter.setPen(QPen(QColor("#444"), 1));
    painter.drawLine(chartRect.left() + m_marginLeft, chartRect.top() + m_marginTop,
                     chartRect.left() + m_marginLeft,
                     chartRect.top() + m_chartHeight + m_volumeHeight + m_marginTop);
}

void KLineWidget::drawTimeAxis(QPainter &painter, const QRect &chartRect) {
    if (m_klineData.isEmpty()) return;

    QFont f = painter.font();
    f.setPixelSize(10);
    painter.setFont(f);

    int barWidth = (m_chartWidth - m_marginLeft - m_marginRight) / m_visibleBars;
    if (barWidth <= 0) return;

    int startIndex = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
    int endIndex = qMin(m_klineData.size(), startIndex + m_visibleBars);
    int visibleCount = endIndex - startIndex;
    if (visibleCount <= 0) return;

    int steps = qMin(5, visibleCount - 1);
    if (steps <= 0) return;
    int timeStep = visibleCount / steps;
    if (timeStep <= 0) timeStep = 1;

    int axisY = chartRect.top() + m_chartHeight + m_volumeHeight + m_marginTop;

    for (int i = 0; i <= steps; ++i) {
        int localIndex = i * timeStep;
        int dataIndex = startIndex + localIndex;
        if (dataIndex < 0 || dataIndex >= m_klineData.size()) continue;

        int x = chartRect.left() + m_marginLeft + localIndex * barWidth + barWidth / 2;

        QDateTime dt = m_klineData[dataIndex].dateTime;
        QString text;
        if (m_currentPeriod == "1d" || m_currentPeriod == "1w")
            text = dt.toString("MM-dd");
        else if (m_currentPeriod == "1h" || m_currentPeriod == "30m")
            text = dt.toString("MM-dd hh:mm");
        else
            text = dt.toString("hh:mm");

        painter.setPen(QColor("#666"));
        painter.drawLine(x, axisY, x, axisY + 4);
        painter.setPen(QColor("#aaa"));
        painter.drawText(x - 50, axisY + 8, 100, 18, Qt::AlignCenter, text);
    }

    // 时间轴横线
    painter.setPen(QPen(QColor("#444"), 1));
    painter.drawLine(chartRect.left() + m_marginLeft, axisY,
                     chartRect.left() + m_chartWidth - m_marginRight, axisY);
}

void KLineWidget::drawGridLines(QPainter &painter, const QRect &chartRect) {
    if (m_maxPrice <= m_minPrice) return;

    painter.setRenderHint(QPainter::Antialiasing, false);
    int steps = 8;
    double priceStep = (m_maxPrice - m_minPrice) / steps;
    QPen gridPen(QColor("#2a2a40"), 1, Qt::DotLine);
    gridPen.setDashPattern({2, 4});

    for (int i = 0; i <= steps; ++i) {
        double price = m_minPrice + i * priceStep;
        int y = chartRect.top() + getPositionAtPrice(price);

        painter.setPen(gridPen);
        painter.drawLine(chartRect.left() + m_marginLeft, y,
                         chartRect.left() + m_chartWidth - m_marginRight, y);
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
}

void KLineWidget::drawMALines(QPainter &painter, const QRect &chartRect) {
    if (m_klineData.isEmpty()) return;

    int barWidth = (m_chartWidth - m_marginLeft - m_marginRight) / m_visibleBars;
    if (barWidth <= 0) return;

    int startIndex = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
    int endIndex = qMin(m_klineData.size(), startIndex + m_visibleBars);

    struct MALine { const QVector<double> &values; QColor color; int width; };
    const MALine lines[] = {
        { m_ma5,  QColor("#ff9800"), 1 },   // MA5 橙色
        { m_ma10, QColor("#e91e63"), 1 },   // MA10 粉色
        { m_ma20, QColor("#2196f3"), 1 },   // MA20 蓝色
    };

    for (const auto &ma : lines) {
        if (ma.values.size() < 2) continue;

        painter.setPen(QPen(ma.color, ma.width));
        QPoint prevPt;
        for (int i = startIndex; i < endIndex; ++i) {
            if (i >= ma.values.size()) break;
            double val = ma.values[i];
            if (val <= 0) continue;

            int x = chartRect.left() + m_marginLeft + (i - startIndex) * barWidth + barWidth / 2;
            int y = chartRect.top() + getPositionAtPrice(val);

            if (!prevPt.isNull())
                painter.drawLine(prevPt, QPoint(x, y));
            prevPt = QPoint(x, y);
        }
    }
}

void KLineWidget::drawSeparator(QPainter &painter, const QRect &chartRect) {
    int sepY = chartRect.top() + m_chartHeight + m_marginTop;
    painter.setPen(QPen(QColor("#444"), 1));
    painter.drawLine(chartRect.left() + m_marginLeft, sepY,
                     chartRect.left() + m_chartWidth - m_marginRight, sepY);

    QFont f = painter.font();
    f.setPixelSize(9);
    painter.setFont(f);
    painter.setPen(QColor("#888"));
    painter.drawText(chartRect.left() + m_marginLeft + 4, sepY - 14, 80, 14,
                     Qt::AlignLeft | Qt::AlignVCenter, "VOL");
}

QVector<double> KLineWidget::calcMA(int period) const {
    QVector<double> result(m_klineData.size(), 0);
    if (m_klineData.size() < period) return result;

    double sum = 0;
    for (int i = 0; i < m_klineData.size(); ++i) {
        sum += m_klineData[i].close;
        if (i >= period)
            sum -= m_klineData[i - period].close;
        if (i >= period - 1)
            result[i] = sum / period;
    }
    return result;
}

void KLineWidget::updateChart() {
    if (m_klineData.isEmpty()) {
        m_minPrice = 0;
        m_maxPrice = 100;
        m_maxVolume = 0;
        m_ma5.clear();
        m_ma10.clear();
        m_ma20.clear();
        update();
        return;
    }

    // 计算价格范围（考虑均线值以避免被裁剪）
    m_minPrice = m_klineData[0].low;
    m_maxPrice = m_klineData[0].high;
    m_maxVolume = m_klineData[0].volume;

    for (const KLineData &data : m_klineData) {
        m_minPrice = qMin(m_minPrice, data.low);
        m_maxPrice = qMax(m_maxPrice, data.high);
        m_maxVolume = qMax(m_maxVolume, data.volume);
    }

    // 计算均线
    m_ma5 = calcMA(5);
    m_ma10 = calcMA(10);
    m_ma20 = calcMA(20);

    // 考虑均线范围（仅可见部分）
    int startIdx = qMax(0, m_klineData.size() - m_visibleBars - m_scrollOffset);
    for (int i = startIdx; i < m_klineData.size(); ++i) {
        if (i < m_ma5.size() && m_ma5[i] > 0) { m_minPrice = qMin(m_minPrice, m_ma5[i]); m_maxPrice = qMax(m_maxPrice, m_ma5[i]); }
        if (i < m_ma10.size() && m_ma10[i] > 0) { m_minPrice = qMin(m_minPrice, m_ma10[i]); m_maxPrice = qMax(m_maxPrice, m_ma10[i]); }
        if (i < m_ma20.size() && m_ma20[i] > 0) { m_minPrice = qMin(m_minPrice, m_ma20[i]); m_maxPrice = qMax(m_maxPrice, m_ma20[i]); }
    }

    double priceRange = m_maxPrice - m_minPrice;
    if (priceRange <= 0) {
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

    if (m_maxVolume <= 0) m_maxVolume = 1;

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
