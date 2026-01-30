#include "DoubleSpinBoxPage.h"
#include <QHBoxLayout>
#include <QtNativeUI/NDoubleSpinBox.h>

DoubleSpinBoxPage::DoubleSpinBoxPage(QWidget* parent)
    : BasePage("DoubleSpinBox 浮点数输入框", "DoubleSpinBox 用于浮点数输入，支持精度设置。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NDoubleSpinBox* normalDoubleSpinBox = new NDoubleSpinBox(this);
    normalDoubleSpinBox->setMinimumWidth(120);
    basicLayout->addWidget(normalDoubleSpinBox);

    NDoubleSpinBox* rangeDoubleSpinBox = new NDoubleSpinBox(this);
    rangeDoubleSpinBox->setRange(0, 100);
    rangeDoubleSpinBox->setValue(50);
    rangeDoubleSpinBox->setMinimumWidth(120);
    basicLayout->addWidget(rangeDoubleSpinBox);

    NDoubleSpinBox* stepDoubleSpinBox = new NDoubleSpinBox(this);
    stepDoubleSpinBox->setRange(0, 100);
    stepDoubleSpinBox->setSingleStep(5);
    stepDoubleSpinBox->setValue(25);
    stepDoubleSpinBox->setMinimumWidth(120);
    basicLayout->addWidget(stepDoubleSpinBox);

    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* prefixLayout = new QHBoxLayout();
    prefixLayout->setSpacing(16);

    NDoubleSpinBox* prefixSuffixDoubleSpinBox = new NDoubleSpinBox(this);
    prefixSuffixDoubleSpinBox->setPrefix("$");
    prefixSuffixDoubleSpinBox->setSuffix(".00");
    prefixSuffixDoubleSpinBox->setRange(0, 1000);
    prefixSuffixDoubleSpinBox->setValue(100);
    prefixSuffixDoubleSpinBox->setMinimumWidth(150);
    prefixLayout->addWidget(prefixSuffixDoubleSpinBox);

    prefixLayout->addStretch();
    addSection("前缀后缀", prefixLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);

    NDoubleSpinBox* disabledDoubleSpinBox = new NDoubleSpinBox(this);
    disabledDoubleSpinBox->setValue(50);
    disabledDoubleSpinBox->setEnabled(false);
    disabledDoubleSpinBox->setMinimumWidth(120);
    stateLayout->addWidget(disabledDoubleSpinBox);

    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);
}
