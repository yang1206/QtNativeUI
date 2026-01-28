#include "ToggleButtonPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NToggleButton.h>

ToggleButtonPage::ToggleButtonPage(QWidget* parent)
    : BasePage("ToggleButton 切换按钮", "ToggleButton 是可以切换选中状态的按钮，适合开关类操作。", parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(24);

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NToggleButton* uncheckedBtn = new NToggleButton("未选中");
    uncheckedBtn->setFixedSize(120, 40);

    NToggleButton* checkedBtn = new NToggleButton("已选中");
    checkedBtn->setFixedSize(120, 40);
    checkedBtn->setChecked(true);

    NToggleButton* disabledBtn = new NToggleButton("禁用");
    disabledBtn->setFixedSize(120, 40);
    disabledBtn->setEnabled(false);

    NToggleButton* disabledCheckedBtn = new NToggleButton("禁用选中");
    disabledCheckedBtn->setFixedSize(120, 40);
    disabledCheckedBtn->setChecked(true);
    disabledCheckedBtn->setEnabled(false);

    basicLayout->addWidget(uncheckedBtn);
    basicLayout->addWidget(checkedBtn);
    basicLayout->addWidget(disabledBtn);
    basicLayout->addWidget(disabledCheckedBtn);
    basicLayout->addStretch();

    mainLayout->addWidget(new QLabel("基本状态："));
    mainLayout->addLayout(basicLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);

    NToggleButton* iconTextBtn = new NToggleButton("收藏");
    iconTextBtn->setFixedSize(120, 40);
    iconTextBtn->setFluentIcon(NRegularIconType::Star24Regular, 20);

    NToggleButton* iconOnlyBtn = new NToggleButton();
    iconOnlyBtn->setFixedSize(40, 40);
    iconOnlyBtn->setFluentIcon(NRegularIconType::Heart24Regular, 24);

    NToggleButton* checkedIconBtn = new NToggleButton("已收藏");
    checkedIconBtn->setFixedSize(120, 40);
    checkedIconBtn->setFluentIcon(NFilledIconType::Star24Filled, 20);
    checkedIconBtn->setChecked(true);

    iconLayout->addWidget(iconTextBtn);
    iconLayout->addWidget(iconOnlyBtn);
    iconLayout->addWidget(checkedIconBtn);
    iconLayout->addStretch();

    mainLayout->addWidget(new QLabel("带图标："));
    mainLayout->addLayout(iconLayout);

    QHBoxLayout* interactiveLayout = new QHBoxLayout();
    interactiveLayout->setSpacing(16);

    NToggleButton* toggleBtn = new NToggleButton("点击切换");
    toggleBtn->setFixedSize(120, 40);
    toggleBtn->setFluentIcon(NRegularIconType::Play24Regular, 20);

    QLabel* statusLabel = new QLabel("状态: 未选中");

    connect(toggleBtn, &NToggleButton::toggled, [statusLabel, toggleBtn](bool checked) {
        statusLabel->setText(checked ? "状态: 已选中" : "状态: 未选中");
        if (checked) {
            toggleBtn->setFluentIcon(NRegularIconType::Pause24Regular, 20);
            toggleBtn->setText("暂停");
        } else {
            toggleBtn->setFluentIcon(NRegularIconType::Play24Regular, 20);
            toggleBtn->setText("播放");
        }
    });

    interactiveLayout->addWidget(toggleBtn);
    interactiveLayout->addWidget(statusLabel);
    interactiveLayout->addStretch();

    mainLayout->addWidget(new QLabel("交互示例："));
    mainLayout->addLayout(interactiveLayout);

    addSection("ToggleButton 完整示例", mainLayout);
}
