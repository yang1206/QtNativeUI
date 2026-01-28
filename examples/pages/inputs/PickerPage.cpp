#include "PickerPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NPicker.h>

PickerPage::PickerPage(QWidget* parent)
    : BasePage("Picker 滚轮选择器", "Picker 提供滚轮式选择界面，支持循环模式和容器模式。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NPicker* basicPicker = new NPicker(this);
    QStringList hours;
    for (int i = 0; i < 24; i++) {
        hours << QString("%1").arg(i, 2, 10, QChar('0'));
    }
    basicPicker->setItems(hours);
    basicPicker->setCurrentIndex(12);
    NLabel* basicResultLabel = new NLabel("选择: 12", this);
    connect(basicPicker, &NPicker::currentDataChanged, [basicResultLabel](const QString& data) {
        basicResultLabel->setText("选择: " + data);
    });
    basicLayout->addWidget(basicPicker);
    basicLayout->addWidget(basicResultLabel);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* loopLayout = new QHBoxLayout();
    loopLayout->setSpacing(32);
    QVBoxLayout* loopEnabledLayout = new QVBoxLayout();
    NLabel*      loopEnabledLabel  = new NLabel("循环模式", this);
    loopEnabledLabel->setAlignment(Qt::AlignCenter);
    NPicker* loopEnabledPicker = new NPicker(this);
    QStringList numbers;
    for (int i = 1; i <= 10; i++) {
        numbers << QString::number(i);
    }
    loopEnabledPicker->setItems(numbers);
    loopEnabledPicker->setLoopEnabled(true);
    loopEnabledPicker->setCurrentIndex(5);
    loopEnabledLayout->addWidget(loopEnabledLabel);
    loopEnabledLayout->addWidget(loopEnabledPicker);
    QVBoxLayout* loopDisabledLayout = new QVBoxLayout();
    NLabel*      loopDisabledLabel  = new NLabel("非循环模式", this);
    loopDisabledLabel->setAlignment(Qt::AlignCenter);
    NPicker* loopDisabledPicker = new NPicker(this);
    loopDisabledPicker->setItems(numbers);
    loopDisabledPicker->setLoopEnabled(false);
    loopDisabledPicker->setCurrentIndex(5);
    loopDisabledLayout->addWidget(loopDisabledLabel);
    loopDisabledLayout->addWidget(loopDisabledPicker);
    loopLayout->addLayout(loopEnabledLayout);
    loopLayout->addLayout(loopDisabledLayout);
    loopLayout->addStretch();
    addSection("循环模式", loopLayout);

    QHBoxLayout* containerLayout = new QHBoxLayout();
    containerLayout->setSpacing(16);
    NPicker* containerPicker = new NPicker(this);
    QStringList months;
    for (int i = 1; i <= 12; i++) {
        months << QString("%1月").arg(i);
    }
    containerPicker->setItems(months);
    containerPicker->setContainer(true);
    containerPicker->setCurrentIndex(0);
    NLabel* containerResultLabel = new NLabel("选择: 1月", this);
    connect(containerPicker, &NPicker::currentDataChanged, [containerResultLabel](const QString& data) {
        containerResultLabel->setText("选择: " + data);
    });
    containerLayout->addWidget(containerPicker);
    containerLayout->addWidget(containerResultLabel);
    containerLayout->addStretch();
    addSection("Container 模式", containerLayout);

    QHBoxLayout* itemHeightLayout = new QHBoxLayout();
    itemHeightLayout->setSpacing(32);
    QVBoxLayout* normalHeightLayout = new QVBoxLayout();
    NLabel*      normalHeightLabel  = new NLabel("默认高度", this);
    normalHeightLabel->setAlignment(Qt::AlignCenter);
    NPicker* normalHeightPicker = new NPicker(this);
    normalHeightPicker->setItems(numbers);
    normalHeightPicker->setCurrentIndex(5);
    normalHeightLayout->addWidget(normalHeightLabel);
    normalHeightLayout->addWidget(normalHeightPicker);
    QVBoxLayout* customHeightLayout = new QVBoxLayout();
    NLabel*      customHeightLabel  = new NLabel("自定义高度", this);
    customHeightLabel->setAlignment(Qt::AlignCenter);
    NPicker* customHeightPicker = new NPicker(this);
    customHeightPicker->setItems(numbers);
    customHeightPicker->setItemHeight(50);
    customHeightPicker->setCurrentIndex(5);
    customHeightLayout->addWidget(customHeightLabel);
    customHeightLayout->addWidget(customHeightPicker);
    itemHeightLayout->addLayout(normalHeightLayout);
    itemHeightLayout->addLayout(customHeightLayout);
    itemHeightLayout->addStretch();
    addSection("自定义项高度", itemHeightLayout);

    QVBoxLayout* timeLayout = new QVBoxLayout();
    timeLayout->setSpacing(8);
    QHBoxLayout* pickersLayout = new QHBoxLayout();
    pickersLayout->setSpacing(8);
    NPicker* hourPicker = new NPicker(this);
    hourPicker->setItems(hours);
    hourPicker->setCurrentIndex(12);
    pickersLayout->addWidget(hourPicker);
    NLabel* colonLabel1 = new NLabel(":", this);
    QFont   colonFont   = colonLabel1->font();
    colonFont.setPointSize(20);
    colonFont.setBold(true);
    colonLabel1->setFont(colonFont);
    pickersLayout->addWidget(colonLabel1);
    NPicker* minutePicker = new NPicker(this);
    QStringList minutes;
    for (int i = 0; i < 60; i++) {
        minutes << QString("%1").arg(i, 2, 10, QChar('0'));
    }
    minutePicker->setItems(minutes);
    minutePicker->setCurrentIndex(30);
    pickersLayout->addWidget(minutePicker);
    NLabel* colonLabel2 = new NLabel(":", this);
    colonLabel2->setFont(colonFont);
    pickersLayout->addWidget(colonLabel2);
    NPicker* secondPicker = new NPicker(this);
    secondPicker->setItems(minutes);
    secondPicker->setCurrentIndex(0);
    pickersLayout->addWidget(secondPicker);
    pickersLayout->addStretch();
    NLabel* timeResultLabel = new NLabel("时间: 12:30:00", this);
    auto updateTime = [=]() {
        QString time = QString("%1:%2:%3")
                           .arg(hourPicker->getCurrentData())
                           .arg(minutePicker->getCurrentData())
                           .arg(secondPicker->getCurrentData());
        timeResultLabel->setText("时间: " + time);
    };
    connect(hourPicker, &NPicker::currentDataChanged, updateTime);
    connect(minutePicker, &NPicker::currentDataChanged, updateTime);
    connect(secondPicker, &NPicker::currentDataChanged, updateTime);
    timeLayout->addLayout(pickersLayout);
    timeLayout->addWidget(timeResultLabel);
    addSection("时间选择器组合", timeLayout);
}
