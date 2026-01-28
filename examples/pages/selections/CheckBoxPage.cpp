#include "CheckBoxPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>

CheckBoxPage::CheckBoxPage(QWidget* parent)
    : BasePage("CheckBox 复选框", "CheckBox 允许用户在多个选项中进行选择，支持三态模式。", parent) {
    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    auto basicCheckBox = new NCheckBox("基本复选框", this);
    basicLayout->addWidget(basicCheckBox);
    auto checkedCheckBox = new NCheckBox("默认选中", this);
    checkedCheckBox->setChecked(true);
    basicLayout->addWidget(checkedCheckBox);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    auto disabledCheckBox = new NCheckBox("禁用状态", this);
    disabledCheckBox->setEnabled(false);
    stateLayout->addWidget(disabledCheckBox);
    auto disabledCheckedCheckBox = new NCheckBox("禁用且选中", this);
    disabledCheckedCheckBox->setChecked(true);
    disabledCheckedCheckBox->setEnabled(false);
    stateLayout->addWidget(disabledCheckedCheckBox);
    basicLayout->addStretch();
    addSection("不同状态", stateLayout);

    QHBoxLayout* tristateLayout = new QHBoxLayout();
    tristateLayout->setSpacing(16);
    auto triStateCheckBox = new NCheckBox("三态复选框", this);
    triStateCheckBox->setTristate(true);
    triStateCheckBox->setCheckState(Qt::PartiallyChecked);
    tristateLayout->addWidget(triStateCheckBox);
    auto triStateUnchecked = new NCheckBox("未选中", this);
    triStateUnchecked->setTristate(true);
    triStateUnchecked->setCheckState(Qt::Unchecked);
    tristateLayout->addWidget(triStateUnchecked);
    auto triStateChecked = new NCheckBox("已选中", this);
    triStateChecked->setTristate(true);
    triStateChecked->setCheckState(Qt::Checked);
    tristateLayout->addWidget(triStateChecked);
    tristateLayout->addStretch();
    addSection("三态模式", tristateLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    auto    interactiveCheckBox = new NCheckBox("点击切换状态", this);
    QLabel* statusLabel         = new QLabel("状态: 未选中", this);
    connect(interactiveCheckBox, &NCheckBox::checkStateChanged, this, [statusLabel](int state) {
        QString stateText = state == Qt::Checked ? "已选中" : "未选中";
        statusLabel->setText("状态: " + stateText);
    });
    interactiveLayout->addWidget(interactiveCheckBox);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
