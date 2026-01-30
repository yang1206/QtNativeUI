#include "PlainTextEditPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NPlainTextEdit.h>

PlainTextEditPage::PlainTextEditPage(QWidget* parent)
    : BasePage("PlainTextEdit 纯文本编辑器", "PlainTextEdit 用于纯文本编辑，性能优于 TextEdit。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NPlainTextEdit* normalTextEdit = new NPlainTextEdit(this);
    normalTextEdit->setPlaceholderText("基本纯文本编辑器");
    normalTextEdit->setMinimumWidth(400);
    normalTextEdit->setMinimumHeight(150);
    basicLayout->addWidget(normalTextEdit);
    NPlainTextEdit* withTextEdit = new NPlainTextEdit("带初始文本的纯文本编辑器\n支持多行文本", this);
    withTextEdit->setMinimumWidth(400);
    withTextEdit->setMinimumHeight(150);
    basicLayout->addWidget(withTextEdit);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    NPlainTextEdit* disabledTextEdit = new NPlainTextEdit(this);
    disabledTextEdit->setPlaceholderText("禁用状态");
    disabledTextEdit->setEnabled(false);
    disabledTextEdit->setMinimumWidth(400);
    disabledTextEdit->setMinimumHeight(150);
    stateLayout->addWidget(disabledTextEdit);
    NPlainTextEdit* readOnlyTextEdit = new NPlainTextEdit("只读状态\n无法编辑此文本", this);
    readOnlyTextEdit->setReadOnly(true);
    readOnlyTextEdit->setMinimumWidth(400);
    readOnlyTextEdit->setMinimumHeight(150);
    stateLayout->addWidget(readOnlyTextEdit);
    stateLayout->addStretch();
    addSection("不同状态", stateLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    NPlainTextEdit* interactiveTextEdit = new NPlainTextEdit(this);
    interactiveTextEdit->setPlaceholderText("输入文本查看信号");
    interactiveTextEdit->setMinimumWidth(400);
    interactiveTextEdit->setMinimumHeight(150);
    QLabel* statusLabel = new QLabel("状态: 等待输入", this);
    connect(interactiveTextEdit, &NPlainTextEdit::textChanged, this, [statusLabel, interactiveTextEdit]() {
        QString text = interactiveTextEdit->toPlainText();
        statusLabel->setText(QString("文本改变: %1 字符").arg(text.length()));
    });
    interactiveLayout->addWidget(interactiveTextEdit);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
