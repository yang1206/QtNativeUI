#include "ToggleSwitchPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NToggleSwitch.h>

ToggleSwitchPage::ToggleSwitchPage(QWidget* parent)
    : BasePage("ToggleSwitch 开关", "ToggleSwitch 用于二态切换，提供平滑的动画效果。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    auto basicSwitch = new NToggleSwitch("基本开关", this);
    basicLayout->addWidget(basicSwitch);
    auto checkedSwitch = new NToggleSwitch("默认选中", this);
    checkedSwitch->setChecked(true);
    basicLayout->addWidget(checkedSwitch);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    auto disabledSwitch = new NToggleSwitch("禁用状态", this);
    disabledSwitch->setEnabled(false);
    stateLayout->addWidget(disabledSwitch);
    auto disabledCheckedSwitch = new NToggleSwitch("禁用且选中", this);
    disabledCheckedSwitch->setChecked(true);
    disabledCheckedSwitch->setEnabled(false);
    stateLayout->addWidget(disabledCheckedSwitch);
    stateLayout->addStretch();
    addSection("不同状态", stateLayout);

    QHBoxLayout* noTextLayout = new QHBoxLayout();
    noTextLayout->setSpacing(16);
    auto noTextSwitch = new NToggleSwitch(this);
    noTextLayout->addWidget(noTextSwitch);
    auto noTextCheckedSwitch = new NToggleSwitch(this);
    noTextCheckedSwitch->setChecked(true);
    noTextLayout->addWidget(noTextCheckedSwitch);
    noTextLayout->addStretch();
    addSection("无文本", noTextLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    auto interactiveSwitch = new NToggleSwitch("点击切换", this);
    QLabel* statusLabel = new QLabel("状态: 关闭", this);
    connect(interactiveSwitch, &NToggleSwitch::checkedChanged, this, [statusLabel](bool checked) {
        statusLabel->setText(checked ? "状态: 开启" : "状态: 关闭");
    });
    interactiveLayout->addWidget(interactiveSwitch);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
