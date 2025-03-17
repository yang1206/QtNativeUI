//
// Created by Yang1206 on 2025/3/17.
//

#include "checkbox.h"
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>

CheckBoxExample::CheckBoxExample(QWidget* parent) : QWidget(parent) { setupUI(); }

CheckBoxExample::~CheckBoxExample() {}

void CheckBoxExample::setupUI() {
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(24, 24, 24, 24);

    // 基本复选框
    auto basicCheckBox = new NCheckBox("基本复选框", this);
    layout->addWidget(basicCheckBox);

    // 默认选中的复选框
    auto checkedCheckBox = new NCheckBox("默认选中的复选框", this);
    checkedCheckBox->setChecked(true);
    layout->addWidget(checkedCheckBox);

    // 禁用状态的复选框
    auto disabledCheckBox = new NCheckBox("禁用状态的复选框", this);
    disabledCheckBox->setEnabled(false);
    layout->addWidget(disabledCheckBox);

    // 禁用且选中状态的复选框
    auto disabledCheckedCheckBox = new NCheckBox("禁用且选中状态的复选框", this);
    disabledCheckedCheckBox->setChecked(true);
    disabledCheckedCheckBox->setEnabled(false);
    layout->addWidget(disabledCheckedCheckBox);

    // 三态复选框
    auto triStateCheckBox = new NCheckBox("三态复选框", this);
    triStateCheckBox->setTristate(true);
    layout->addWidget(triStateCheckBox);

    layout->addStretch();
}
