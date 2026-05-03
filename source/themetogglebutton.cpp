#include "themetogglebutton.h"
#include <QPainter>
#include <QPainterPath>

ThemeToggleButton::ThemeToggleButton(QWidget *parent)
    : QPushButton(parent)
    , m_rotAnim(new QPropertyAnimation(this, "rotation", this))
{
    setFixedSize(36, 36);
    setCursor(Qt::PointingHandCursor);
    setToolTip(QString::fromUtf8("切换日间/夜间模式"));
    m_rotAnim->setDuration(450);
    m_rotAnim->setEasingCurve(QEasingCurve::OutCubic);

    connect(this, &QPushButton::clicked, this, [this]() {
        m_dark = !m_dark;
        m_rotAnim->stop();
        m_rotAnim->setStartValue(m_rotation);
        m_rotAnim->setEndValue(m_rotation + 360);
        m_rotAnim->start();
        update();
        emit toggled();
    });
}

void ThemeToggleButton::setDark(bool dark) {
    m_dark = dark;
    update();
}

void ThemeToggleButton::setRotation(int r) {
    m_rotation = r;
    update();
}

void ThemeToggleButton::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect iconRect = rect().adjusted(5, 5, -5, -5);
    QPoint center = iconRect.center();

    // Hover glow ring
    if (m_hovered) {
        p.setPen(Qt::NoPen);
        QColor glow = m_dark ? QColor(255, 183, 77, 30) : QColor(255, 143, 0, 25);
        p.setBrush(glow);
        p.drawEllipse(center, 16, 16);
    }

    p.save();
    p.translate(center);
    p.rotate(m_rotation);
    p.translate(-center);

    QFont f = p.font();
    f.setPixelSize(20);
    p.setFont(f);

    p.setPen(m_dark ? QColor("#FFB74D") : QColor("#FF8F00"));
    p.drawText(iconRect, Qt::AlignCenter, m_dark ? QString::fromUtf8("☽") : QString::fromUtf8("☀"));
    p.restore();
}

void ThemeToggleButton::enterEvent(QEnterEvent *) {
    m_hovered = true;
    update();
}

void ThemeToggleButton::leaveEvent(QEvent *) {
    m_hovered = false;
    update();
}
