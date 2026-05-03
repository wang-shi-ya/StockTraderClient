/********************************************************************************
** Form generated from reading UI file 'newswidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWSWIDGET_H
#define UI_NEWSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *controlBar;
    QHBoxLayout *controlBarLayout;
    QWidget *searchGroup;
    QHBoxLayout *searchGroupLayout;
    QLineEdit *searchEdit;
    QPushButton *searchBtn;
    QWidget *divider1;
    QWidget *categoryGroup;
    QHBoxLayout *categoryGroupLayout;
    QLabel *labelCategory;
    QComboBox *categoryCombo;
    QPushButton *refreshBtn;
    QSpacerItem *horizontalSpacer;
    QSplitter *splitter;
    QWidget *listPanel;
    QVBoxLayout *listPanelLayout;
    QLabel *listHeader;
    QListWidget *newsListWidget;
    QWidget *paginationBar;
    QHBoxLayout *paginationBarLayout;
    QWidget *detailPanel;
    QVBoxLayout *detailPanelLayout;
    QLabel *detailHeader;
    QLabel *titleLabel;
    QWidget *metaBar;
    QHBoxLayout *metaBarLayout;
    QLabel *sourceLabel;
    QLabel *timeLabel;
    QSpacerItem *metaBarSpacer;
    QWidget *detailSeparator;
    QTextEdit *detailTextEdit;

    void setupUi(QWidget *NewsWidget)
    {
        if (NewsWidget->objectName().isEmpty())
            NewsWidget->setObjectName("NewsWidget");
        NewsWidget->resize(960, 640);
        verticalLayout = new QVBoxLayout(NewsWidget);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(12, 12, 12, 8);
        controlBar = new QWidget(NewsWidget);
        controlBar->setObjectName("controlBar");
        controlBar->setMinimumHeight(44);
        controlBar->setMaximumHeight(48);
        controlBarLayout = new QHBoxLayout(controlBar);
        controlBarLayout->setSpacing(8);
        controlBarLayout->setObjectName("controlBarLayout");
        controlBarLayout->setContentsMargins(0, 0, 0, 0);
        searchGroup = new QWidget(controlBar);
        searchGroup->setObjectName("searchGroup");
        searchGroupLayout = new QHBoxLayout(searchGroup);
        searchGroupLayout->setSpacing(6);
        searchGroupLayout->setObjectName("searchGroupLayout");
        searchGroupLayout->setContentsMargins(0, 0, 0, 0);
        searchEdit = new QLineEdit(searchGroup);
        searchEdit->setObjectName("searchEdit");
        searchEdit->setMinimumWidth(200);
        searchEdit->setClearButtonEnabled(true);

        searchGroupLayout->addWidget(searchEdit);

        searchBtn = new QPushButton(searchGroup);
        searchBtn->setObjectName("searchBtn");

        searchGroupLayout->addWidget(searchBtn);


        controlBarLayout->addWidget(searchGroup);

        divider1 = new QWidget(controlBar);
        divider1->setObjectName("divider1");
        divider1->setMinimumWidth(1);
        divider1->setMaximumWidth(1);
        divider1->setMinimumHeight(24);

        controlBarLayout->addWidget(divider1);

        categoryGroup = new QWidget(controlBar);
        categoryGroup->setObjectName("categoryGroup");
        categoryGroupLayout = new QHBoxLayout(categoryGroup);
        categoryGroupLayout->setSpacing(6);
        categoryGroupLayout->setObjectName("categoryGroupLayout");
        categoryGroupLayout->setContentsMargins(0, 0, 0, 0);
        labelCategory = new QLabel(categoryGroup);
        labelCategory->setObjectName("labelCategory");

        categoryGroupLayout->addWidget(labelCategory);

        categoryCombo = new QComboBox(categoryGroup);
        categoryCombo->setObjectName("categoryCombo");
        categoryCombo->setMinimumWidth(90);

        categoryGroupLayout->addWidget(categoryCombo);


        controlBarLayout->addWidget(categoryGroup);

        refreshBtn = new QPushButton(controlBar);
        refreshBtn->setObjectName("refreshBtn");

        controlBarLayout->addWidget(refreshBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlBarLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(controlBar);

        splitter = new QSplitter(NewsWidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(3);
        listPanel = new QWidget(splitter);
        listPanel->setObjectName("listPanel");
        listPanelLayout = new QVBoxLayout(listPanel);
        listPanelLayout->setSpacing(4);
        listPanelLayout->setObjectName("listPanelLayout");
        listPanelLayout->setContentsMargins(0, 0, 0, 0);
        listHeader = new QLabel(listPanel);
        listHeader->setObjectName("listHeader");

        listPanelLayout->addWidget(listHeader);

        newsListWidget = new QListWidget(listPanel);
        newsListWidget->setObjectName("newsListWidget");
        newsListWidget->setFrameShape(QFrame::NoFrame);

        listPanelLayout->addWidget(newsListWidget);

        paginationBar = new QWidget(listPanel);
        paginationBar->setObjectName("paginationBar");
        paginationBar->setMinimumHeight(36);
        paginationBar->setMaximumHeight(36);
        paginationBarLayout = new QHBoxLayout(paginationBar);
        paginationBarLayout->setSpacing(8);
        paginationBarLayout->setObjectName("paginationBarLayout");
        paginationBarLayout->setContentsMargins(0, 0, 0, 0);

        listPanelLayout->addWidget(paginationBar);

        splitter->addWidget(listPanel);
        detailPanel = new QWidget(splitter);
        detailPanel->setObjectName("detailPanel");
        detailPanelLayout = new QVBoxLayout(detailPanel);
        detailPanelLayout->setSpacing(10);
        detailPanelLayout->setObjectName("detailPanelLayout");
        detailPanelLayout->setContentsMargins(16, 8, 16, 8);
        detailHeader = new QLabel(detailPanel);
        detailHeader->setObjectName("detailHeader");

        detailPanelLayout->addWidget(detailHeader);

        titleLabel = new QLabel(detailPanel);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setWordWrap(true);

        detailPanelLayout->addWidget(titleLabel);

        metaBar = new QWidget(detailPanel);
        metaBar->setObjectName("metaBar");
        metaBarLayout = new QHBoxLayout(metaBar);
        metaBarLayout->setSpacing(16);
        metaBarLayout->setObjectName("metaBarLayout");
        metaBarLayout->setContentsMargins(0, 0, 0, 0);
        sourceLabel = new QLabel(metaBar);
        sourceLabel->setObjectName("sourceLabel");

        metaBarLayout->addWidget(sourceLabel);

        timeLabel = new QLabel(metaBar);
        timeLabel->setObjectName("timeLabel");

        metaBarLayout->addWidget(timeLabel);

        metaBarSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        metaBarLayout->addItem(metaBarSpacer);


        detailPanelLayout->addWidget(metaBar);

        detailSeparator = new QWidget(detailPanel);
        detailSeparator->setObjectName("detailSeparator");
        detailSeparator->setMinimumHeight(1);
        detailSeparator->setMaximumHeight(1);

        detailPanelLayout->addWidget(detailSeparator);

        detailTextEdit = new QTextEdit(detailPanel);
        detailTextEdit->setObjectName("detailTextEdit");
        detailTextEdit->setReadOnly(true);
        detailTextEdit->setFrameShape(QFrame::NoFrame);

        detailPanelLayout->addWidget(detailTextEdit);

        splitter->addWidget(detailPanel);

        verticalLayout->addWidget(splitter);


        retranslateUi(NewsWidget);

        QMetaObject::connectSlotsByName(NewsWidget);
    } // setupUi

    void retranslateUi(QWidget *NewsWidget)
    {
        searchEdit->setPlaceholderText(QCoreApplication::translate("NewsWidget", "\350\276\223\345\205\245\345\205\263\351\224\256\350\257\215\346\220\234\347\264\242\346\226\260\351\227\273...", nullptr));
        searchBtn->setText(QCoreApplication::translate("NewsWidget", "\346\220\234\347\264\242", nullptr));
        labelCategory->setText(QCoreApplication::translate("NewsWidget", "\345\210\206\347\261\273", nullptr));
        refreshBtn->setText(QCoreApplication::translate("NewsWidget", "\345\210\267\346\226\260", nullptr));
        listHeader->setText(QCoreApplication::translate("NewsWidget", "\346\226\260\351\227\273\345\210\227\350\241\250", nullptr));
        detailHeader->setText(QCoreApplication::translate("NewsWidget", "\346\226\260\351\227\273\350\257\246\346\203\205", nullptr));
        titleLabel->setText(QCoreApplication::translate("NewsWidget", "\350\257\267\351\200\211\346\213\251\344\270\200\346\235\241\346\226\260\351\227\273\346\237\245\347\234\213\350\257\246\346\203\205", nullptr));
        sourceLabel->setText(QString());
        timeLabel->setText(QString());
        (void)NewsWidget;
    } // retranslateUi

};

namespace Ui {
    class NewsWidget: public Ui_NewsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWSWIDGET_H
