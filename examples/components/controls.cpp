#include "controls.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NToggleSwitch.h>
#include "widgets/ExampleSection.h"

ControlsExample::ControlsExample(QWidget* parent) : QWidget(parent) {
    initUI();
}

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
    QWidget* contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // 添加各个控件区域
    contentLayout->addWidget(new ExampleSection("CheckBox", createCheckBoxes()));
    contentLayout->addWidget(new ExampleSection("ToggleSwitch", createToggleSwitches()));
    contentLayout->addWidget(new ExampleSection("LineEdit", createLineEdits()));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
    setMinimumWidth(600);
}

QWidget* ControlsExample::createCheckBoxes() {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
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
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
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
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
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