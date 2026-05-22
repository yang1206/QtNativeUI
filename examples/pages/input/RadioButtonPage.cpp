#include "RadioButtonPage.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NRadioButton.h>

RadioButtonPage::RadioButtonPage(QWidget* parent)
    : BasePage("RadioButton 单选按钮", "RadioButton 用于单选场景，同一组内只能选择一个。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    
    QButtonGroup* basicGroup = new QButtonGroup(this);
    
    auto radio1 = new NRadioButton("选项 1", this);
    radio1->setChecked(true);
    basicGroup->addButton(radio1, 0);
    basicLayout->addWidget(radio1);
    
    auto radio2 = new NRadioButton("选项 2", this);
    basicGroup->addButton(radio2, 1);
    basicLayout->addWidget(radio2);
    
    auto radio3 = new NRadioButton("选项 3", this);
    basicGroup->addButton(radio3, 2);
    basicLayout->addWidget(radio3);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    
    QButtonGroup* stateGroup = new QButtonGroup(this);
    
    auto disabledRadio = new NRadioButton("禁用状态", this);
    disabledRadio->setEnabled(false);
    stateGroup->addButton(disabledRadio, 0);
    stateLayout->addWidget(disabledRadio);
    
    auto disabledCheckedRadio = new NRadioButton("禁用且选中", this);
    disabledCheckedRadio->setChecked(true);
    disabledCheckedRadio->setEnabled(false);
    stateGroup->addButton(disabledCheckedRadio, 1);
    stateLayout->addWidget(disabledCheckedRadio);
    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->setSpacing(16);
    
    QButtonGroup* buttonGroup = new QButtonGroup(this);
    
    auto interactiveRadio1 = new NRadioButton("红色", this);
    interactiveRadio1->setChecked(true);
    buttonGroup->addButton(interactiveRadio1, 0);
    radioLayout->addWidget(interactiveRadio1);
    
    auto interactiveRadio2 = new NRadioButton("绿色", this);
    buttonGroup->addButton(interactiveRadio2, 1);
    radioLayout->addWidget(interactiveRadio2);
    
    auto interactiveRadio3 = new NRadioButton("蓝色", this);
    buttonGroup->addButton(interactiveRadio3, 2);
    radioLayout->addWidget(interactiveRadio3);
    radioLayout->addStretch();
    
    QLabel* statusLabel = new QLabel("当前选择: 红色", this);
    connect(buttonGroup, &QButtonGroup::idClicked, this, [statusLabel](int id) {
        switch (id) {
            case 0: statusLabel->setText("当前选择: 红色"); break;
            case 1: statusLabel->setText("当前选择: 绿色"); break;
            case 2: statusLabel->setText("当前选择: 蓝色"); break;
        }
    });
    interactiveLayout->addLayout(radioLayout);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
