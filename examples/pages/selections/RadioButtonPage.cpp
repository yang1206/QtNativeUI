#include "RadioButtonPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NRadioButton.h>

RadioButtonPage::RadioButtonPage(QWidget* parent)
    : BasePage("RadioButton 单选按钮", "RadioButton 用于单选场景，同一组内只能选择一个。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    auto radio1 = new NRadioButton("选项 1", this);
    radio1->setChecked(true);
    basicLayout->addWidget(radio1);
    auto radio2 = new NRadioButton("选项 2", this);
    basicLayout->addWidget(radio2);
    auto radio3 = new NRadioButton("选项 3", this);
    basicLayout->addWidget(radio3);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    auto disabledRadio = new NRadioButton("禁用状态", this);
    disabledRadio->setEnabled(false);
    stateLayout->addWidget(disabledRadio);
    auto disabledCheckedRadio = new NRadioButton("禁用且选中", this);
    disabledCheckedRadio->setChecked(true);
    disabledCheckedRadio->setEnabled(false);
    stateLayout->addWidget(disabledCheckedRadio);
    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->setSpacing(16);
    auto interactiveRadio1 = new NRadioButton("红色", this);
    interactiveRadio1->setChecked(true);
    radioLayout->addWidget(interactiveRadio1);
    auto interactiveRadio2 = new NRadioButton("绿色", this);
    radioLayout->addWidget(interactiveRadio2);
    auto interactiveRadio3 = new NRadioButton("蓝色", this);
    radioLayout->addWidget(interactiveRadio3);
    radioLayout->addStretch();
    QLabel* statusLabel = new QLabel("当前选择: 红色", this);
    connect(interactiveRadio1, &NRadioButton::toggled, this, [statusLabel](bool checked) {
        if (checked) statusLabel->setText("当前选择: 红色");
    });
    connect(interactiveRadio2, &NRadioButton::toggled, this, [statusLabel](bool checked) {
        if (checked) statusLabel->setText("当前选择: 绿色");
    });
    connect(interactiveRadio3, &NRadioButton::toggled, this, [statusLabel](bool checked) {
        if (checked) statusLabel->setText("当前选择: 蓝色");
    });
    interactiveLayout->addLayout(radioLayout);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
