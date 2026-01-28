#include "ToolTipPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NToolTip.h>

ToolTipPage::ToolTipPage(QWidget* parent)
    : BasePage("ToolTip 工具提示", "鼠标悬停时显示的提示信息，支持多种位置。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NPushButton* topButton = new NPushButton("顶部提示");
    topButton->setFixedSize(120, 40);
    topButton->setToolTip("这是顶部的工具提示");
    new NToolTipFilter(topButton, 300, NToolTipPosition::TOP);

    NPushButton* bottomButton = new NPushButton("底部提示");
    bottomButton->setFixedSize(120, 40);
    bottomButton->setToolTip("这是底部的工具提示");
    new NToolTipFilter(bottomButton, 300, NToolTipPosition::BOTTOM);

    NPushButton* leftButton = new NPushButton("左侧提示");
    leftButton->setFixedSize(120, 40);
    leftButton->setToolTip("这是左侧的工具提示");
    new NToolTipFilter(leftButton, 300, NToolTipPosition::LEFT);

    NPushButton* rightButton = new NPushButton("右侧提示");
    rightButton->setFixedSize(120, 40);
    rightButton->setToolTip("这是右侧的工具提示");
    new NToolTipFilter(rightButton, 300, NToolTipPosition::RIGHT);

    basicLayout->addWidget(topButton);
    basicLayout->addWidget(bottomButton);
    basicLayout->addWidget(leftButton);
    basicLayout->addWidget(rightButton);
    basicLayout->addStretch();

    addSection("不同位置", basicLayout);

    QHBoxLayout* cornerLayout = new QHBoxLayout();
    cornerLayout->setSpacing(16);

    NPushButton* topLeftButton = new NPushButton("左上角");
    topLeftButton->setFixedSize(120, 40);
    topLeftButton->setToolTip("这是左上角的工具提示");
    new NToolTipFilter(topLeftButton, 300, NToolTipPosition::TOP_LEFT);

    NPushButton* topRightButton = new NPushButton("右上角");
    topRightButton->setFixedSize(120, 40);
    topRightButton->setToolTip("这是右上角的工具提示");
    new NToolTipFilter(topRightButton, 300, NToolTipPosition::TOP_RIGHT);

    NPushButton* bottomLeftButton = new NPushButton("左下角");
    bottomLeftButton->setFixedSize(120, 40);
    bottomLeftButton->setToolTip("这是左下角的工具提示");
    new NToolTipFilter(bottomLeftButton, 300, NToolTipPosition::BOTTOM_LEFT);

    NPushButton* bottomRightButton = new NPushButton("右下角");
    bottomRightButton->setFixedSize(120, 40);
    bottomRightButton->setToolTip("这是右下角的工具提示");
    new NToolTipFilter(bottomRightButton, 300, NToolTipPosition::BOTTOM_RIGHT);

    cornerLayout->addWidget(topLeftButton);
    cornerLayout->addWidget(topRightButton);
    cornerLayout->addWidget(bottomLeftButton);
    cornerLayout->addWidget(bottomRightButton);
    cornerLayout->addStretch();

    addSection("角落位置", cornerLayout);

    QHBoxLayout* delayLayout = new QHBoxLayout();
    delayLayout->setSpacing(16);

    NPushButton* fastButton = new NPushButton("快速显示(100ms)");
    fastButton->setFixedSize(160, 40);
    fastButton->setToolTip("这个提示会快速显示");
    new NToolTipFilter(fastButton, 100, NToolTipPosition::TOP);

    NPushButton* normalButton = new NPushButton("正常显示(300ms)");
    normalButton->setFixedSize(160, 40);
    normalButton->setToolTip("这个提示正常速度显示");
    new NToolTipFilter(normalButton, 300, NToolTipPosition::TOP);

    NPushButton* slowButton = new NPushButton("慢速显示(800ms)");
    slowButton->setFixedSize(160, 40);
    slowButton->setToolTip("这个提示会慢速显示");
    new NToolTipFilter(slowButton, 800, NToolTipPosition::TOP);

    delayLayout->addWidget(fastButton);
    delayLayout->addWidget(normalButton);
    delayLayout->addWidget(slowButton);
    delayLayout->addStretch();

    addSection("不同延迟", delayLayout);

    QHBoxLayout* clickLayout = new QHBoxLayout();
    clickLayout->setSpacing(16);

    NPushButton* clickButton = new NPushButton("点击显示提示");
    clickButton->setFixedSize(140, 40);
    connect(clickButton, &NPushButton::clicked, this, [clickButton]() {
        NToolTip::showText("这是点击后显示的提示", clickButton, NToolTipPosition::TOP);
    });

    clickLayout->addWidget(clickButton);
    clickLayout->addStretch();

    addSection("点击触发", clickLayout);
}
