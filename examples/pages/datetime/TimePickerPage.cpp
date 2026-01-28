#include "TimePickerPage.h"
#include <QHBoxLayout>
#include <QTime>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NTimePicker.h>

TimePickerPage::TimePickerPage(QWidget* parent)
    : BasePage("TimePicker 时间选择器", "TimePicker 提供时间选择功能，支持 12/24 小时制和秒数显示。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NTimePicker* basicTimePicker = new NTimePicker(this);
    basicTimePicker->setTime(QTime::currentTime());
    NLabel* basicResultLabel = new NLabel("选择时间: " + basicTimePicker->getTime().toString("HH:mm"), this);
    connect(basicTimePicker, &NTimePicker::timeChanged, [basicResultLabel](const QTime& time) {
        basicResultLabel->setText("选择时间: " + time.toString("HH:mm"));
    });
    basicLayout->addWidget(basicTimePicker);
    basicLayout->addWidget(basicResultLabel);
    basicLayout->addStretch();
    addSection("基本用法 (24小时制)", basicLayout);

    QHBoxLayout* secondsLayout = new QHBoxLayout();
    secondsLayout->setSpacing(16);
    NTimePicker* secondsTimePicker = new NTimePicker(this);
    secondsTimePicker->setShowSeconds(true);
    secondsTimePicker->setTime(QTime::currentTime());
    secondsTimePicker->setFixedWidth(180);
    NLabel* secondsResultLabel = new NLabel("选择时间: " + secondsTimePicker->getTime().toString("HH:mm:ss"), this);
    connect(secondsTimePicker, &NTimePicker::timeChanged, [secondsResultLabel](const QTime& time) {
        secondsResultLabel->setText("选择时间: " + time.toString("HH:mm:ss"));
    });
    secondsLayout->addWidget(secondsTimePicker);
    secondsLayout->addWidget(secondsResultLabel);
    secondsLayout->addStretch();
    addSection("显示秒数", secondsLayout);

    QHBoxLayout* formatLayout = new QHBoxLayout();
    formatLayout->setSpacing(32);
    QVBoxLayout* format24Layout = new QVBoxLayout();
    NLabel*      format24Label  = new NLabel("24小时制", this);
    format24Label->setAlignment(Qt::AlignCenter);
    NTimePicker* format24Picker = new NTimePicker(this);
    format24Picker->setUse24HourFormat(true);
    format24Picker->setTime(QTime(14, 30));
    NLabel* format24Result = new NLabel("14:30", this);
    format24Result->setAlignment(Qt::AlignCenter);
    connect(format24Picker, &NTimePicker::timeChanged, [format24Result](const QTime& time) {
        format24Result->setText(time.toString("HH:mm"));
    });
    format24Layout->addWidget(format24Label);
    format24Layout->addWidget(format24Picker);
    format24Layout->addWidget(format24Result);
    QVBoxLayout* format12Layout = new QVBoxLayout();
    NLabel*      format12Label  = new NLabel("12小时制", this);
    format12Label->setAlignment(Qt::AlignCenter);
    NTimePicker* format12Picker = new NTimePicker(this);
    format12Picker->setUse24HourFormat(false);
    format12Picker->setTime(QTime(14, 30));
    NLabel* format12Result = new NLabel("02:30 PM", this);
    format12Result->setAlignment(Qt::AlignCenter);
    connect(format12Picker, &NTimePicker::timeChanged, [format12Result](const QTime& time) {
        format12Result->setText(time.toString("hh:mm AP"));
    });
    format12Layout->addWidget(format12Label);
    format12Layout->addWidget(format12Picker);
    format12Layout->addWidget(format12Result);
    formatLayout->addLayout(format24Layout);
    formatLayout->addLayout(format12Layout);
    formatLayout->addStretch();
    addSection("12小时制 vs 24小时制", formatLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    QVBoxLayout* disabledLayout = new QVBoxLayout();
    NTimePicker* disabledPicker = new NTimePicker(this);
    disabledPicker->setTime(QTime(10, 30));
    disabledPicker->setEnabled(false);
    NLabel* disabledLabel = new NLabel("禁用状态", this);
    disabledLabel->setAlignment(Qt::AlignCenter);
    disabledLayout->addWidget(disabledPicker);
    disabledLayout->addWidget(disabledLabel);
    stateLayout->addLayout(disabledLayout);
    stateLayout->addStretch();
    addSection("不同状态", stateLayout);
}
