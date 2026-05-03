#include "themehelper.h"
#include <QApplication>

static ThemeHelper *s_instance = nullptr;

ThemeHelper *ThemeHelper::instance() {
    if (!s_instance) {
        s_instance = new ThemeHelper(qApp);
    }
    return s_instance;
}

bool ThemeHelper::isDarkMode() {
    return instance()->m_currentTheme == Night;
}

QString ThemeHelper::currentStyleSheet() {
    return instance()->m_currentTheme == Night ? nightStyleSheet() : dayStyleSheet();
}

ThemeHelper::ThemeHelper(QObject *parent)
    : QObject(parent), m_currentTheme(Day) {
}

void ThemeHelper::toggle() {
    m_currentTheme = (m_currentTheme == Day) ? Night : Day;
    qApp->setStyleSheet(currentStyleSheet());
    emit themeChanged(m_currentTheme == Night);
}

void ThemeHelper::setTheme(Theme theme) {
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        qApp->setStyleSheet(currentStyleSheet());
        emit themeChanged(m_currentTheme == Night);
    }
}

QString ThemeHelper::dayStyleSheet() {
    return QStringLiteral(
        // ── 全局 ──
        "QMainWindow, QDialog { background-color: #f5f5f5; font-family: 'Microsoft YaHei', Arial, sans-serif; }"
        "QLabel { font-size: 14px; color: #333; }"

        // ── 输入框 ──
        "QLineEdit {"
        "  border: 1px solid #ddd; border-radius: 4px; padding: 8px 12px;"
        "  font-size: 14px; background-color: white; color: #333;"
        "}"
        "QLineEdit:hover { border-color: #1E88E5; }"
        "QLineEdit:focus { border-color: #1E88E5; }"

        // ── 按钮 ──
        "QPushButton {"
        "  background-color: #1E88E5; color: white; border: none; border-radius: 4px;"
        "  padding: 8px 16px; font-size: 14px; font-weight: 500;"
        "}"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:pressed { background-color: #1565C0; }"

        // ── 输入控件 ──
        "QComboBox, QSpinBox, QDoubleSpinBox {"
        "  border: 1px solid #ddd; border-radius: 4px; padding: 6px 12px;"
        "  font-size: 14px; background-color: white; color: #333;"
        "}"
        "QComboBox:hover, QSpinBox:hover, QDoubleSpinBox:hover { border-color: #1E88E5; }"
        "QComboBox QAbstractItemView {"
        "  background-color: white; color: #333;"
        "  selection-background-color: #E3F2FD; selection-color: #1E88E5;"
        "}"
        // ── 表格 ──
        "QTableWidget {"
        "  background-color: white; alternate-background-color: #f5f7fa;"
        "  border: 1px solid #ddd; border-radius: 4px; font-size: 14px;"
        "  gridline-color: #eee; color: #333;"
        "}"
        "QTableWidget::item { padding: 8px; }"
        "QTableWidget::item:hover { background-color: #f0f4ff; }"
        "QTableWidget::item:selected { background-color: #E3F2FD; color: #1E88E5; }"
        "QHeaderView { background-color: #f0f2f5; }"
        "QHeaderView::section {"
        "  background-color: #f0f2f5; color: #333; font-weight: bold;"
        "  font-size: 14px; padding-left: 8px; padding-right: 8px;"
        "  border: none; border-bottom: 2px solid #e0e0e0;"
        "}"

        // ── 标签页 ──
        "QTabWidget::pane { border: 1px solid #ddd; background-color: #f5f5f5; }"
        "QTabBar::tab {"
        "  background-color: #e8e8e8; color: #666; padding: 8px 20px;"
        "  border: 1px solid #ddd; border-bottom: none;"
        "  border-top-left-radius: 4px; border-top-right-radius: 4px;"
        "}"
        "QTabBar::tab:selected { background-color: white; color: #1E88E5; font-weight: bold; }"
        "QTabBar::tab:hover { background-color: #f0f0f0; }"

        // ── 状态栏 ──
        "QStatusBar { background-color: white; border-top: 1px solid #ddd; font-size: 12px; color: #666; }"

        // ── 滚动条 ──
        "QScrollBar:vertical { background: #f0f0f0; width: 10px; }"
        "QScrollBar::handle:vertical { background: #c0c0c0; border-radius: 5px; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar:horizontal { background: #f0f0f0; height: 10px; }"
        "QScrollBar::handle:horizontal { background: #c0c0c0; border-radius: 5px; min-width: 20px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }"

        // ── 文本框 ──
        "QTextEdit { background-color: white; color: #333; border: 1px solid #ddd; border-radius: 4px; }"

        // ── 分组框 ──
        "QGroupBox { font-weight: bold; color: #333; border: 1px solid #ddd; border-radius: 4px; margin-top: 8px; padding-top: 16px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }"

        // ── 进度条 ──
        "QProgressBar { border: 1px solid #ddd; border-radius: 4px; background-color: #f0f0f0; text-align: center; }"
        "QProgressBar::chunk { background-color: #1E88E5; border-radius: 3px; }"

        // ── 特定命名控件（卡片背景） ──
        "QWidget#quoteWidget, QWidget#orderWidget, QWidget#ordersWidget, QWidget#positionsWidget {"
        "  background-color: white; border-radius: 6px; padding: 10px;"
        "}"
        // ── 公告卡片 ──
        "QWidget#announcementCard { background: #fff; border: 1px solid #e0e0e0; border-radius: 6px; }"
        "QScrollArea#announcementScroll { background: white; }"
        "QWidget#announcementContainer { background: white; }"

        // ── 特定命名控件（静态样式）──
        "QPushButton#btnPlace {"
        "  background-color: #4CAF50; color: white; border: none; border-radius: 4px;"
        "  padding: 8px 24px; font-size: 14px; font-weight: 500;"
        "}"
        "QPushButton#btnPlace:hover { background-color: #43A047; }"
        "QLabel#labelOrdersTitle, QLabel#labelPositionsTitle {"
        "  font-size: 15px; font-weight: bold; padding: 4px 0;"
        "}"
        "QLabel#labelStatusFilter { font-size: 13px; }"
    );
}

QString ThemeHelper::nightStyleSheet() {
    return QStringLiteral(
        // ── 全局 ──
        "QMainWindow, QDialog { background-color: #0f0f1a; font-family: 'Microsoft YaHei', Arial, sans-serif; }"
        "QLabel { font-size: 14px; color: #c8c8d0; }"

        // ── 输入框 ──
        "QLineEdit {"
        "  border: 1px solid #2a2a3a; border-radius: 4px; padding: 8px 12px;"
        "  font-size: 14px; background-color: #1a1a2e; color: #c8c8d0;"
        "}"
        "QLineEdit:hover { border-color: #64B5F6; }"
        "QLineEdit:focus { border-color: #64B5F6; }"

        // ── 按钮 ──
        "QPushButton {"
        "  background-color: #64B5F6; color: #0f0f1a; border: none; border-radius: 4px;"
        "  padding: 8px 16px; font-size: 14px; font-weight: 500;"
        "}"
        "QPushButton:hover { background-color: #42A5F5; }"
        "QPushButton:pressed { background-color: #1E88E5; }"

        // ── 输入控件 ──
        "QComboBox, QSpinBox, QDoubleSpinBox {"
        "  border: 1px solid #2a2a3a; border-radius: 4px; padding: 6px 12px;"
        "  font-size: 14px; background-color: #1a1a2e; color: #c8c8d0;"
        "}"
        "QComboBox:hover, QSpinBox:hover, QDoubleSpinBox:hover { border-color: #64B5F6; }"
        "QComboBox QAbstractItemView {"
        "  background-color: #1a1a2e; color: #c8c8d0;"
        "  selection-background-color: #132744; selection-color: #64B5F6;"
        "}"
        // ── 表格 ──
        "QTableWidget {"
        "  background-color: #181825; alternate-background-color: #1c1c30;"
        "  border: 1px solid #2a2a3a; border-radius: 4px; font-size: 14px;"
        "  gridline-color: #222; color: #c8c8d0;"
        "}"
        "QTableWidget::item { padding: 8px; }"
        "QTableWidget::item:hover { background-color: #1e1e35; }"
        "QTableWidget::item:selected { background-color: #132744; color: #64B5F6; }"
        "QHeaderView { background-color: #141420; }"
        "QHeaderView::section {"
        "  background-color: #141420; color: #c8c8d0; font-weight: bold;"
        "  font-size: 14px; padding-left: 8px; padding-right: 8px;"
        "  border: none; border-bottom: 2px solid #2a2a3a;"
        "}"
        "QHeaderView::section:vertical {"
        "  background-color: #141420; color: #c8c8d0;"
        "  border: none; border-right: 1px solid #2a2a3a;"
        "}"

        // ── 标签页 ──
        "QTabWidget::pane { border: 1px solid #2a2a3a; background-color: #0f0f1a; }"
        "QTabBar::tab {"
        "  background-color: #141420; color: #888; padding: 8px 20px;"
        "  border: 1px solid #2a2a3a; border-bottom: none;"
        "  border-top-left-radius: 4px; border-top-right-radius: 4px;"
        "}"
        "QTabBar::tab:selected { background-color: #0f0f1a; color: #64B5F6; font-weight: bold; }"
        "QTabBar::tab:hover { background-color: #1a1a30; }"

        // ── 状态栏 ──
        "QStatusBar { background-color: #141420; border-top: 1px solid #2a2a3a; font-size: 12px; color: #888; }"

        // ── 滚动条 ──
        "QScrollBar:vertical { background: #0f0f1a; width: 10px; }"
        "QScrollBar::handle:vertical { background: #3a3a4a; border-radius: 5px; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar:horizontal { background: #0f0f1a; height: 10px; }"
        "QScrollBar::handle:horizontal { background: #3a3a4a; border-radius: 5px; min-width: 20px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }"

        // ── 文本框 ──
        "QTextEdit { background-color: #1a1a2e; color: #c8c8d0; border: 1px solid #2a2a3a; border-radius: 4px; }"

        // ── 分组框 ──
        "QGroupBox { font-weight: bold; color: #c8c8d0; border: 1px solid #2a2a3a; border-radius: 4px; margin-top: 8px; padding-top: 16px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }"

        // ── 进度条 ──
        "QProgressBar { border: 1px solid #2a2a3a; border-radius: 4px; background-color: #141420; text-align: center; color: #c8c8d0; }"
        "QProgressBar::chunk { background-color: #64B5F6; border-radius: 3px; }"

        // ── 特定命名控件（卡片背景） ──
        "QWidget#quoteWidget, QWidget#orderWidget, QWidget#ordersWidget, QWidget#positionsWidget {"
        "  background-color: #181825; border-radius: 6px; padding: 10px;"
        "}"
        // ── 公告卡片 ──
        "QWidget#announcementCard { background: #181825; border: 1px solid #2a2a3a; border-radius: 6px; }"
        "QScrollArea#announcementScroll { background: #0f0f1a; }"
        "QWidget#announcementContainer { background: #0f0f1a; }"

        // ── 特定命名控件（静态样式）──
        "QPushButton#btnPlace {"
        "  background-color: #4CAF50; color: white; border: none; border-radius: 4px;"
        "  padding: 8px 24px; font-size: 14px; font-weight: 500;"
        "}"
        "QPushButton#btnPlace:hover { background-color: #43A047; }"
        "QLabel#labelOrdersTitle, QLabel#labelPositionsTitle {"
        "  font-size: 15px; font-weight: bold; padding: 4px 0;"
        "}"
        "QLabel#labelStatusFilter { font-size: 13px; }"
    );
}
