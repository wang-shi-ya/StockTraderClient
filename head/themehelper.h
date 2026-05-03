#pragma once

#include <QObject>
#include <QString>

class ThemeHelper : public QObject {
    Q_OBJECT

public:
    enum Theme { Day, Night };

    static ThemeHelper *instance();
    static bool isDarkMode();
    static QString currentStyleSheet();

public slots:
    void toggle();
    void setTheme(Theme theme);

signals:
    void themeChanged(bool dark);

private:
    explicit ThemeHelper(QObject *parent = nullptr);

    Theme m_currentTheme = Day;

    static QString dayStyleSheet();
    static QString nightStyleSheet();
};
