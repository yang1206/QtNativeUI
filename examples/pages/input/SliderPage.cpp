#include "SliderPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NRangeSlider.h>
#include <QtNativeUI/NSlider.h>
#include <QtNativeUI/NTheme.h>

SliderPage::SliderPage(QWidget* parent)
    : BasePage("Slider 滑块", "Slider 用于数值选择，支持单值和范围选择。", parent) {

    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(16);
    NSlider* horizontalSlider = new NSlider(Qt::Horizontal, this);
    horizontalSlider->setRange(0, 100);
    horizontalSlider->setValue(50);
    horizontalSlider->setMinimumWidth(300);
    horizontalSlider->setShowTooltip(true);
    NLabel* horizontalValueLabel = new NLabel("值: 50", this);
    connect(horizontalSlider, &NSlider::valueChanged, [horizontalValueLabel](int value) {
        horizontalValueLabel->setText(QString("值: %1").arg(value));
    });
    horizontalLayout->addWidget(horizontalSlider);
    horizontalLayout->addWidget(horizontalValueLabel);
    horizontalLayout->addStretch();
    addSection("单滑块", horizontalLayout);

    QHBoxLayout* verticalLayout = new QHBoxLayout();
    verticalLayout->setSpacing(16);
    NSlider* verticalSlider = new NSlider(Qt::Vertical, this);
    verticalSlider->setRange(0, 100);
    verticalSlider->setValue(50);
    verticalSlider->setMinimumHeight(200);
    NLabel* verticalValueLabel = new NLabel("值: 50", this);
    connect(verticalSlider, &NSlider::valueChanged, [verticalValueLabel](int value) {
        verticalValueLabel->setText(QString("值: %1").arg(value));
    });
    verticalLayout->addWidget(verticalSlider);
    verticalLayout->addWidget(verticalValueLabel);
    verticalLayout->addStretch();
    addSection("垂直滑块", verticalLayout);

    QHBoxLayout* tickLayout = new QHBoxLayout();
    tickLayout->setSpacing(16);
    NSlider* tickSlider = new NSlider(Qt::Horizontal, this);
    tickSlider->setRange(0, 100);
    tickSlider->setValue(40);
    tickSlider->setTickPosition(QSlider::TicksBelow);
    tickSlider->setTickInterval(10);
    tickSlider->setMinimumWidth(300);
    NLabel* tickValueLabel = new NLabel("值: 40", this);
    connect(tickSlider, &NSlider::valueChanged, [tickValueLabel](int value) {
        tickValueLabel->setText(QString("值: %1").arg(value));
    });
    tickLayout->addWidget(tickSlider);
    tickLayout->addWidget(tickValueLabel);
    tickLayout->addStretch();
    addSection("带刻度", tickLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    NSlider* disabledSlider = new NSlider(Qt::Horizontal, this);
    disabledSlider->setRange(0, 100);
    disabledSlider->setValue(30);
    disabledSlider->setEnabled(false);
    disabledSlider->setMinimumWidth(300);
    stateLayout->addWidget(disabledSlider);
    stateLayout->addWidget(new NLabel("禁用状态", this));
    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);

    QHBoxLayout* basicRangeLayout = new QHBoxLayout();
    basicRangeLayout->setSpacing(16);
    NRangeSlider* basicRangeSlider = new NRangeSlider(Qt::Horizontal, this);
    basicRangeSlider->setRange(0, 100);
    basicRangeSlider->setValues(25, 75);
    basicRangeSlider->setMinimumWidth(300);
    basicRangeSlider->setShowTooltip(true);
    NLabel* rangeValueLabel = new NLabel("范围: 25 - 75", this);
    connect(basicRangeSlider, &NRangeSlider::rangeChanged, [rangeValueLabel](int lower, int upper) {
        rangeValueLabel->setText(QString("范围: %1 - %2").arg(lower).arg(upper));
    });
    basicRangeLayout->addWidget(basicRangeSlider);
    basicRangeLayout->addWidget(rangeValueLabel);
    basicRangeLayout->addStretch();
    addSection("范围滑块", basicRangeLayout);

    QHBoxLayout* verticalRangeLayout = new QHBoxLayout();
    verticalRangeLayout->setSpacing(16);
    NRangeSlider* verticalRangeSlider = new NRangeSlider(Qt::Vertical, this);
    verticalRangeSlider->setRange(0, 100);
    verticalRangeSlider->setValues(30, 70);
    verticalRangeSlider->setMinimumHeight(200);
    NLabel* verticalRangeValueLabel = new NLabel("范围: 30 - 70", this);
    connect(verticalRangeSlider, &NRangeSlider::rangeChanged, [verticalRangeValueLabel](int lower, int upper) {
        verticalRangeValueLabel->setText(QString("范围: %1 - %2").arg(lower).arg(upper));
    });
    verticalRangeLayout->addWidget(verticalRangeSlider);
    verticalRangeLayout->addWidget(verticalRangeValueLabel);
    verticalRangeLayout->addStretch();
    addSection("垂直范围滑块", verticalRangeLayout);

    QHBoxLayout* disabledRangeLayout = new QHBoxLayout();
    disabledRangeLayout->setSpacing(16);
    NRangeSlider* disabledRangeSlider = new NRangeSlider(Qt::Horizontal, this);
    disabledRangeSlider->setRange(0, 100);
    disabledRangeSlider->setValues(20, 80);
    disabledRangeSlider->setEnabled(false);
    disabledRangeSlider->setMinimumWidth(300);
    disabledRangeLayout->addWidget(disabledRangeSlider);
    disabledRangeLayout->addWidget(new NLabel("禁用状态 (20 - 80)", this));
    disabledRangeLayout->addStretch();
    addSection("禁用范围滑块", disabledRangeLayout);

    QHBoxLayout* accentRangeLayout = new QHBoxLayout();
    accentRangeLayout->setSpacing(16);
    NRangeSlider* accentRangeSlider = new NRangeSlider(Qt::Horizontal, this);
    accentRangeSlider->setRange(0, 100);
    accentRangeSlider->setValues(40, 60);
    accentRangeSlider->setAccentColor(QColor(255, 0, 128));
    accentRangeSlider->setMinimumWidth(300);
    accentRangeLayout->addWidget(accentRangeSlider);
    accentRangeLayout->addWidget(new NLabel("自定义颜色 (40 - 60)", this));
    accentRangeLayout->addStretch();
    addSection("自定义颜色", accentRangeLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    NRangeSlider* interactiveSlider = new NRangeSlider(Qt::Horizontal, this);
    interactiveSlider->setRange(0, 100);
    interactiveSlider->setValues(10, 90);
    interactiveSlider->setMinimumWidth(300);
    QHBoxLayout* labelsLayout = new QHBoxLayout();
    labelsLayout->setSpacing(16);
    NLabel* lowerLabel = new NLabel("下限: 10", this);
    NLabel* upperLabel = new NLabel("上限: 90", this);
    NLabel* spanLabel  = new NLabel("跨度: 80", this);
    connect(interactiveSlider, &NRangeSlider::lowerValueChanged, [lowerLabel, spanLabel, interactiveSlider](int value) {
        lowerLabel->setText(QString("下限: %1").arg(value));
        spanLabel->setText(QString("跨度: %1").arg(interactiveSlider->getUpperValue() - value));
    });
    connect(interactiveSlider, &NRangeSlider::upperValueChanged, [upperLabel, spanLabel, interactiveSlider](int value) {
        upperLabel->setText(QString("上限: %1").arg(value));
        spanLabel->setText(QString("跨度: %1").arg(value - interactiveSlider->getLowerValue()));
    });
    labelsLayout->addWidget(lowerLabel);
    labelsLayout->addWidget(upperLabel);
    labelsLayout->addWidget(spanLabel);
    labelsLayout->addStretch();
    interactiveLayout->addWidget(interactiveSlider);
    interactiveLayout->addLayout(labelsLayout);
    addSection("交互示例", interactiveLayout);
}
