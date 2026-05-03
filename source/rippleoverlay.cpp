#include "rippleoverlay.h"
#include <QPainter>
#include <QRadialGradient>
#include <cmath>

RippleOverlay::RippleOverlay(QWidget *parent)
    : QWidget(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |
                   Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_DeleteOnClose);

    if (parent) {
        QPoint topLeft = parent->mapToGlobal(QPoint(0, 0));
        setGeometry(topLeft.x(), topLeft.y(), parent->width(), parent->height());
    }
}

void RippleOverlay::animate(const QPoint &buttonGlobalPos, const QPixmap &oldScreenshot) {
    m_oldScreenshot = oldScreenshot;
    m_origin = mapFromGlobal(buttonGlobalPos);
    m_radius = 0;

    int w = width();
    int h = height();
    auto sq = [](int v) { return v * v; };
    int maxD = 0;
    for (int cx : {0, w}) {
        for (int cy : {0, h}) {
            int d = sq(cx - m_origin.x()) + sq(cy - m_origin.y());
            if (d > maxD) maxD = d;
        }
    }
    m_maxRadius = static_cast<int>(std::sqrt(maxD)) + 20;

    m_radiusAnim = new QPropertyAnimation(this, "rippleRadius", this);
    m_radiusAnim->setDuration(600);
    m_radiusAnim->setStartValue(0);
    m_radiusAnim->setEndValue(m_maxRadius);
    m_radiusAnim->setEasingCurve(QEasingCurve::OutExpo);

    connect(m_radiusAnim, &QPropertyAnimation::finished, this, [this]() {
        close();
        emit finished();
    });

    show();
    raise();
    m_radiusAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void RippleOverlay::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Draw the old theme screenshot covering the entire window
    p.drawPixmap(rect(), m_oldScreenshot);

    if (m_radius <= 0) return;

    // Punch a circular hole with soft edge, revealing the new theme underneath
    p.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    QRadialGradient g(m_origin, m_radius);
    g.setColorAt(0.0, Qt::black);      // opaque → fully clears to reveal new theme
    g.setColorAt(1.0, Qt::black);      // opaque → hard edge
    p.setBrush(g);
    p.setPen(Qt::NoPen);
    p.drawEllipse(m_origin, m_radius, m_radius);
}
