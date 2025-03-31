#include "lineedit.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStyle>
#include <QVBoxLayout>
#include <QtGui/qvalidator.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NTheme.h>

LineEditExample::LineEditExample(QWidget* parent) : QWidget(parent) { initUI(); }

void LineEditExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(24);

    // 标准输入框展示区
    QWidget*     standardSection = new QWidget;
    QVBoxLayout* standardLayout  = new QVBoxLayout(standardSection);
    standardLayout->setSpacing(16);

    QLabel* standardTitle = new QLabel("Line Edit Controls");
    QFont   titleFont     = standardTitle->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    standardTitle->setFont(titleFont);
    standardLayout->addWidget(standardTitle);

    // 基本输入框示例
    QLabel* basicLabel = new QLabel("Basic Line Edit");
    basicLabel->setFont(titleFont);
    standardLayout->addWidget(basicLabel);

    QHBoxLayout* basicLayout = new QHBoxLayout;
    basicLayout->setSpacing(16);

    // 普通状态
    NLineEdit* normalLineEdit = new NLineEdit("Normal Line Edit");
    normalLineEdit->setPlaceholderText("Enter text here");
    normalLineEdit->setMinimumWidth(200);

    // 禁用状态
    NLineEdit* disabledLineEdit = new NLineEdit("Disabled Line Edit");
    disabledLineEdit->setEnabled(false);
    disabledLineEdit->setMinimumWidth(200);

    // 只读状态
    NLineEdit* readOnlyLineEdit = new NLineEdit("Read Only Line Edit");
    readOnlyLineEdit->setReadOnly(true);
    readOnlyLineEdit->setMinimumWidth(200);

    basicLayout->addWidget(normalLineEdit);
    basicLayout->addWidget(disabledLineEdit);
    basicLayout->addWidget(readOnlyLineEdit);
    basicLayout->addStretch();

    standardLayout->addLayout(basicLayout);

    // 带前缀和后缀的输入框
    QLabel* affixLabel = new QLabel("Line Edit with Prefix/Suffix");
    affixLabel->setFont(titleFont);
    standardLayout->addWidget(affixLabel);

    QHBoxLayout* affixLayout = new QHBoxLayout;
    affixLayout->setSpacing(16);

    // 带前缀的输入框
    NLineEdit* prefixLineEdit = new NLineEdit();
    prefixLineEdit->setPlaceholderText("With prefix");
    prefixLineEdit->setMinimumWidth(200);
    prefixLineEdit->setText("10.0.0.1");

    // 带后缀的输入框
    NLineEdit* suffixLineEdit = new NLineEdit();
    suffixLineEdit->setPlaceholderText("With suffix");
    suffixLineEdit->setMinimumWidth(200);
    suffixLineEdit->setText("example");

    // 清空按钮示例
    NLineEdit* clearableLineEdit = new NLineEdit();
    clearableLineEdit->setPlaceholderText("With clear button");
    clearableLineEdit->setClearButtonEnabled(true);
    clearableLineEdit->setMinimumWidth(200);

    affixLayout->addWidget(prefixLineEdit);
    affixLayout->addWidget(suffixLineEdit);
    affixLayout->addWidget(clearableLineEdit);
    affixLayout->addStretch();

    standardLayout->addLayout(affixLayout);

    // 验证输入示例
    QLabel* validationLabel = new QLabel("Input Validation");
    validationLabel->setFont(titleFont);
    standardLayout->addWidget(validationLabel);

    QHBoxLayout* validationLayout = new QHBoxLayout;
    validationLayout->setSpacing(16);

    // 数字输入框
    NLineEdit* numberLineEdit = new NLineEdit();
    numberLineEdit->setPlaceholderText("Numbers only");
    numberLineEdit->setValidator(new QIntValidator(0, 999, this));
    numberLineEdit->setMinimumWidth(200);

    // 密码输入框
    NLineEdit* passwordLineEdit = new NLineEdit();
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setMinimumWidth(200);

    validationLayout->addWidget(numberLineEdit);
    validationLayout->addWidget(passwordLineEdit);
    validationLayout->addStretch();

    standardLayout->addLayout(validationLayout);

    // 比较标准Qt风格和NLineEdit风格
    QLabel* comparisonLabel = new QLabel("Qt vs NLineEdit Comparison");
    comparisonLabel->setFont(titleFont);
    standardLayout->addWidget(comparisonLabel);

    QHBoxLayout* comparisonLayout = new QHBoxLayout;
    comparisonLayout->setSpacing(16);

    // 标准Qt LineEdit
    QLineEdit* qtLineEdit = new QLineEdit("Standard Qt LineEdit");
    qtLineEdit->setMinimumWidth(200);

    // NLineEdit
    NLineEdit* nativeLineEdit = new NLineEdit("NLineEdit Style");
    nativeLineEdit->setMinimumWidth(200);

    comparisonLayout->addWidget(qtLineEdit);
    comparisonLayout->addWidget(nativeLineEdit);
    comparisonLayout->addStretch();

    standardLayout->addLayout(comparisonLayout);

    mainLayout->addWidget(standardSection);
    mainLayout->addStretch();
}