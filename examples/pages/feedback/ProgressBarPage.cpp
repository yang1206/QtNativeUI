#include "ProgressBarPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NProgressBar.h>
#include <QtNativeUI/NPushButton.h>

ProgressBarPage::ProgressBarPage(QWidget* parent)
    : BasePage("ProgressBar 进度条", "ProgressBar 显示操作进度，支持确定和不确定状态，以及垂直和水平方向。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NProgressBar* basicProgressBar = new NProgressBar(this);
    basicProgressBar->setValue(50);
    basicProgressBar->setMinimumWidth(300);

    basicLayout->addWidget(basicProgressBar);

    addSection("基本进度条", basicLayout);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(16);

    NProgressBar* textProgressBar = new NProgressBar(this);
    textProgressBar->setProgressThickness(20);
    textProgressBar->setBorderRadius(10);
    textProgressBar->setValue(75);
    textProgressBar->setFormat("已完成 %p%");
    textProgressBar->setTextVisible(true);
    textProgressBar->setMinimumWidth(300);

    textLayout->addWidget(textProgressBar);

    addSection("带文本的进度条", textLayout);

    QVBoxLayout* indeterminateLayout = new QVBoxLayout();
    indeterminateLayout->setSpacing(16);

    NProgressBar* indeterminateProgressBar = new NProgressBar(this);
    indeterminateProgressBar->setIndeterminate(true);
    indeterminateProgressBar->setMinimumWidth(300);

    indeterminateLayout->addWidget(indeterminateProgressBar);

    addSection("不确定状态进度条", indeterminateLayout);

    QHBoxLayout* verticalLayout = new QHBoxLayout();
    verticalLayout->setSpacing(32);

    NProgressBar* verticalProgressBar = new NProgressBar(Qt::Vertical, this);
    verticalProgressBar->setValue(75);
    verticalProgressBar->setMinimumHeight(150);
    verticalProgressBar->setProgressThickness(8);

    NProgressBar* verticalIndeterminateProgressBar = new NProgressBar(Qt::Vertical, this);
    verticalIndeterminateProgressBar->setIndeterminate(true);
    verticalIndeterminateProgressBar->setMinimumHeight(150);
    verticalIndeterminateProgressBar->setProgressThickness(8);

    QLabel* verticalLabel1 = new QLabel("确定状态", this);
    verticalLabel1->setAlignment(Qt::AlignCenter);
    QLabel* verticalLabel2 = new QLabel("不确定状态", this);
    verticalLabel2->setAlignment(Qt::AlignCenter);

    QVBoxLayout* vertical1Layout = new QVBoxLayout();
    vertical1Layout->addWidget(verticalProgressBar);
    vertical1Layout->addWidget(verticalLabel1);

    QVBoxLayout* vertical2Layout = new QVBoxLayout();
    vertical2Layout->addWidget(verticalIndeterminateProgressBar);
    vertical2Layout->addWidget(verticalLabel2);

    verticalLayout->addLayout(vertical1Layout);
    verticalLayout->addLayout(vertical2Layout);
    verticalLayout->addStretch();

    addSection("垂直进度条", verticalLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(32);

    NProgressBar* normalProgressBar = new NProgressBar(this);
    normalProgressBar->setValue(80);
    normalProgressBar->setMinimumWidth(200);
    QLabel* normalLabel = new QLabel("正常状态", this);
    normalLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* normalLayout = new QVBoxLayout();
    normalLayout->addWidget(normalProgressBar);
    normalLayout->addWidget(normalLabel);

    NProgressBar* pausedProgressBar = new NProgressBar(this);
    pausedProgressBar->setValue(60);
    pausedProgressBar->setPaused(true);
    pausedProgressBar->setMinimumWidth(200);
    QLabel* pausedLabel = new QLabel("暂停状态", this);
    pausedLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* pausedLayout = new QVBoxLayout();
    pausedLayout->addWidget(pausedProgressBar);
    pausedLayout->addWidget(pausedLabel);

    NProgressBar* errorProgressBar = new NProgressBar(this);
    errorProgressBar->setValue(30);
    errorProgressBar->setError(true);
    errorProgressBar->setMinimumWidth(200);
    QLabel* errorLabel = new QLabel("错误状态", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* errorLayout = new QVBoxLayout();
    errorLayout->addWidget(errorProgressBar);
    errorLayout->addWidget(errorLabel);

    NProgressBar* disabledProgressBar = new NProgressBar(this);
    disabledProgressBar->setValue(45);
    disabledProgressBar->setEnabled(false);
    disabledProgressBar->setMinimumWidth(200);
    QLabel* disabledLabel = new QLabel("禁用状态", this);
    disabledLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* disabledLayout = new QVBoxLayout();
    disabledLayout->addWidget(disabledProgressBar);
    disabledLayout->addWidget(disabledLabel);

    stateLayout->addLayout(normalLayout);
    stateLayout->addLayout(pausedLayout);
    stateLayout->addLayout(errorLayout);
    stateLayout->addLayout(disabledLayout);
    stateLayout->addStretch();

    addSection("不同状态", stateLayout);

    QHBoxLayout* dynamicLayout = new QHBoxLayout();
    dynamicLayout->setSpacing(24);

    NProgressBar* dynamicProgressBar = new NProgressBar(this);
    dynamicProgressBar->setValue(0);
    dynamicProgressBar->setMinimumWidth(300);
    dynamicProgressBar->setTextVisible(true);
    dynamicProgressBar->setFormat("%p%");

    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(8);

    NPushButton* startButton = new NPushButton("开始", this);
    startButton->setFixedSize(80, 32);
    connect(startButton, &NPushButton::clicked, [dynamicProgressBar]() {
        QTimer* timer = new QTimer(dynamicProgressBar);
        dynamicProgressBar->setProperty("timer", QVariant::fromValue(timer));
        connect(timer, &QTimer::timeout, [dynamicProgressBar]() {
            int value = dynamicProgressBar->value();
            if (value < 100) {
                dynamicProgressBar->setValue(value + 1);
            } else {
                QTimer* timer = dynamicProgressBar->property("timer").value<QTimer*>();
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
    connect(resetButton, &NPushButton::clicked, [dynamicProgressBar]() {
        QTimer* timer = dynamicProgressBar->property("timer").value<QTimer*>();
        if (timer) {
            timer->stop();
        }
        dynamicProgressBar->setValue(0);
    });
    controlLayout->addWidget(resetButton);

    NPushButton* pauseButton = new NPushButton("暂停/继续", this);
    pauseButton->setFixedSize(80, 32);
    connect(pauseButton, &NPushButton::clicked, [dynamicProgressBar]() {
        QTimer* timer = dynamicProgressBar->property("timer").value<QTimer*>();

        if (dynamicProgressBar->isPaused()) {
            dynamicProgressBar->resume();
            if (timer && dynamicProgressBar->value() < 100) {
                timer->start();
            }
        } else {
            dynamicProgressBar->pause();
            if (timer) {
                timer->stop();
            }
        }
    });
    controlLayout->addWidget(pauseButton);

    NPushButton* errorButton = new NPushButton("错误", this);
    errorButton->setFixedSize(80, 32);
    connect(errorButton, &NPushButton::clicked, [dynamicProgressBar]() {
        dynamicProgressBar->setError(!dynamicProgressBar->isError());
    });
    controlLayout->addWidget(errorButton);

    controlLayout->addStretch();

    dynamicLayout->addWidget(dynamicProgressBar);
    dynamicLayout->addLayout(controlLayout);
    dynamicLayout->addStretch();

    addSection("动态进度条", dynamicLayout);
}
