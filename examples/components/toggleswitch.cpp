//
// Created by Yang1206 on 2025/3/24.
//

#include "toggleswitch.h"
#include <QVBoxLayout>
#include <QtNativeUI/NToggleSwitch.h>

ToggleSwitchExample::ToggleSwitchExample(QWidget* parent) : QWidget(parent) { setupUI(); }

ToggleSwitchExample::~ToggleSwitchExample() {}

void ToggleSwitchExample::setupUI() {
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(24, 24, 24, 24);

    // 基本开关
    auto basicSwitch = new NToggleSwitch("基本开关", this);
    layout->addWidget(basicSwitch);

    // 默认选中的开关
    auto checkedSwitch = new NToggleSwitch("默认选中的开关", this);
    checkedSwitch->setChecked(true);
    layout->addWidget(checkedSwitch);

    // 禁用状态的开关
    auto disabledSwitch = new NToggleSwitch("禁用状态的开关", this);
    disabledSwitch->setEnabled(false);
    layout->addWidget(disabledSwitch);

    // 禁用且选中状态的开关
    auto disabledCheckedSwitch = new NToggleSwitch("禁用且选中状态的开关", this);
    disabledCheckedSwitch->setChecked(true);
    disabledCheckedSwitch->setEnabled(false);
    layout->addWidget(disabledCheckedSwitch);

    // 无文本的开关
    auto noTextSwitch = new NToggleSwitch(this);
    layout->addWidget(noTextSwitch);

    // 连接信号槽示例
    connect(basicSwitch, &NToggleSwitch::toggled, this, [](bool checked) {
        qDebug() << "开关状态：" << (checked ? "打开" : "关闭");
    });

    layout->addStretch();
} 