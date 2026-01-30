#include "SpinBoxPage.h"
#include <QHBoxLayout>
#include <QtNativeUI/NSpinBox.h>

SpinBoxPage::SpinBoxPage(QWidget* parent)
    : BasePage("SpinBox 数字输入框", "SpinBox 用于整数输入，支持范围限制和步长设置。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NSpinBox* normalSpinBox = new NSpinBox(this);
    normalSpinBox->setMinimumWidth(120);
    basicLayout->addWidget(normalSpinBox);

    NSpinBox* rangeSpinBox = new NSpinBox(this);
    rangeSpinBox->setRange(0, 100);
    rangeSpinBox->setValue(50);
    rangeSpinBox->setMinimumWidth(120);
    basicLayout->addWidget(rangeSpinBox);

    NSpinBox* stepSpinBox = new NSpinBox(this);
    stepSpinBox->setRange(0, 100);
    stepSpinBox->setSingleStep(5);
    stepSpinBox->setValue(25);
    stepSpinBox->setMinimumWidth(120);
    basicLayout->addWidget(stepSpinBox);

    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* prefixLayout = new QHBoxLayout();
    prefixLayout->setSpacing(16);

    NSpinBox* prefixSuffixSpinBox = new NSpinBox(this);
    prefixSuffixSpinBox->setPrefix("$");
    prefixSuffixSpinBox->setSuffix(".00");
    prefixSuffixSpinBox->setRange(0, 1000);
    prefixSuffixSpinBox->setValue(100);
    prefixSuffixSpinBox->setMinimumWidth(150);
    prefixLayout->addWidget(prefixSuffixSpinBox);

    prefixLayout->addStretch();
    addSection("前缀后缀", prefixLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);

    NSpinBox* disabledSpinBox = new NSpinBox(this);
    disabledSpinBox->setValue(50);
    disabledSpinBox->setEnabled(false);
    disabledSpinBox->setMinimumWidth(120);
    stateLayout->addWidget(disabledSpinBox);

    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);
}
