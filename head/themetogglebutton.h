#pragma once
#include <QPushButton>
#include <QPropertyAnimation>

class ThemeToggleButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
    explicit ThemeToggleButton(QWidget *parent = nullptr);
    void setDark(bool dark);
    bool isDark() const { return m_dark; }
    int rotation() const { return m_rotation; }
    void setRotation(int r);
signals:
    void toggled();
protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
private:
    bool m_dark = false;
    bool m_hovered = false;
    int m_rotation = 0;
    QPropertyAnimation *m_rotAnim = nullptr;
};
