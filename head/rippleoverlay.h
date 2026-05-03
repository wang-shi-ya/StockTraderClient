#pragma once

#include <QWidget>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QPoint>

class RippleOverlay : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int rippleRadius READ rippleRadius WRITE setRippleRadius)

public:
    explicit RippleOverlay(QWidget *parent = nullptr);

    void animate(const QPoint &buttonGlobalPos, const QPixmap &oldScreenshot);

signals:
    void finished();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int rippleRadius() const { return m_radius; }
    void setRippleRadius(int r) { if (r != m_radius) { m_radius = r; update(); } }

    QPoint m_origin;
    QPixmap m_oldScreenshot;
    int m_radius = 0;
    int m_maxRadius = 0;

    QPropertyAnimation *m_radiusAnim = nullptr;
};
