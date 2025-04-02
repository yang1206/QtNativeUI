#include "controls.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NToggleSwitch.h>

#include "QtNativeUI/NTextEdit.h"
#include "widgets/ExampleSection.h"

// 在头文件包含部分添加
#include <QtNativeUI/NSlider.h>
#include <QtNativeUI/NSpinBox.h>

#include "QtNativeUI/NPlainTextEdit.h"

ControlsExample::ControlsExample(QWidget* parent) : QWidget(parent) { initUI(); }

// 在 initUI() 函数中添加 SpinBox 部分
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
    contentLayout->addWidget(new ExampleSection("Slider", createSliders()));

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
    suffixLineEdit->addAction(NFilledIconType::Search16Filled, NLineEdit::ActionPosition::TrailingPosition);
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