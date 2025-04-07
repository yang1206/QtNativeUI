#include "controls.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NToggleSwitch.h>

#include "QtNativeUI/NTextEdit.h"
#include "widgets/ExampleSection.h"

// 在头文件包含部分添加
#include <QTimer>
#include <QtNativeUI/NSlider.h>
#include <QtNativeUI/NSpinBox.h>

#include <QtNativeUI/NPushButton.h>
#include "QtNativeUI/NDoubleSpinBox.h"
#include "QtNativeUI/NPlainTextEdit.h"
#include "QtNativeUI/NProgressBar.h"
#include "QtNativeUI/NScrollArea.h"
#include "QtNativeUI/NToolTip.h"

ControlsExample::ControlsExample(QWidget* parent) : QWidget(parent) { initUI(); }

// 在 initUI() 函数中添加 ProgressBar 部分
void ControlsExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建内容容器
    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // 添加各个控件区域
    contentLayout->addWidget(new ExampleSection("CheckBox", createCheckBoxes()));
    contentLayout->addWidget(new ExampleSection("ToggleSwitch", createToggleSwitches()));
    contentLayout->addWidget(new ExampleSection("LineEdit", createLineEdits()));
    contentLayout->addWidget(new ExampleSection("TextEdit", createTextEdits()));
    contentLayout->addWidget(new ExampleSection("PlainTextEdit", createPlainTextEdits()));
    contentLayout->addWidget(new ExampleSection("SpinBox", createSpinBoxes()));
    contentLayout->addWidget(new ExampleSection("DoubleSpinBox", createDoubleSpinBoxes()));
    contentLayout->addWidget(new ExampleSection("Slider", createSliders()));
    contentLayout->addWidget(new ExampleSection("ProgressBar", createProgressBars()));
    contentLayout->addWidget(new ExampleSection("ScrollArea", createScrollAreas()));
    contentLayout->addWidget(new ExampleSection("ToolTip", createToolTips()));

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
    NProgressBar* basicProgressBar = new NProgressBar(container);
    basicProgressBar->setValue(50);
    basicProgressBar->setMinimumWidth(300);
    layout->addWidget(basicProgressBar);

    // 带文本的进度条
    NProgressBar* textProgressBar = new NProgressBar(container);
    textProgressBar->setValue(75);
    textProgressBar->setFormat("已完成 %p%");
    textProgressBar->setMinimumWidth(300);
    layout->addWidget(textProgressBar);

    // 不确定状态的进度条
    NProgressBar* indeterminateProgressBar = new NProgressBar(container);
    indeterminateProgressBar->setMinimum(0);
    indeterminateProgressBar->setMaximum(0); // 设置为不确定状态
    indeterminateProgressBar->setMinimumWidth(300);
    layout->addWidget(indeterminateProgressBar);

    // 反向进度条
    NProgressBar* invertedProgressBar = new NProgressBar(container);
    invertedProgressBar->setValue(60);
    invertedProgressBar->setInvertedAppearance(true);
    invertedProgressBar->setMinimumWidth(300);
    layout->addWidget(invertedProgressBar);

    // 垂直进度条
    QHBoxLayout* verticalLayout = new QHBoxLayout();
    
    NProgressBar* verticalProgressBar = new NProgressBar(container);
    verticalProgressBar->setOrientation(Qt::Vertical);
    verticalProgressBar->setValue(80);
    verticalProgressBar->setMinimumHeight(150);
    verticalLayout->addWidget(verticalProgressBar);
    
    // 垂直反向进度条
    NProgressBar* verticalInvertedProgressBar = new NProgressBar(container);
    verticalInvertedProgressBar->setOrientation(Qt::Vertical);
    verticalInvertedProgressBar->setValue(40);
    verticalInvertedProgressBar->setInvertedAppearance(true);
    verticalInvertedProgressBar->setMinimumHeight(150);
    verticalLayout->addWidget(verticalInvertedProgressBar);
    
    verticalLayout->addStretch();
    layout->addLayout(verticalLayout);

    // 禁用状态的进度条
    NProgressBar* disabledProgressBar = new NProgressBar(container);
    disabledProgressBar->setValue(30);
    disabledProgressBar->setEnabled(false);
    disabledProgressBar->setMinimumWidth(300);
    layout->addWidget(disabledProgressBar);

    // 添加一个动态更新的进度条
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
    
    controlLayout->addStretch();
    layout->addLayout(controlLayout);

    return container;
}

// 添加 createScrollAreas 函数实现
QWidget* ControlsExample::createScrollAreas() {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基本滚动区域
    QLabel* basicLabel = new QLabel("基本滚动区域：", container);
    QFont labelFont = basicLabel->font();
    labelFont.setBold(true);
    basicLabel->setFont(labelFont);
    layout->addWidget(basicLabel);

    NScrollArea* basicScrollArea = new NScrollArea(container);
    basicScrollArea->setMinimumHeight(200);
    basicScrollArea->setMinimumWidth(400);
    
    // 创建内容
    QWidget* contentWidget = new QWidget(basicScrollArea);
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
    QWidget* horizontalContentWidget = new QWidget(horizontalScrollArea);
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
    QWidget* disabledContentWidget = new QWidget(disabledScrollArea);
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