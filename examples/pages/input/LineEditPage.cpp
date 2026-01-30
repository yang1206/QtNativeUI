#include "LineEditPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NLineEdit.h>

LineEditPage::LineEditPage(QWidget* parent)
    : BasePage("LineEdit 单行输入框", "LineEdit 用于单行文本输入，支持图标、清除按钮等功能。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NLineEdit* normalLineEdit = new NLineEdit(this);
    normalLineEdit->setPlaceholderText("基本输入框");
    normalLineEdit->setMinimumWidth(300);
    basicLayout->addWidget(normalLineEdit);
    NLineEdit* withTextLineEdit = new NLineEdit("带初始文本的输入框", this);
    withTextLineEdit->setMinimumWidth(300);
    basicLayout->addWidget(withTextLineEdit);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);
    NLineEdit* prefixLineEdit = new NLineEdit(this);
    prefixLineEdit->setPlaceholderText("带前缀图标");
    prefixLineEdit->addAction(NFilledIconType::Server16Filled, NLineEdit::ActionPosition::LeadingPosition);
    prefixLineEdit->setMinimumWidth(300);
    iconLayout->addWidget(prefixLineEdit);
    NLineEdit* suffixLineEdit = new NLineEdit(this);
    suffixLineEdit->setPlaceholderText("带后缀图标");
    suffixLineEdit->addAction(NFilledIconType::Search16Filled, NLineEdit::ActionPosition::TrailingPosition);
    suffixLineEdit->setMinimumWidth(300);
    iconLayout->addWidget(suffixLineEdit);
    iconLayout->addStretch();
    addSection("带图标", iconLayout);

    QHBoxLayout* clearLayout = new QHBoxLayout();
    clearLayout->setSpacing(16);
    NLineEdit* clearableLineEdit = new NLineEdit(this);
    clearableLineEdit->setPlaceholderText("带清除按钮");
    clearableLineEdit->setClearButtonEnabled(true);
    clearableLineEdit->setMinimumWidth(300);
    clearLayout->addWidget(clearableLineEdit);
    NLineEdit* clearWithIconLineEdit = new NLineEdit(this);
    clearWithIconLineEdit->setPlaceholderText("清除按钮 + 图标");
    clearWithIconLineEdit->addAction(NRegularIconType::Search16Regular, NLineEdit::ActionPosition::LeadingPosition);
    clearWithIconLineEdit->setClearButtonEnabled(true);
    clearWithIconLineEdit->setMinimumWidth(300);
    clearLayout->addWidget(clearWithIconLineEdit);
    clearLayout->addStretch();
    addSection("清除按钮", clearLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    NLineEdit* disabledLineEdit = new NLineEdit(this);
    disabledLineEdit->setPlaceholderText("禁用状态");
    disabledLineEdit->setEnabled(false);
    disabledLineEdit->setMinimumWidth(300);
    stateLayout->addWidget(disabledLineEdit);
    NLineEdit* readOnlyLineEdit = new NLineEdit("只读状态", this);
    readOnlyLineEdit->setReadOnly(true);
    readOnlyLineEdit->setMinimumWidth(300);
    stateLayout->addWidget(readOnlyLineEdit);
    stateLayout->addStretch();
    addSection("不同状态", stateLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    NLineEdit*       interactiveLineEdit = new NLineEdit(this);
    interactiveLineEdit->setPlaceholderText("输入文本查看信号");
    interactiveLineEdit->setMinimumWidth(300);
    QLabel* statusLabel = new QLabel("状态: 等待输入", this);
    connect(interactiveLineEdit, &NLineEdit::textChanged, this, [statusLabel](const QString& text) {
        statusLabel->setText(QString("文本改变: %1").arg(text.isEmpty() ? "(空)" : text));
    });
    connect(interactiveLineEdit, &NLineEdit::focusIn, this, [statusLabel](const QString& text) {
        statusLabel->setText(QString("获得焦点: %1").arg(text.isEmpty() ? "(空)" : text));
    });
    connect(interactiveLineEdit, &NLineEdit::focusOut, this, [statusLabel](const QString& text) {
        statusLabel->setText(QString("失去焦点: %1").arg(text.isEmpty() ? "(空)" : text));
    });
    interactiveLayout->addWidget(interactiveLineEdit);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
