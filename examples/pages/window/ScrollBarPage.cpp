#include "ScrollBarPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QtNativeUI/NScrollArea.h>
#include <QtNativeUI/NScrollBar.h>

ScrollBarPage::ScrollBarPage(QWidget* parent)
    : BasePage("ScrollBar 滚动条", "现代化的滚动条组件，支持自定义颜色和样式。", parent) {

    QHBoxLayout* orientationLayout = new QHBoxLayout();
    orientationLayout->setSpacing(32);

    QWidget*     verticalWidget = new QWidget();
    QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);
    verticalLayout->setSpacing(8);

    QLabel* verticalLabel = new QLabel("垂直滚动条:");
    NScrollBar* verticalScrollBar = new NScrollBar(Qt::Vertical, verticalWidget);
    verticalScrollBar->setFixedSize(20, 200);
    verticalScrollBar->setRange(0, 100);
    verticalScrollBar->setValue(50);

    verticalLayout->addWidget(verticalLabel);
    verticalLayout->addWidget(verticalScrollBar);

    QWidget*     horizontalWidget = new QWidget();
    QVBoxLayout* horizontalLayout = new QVBoxLayout(horizontalWidget);
    horizontalLayout->setSpacing(8);

    QLabel* horizontalLabel = new QLabel("水平滚动条:");
    NScrollBar* horizontalScrollBar = new NScrollBar(Qt::Horizontal, horizontalWidget);
    horizontalScrollBar->setFixedSize(200, 20);
    horizontalScrollBar->setRange(0, 100);
    horizontalScrollBar->setValue(50);

    horizontalLayout->addWidget(horizontalLabel);
    horizontalLayout->addWidget(horizontalScrollBar);

    orientationLayout->addWidget(verticalWidget);
    orientationLayout->addWidget(horizontalWidget);
    orientationLayout->addStretch();

    addSection("基本滚动条", orientationLayout);

    QVBoxLayout* scrollAreaLayout = new QVBoxLayout();
    scrollAreaLayout->setSpacing(8);

    QLabel* scrollAreaLabel = new QLabel("带 NScrollBar 的滚动区域:");

    NScrollArea* scrollArea = new NScrollArea();
    scrollArea->setFixedSize(400, 200);

    QWidget*     content       = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(content);

    for (int i = 1; i <= 20; i++) {
        QLabel* itemLabel = new QLabel(QString("列表项 %1").arg(i), content);
        contentLayout->addWidget(itemLabel);
    }

    scrollArea->setWidget(content);

    scrollAreaLayout->addWidget(scrollAreaLabel);
    scrollAreaLayout->addWidget(scrollArea);

    addSection("滚动区域示例", scrollAreaLayout);

    QVBoxLayout* textEditLayout = new QVBoxLayout();
    textEditLayout->setSpacing(8);

    QLabel* textEditLabel = new QLabel("文本编辑器中的滚动条:");

    QPlainTextEdit* textEdit = new QPlainTextEdit();
    textEdit->setFixedSize(400, 200);

    QString longText;
    for (int i = 1; i <= 50; i++) {
        longText += QString("这是第 %1 行文本内容，用于演示滚动条效果。\n").arg(i);
    }
    textEdit->setPlainText(longText);

    NScrollBar* vScrollBar = new NScrollBar(Qt::Vertical, textEdit);
    textEdit->setVerticalScrollBar(vScrollBar);

    NScrollBar* hScrollBar = new NScrollBar(Qt::Horizontal, textEdit);
    textEdit->setHorizontalScrollBar(hScrollBar);

    textEditLayout->addWidget(textEditLabel);
    textEditLayout->addWidget(textEdit);

    addSection("文本编辑器示例", textEditLayout);

    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(8);

    QLabel* infoLabel = new QLabel(
        "NScrollBar 特性：\n\n"
        "• 支持垂直和水平方向\n"
        "• 可自定义滑块和轨道颜色\n"
        "• 支持悬停和按下状态的颜色变化\n"
        "• 可调整滑块和轨道的粗细\n"
        "• 支持圆角设置\n"
        "• 自动适配明暗主题\n\n"
        "NScrollBar 可以替换任何 QScrollBar，包括 QScrollArea、QTextEdit、QPlainTextEdit 等组件的滚动条。",
        this);
    infoLabel->setWordWrap(true);

    infoLayout->addWidget(infoLabel);

    addSection("组件说明", infoLayout);
}
