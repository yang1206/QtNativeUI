#include "ProgressRingPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NProgressRing.h>
#include <QtNativeUI/NPushButton.h>

ProgressRingPage::ProgressRingPage(QWidget* parent)
    : BasePage("ProgressRing 进度环", "ProgressRing 以环形方式显示操作进度，适合空间受限的场景。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(32);

    NProgressRing* basicProgressRing = new NProgressRing(this);
    basicProgressRing->setValue(50);
    basicProgressRing->setFixedSize(100, 100);

    NProgressRing* thickProgressRing = new NProgressRing(this);
    thickProgressRing->setValue(75);
    thickProgressRing->setStrokeWidth(10);
    thickProgressRing->setFixedSize(100, 100);

    NProgressRing* textProgressRing = new NProgressRing(this);
    textProgressRing->setValue(65);
    textProgressRing->setFormat("%p%");
    textProgressRing->setTextVisible(true);
    textProgressRing->setFixedSize(100, 100);

    QLabel* basicLabel = new QLabel("基本", this);
    basicLabel->setAlignment(Qt::AlignCenter);
    QLabel* thickLabel = new QLabel("粗线条", this);
    thickLabel->setAlignment(Qt::AlignCenter);
    QLabel* textLabel = new QLabel("带文本", this);
    textLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* basic1Layout = new QVBoxLayout();
    basic1Layout->addWidget(basicProgressRing);
    basic1Layout->addWidget(basicLabel);

    QVBoxLayout* basic2Layout = new QVBoxLayout();
    basic2Layout->addWidget(thickProgressRing);
    basic2Layout->addWidget(thickLabel);

    QVBoxLayout* basic3Layout = new QVBoxLayout();
    basic3Layout->addWidget(textProgressRing);
    basic3Layout->addWidget(textLabel);

    basicLayout->addLayout(basic1Layout);
    basicLayout->addLayout(basic2Layout);
    basicLayout->addLayout(basic3Layout);
    basicLayout->addStretch();

    addSection("基本进度环", basicLayout);

    QHBoxLayout* indeterminateLayout = new QHBoxLayout();
    indeterminateLayout->setSpacing(32);

    NProgressRing* indeterminateProgressRing = new NProgressRing(this);
    indeterminateProgressRing->setMinimum(0);
    indeterminateProgressRing->setMaximum(0);
    indeterminateProgressRing->setFixedSize(100, 100);

    NProgressRing* textIndeterminateProgressRing = new NProgressRing(this);
    textIndeterminateProgressRing->setMinimum(0);
    textIndeterminateProgressRing->setMaximum(0);
    textIndeterminateProgressRing->setFormat("加载中...");
    textIndeterminateProgressRing->setTextVisible(true);
    textIndeterminateProgressRing->setFixedSize(100, 100);

    NProgressRing* specialIndeterminateRing = new NProgressRing(this);
    specialIndeterminateRing->setIndeterminate(true);
    specialIndeterminateRing->setFixedSize(100, 100);

    QLabel* indeterminateLabel1 = new QLabel("不确定状态", this);
    indeterminateLabel1->setAlignment(Qt::AlignCenter);
    QLabel* indeterminateLabel2 = new QLabel("带文本", this);
    indeterminateLabel2->setAlignment(Qt::AlignCenter);
    QLabel* indeterminateLabel3 = new QLabel("专用方法", this);
    indeterminateLabel3->setAlignment(Qt::AlignCenter);

    QVBoxLayout* indeterminate1Layout = new QVBoxLayout();
    indeterminate1Layout->addWidget(indeterminateProgressRing);
    indeterminate1Layout->addWidget(indeterminateLabel1);

    QVBoxLayout* indeterminate2Layout = new QVBoxLayout();
    indeterminate2Layout->addWidget(textIndeterminateProgressRing);
    indeterminate2Layout->addWidget(indeterminateLabel2);

    QVBoxLayout* indeterminate3Layout = new QVBoxLayout();
    indeterminate3Layout->addWidget(specialIndeterminateRing);
    indeterminate3Layout->addWidget(indeterminateLabel3);

    indeterminateLayout->addLayout(indeterminate1Layout);
    indeterminateLayout->addLayout(indeterminate2Layout);
    indeterminateLayout->addLayout(indeterminate3Layout);
    indeterminateLayout->addStretch();

    addSection("不确定状态进度环", indeterminateLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(32);

    NProgressRing* normalProgressRing = new NProgressRing(this);
    normalProgressRing->setValue(80);
    normalProgressRing->setFixedSize(100, 100);
    QLabel* normalLabel = new QLabel("正常状态", this);
    normalLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* normalLayout = new QVBoxLayout();
    normalLayout->addWidget(normalProgressRing);
    normalLayout->addWidget(normalLabel);

    NProgressRing* pausedProgressRing = new NProgressRing(this);
    pausedProgressRing->setValue(60);
    pausedProgressRing->setPaused(true);
    pausedProgressRing->setFixedSize(100, 100);
    QLabel* pausedLabel = new QLabel("暂停状态", this);
    pausedLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* pausedLayout = new QVBoxLayout();
    pausedLayout->addWidget(pausedProgressRing);
    pausedLayout->addWidget(pausedLabel);

    NProgressRing* errorProgressRing = new NProgressRing(this);
    errorProgressRing->setValue(30);
    errorProgressRing->setError(true);
    errorProgressRing->setFixedSize(100, 100);
    QLabel* errorLabel = new QLabel("错误状态", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* errorLayout = new QVBoxLayout();
    errorLayout->addWidget(errorProgressRing);
    errorLayout->addWidget(errorLabel);

    NProgressRing* disabledProgressRing = new NProgressRing(this);
    disabledProgressRing->setValue(45);
    disabledProgressRing->setEnabled(false);
    disabledProgressRing->setFixedSize(100, 100);
    QLabel* disabledLabel = new QLabel("禁用状态", this);
    disabledLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* disabledLayout = new QVBoxLayout();
    disabledLayout->addWidget(disabledProgressRing);
    disabledLayout->addWidget(disabledLabel);

    stateLayout->addLayout(normalLayout);
    stateLayout->addLayout(pausedLayout);
    stateLayout->addLayout(errorLayout);
    stateLayout->addLayout(disabledLayout);
    stateLayout->addStretch();

    addSection("不同状态", stateLayout);

    QHBoxLayout* dynamicLayout = new QHBoxLayout();
    dynamicLayout->setSpacing(24);

    NProgressRing* dynamicProgressRing = new NProgressRing(this);
    dynamicProgressRing->setValue(0);
    dynamicProgressRing->setFixedSize(120, 120);
    dynamicProgressRing->setTextVisible(true);
    dynamicProgressRing->setFormat("%p%");

    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(8);

    NPushButton* startButton = new NPushButton("开始", this);
    startButton->setFixedSize(80, 32);
    connect(startButton, &NPushButton::clicked, [dynamicProgressRing]() {
        QTimer* timer = new QTimer(dynamicProgressRing);
        dynamicProgressRing->setProperty("timer", QVariant::fromValue(timer));
        connect(timer, &QTimer::timeout, [dynamicProgressRing]() {
            int value = dynamicProgressRing->value();
            if (value < 100) {
                dynamicProgressRing->setValue(value + 1);
            } else {
                QTimer* timer = dynamicProgressRing->property("timer").value<QTimer*>();
                if (timer) {
                    timer->stop();
                }
            }
        });
        timer->start(50);
    });
    controlLayout->addWidget(startButton);

    NPushButton* resetButton = new NPushButton("重置", this);
    resetButton->setFixedSize(80, 32);
    connect(resetButton, &NPushButton::clicked, [dynamicProgressRing]() {
        QTimer* timer = dynamicProgressRing->property("timer").value<QTimer*>();
        if (timer) {
            timer->stop();
        }
        dynamicProgressRing->setValue(0);
    });
    controlLayout->addWidget(resetButton);

    NPushButton* pauseButton = new NPushButton("暂停/继续", this);
    pauseButton->setFixedSize(80, 32);
    connect(pauseButton, &NPushButton::clicked, [dynamicProgressRing]() {
        QTimer* timer = dynamicProgressRing->property("timer").value<QTimer*>();

        if (dynamicProgressRing->isPaused()) {
            dynamicProgressRing->resume();
            if (timer && dynamicProgressRing->value() < 100) {
                timer->start();
            }
        } else {
            dynamicProgressRing->pause();
            if (timer) {
                timer->stop();
            }
        }
    });
    controlLayout->addWidget(pauseButton);

    NPushButton* errorButton = new NPushButton("错误", this);
    errorButton->setFixedSize(80, 32);
    connect(errorButton, &NPushButton::clicked, [dynamicProgressRing]() {
        dynamicProgressRing->setError(!dynamicProgressRing->isError());
    });
    controlLayout->addWidget(errorButton);

    controlLayout->addStretch();

    dynamicLayout->addWidget(dynamicProgressRing);
    dynamicLayout->addLayout(controlLayout);
    dynamicLayout->addStretch();

    addSection("动态进度环", dynamicLayout);
}
