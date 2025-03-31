#include "lineedit.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NTheme.h>

LineEditExample::LineEditExample(QWidget* parent) : QWidget(parent) { initUI(); }

LineEditExample::~LineEditExample() {}

void LineEditExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(24);

    // 标准输入框展示区 - 明确设置父对象
    QWidget*     standardSection = new QWidget(this);
    QVBoxLayout* standardLayout  = new QVBoxLayout(standardSection);
    standardLayout->setSpacing(16);

    // 为所有控件设置父对象
    QLabel* standardTitle = new QLabel("Line Edit Controls", standardSection);
    QFont   titleFont     = standardTitle->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    standardTitle->setFont(titleFont);
    standardLayout->addWidget(standardTitle);

    // 基本输入框示例
    QLabel* basicLabel = new QLabel("Basic Line Edit", standardSection);
    basicLabel->setFont(titleFont);
    standardLayout->addWidget(basicLabel);

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    // 普通状态 - 设置父对象
    NLineEdit* normalLineEdit = new NLineEdit("Normal Line Edit", standardSection);
    normalLineEdit->setPlaceholderText("Enter text here");
    normalLineEdit->setMinimumWidth(200);

    // 禁用状态 - 设置父对象
    NLineEdit* disabledLineEdit = new NLineEdit("Disabled Line Edit", standardSection);
    disabledLineEdit->setEnabled(false);
    disabledLineEdit->setMinimumWidth(200);

    // 只读状态 - 设置父对象
    NLineEdit* readOnlyLineEdit = new NLineEdit("Read Only Line Edit", standardSection);
    readOnlyLineEdit->setReadOnly(true);
    readOnlyLineEdit->setMinimumWidth(200);

    basicLayout->addWidget(normalLineEdit);
    basicLayout->addWidget(disabledLineEdit);
    basicLayout->addWidget(readOnlyLineEdit);
    basicLayout->addStretch();

    standardLayout->addLayout(basicLayout);

    // 带前缀和后缀的输入框
    QLabel* affixLabel = new QLabel("Line Edit with Prefix/Suffix", standardSection);
    affixLabel->setFont(titleFont);
    standardLayout->addWidget(affixLabel);

    QHBoxLayout* affixLayout = new QHBoxLayout();
    affixLayout->setSpacing(16);

    // 带前缀的输入框 - 设置父对象
    NLineEdit* prefixLineEdit = new NLineEdit(QString(), standardSection);
    prefixLineEdit->setPlaceholderText("With prefix");
    prefixLineEdit->setMinimumWidth(200);
    prefixLineEdit->setText("10.0.0.1");

    // 带后缀的输入框 - 设置父对象
    NLineEdit* suffixLineEdit = new NLineEdit(QString(), standardSection);
    suffixLineEdit->setPlaceholderText("With suffix");
    suffixLineEdit->setMinimumWidth(200);
    suffixLineEdit->setText("example");

    // 清空按钮示例 - 设置父对象
    NLineEdit* clearableLineEdit = new NLineEdit(QString(), standardSection);
    clearableLineEdit->setPlaceholderText("With clear button");
    clearableLineEdit->setClearButtonEnabled(true);
    clearableLineEdit->setMinimumWidth(200);

    affixLayout->addWidget(prefixLineEdit);
    affixLayout->addWidget(suffixLineEdit);
    affixLayout->addWidget(clearableLineEdit);
    affixLayout->addStretch();

    standardLayout->addLayout(affixLayout);

    // 比较标准Qt风格和NLineEdit风格
    QLabel* comparisonLabel = new QLabel("Qt vs NLineEdit Comparison", standardSection);
    comparisonLabel->setFont(titleFont);
    standardLayout->addWidget(comparisonLabel);

    QHBoxLayout* comparisonLayout = new QHBoxLayout();
    comparisonLayout->setSpacing(16);

    // 标准Qt LineEdit - 设置父对象
    QLineEdit* qtLineEdit = new QLineEdit("Standard Qt LineEdit", standardSection);
    qtLineEdit->setMinimumWidth(200);

    // NLineEdit - 设置父对象
    NLineEdit* nativeLineEdit = new NLineEdit("NLineEdit Style", standardSection);
    nativeLineEdit->setMinimumWidth(200);

    comparisonLayout->addWidget(qtLineEdit);
    comparisonLayout->addWidget(nativeLineEdit);
    comparisonLayout->addStretch();

    standardLayout->addLayout(comparisonLayout);

    mainLayout->addWidget(standardSection);
    mainLayout->addStretch();
}