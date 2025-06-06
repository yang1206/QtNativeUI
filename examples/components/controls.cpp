#include "controls.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NSlider.h>
#include <QtNativeUI/NSpinBox.h>
#include <QtNativeUI/NToggleSwitch.h>

#include "QtNativeUI/NCalendarDatePicker.h"
#include "QtNativeUI/NCalendarWidget.h"
#include "QtNativeUI/NComboBox.h"
#include "QtNativeUI/NDoubleSpinBox.h"
#include "QtNativeUI/NLabel.h"
#include "QtNativeUI/NPlainTextEdit.h"
#include "QtNativeUI/NProgressBar.h"
#include "QtNativeUI/NProgressRing.h"
#include "QtNativeUI/NScrollArea.h"
#include "QtNativeUI/NTextEdit.h"
#include "QtNativeUI/NToolTip.h"
#include "widgets/ExampleSection.h"

ControlsExample::ControlsExample(QWidget* parent) : QWidget(parent) { initUI(); }

// 在 initUI() 函数中添加 ProgressBar 部分
void ControlsExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建滚动区域
    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建内容容器
    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // 添加各个控件区域
    contentLayout->addWidget(new ExampleSection("Label", createLabels()));
    contentLayout->addWidget(new ExampleSection("CheckBox", createCheckBoxes()));
    contentLayout->addWidget(new ExampleSection("ToggleSwitch", createToggleSwitches()));
    contentLayout->addWidget(new ExampleSection("LineEdit", createLineEdits()));
    contentLayout->addWidget(new ExampleSection("TextEdit", createTextEdits()));
    contentLayout->addWidget(new ExampleSection("PlainTextEdit", createPlainTextEdits()));
    contentLayout->addWidget(new ExampleSection("ComboBox", createComboBoxes()));
    contentLayout->addWidget(new ExampleSection("SpinBox", createSpinBoxes()));
    contentLayout->addWidget(new ExampleSection("DoubleSpinBox", createDoubleSpinBoxes()));
    contentLayout->addWidget(new ExampleSection("Slider", createSliders()));
    contentLayout->addWidget(new ExampleSection("ProgressBar", createProgressBars()));
    contentLayout->addWidget(new ExampleSection("ProgressRing", createProgressRings()));
    contentLayout->addWidget(new ExampleSection("ScrollArea", createScrollAreas()));
    contentLayout->addWidget(new ExampleSection("ToolTip", createToolTips()));
    contentLayout->addWidget(new ExampleSection("CalendarWidget", createCalendarWidgets()));
    contentLayout->addWidget(new ExampleSection("CalendarDatePicker", createCalendarDatePickers()));
    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
    setMinimumWidth(600);
}

QWidget* ControlsExample::createCheckBoxes() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本复选框
    auto basicCheckBox = new NCheckBox("基本复选框", container);
    layout->addWidget(basicCheckBox);

    // 默认选中的复选框
    auto checkedCheckBox = new NCheckBox("默认选中的复选框", container);
    checkedCheckBox->setChecked(true);
    layout->addWidget(checkedCheckBox);

    // 禁用状态的复选框
    auto disabledCheckBox = new NCheckBox("禁用状态的复选框", container);
    disabledCheckBox->setEnabled(false);
    layout->addWidget(disabledCheckBox);

    // 禁用且选中状态的复选框
    auto disabledCheckedCheckBox = new NCheckBox("禁用且选中状态的复选框", container);
    disabledCheckedCheckBox->setChecked(true);
    disabledCheckedCheckBox->setEnabled(false);
    layout->addWidget(disabledCheckedCheckBox);

    // 三态复选框
    auto triStateCheckBox = new NCheckBox("三态复选框", container);
    triStateCheckBox->setTristate(true);
    layout->addWidget(triStateCheckBox);

    return container;
}

QWidget* ControlsExample::createToggleSwitches() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本开关
    auto basicSwitch = new NToggleSwitch("基本开关", container);
    connect(basicSwitch, &NToggleSwitch::checkedChanged, this, [](bool checked) {
        qDebug() << "checkedChanged: " << checked;
    });
    layout->addWidget(basicSwitch);

    // 默认选中的开关
    auto checkedSwitch = new NToggleSwitch("默认选中的开关", container);
    checkedSwitch->setChecked(true);
    layout->addWidget(checkedSwitch);

    // 禁用状态的开关
    auto disabledSwitch = new NToggleSwitch("禁用状态的开关", container);
    disabledSwitch->setEnabled(false);
    layout->addWidget(disabledSwitch);

    // 禁用且选中状态的开关
    auto disabledCheckedSwitch = new NToggleSwitch("禁用且选中状态的开关", container);
    disabledCheckedSwitch->setChecked(true);
    disabledCheckedSwitch->setEnabled(false);
    layout->addWidget(disabledCheckedSwitch);

    // 无文本的开关
    auto noTextSwitch = new NToggleSwitch(container);
    layout->addWidget(noTextSwitch);

    return container;
}

QWidget* ControlsExample::createLineEdits() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本输入框
    NLineEdit* normalLineEdit = new NLineEdit(container);
    normalLineEdit->setPlaceholderText("基本输入框");
    normalLineEdit->setMinimumWidth(200);
    layout->addWidget(normalLineEdit);

    // 禁用状态
    NLineEdit* disabledLineEdit = new NLineEdit(container);
    disabledLineEdit->setPlaceholderText("禁用状态");
    disabledLineEdit->setEnabled(false);
    disabledLineEdit->setMinimumWidth(200);
    layout->addWidget(disabledLineEdit);

    // 只读状态
    NLineEdit* readOnlyLineEdit = new NLineEdit("只读状态", container);
    readOnlyLineEdit->setReadOnly(true);
    readOnlyLineEdit->setMinimumWidth(200);
    layout->addWidget(readOnlyLineEdit);

    // 带前缀的输入框
    NLineEdit* prefixLineEdit = new NLineEdit(container);
    prefixLineEdit->setPlaceholderText("带前缀图标的输入框");
    prefixLineEdit->addAction(NFilledIconType::Server16Filled, NLineEdit::ActionPosition::LeadingPosition);
    prefixLineEdit->setMinimumWidth(200);
    layout->addWidget(prefixLineEdit);

    // 带后缀的输入框
    NLineEdit* suffixLineEdit = new NLineEdit(container);
    suffixLineEdit->setPlaceholderText("带后缀图标的输入框");
    auto searchAction =
        suffixLineEdit->addAction(NFilledIconType::Search16Filled, NLineEdit::ActionPosition::TrailingPosition);
    connect(searchAction, &QAction::triggered, this, [=]() { qDebug() << "搜索"; });
    suffixLineEdit->setMinimumWidth(200);
    layout->addWidget(suffixLineEdit);

    // 带清除按钮的输入框
    NLineEdit* clearableLineEdit = new NLineEdit(container);
    clearableLineEdit->setPlaceholderText("带清除按钮的输入框");
    clearableLineEdit->setClearButtonEnabled(true);
    clearableLineEdit->setMinimumWidth(200);
    layout->addWidget(clearableLineEdit);

    return container;
}

QWidget* ControlsExample::createTextEdits() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本输入框
    NTextEdit* normalTextEdit = new NTextEdit(container);
    normalTextEdit->setPlaceholderText("基本输入框");
    normalTextEdit->setMinimumWidth(200);
    layout->addWidget(normalTextEdit);

    // 禁用状态
    NTextEdit* disabledTextEdit = new NTextEdit(container);
    disabledTextEdit->setPlaceholderText("禁用状态");
    disabledTextEdit->setEnabled(false);
    disabledTextEdit->setMinimumWidth(200);
    layout->addWidget(disabledTextEdit);

    return container;
}

QWidget* ControlsExample::createPlainTextEdits() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本输入框
    NPlainTextEdit* normalTextEdit = new NPlainTextEdit(container);
    normalTextEdit->setPlaceholderText("基本输入框");
    normalTextEdit->setMinimumWidth(200);
    layout->addWidget(normalTextEdit);

    // 禁用状态
    NPlainTextEdit* disabledTextEdit = new NPlainTextEdit(container);
    disabledTextEdit->setPlaceholderText("禁用状态");
    disabledTextEdit->setEnabled(false);
    disabledTextEdit->setMinimumWidth(200);
    layout->addWidget(disabledTextEdit);

    return container;
}

// 添加 createSpinBoxes 函数实现
QWidget* ControlsExample::createSpinBoxes() {
    QWidget*     container = new QWidget;
    QHBoxLayout* layout    = new QHBoxLayout(container);
    layout->setSpacing(16);

    // 基本数字输入框
    NSpinBox* normalSpinBox = new NSpinBox(container);
    layout->addWidget(normalSpinBox);

    // 设置范围的数字输入框
    NSpinBox* rangeSpinBox = new NSpinBox(container);
    rangeSpinBox->setRange(0, 100);
    rangeSpinBox->setValue(50);
    layout->addWidget(rangeSpinBox);

    // 设置步长的数字输入框
    NSpinBox* stepSpinBox = new NSpinBox(container);
    stepSpinBox->setRange(0, 100);
    stepSpinBox->setSingleStep(5);
    stepSpinBox->setValue(25);
    layout->addWidget(stepSpinBox);

    // 前缀后缀的数字输入框
    NSpinBox* prefixSuffixSpinBox = new NSpinBox(container);
    prefixSuffixSpinBox->setPrefix("$");
    prefixSuffixSpinBox->setSuffix(".00");
    prefixSuffixSpinBox->setRange(0, 1000);
    prefixSuffixSpinBox->setValue(100);
    layout->addWidget(prefixSuffixSpinBox);

    // 禁用状态的数字输入框
    NSpinBox* disabledSpinBox = new NSpinBox(container);
    disabledSpinBox->setValue(50);
    disabledSpinBox->setEnabled(false);
    layout->addWidget(disabledSpinBox);

    return container;
}

// 添加 createDoubleSpinBoxes 函数实现
QWidget* ControlsExample::createDoubleSpinBoxes() {
    QWidget*     container = new QWidget;
    QHBoxLayout* layout    = new QHBoxLayout(container);
    layout->setSpacing(16);

    // 基本数字输入框
    NDoubleSpinBox* normalDoubleSpinBox = new NDoubleSpinBox(container);
    layout->addWidget(normalDoubleSpinBox);

    // 设置范围的数字输入框
    NDoubleSpinBox* rangeDoubleSpinBox = new NDoubleSpinBox(container);
    rangeDoubleSpinBox->setRange(0, 100);
    rangeDoubleSpinBox->setValue(50);
    layout->addWidget(rangeDoubleSpinBox);

    // 设置步长的数字输入框
    NDoubleSpinBox* stepDoubleSpinBox = new NDoubleSpinBox(container);
    stepDoubleSpinBox->setRange(0, 100);
    stepDoubleSpinBox->setSingleStep(5);
    stepDoubleSpinBox->setValue(25);
    layout->addWidget(stepDoubleSpinBox);

    // 前缀后缀的数字输入框
    NDoubleSpinBox* prefixSuffixDoubleSpinBox = new NDoubleSpinBox(container);
    prefixSuffixDoubleSpinBox->setPrefix("$");
    prefixSuffixDoubleSpinBox->setSuffix(".00");
    prefixSuffixDoubleSpinBox->setRange(0, 1000);
    prefixSuffixDoubleSpinBox->setValue(100);
    layout->addWidget(prefixSuffixDoubleSpinBox);

    // 禁用状态的数字输入框
    NDoubleSpinBox* disabledDoubleSpinBox = new NDoubleSpinBox(container);
    disabledDoubleSpinBox->setValue(50);
    disabledDoubleSpinBox->setEnabled(false);
    layout->addWidget(disabledDoubleSpinBox);

    return container;
}

// 添加 createSliders 函数实现
QWidget* ControlsExample::createSliders() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(32);

    // 水平滑块
    NSlider* horizontalSlider = new NSlider(Qt::Horizontal, container);
    horizontalSlider->setRange(0, 100);
    horizontalSlider->setValue(50);
    horizontalSlider->setMinimumWidth(200);
    layout->addWidget(horizontalSlider);

    // 垂直滑块
    NSlider* verticalSlider = new NSlider(Qt::Vertical, container);
    verticalSlider->setRange(0, 100);
    verticalSlider->setValue(50);
    verticalSlider->setMinimumHeight(200);
    layout->addWidget(verticalSlider);

    // 禁用状态的滑块
    NSlider* disabledSlider = new NSlider(Qt::Horizontal, container);
    disabledSlider->setRange(0, 100);
    disabledSlider->setValue(30);
    disabledSlider->setEnabled(false);
    disabledSlider->setMinimumWidth(200);
    layout->addWidget(disabledSlider);

    // 带刻度的滑块
    NSlider* tickSlider = new NSlider(Qt::Horizontal, container);
    tickSlider->setRange(0, 100);
    tickSlider->setValue(40);
    tickSlider->setTickPosition(QSlider::TicksBelow);
    tickSlider->setTickInterval(10);
    tickSlider->setMinimumWidth(200);
    layout->addWidget(tickSlider);

    return container;
}

QWidget* ControlsExample::createToolTips() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(32);

    // 基本提示按钮
    QHBoxLayout* basicRow    = new QHBoxLayout();
    NPushButton* basicButton = new NPushButton("悬停显示默认提示", container);
    basicButton->setFixedSize(140, 40);
    basicButton->setToolTip("这是一个基本工具提示");
    NToolTipFilter* basicFilter = new NToolTipFilter(basicButton);
    basicRow->addWidget(basicButton);
    basicRow->addStretch();
    layout->addLayout(basicRow);

    // 不同位置的提示
    QLabel* positionLabel = new QLabel("不同位置的工具提示：", container);
    QFont   labelFont     = positionLabel->font();
    labelFont.setBold(true);
    positionLabel->setFont(labelFont);
    layout->addWidget(positionLabel);

    // 创建不同位置的工具提示按钮
    QGridLayout* positionGrid = new QGridLayout();
    positionGrid->setSpacing(16);

    struct PositionInfo {
        QString          name;
        NToolTipPosition position;
    };

    QList<PositionInfo> positions = {{"上方", NToolTipPosition::TOP},
                                     {"下方", NToolTipPosition::BOTTOM},
                                     {"左侧", NToolTipPosition::LEFT},
                                     {"右侧", NToolTipPosition::RIGHT},
                                     {"左上", NToolTipPosition::TOP_LEFT},
                                     {"右上", NToolTipPosition::TOP_RIGHT},
                                     {"左下", NToolTipPosition::BOTTOM_LEFT},
                                     {"右下", NToolTipPosition::BOTTOM_RIGHT}};

    int row    = 0;
    int col    = 0;
    int maxCol = 4;

    for (const auto& pos : positions) {
        NPushButton* button = new NPushButton(pos.name + "提示", container);
        button->setToolTip("这是一个" + pos.name + "位置的工具提示");
        button->setMinimumWidth(120);
        NToolTipFilter* filter = new NToolTipFilter(button, 300, pos.position);

        positionGrid->addWidget(button, row, col);

        col++;
        if (col >= maxCol) {
            col = 0;
            row++;
        }
    }

    layout->addLayout(positionGrid);

    // 自定义延迟和持续时间
    QLabel* customLabel = new QLabel("自定义延迟和持续时间：", container);
    customLabel->setFont(labelFont);
    layout->addWidget(customLabel);

    QHBoxLayout* customRow = new QHBoxLayout();

    // 快速显示的提示
    NPushButton* quickButton = new NPushButton("快速显示(100ms延迟)", container);
    quickButton->setToolTip("这个提示会快速显示");
    NToolTipFilter* quickFilter = new NToolTipFilter(quickButton, 100);
    customRow->addWidget(quickButton);

    // 长时间显示的提示
    NPushButton* longButton = new NPushButton("长时间显示(5秒)", container);
    longButton->setToolTip("这个提示会显示5秒钟");
    NToolTipFilter* longFilter = new NToolTipFilter(longButton);
    longButton->setProperty("toolTipDuration", 5000);
    customRow->addWidget(longButton);

    // 不自动隐藏的提示
    NPushButton* persistentButton = new NPushButton("不自动隐藏", container);
    persistentButton->setToolTip("这个提示不会自动隐藏，直到鼠标移开");
    NToolTipFilter* persistentFilter = new NToolTipFilter(persistentButton);
    persistentButton->setProperty("toolTipDuration", -1);
    customRow->addWidget(persistentButton);

    layout->addLayout(customRow);

    // 静态方法演示
    QLabel* staticLabel = new QLabel("静态方法演示：", container);
    staticLabel->setFont(labelFont);
    layout->addWidget(staticLabel);

    QHBoxLayout* staticRow = new QHBoxLayout();

    NPushButton* showTextButton = new NPushButton("点击显示临时提示", container);
    showTextButton->setFixedSize(120, 40);
    connect(showTextButton, &NPushButton::clicked, [=]() {
        NToolTip::showText("这是一个临时工具提示，3秒后消失", showTextButton, NToolTipPosition::TOP);
    });
    staticRow->addWidget(showTextButton);

    // 自定义创建并显示
    NPushButton* customCreateButton = new NPushButton("点击创建自定义提示", container);
    customCreateButton->setFixedSize(140, 40);
    connect(customCreateButton, &NPushButton::clicked, [=]() {
        NToolTip* tooltip = NToolTip::createToolTip("这是一个自定义创建的工具提示", customCreateButton);
        tooltip->setDuration(3000);
        tooltip->adjustPosition(customCreateButton, NToolTipPosition::BOTTOM);
        tooltip->show();
    });
    staticRow->addWidget(customCreateButton);

    staticRow->addStretch();
    layout->addLayout(staticRow);

    layout->addStretch();
    return container;
}

// 添加 createProgressBars 函数实现
QWidget* ControlsExample::createProgressBars() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本进度条
    QLabel* basicLabel = new QLabel("基本进度条:", container);
    layout->addWidget(basicLabel);

    NProgressBar* basicProgressBar = new NProgressBar(container);
    basicProgressBar->setValue(50);
    basicProgressBar->setMinimumWidth(300);
    layout->addWidget(basicProgressBar);

    // 带文本的进度条
    QLabel* textLabel = new QLabel("带文本的进度条:", container);
    layout->addWidget(textLabel);

    NProgressBar* textProgressBar = new NProgressBar(container);
    textProgressBar->setProgressThickness(20);
    textProgressBar->setBorderRadius(10);
    textProgressBar->setValue(75);
    textProgressBar->setFormat("已完成 %p%");
    textProgressBar->setTextVisible(true);
    textProgressBar->setMinimumWidth(300);
    layout->addWidget(textProgressBar);

    // 不确定状态的进度条
    QLabel* indeterminateLabel = new QLabel("不确定状态进度条:", container);
    layout->addWidget(indeterminateLabel);

    NProgressBar* indeterminateProgressBar = new NProgressBar(container);
    indeterminateProgressBar->setIndeterminate(true);
    indeterminateProgressBar->setMinimumWidth(300);
    layout->addWidget(indeterminateProgressBar);

    // 垂直进度条
    QLabel* verticalLabel = new QLabel("垂直进度条:", container);
    layout->addWidget(verticalLabel);

    QHBoxLayout* verticalLayout = new QHBoxLayout();
    verticalLayout->setSpacing(16);

    NProgressBar* verticalProgressBar = new NProgressBar(Qt::Vertical, container);
    verticalProgressBar->setValue(75);
    verticalProgressBar->setMinimumHeight(150);
    verticalProgressBar->setProgressThickness(8);
    verticalLayout->addWidget(verticalProgressBar);

    // 垂直不确定状态进度条
    NProgressBar* verticalIndeterminateProgressBar = new NProgressBar(Qt::Vertical, container);
    verticalIndeterminateProgressBar->setIndeterminate(true);
    verticalIndeterminateProgressBar->setMinimumHeight(150);
    verticalIndeterminateProgressBar->setProgressThickness(8);
    verticalLayout->addWidget(verticalIndeterminateProgressBar);

    verticalLayout->addStretch();
    layout->addLayout(verticalLayout);

    // 状态示例
    QLabel* stateLabel = new QLabel("状态示例:", container);
    layout->addWidget(stateLabel);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);

    // 暂停状态
    NProgressBar* pausedProgressBar = new NProgressBar(container);
    pausedProgressBar->setValue(60);
    pausedProgressBar->setMinimumWidth(200);
    pausedProgressBar->setPaused(true);
    QLabel* pausedLabel = new QLabel("暂停状态", container);
    pausedLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* pausedLayout = new QVBoxLayout();
    pausedLayout->addWidget(pausedProgressBar);
    pausedLayout->addWidget(pausedLabel);
    stateLayout->addLayout(pausedLayout);

    // 错误状态
    NProgressBar* errorProgressBar = new NProgressBar(container);
    errorProgressBar->setValue(30);
    errorProgressBar->setMinimumWidth(200);
    errorProgressBar->setError(true);
    QLabel* errorLabel = new QLabel("错误状态", container);
    errorLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* errorLayout = new QVBoxLayout();
    errorLayout->addWidget(errorProgressBar);
    errorLayout->addWidget(errorLabel);
    stateLayout->addLayout(errorLayout);

    // 禁用状态
    NProgressBar* disabledProgressBar = new NProgressBar(container);
    disabledProgressBar->setValue(45);
    disabledProgressBar->setMinimumWidth(200);
    disabledProgressBar->setEnabled(false);
    QLabel* disabledLabel = new QLabel("禁用状态", container);
    disabledLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* disabledLayout = new QVBoxLayout();
    disabledLayout->addWidget(disabledProgressBar);
    disabledLayout->addWidget(disabledLabel);
    stateLayout->addLayout(disabledLayout);

    layout->addLayout(stateLayout);

    // 添加一个动态更新的进度条
    QLabel* dynamicLabel = new QLabel("动态进度条:", container);
    layout->addWidget(dynamicLabel);

    NProgressBar* dynamicProgressBar = new NProgressBar(container);
    dynamicProgressBar->setValue(0);
    dynamicProgressBar->setMinimumWidth(300);
    layout->addWidget(dynamicProgressBar);

    // 添加控制按钮
    QHBoxLayout* controlLayout = new QHBoxLayout();

    NPushButton* startButton = new NPushButton("开始", container);
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

    NPushButton* resetButton = new NPushButton("重置", container);
    connect(resetButton, &NPushButton::clicked, [dynamicProgressBar]() {
        QTimer* timer = dynamicProgressBar->property("timer").value<QTimer*>();
        if (timer) {
            timer->stop();
        }
        dynamicProgressBar->setValue(0);
    });
    controlLayout->addWidget(resetButton);

    NPushButton* pauseButton = new NPushButton("暂停/继续", container);
    connect(pauseButton, &NPushButton::clicked, [dynamicProgressBar]() {
        QTimer* timer = dynamicProgressBar->property("timer").value<QTimer*>();

        if (dynamicProgressBar->isPaused()) {
            dynamicProgressBar->resume();
            // 恢复定时器
            if (timer && dynamicProgressBar->value() < 100) {
                timer->start();
            }
        } else {
            dynamicProgressBar->pause();
            // 暂停定时器
            if (timer) {
                timer->stop();
            }
        }
    });
    controlLayout->addWidget(pauseButton);

    NPushButton* errorButton = new NPushButton("错误", container);
    connect(errorButton, &NPushButton::clicked, [dynamicProgressBar]() {
        dynamicProgressBar->setError(!dynamicProgressBar->isError());
    });
    controlLayout->addWidget(errorButton);

    controlLayout->addStretch();
    layout->addLayout(controlLayout);

    return container;
}

// 添加 createProgressRings 函数实现
QWidget* ControlsExample::createProgressRings() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本进度环
    QLabel* basicLabel = new QLabel("基本进度环:", container);
    layout->addWidget(basicLabel);

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(32);

    NProgressRing* basicProgressRing = new NProgressRing(container);
    basicProgressRing->setValue(50);
    basicProgressRing->setFixedSize(100, 100);
    basicLayout->addWidget(basicProgressRing);

    // 自定义宽度的进度环
    NProgressRing* thickProgressRing = new NProgressRing(container);
    thickProgressRing->setValue(75);
    thickProgressRing->setStrokeWidth(10);
    thickProgressRing->setFixedSize(100, 100);
    basicLayout->addWidget(thickProgressRing);

    // 带文本的进度环
    NProgressRing* textProgressRing = new NProgressRing(container);
    textProgressRing->setValue(65);
    textProgressRing->setFormat("%p%");
    textProgressRing->setTextVisible(true);
    textProgressRing->setFixedSize(100, 100);
    basicLayout->addWidget(textProgressRing);

    basicLayout->addStretch();
    layout->addLayout(basicLayout);

    // 不确定状态的进度环
    QLabel* indeterminateLabel = new QLabel("不确定状态进度环:", container);
    layout->addWidget(indeterminateLabel);

    QHBoxLayout* indeterminateLayout = new QHBoxLayout();
    indeterminateLayout->setSpacing(32);

    NProgressRing* indeterminateProgressRing = new NProgressRing(container);
    indeterminateProgressRing->setMinimum(0);
    indeterminateProgressRing->setMaximum(0); // 设置为不确定状态
    indeterminateProgressRing->setFixedSize(100, 100);
    indeterminateLayout->addWidget(indeterminateProgressRing);

    // 不确定状态带文本的进度环
    NProgressRing* textIndeterminateProgressRing = new NProgressRing(container);
    textIndeterminateProgressRing->setMinimum(0);
    textIndeterminateProgressRing->setMaximum(0);
    textIndeterminateProgressRing->setFormat("加载中...");
    textIndeterminateProgressRing->setTextVisible(true);
    textIndeterminateProgressRing->setFixedSize(100, 100);
    indeterminateLayout->addWidget(textIndeterminateProgressRing);

    // 不确定状态的专用进度环组件
    NProgressRing* specialIndeterminateRing = new NProgressRing(container);
    specialIndeterminateRing->setIndeterminate(true);
    indeterminateLayout->addWidget(specialIndeterminateRing);

    indeterminateLayout->addStretch();
    layout->addLayout(indeterminateLayout);

    // 状态示例
    QLabel* stateLabel = new QLabel("状态示例:", container);
    layout->addWidget(stateLabel);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(32);

    // 正常状态
    NProgressRing* normalProgressRing = new NProgressRing(container);
    normalProgressRing->setValue(80);
    normalProgressRing->setFixedSize(100, 100);
    QLabel* normalLabel = new QLabel("正常状态", container);
    normalLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* normalLayout = new QVBoxLayout();
    normalLayout->addWidget(normalProgressRing);
    normalLayout->addWidget(normalLabel);
    stateLayout->addLayout(normalLayout);

    // 暂停状态
    NProgressRing* pausedProgressRing = new NProgressRing(container);
    pausedProgressRing->setValue(60);
    pausedProgressRing->setPaused(true);
    pausedProgressRing->setFixedSize(100, 100);
    QLabel* pausedLabel = new QLabel("暂停状态", container);
    pausedLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* pausedLayout = new QVBoxLayout();
    pausedLayout->addWidget(pausedProgressRing);
    pausedLayout->addWidget(pausedLabel);
    stateLayout->addLayout(pausedLayout);

    // 错误状态
    NProgressRing* errorProgressRing = new NProgressRing(container);
    errorProgressRing->setValue(30);
    errorProgressRing->setError(true);
    errorProgressRing->setFixedSize(100, 100);
    QLabel* errorLabel = new QLabel("错误状态", container);
    errorLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* errorLayout = new QVBoxLayout();
    errorLayout->addWidget(errorProgressRing);
    errorLayout->addWidget(errorLabel);
    stateLayout->addLayout(errorLayout);

    // 禁用状态
    NProgressRing* disabledProgressRing = new NProgressRing(container);
    disabledProgressRing->setValue(45);
    disabledProgressRing->setEnabled(false);
    disabledProgressRing->setFixedSize(100, 100);
    QLabel* disabledLabel = new QLabel("禁用状态", container);
    disabledLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* disabledLayout = new QVBoxLayout();
    disabledLayout->addWidget(disabledProgressRing);
    disabledLayout->addWidget(disabledLabel);
    stateLayout->addLayout(disabledLayout);

    stateLayout->addStretch();
    layout->addLayout(stateLayout);

    // 添加一个动态更新的进度环
    QLabel* dynamicLabel = new QLabel("动态进度环:", container);
    layout->addWidget(dynamicLabel);

    QHBoxLayout* dynamicRingLayout = new QHBoxLayout();

    NProgressRing* dynamicProgressRing = new NProgressRing(container);
    dynamicProgressRing->setValue(0);
    dynamicProgressRing->setFixedSize(120, 120);
    dynamicProgressRing->setTextVisible(true);
    dynamicProgressRing->setFormat("%p%");

    dynamicRingLayout->addWidget(dynamicProgressRing);

    // 控制按钮容器
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(8);

    NPushButton* startButton = new NPushButton("开始", container);
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

    NPushButton* resetButton = new NPushButton("重置", container);
    connect(resetButton, &NPushButton::clicked, [dynamicProgressRing]() {
        QTimer* timer = dynamicProgressRing->property("timer").value<QTimer*>();
        if (timer) {
            timer->stop();
        }
        dynamicProgressRing->setValue(0);
    });
    controlLayout->addWidget(resetButton);

    NPushButton* pauseButton = new NPushButton("暂停/继续", container);
    connect(pauseButton, &NPushButton::clicked, [dynamicProgressRing]() {
        QTimer* timer = dynamicProgressRing->property("timer").value<QTimer*>();

        if (dynamicProgressRing->isPaused()) {
            dynamicProgressRing->resume();
            // 恢复定时器
            if (timer && dynamicProgressRing->value() < 100) {
                timer->start();
            }
        } else {
            dynamicProgressRing->pause();
            // 暂停定时器
            if (timer) {
                timer->stop();
            }
        }
    });
    controlLayout->addWidget(pauseButton);

    NPushButton* errorButton = new NPushButton("错误", container);
    connect(errorButton, &NPushButton::clicked, [dynamicProgressRing]() {
        dynamicProgressRing->setError(!dynamicProgressRing->isError());
    });
    controlLayout->addWidget(errorButton);

    dynamicRingLayout->addLayout(controlLayout);
    dynamicRingLayout->addStretch();

    layout->addLayout(dynamicRingLayout);

    layout->addStretch();
    return container;
}

// 添加 createScrollAreas 函数实现
QWidget* ControlsExample::createScrollAreas() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本滚动区域
    QLabel* basicLabel = new QLabel("基本滚动区域：", container);
    QFont   labelFont  = basicLabel->font();
    labelFont.setBold(true);
    basicLabel->setFont(labelFont);
    layout->addWidget(basicLabel);

    NScrollArea* basicScrollArea = new NScrollArea(container);
    basicScrollArea->setMinimumHeight(200);
    basicScrollArea->setMinimumWidth(400);

    // 创建内容
    QWidget*     contentWidget = new QWidget(basicScrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    // 添加一些内容使其可滚动
    for (int i = 1; i <= 20; ++i) {
        NPushButton* button = new NPushButton(QString("按钮 %1").arg(i), contentWidget);
        button->setMinimumWidth(380);
        contentLayout->addWidget(button);
    }

    contentLayout->addStretch();
    basicScrollArea->setWidget(contentWidget);
    layout->addWidget(basicScrollArea);

    // 水平滚动区域
    QLabel* horizontalLabel = new QLabel("水平滚动区域：", container);
    horizontalLabel->setFont(labelFont);
    layout->addWidget(horizontalLabel);

    NScrollArea* horizontalScrollArea = new NScrollArea(container);
    horizontalScrollArea->setMinimumHeight(100);
    horizontalScrollArea->setMinimumWidth(400);
    horizontalScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    horizontalScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 创建水平内容
    QWidget*     horizontalContentWidget = new QWidget(horizontalScrollArea);
    QHBoxLayout* horizontalContentLayout = new QHBoxLayout(horizontalContentWidget);

    // 添加一些内容使其可水平滚动
    for (int i = 1; i <= 10; ++i) {
        NPushButton* button = new NPushButton(QString("按钮 %1").arg(i), horizontalContentWidget);
        button->setMinimumWidth(100);
        horizontalContentLayout->addWidget(button);
    }

    horizontalContentLayout->addStretch();
    horizontalScrollArea->setWidget(horizontalContentWidget);
    layout->addWidget(horizontalScrollArea);

    // 禁用状态的滚动区域
    QLabel* disabledLabel = new QLabel("禁用状态的滚动区域：", container);
    disabledLabel->setFont(labelFont);
    layout->addWidget(disabledLabel);

    NScrollArea* disabledScrollArea = new NScrollArea(container);
    disabledScrollArea->setMinimumHeight(150);
    disabledScrollArea->setMinimumWidth(400);
    disabledScrollArea->setEnabled(false);

    // 创建内容
    QWidget*     disabledContentWidget = new QWidget(disabledScrollArea);
    QVBoxLayout* disabledContentLayout = new QVBoxLayout(disabledContentWidget);

    // 添加一些内容
    for (int i = 1; i <= 10; ++i) {
        NPushButton* button = new NPushButton(QString("按钮 %1").arg(i), disabledContentWidget);
        button->setMinimumWidth(380);
        disabledContentLayout->addWidget(button);
    }

    disabledContentLayout->addStretch();
    disabledScrollArea->setWidget(disabledContentWidget);
    layout->addWidget(disabledScrollArea);

    layout->addStretch();
    return container;
}

QWidget* ControlsExample::createCalendarWidgets() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    QLabel* titleLabel = new QLabel("日历控件演示", container);
    QFont   titleFont  = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    QVBoxLayout* calendarLayout = new QVBoxLayout();
    calendarLayout->setSpacing(20);

    // 1. 单日期选择
    QVBoxLayout* singleDateLayout = new QVBoxLayout();
    QLabel*      singleLabel      = new QLabel("单日期选择模式", container);
    QFont        labelFont        = singleLabel->font();
    labelFont.setBold(true);
    singleLabel->setFont(labelFont);
    singleDateLayout->addWidget(singleLabel);

    NCalendarWidget* singleCalendar = new NCalendarWidget(container);
    singleCalendar->setDateSelectionMode(NCalendarWidget::SingleDate);

    QLabel* selectedDateLabel = new QLabel("选择的日期：", container);
    singleDateLayout->addWidget(singleCalendar);
    singleDateLayout->addWidget(selectedDateLabel);

    // 连接信号槽，显示选择的日期
    connect(singleCalendar, &NCalendarWidget::clicked, [=](QDate date) {
        selectedDateLabel->setText("选择的日期：" + date.toString("yyyy-MM-dd"));
    });

    calendarLayout->addLayout(singleDateLayout);

    // 2. 多日期选择
    QVBoxLayout* multipleDateLayout = new QVBoxLayout();
    QLabel*      multipleLabel      = new QLabel("多日期选择模式", container);
    multipleLabel->setFont(labelFont);
    multipleDateLayout->addWidget(multipleLabel);

    NCalendarWidget* multipleCalendar = new NCalendarWidget(container);
    multipleCalendar->setDateSelectionMode(NCalendarWidget::MultipleDate);

    QLabel* multipleDatesLabel = new QLabel("选择的日期：", container);
    multipleDateLayout->addWidget(multipleCalendar);
    multipleDateLayout->addWidget(multipleDatesLabel);

    // 连接信号槽，显示选择的多个日期
    connect(multipleCalendar, &NCalendarWidget::selectedDatesChanged, [=](const QList<QDate>& dates) {
        QString dateStr = "选择的日期：";
        for (const QDate& date : dates) {
            dateStr += date.toString("yyyy-MM-dd") + "\n";
        }
        multipleDatesLabel->setText(dateStr);
    });

    calendarLayout->addLayout(multipleDateLayout);

    // 3. 日期范围选择
    QVBoxLayout* rangeDateLayout = new QVBoxLayout();
    QLabel*      rangeLabel      = new QLabel("日期范围选择模式", container);
    rangeLabel->setFont(labelFont);
    rangeDateLayout->addWidget(rangeLabel);

    NCalendarWidget* rangeCalendar = new NCalendarWidget(container);
    rangeCalendar->setDateSelectionMode(NCalendarWidget::DateRange);

    QLabel* rangeDatesLabel = new QLabel("选择的日期范围：", container);
    rangeDateLayout->addWidget(rangeCalendar);
    rangeDateLayout->addWidget(rangeDatesLabel);

    // 连接信号槽，显示选择的日期范围
    connect(rangeCalendar, &NCalendarWidget::selectedDateRangeChanged, [=](const QPair<QDate, QDate>& range) {
        if (range.first.isValid() && range.second.isValid()) {
            rangeDatesLabel->setText("选择的日期范围：" + range.first.toString("yyyy-MM-dd") + " 至 " +
                                     range.second.toString("yyyy-MM-dd"));
        } else if (range.first.isValid()) {
            rangeDatesLabel->setText("选择的日期：" + range.first.toString("yyyy-MM-dd"));
        }
    });

    calendarLayout->addLayout(rangeDateLayout);

    layout->addLayout(calendarLayout);

    // 添加操作控制区域
    QGroupBox*   controlsGroup  = new QGroupBox("日历控制", container);
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);

    // 设置最小和最大日期
    QHBoxLayout* dateRangeLayout = new QHBoxLayout();
    QLabel*      minDateLabel    = new QLabel("最小日期:", container);
    QLabel*      maxDateLabel    = new QLabel("最大日期:", container);

    NLineEdit* minDateEdit = new NLineEdit(container);
    minDateEdit->setPlaceholderText("YYYY-MM-DD");

    NLineEdit* maxDateEdit = new NLineEdit(container);
    maxDateEdit->setPlaceholderText("YYYY-MM-DD");

    NPushButton* setRangeButton = new NPushButton("设置日期范围", container);

    dateRangeLayout->addWidget(minDateLabel);
    dateRangeLayout->addWidget(minDateEdit);
    dateRangeLayout->addWidget(maxDateLabel);
    dateRangeLayout->addWidget(maxDateEdit);
    dateRangeLayout->addWidget(setRangeButton);

    connect(setRangeButton, &NPushButton::clicked, [=]() {
        QDate minDate = QDate::fromString(minDateEdit->text(), "yyyy-MM-dd");
        QDate maxDate = QDate::fromString(maxDateEdit->text(), "yyyy-MM-dd");

        if (minDate.isValid() && maxDate.isValid()) {
            singleCalendar->setMinimumDate(minDate);
            singleCalendar->setMaximumDate(maxDate);

            multipleCalendar->setMinimumDate(minDate);
            multipleCalendar->setMaximumDate(maxDate);

            rangeCalendar->setMinimumDate(minDate);
            rangeCalendar->setMaximumDate(maxDate);
        }
    });

    controlsLayout->addLayout(dateRangeLayout);

    // 设置区域语言
    QHBoxLayout* localeLayout = new QHBoxLayout();
    QLabel*      localeLabel  = new QLabel("设置语言:", container);

    NComboBox* localeCombo = new NComboBox(container);
    localeCombo->addItem("中文", QLocale::Chinese);
    localeCombo->addItem("英文", QLocale::English);
    localeCombo->addItem("法文", QLocale::French);
    localeCombo->addItem("德文", QLocale::German);
    localeCombo->addItem("日文", QLocale::Japanese);

    NPushButton* setLocaleButton = new NPushButton("应用", container);

    localeLayout->addWidget(localeLabel);
    localeLayout->addWidget(localeCombo);
    localeLayout->addWidget(setLocaleButton);
    localeLayout->addStretch();

    connect(setLocaleButton, &NPushButton::clicked, [=]() {
        QLocale::Language lang = static_cast<QLocale::Language>(localeCombo->currentData().toInt());
        QLocale           locale(lang);

        singleCalendar->setLocale(locale);
        multipleCalendar->setLocale(locale);
        rangeCalendar->setLocale(locale);
    });

    controlsLayout->addLayout(localeLayout);

    layout->addWidget(controlsGroup);
    layout->addStretch();

    return container;
}

QWidget* ControlsExample::createComboBoxes() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 1. 基本下拉框
    QLabel* basicLabel = new QLabel("基本下拉框:", container);
    layout->addWidget(basicLabel);

    NComboBox* basicComboBox = new NComboBox(container);
    basicComboBox->addItem("选项 1");
    basicComboBox->addItem("选项 2");
    basicComboBox->addItem("选项 3");
    basicComboBox->setMinimumWidth(200);
    layout->addWidget(basicComboBox);

    // 2. 预设选项的下拉框
    QLabel* presetLabel = new QLabel("预设选项的下拉框:", container);
    layout->addWidget(presetLabel);

    QStringList cityList          = {"北京", "上海", "广州", "深圳", "杭州"};
    NComboBox*  preselectComboBox = new NComboBox(cityList, container);
    preselectComboBox->setCurrentIndex(2); // 选择"广州"
    preselectComboBox->setMinimumWidth(200);
    layout->addWidget(preselectComboBox);

    // 3. 可编辑的下拉框
    QLabel* editableLabel = new QLabel("可编辑的下拉框:", container);
    layout->addWidget(editableLabel);

    NComboBox* editableComboBox = new NComboBox(container);
    editableComboBox->addItems({"选项 A", "选项 B", "选项 C"});
    editableComboBox->setEditable(true);
    editableComboBox->setMinimumWidth(200);
    layout->addWidget(editableComboBox);

    // 4. 禁用状态的下拉框
    QLabel* disabledLabel = new QLabel("禁用状态的下拉框:", container);
    layout->addWidget(disabledLabel);

    NComboBox* disabledComboBox = new NComboBox(container);
    disabledComboBox->addItems({"禁用项 1", "禁用项 2", "禁用项 3"});
    disabledComboBox->setEnabled(false);
    disabledComboBox->setMinimumWidth(200);
    layout->addWidget(disabledComboBox);

    // 5. 信号演示
    QLabel* signalLabel = new QLabel("信号演示:", container);
    layout->addWidget(signalLabel);

    NComboBox* signalComboBox = new NComboBox(container);
    signalComboBox->addItems({"红色", "绿色", "蓝色", "黄色", "紫色"});
    signalComboBox->setMinimumWidth(200);
    QLabel* selectionLabel = new QLabel("当前选择: 红色", container);
    connect(signalComboBox,
            QOverload<int>::of(&NComboBox::currentIndexChanged),
            [selectionLabel, signalComboBox](int index) {
                selectionLabel->setText("当前选择: " + signalComboBox->currentText());
            });

    layout->addWidget(signalComboBox);
    layout->addWidget(selectionLabel);

    return container;
}

QWidget* ControlsExample::createCalendarDatePickers() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    QLabel* titleLabel = new QLabel("日期选择器控件演示", container);
    QFont   titleFont  = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    // 1. 基本日期选择器
    QLabel* basicLabel = new QLabel("基本日期选择器:", container);
    QFont   labelFont  = basicLabel->font();
    labelFont.setBold(true);
    basicLabel->setFont(labelFont);
    layout->addWidget(basicLabel);

    NCalendarDatePicker* basicPicker = new NCalendarDatePicker(container);
    basicPicker->setMinimumWidth(250);
    QLabel* basicSelectedLabel = new QLabel("选择的日期: 无", container);
    connect(basicPicker, &NCalendarDatePicker::dateSelected, [=](QDate date) {
        basicSelectedLabel->setText("选择的日期: " + date.toString("yyyy-MM-dd"));
    });
    layout->addWidget(basicPicker);
    layout->addWidget(basicSelectedLabel);
    layout->addSpacing(8);

    // 2. 预设日期的选择器
    QLabel* presetLabel = new QLabel("预设日期的选择器:", container);
    presetLabel->setFont(labelFont);
    layout->addWidget(presetLabel);

    NCalendarDatePicker* presetPicker = new NCalendarDatePicker(container);
    presetPicker->setSelectedDate(QDate::currentDate());
    presetPicker->setMinimumWidth(250);
    layout->addWidget(presetPicker);
    layout->addSpacing(8);

    // 3. 自定义日期格式
    QLabel* formatLabel = new QLabel("自定义日期格式:", container);
    formatLabel->setFont(labelFont);
    layout->addWidget(formatLabel);

    QHBoxLayout*         formatLayout  = new QHBoxLayout();
    NCalendarDatePicker* formatPicker1 = new NCalendarDatePicker(container);
    formatPicker1->setSelectedDate(QDate::currentDate());
    formatPicker1->setDateFormat("yyyy年MM月dd日");
    formatPicker1->setMinimumWidth(200);
    NCalendarDatePicker* formatPicker2 = new NCalendarDatePicker(container);
    formatPicker2->setSelectedDate(QDate::currentDate());
    formatPicker2->setDateFormat("MM/dd/yyyy");
    formatPicker2->setMinimumWidth(200);
    formatLayout->addWidget(formatPicker1);
    formatLayout->addWidget(formatPicker2);
    formatLayout->addStretch();
    layout->addLayout(formatLayout);
    layout->addSpacing(8);

    // 4. 不同选择模式
    QLabel* modeLabel = new QLabel("不同选择模式:", container);
    modeLabel->setFont(labelFont);
    layout->addWidget(modeLabel);

    QHBoxLayout* modeLayout = new QHBoxLayout();
    // 单日期选择
    QVBoxLayout* singleLayout    = new QVBoxLayout();
    QLabel*      singleModeLabel = new QLabel("单日期选择", container);
    singleModeLabel->setAlignment(Qt::AlignCenter);
    NCalendarDatePicker* singlePicker = new NCalendarDatePicker(container);
    singlePicker->setDateSelectionMode(NCalendarWidget::SingleDate);
    singlePicker->setMinimumWidth(200);
    QLabel* singleSelectedLabel = new QLabel("选择的日期: 无", container);
    connect(singlePicker, &NCalendarDatePicker::dateSelected, [=](QDate date) {
        singleSelectedLabel->setText("选择的日期: " + date.toString("yyyy-MM-dd"));
    });
    singleLayout->addWidget(singleModeLabel);
    singleLayout->addWidget(singlePicker);
    singleLayout->addWidget(singleSelectedLabel);
    // 多日期选择
    QVBoxLayout* multiLayout    = new QVBoxLayout();
    QLabel*      multiModeLabel = new QLabel("多日期选择", container);
    multiModeLabel->setAlignment(Qt::AlignCenter);
    NCalendarDatePicker* multiPicker = new NCalendarDatePicker(container);
    multiPicker->setDateSelectionMode(NCalendarWidget::MultipleDate);
    multiPicker->setMinimumWidth(200);
    QLabel* multiSelectedLabel = new QLabel("选择的日期: 无", container);
    connect(multiPicker, &NCalendarDatePicker::selectedDatesChanged, [=](const QList<QDate>& dates) {
        QString dateString = "选择的日期:\n";
        for (const QDate& date : dates) {
            dateString += date.toString("yyyy-MM-dd") + "\n";
        }
        multiSelectedLabel->setText(dateString);
    });
    multiLayout->addWidget(multiModeLabel);
    multiLayout->addWidget(multiPicker);
    multiLayout->addWidget(multiSelectedLabel);
    // 日期范围选择
    QVBoxLayout* rangeLayout    = new QVBoxLayout();
    QLabel*      rangeModeLabel = new QLabel("日期范围选择", container);
    rangeModeLabel->setAlignment(Qt::AlignCenter);
    NCalendarDatePicker* rangePicker = new NCalendarDatePicker(container);
    rangePicker->setDateSelectionMode(NCalendarWidget::DateRange);
    rangePicker->setMinimumWidth(200);
    QLabel* rangeSelectedLabel = new QLabel("选择的日期范围: 无", container);
    connect(rangePicker, &NCalendarDatePicker::selectedDateRangeChanged, [=](const QPair<QDate, QDate>& range) {
        if (range.first.isValid() && range.second.isValid()) {
            rangeSelectedLabel->setText("选择的日期范围:\n" + range.first.toString("yyyy-MM-dd") + " 至\n" +
                                        range.second.toString("yyyy-MM-dd"));
        }
    });
    rangeLayout->addWidget(rangeModeLabel);
    rangeLayout->addWidget(rangePicker);
    rangeLayout->addWidget(rangeSelectedLabel);
    modeLayout->addLayout(singleLayout);
    modeLayout->addLayout(multiLayout);
    modeLayout->addLayout(rangeLayout);
    layout->addLayout(modeLayout);
    layout->addSpacing(16);

    // 5. 不同状态的选择器
    QLabel* stateLabel = new QLabel("不同状态的选择器:", container);
    stateLabel->setFont(labelFont);
    layout->addWidget(stateLabel);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    // 禁用状态
    NCalendarDatePicker* disabledPicker = new NCalendarDatePicker(container);
    disabledPicker->setSelectedDate(QDate::currentDate());
    disabledPicker->setEnabled(false);
    disabledPicker->setMinimumWidth(180);
    QLabel* disabledLabel = new QLabel("禁用状态", container);
    disabledLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* disabledLayout = new QVBoxLayout();
    disabledLayout->addWidget(disabledPicker);
    disabledLayout->addWidget(disabledLabel);
    // 自定义占位符文本
    NCalendarDatePicker* placeholderPicker = new NCalendarDatePicker(container);
    placeholderPicker->setPlaceholderText("请选择一个日期");
    placeholderPicker->setMinimumWidth(180);
    QLabel* placeholderLabel = new QLabel("自定义占位符", container);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* placeholderLayout = new QVBoxLayout();
    placeholderLayout->addWidget(placeholderPicker);
    placeholderLayout->addWidget(placeholderLabel);
    stateLayout->addLayout(disabledLayout);
    stateLayout->addLayout(placeholderLayout);
    layout->addLayout(stateLayout);
    layout->addSpacing(16);

    // 6. 日期限制
    QLabel* limitLabel = new QLabel("日期范围限制:", container);
    limitLabel->setFont(labelFont);
    layout->addWidget(limitLabel);

    QHBoxLayout*         limitLayout   = new QHBoxLayout();
    NCalendarDatePicker* limitedPicker = new NCalendarDatePicker(container);
    limitedPicker->setMinimumDate(QDate::currentDate().addDays(-10));
    limitedPicker->setMaximumDate(QDate::currentDate().addDays(10));
    limitedPicker->setMinimumWidth(250);
    QLabel* rangeInfoLabel = new QLabel(QString("限制范围: %1 至 %2")
                                            .arg(QDate::currentDate().addDays(-10).toString("yyyy-MM-dd"),
                                                 QDate::currentDate().addDays(10).toString("yyyy-MM-dd")),
                                        container);

    QVBoxLayout* limitInfoLayout = new QVBoxLayout();
    limitInfoLayout->addWidget(limitedPicker);
    limitInfoLayout->addWidget(rangeInfoLabel);

    limitLayout->addLayout(limitInfoLayout);
    limitLayout->addStretch();

    layout->addLayout(limitLayout);

    layout->addStretch();
    return container;
}

QWidget* ControlsExample::createLabels() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    // 创建表格式布局
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setHorizontalSpacing(24);
    gridLayout->setVerticalSpacing(0);
    // 添加表头
    NLabel* exampleHeader = new NLabel("示例", NLabelType::BodyStrong, container);
    NLabel* fontHeader    = new NLabel("字体", NLabelType::BodyStrong, container);
    NLabel* sizeHeader    = new NLabel("大小/行高", NLabelType::BodyStrong, container);
    gridLayout->addWidget(exampleHeader, 0, 0);
    gridLayout->addWidget(fontHeader, 0, 1);
    gridLayout->addWidget(sizeHeader, 0, 2);

    // 1. Caption 样式
    int     row            = 1;
    NLabel* captionExample = new NLabel("Caption", NLabelType::Caption, container);
    NLabel* captionFont    = new NLabel("Small, Regular", NLabelType::Body, container);
    NLabel* captionSize    = new NLabel("12/16 epx", NLabelType::Body, container);
    gridLayout->addWidget(captionExample, row, 0);
    gridLayout->addWidget(captionFont, row, 1);
    gridLayout->addWidget(captionSize, row, 2);
    // 添加行分隔器
    QFrame* captionSeparator = new QFrame(container);
    captionSeparator->setFrameShape(QFrame::HLine);
    captionSeparator->setFrameShadow(QFrame::Plain);
    captionSeparator->setStyleSheet("background-color: #f0f0f0;");
    captionSeparator->setFixedHeight(1);
    gridLayout->addWidget(captionSeparator, row + 1, 0, 1, 4);
    // 2. Body 样式
    row += 2;
    NLabel* bodyExample = new NLabel("Body", NLabelType::Body, container);
    NLabel* bodyFont    = new NLabel("Text, Regular", NLabelType::Body, container);
    NLabel* bodySize    = new NLabel("14/20 epx", NLabelType::Body, container);
    gridLayout->addWidget(bodyExample, row, 0);
    gridLayout->addWidget(bodyFont, row, 1);
    gridLayout->addWidget(bodySize, row, 2);
    QFrame* bodySeparator = new QFrame(container);
    bodySeparator->setFrameShape(QFrame::HLine);
    bodySeparator->setFrameShadow(QFrame::Plain);
    bodySeparator->setStyleSheet("background-color: #f0f0f0;");
    bodySeparator->setFixedHeight(1);
    gridLayout->addWidget(bodySeparator, row + 1, 0, 1, 4);
    // 3. Body Strong 样式
    row += 2;
    NLabel* bodyStrongExample = new NLabel("Body Strong", NLabelType::BodyStrong, container);
    NLabel* bodyStrongFont    = new NLabel("Text, SemiBold", NLabelType::Body, container);
    NLabel* bodyStrongSize    = new NLabel("14/20 epx", NLabelType::Body, container);
    gridLayout->addWidget(bodyStrongExample, row, 0);
    gridLayout->addWidget(bodyStrongFont, row, 1);
    gridLayout->addWidget(bodyStrongSize, row, 2);
    QFrame* bodyStrongSeparator = new QFrame(container);
    bodyStrongSeparator->setFrameShape(QFrame::HLine);
    bodyStrongSeparator->setFrameShadow(QFrame::Plain);
    bodyStrongSeparator->setStyleSheet("background-color: #f0f0f0;");
    bodyStrongSeparator->setFixedHeight(1);
    gridLayout->addWidget(bodyStrongSeparator, row + 1, 0, 1, 4);
    // 4. Subtitle 样式
    row += 2;
    NLabel* subtitleExample = new NLabel("Subtitle", NLabelType::Subtitle, container);
    NLabel* subtitleFont    = new NLabel("Display, SemiBold", NLabelType::Body, container);
    NLabel* subtitleSize    = new NLabel("20/28 epx", NLabelType::Body, container);
    gridLayout->addWidget(subtitleExample, row, 0);
    gridLayout->addWidget(subtitleFont, row, 1);
    gridLayout->addWidget(subtitleSize, row, 2);
    QFrame* subtitleSeparator = new QFrame(container);
    subtitleSeparator->setFrameShape(QFrame::HLine);
    subtitleSeparator->setFrameShadow(QFrame::Plain);
    subtitleSeparator->setStyleSheet("background-color: #f0f0f0;");
    subtitleSeparator->setFixedHeight(1);
    gridLayout->addWidget(subtitleSeparator, row + 1, 0, 1, 4);
    // 5. Title 样式
    row += 2;
    NLabel* titleExample = new NLabel("Title", NLabelType::Title, container);
    NLabel* titleFont    = new NLabel("Display, SemiBold", NLabelType::Body, container);
    NLabel* titleSize    = new NLabel("28/36 epx", NLabelType::Body, container);
    gridLayout->addWidget(titleExample, row, 0);
    gridLayout->addWidget(titleFont, row, 1);
    gridLayout->addWidget(titleSize, row, 2);
    QFrame* titleSeparator = new QFrame(container);
    titleSeparator->setFrameShape(QFrame::HLine);
    titleSeparator->setFrameShadow(QFrame::Plain);
    titleSeparator->setStyleSheet("background-color: #f0f0f0;");
    titleSeparator->setFixedHeight(1);
    gridLayout->addWidget(titleSeparator, row + 1, 0, 1, 4);
    // 6. Title Large 样式
    row += 2;
    NLabel* titleLargeExample = new NLabel("Title Large", NLabelType::TitleLarge, container);
    titleLargeExample->setType(NLabelType::TitleLarge);
    NLabel* titleLargeFontLabel = new NLabel("Display, SemiBold", NLabelType::Body, container);
    NLabel* titleLargeSize      = new NLabel("40/52 epx", NLabelType::Body, container);

    gridLayout->addWidget(titleLargeExample, row, 0);
    gridLayout->addWidget(titleLargeFontLabel, row, 1);
    gridLayout->addWidget(titleLargeSize, row, 2);
    QFrame* titleLargeSeparator = new QFrame(container);
    titleLargeSeparator->setFrameShape(QFrame::HLine);
    titleLargeSeparator->setFrameShadow(QFrame::Plain);
    titleLargeSeparator->setStyleSheet("background-color: #f0f0f0;");
    titleLargeSeparator->setFixedHeight(1);
    gridLayout->addWidget(titleLargeSeparator, row + 1, 0, 1, 4);
    // 7. Display 样式
    row += 2;
    NLabel* displayExample = new NLabel("Display", NLabelType::Display, container);
    NLabel* displayFont    = new NLabel("Display, SemiBold", NLabelType::Body, container);
    NLabel* displaySize    = new NLabel("68/92 epx", NLabelType::Body, container);
    gridLayout->addWidget(displayExample, row, 0);
    gridLayout->addWidget(displayFont, row, 1);
    gridLayout->addWidget(displaySize, row, 2);
    // 设置列宽度比例
    gridLayout->setColumnStretch(0, 2);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);
    gridLayout->setColumnStretch(3, 2);

    // 设置行高
    for (int i = 0; i <= row; i += 2) {
        gridLayout->setRowMinimumHeight(i, 48);
    }

    // 添加到主布局
    layout->addLayout(gridLayout);
    layout->addStretch();

    return container;
}
