#include "ScrollAreaPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NScrollArea.h>

ScrollAreaPage::ScrollAreaPage(QWidget* parent)
    : BasePage("ScrollArea 滚动区域", "ScrollArea 提供可滚动的内容区域，支持自定义滚动条样式。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NScrollArea* basicScrollArea = new NScrollArea(this);
    basicScrollArea->setMinimumHeight(200);
    basicScrollArea->setMinimumWidth(400);

    QWidget*     contentWidget = new QWidget(basicScrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    for (int i = 1; i <= 20; ++i) {
        NPushButton* button = new NPushButton(QString("按钮 %1").arg(i), contentWidget);
        button->setMinimumWidth(380);
        button->setFixedHeight(40);
        contentLayout->addWidget(button);
    }

    contentLayout->addStretch();
    basicScrollArea->setWidget(contentWidget);

    basicLayout->addWidget(basicScrollArea);

    addSection("基本滚动区域", basicLayout);

    QVBoxLayout* horizontalLayout = new QVBoxLayout();
    horizontalLayout->setSpacing(16);

    NScrollArea* horizontalScrollArea = new NScrollArea(this);
    horizontalScrollArea->setMinimumHeight(100);
    horizontalScrollArea->setMinimumWidth(400);
    horizontalScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    horizontalScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget*     horizontalContentWidget = new QWidget(horizontalScrollArea);
    QHBoxLayout* horizontalContentLayout = new QHBoxLayout(horizontalContentWidget);

    for (int i = 1; i <= 10; ++i) {
        NPushButton* button = new NPushButton(QString("按钮 %1").arg(i), horizontalContentWidget);
        button->setMinimumWidth(100);
        button->setFixedHeight(40);
        horizontalContentLayout->addWidget(button);
    }

    horizontalContentLayout->addStretch();
    horizontalScrollArea->setWidget(horizontalContentWidget);

    horizontalLayout->addWidget(horizontalScrollArea);

    addSection("水平滚动区域", horizontalLayout);

    QVBoxLayout* disabledLayout = new QVBoxLayout();
    disabledLayout->setSpacing(16);

    NScrollArea* disabledScrollArea = new NScrollArea(this);
    disabledScrollArea->setMinimumHeight(150);
    disabledScrollArea->setMinimumWidth(400);
    disabledScrollArea->setEnabled(false);

    QWidget*     disabledContentWidget = new QWidget(disabledScrollArea);
    QVBoxLayout* disabledContentLayout = new QVBoxLayout(disabledContentWidget);

    for (int i = 1; i <= 10; ++i) {
        NPushButton* button = new NPushButton(QString("按钮 %1").arg(i), disabledContentWidget);
        button->setMinimumWidth(380);
        button->setFixedHeight(40);
        disabledContentLayout->addWidget(button);
    }

    disabledContentLayout->addStretch();
    disabledScrollArea->setWidget(disabledContentWidget);

    disabledLayout->addWidget(disabledScrollArea);

    addSection("禁用状态的滚动区域", disabledLayout);
}
