#include "ComboBoxPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NComboBox.h>
#include <QtNativeUI/NLabel.h>

ComboBoxPage::ComboBoxPage(QWidget* parent)
    : BasePage("ComboBox 下拉框", "ComboBox 提供下拉选择功能，支持可编辑模式。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NComboBox* basicComboBox = new NComboBox(this);
    basicComboBox->addItem("选项 1");
    basicComboBox->addItem("选项 2");
    basicComboBox->addItem("选项 3");
    basicComboBox->setMinimumWidth(200);
    basicLayout->addWidget(basicComboBox);
    QStringList cityList          = {"北京", "上海", "广州", "深圳", "杭州"};
    NComboBox*  preselectComboBox = new NComboBox(cityList, this);
    preselectComboBox->setCurrentIndex(2);
    preselectComboBox->setMinimumWidth(200);
    basicLayout->addWidget(preselectComboBox);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* editableLayout = new QHBoxLayout();
    editableLayout->setSpacing(16);
    NComboBox* editableComboBox = new NComboBox(this);
    editableComboBox->addItems({"选项 A", "选项 B", "选项 C"});
    editableComboBox->setEditable(true);
    editableComboBox->setMinimumWidth(200);
    editableLayout->addWidget(editableComboBox);
    NComboBox* editableWithTextComboBox = new NComboBox(this);
    editableWithTextComboBox->addItems({"Apple", "Banana", "Cherry"});
    editableWithTextComboBox->setEditable(true);
    editableWithTextComboBox->setCurrentText("输入或选择");
    editableWithTextComboBox->setMinimumWidth(200);
    editableLayout->addWidget(editableWithTextComboBox);
    editableLayout->addStretch();
    addSection("可编辑模式", editableLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    NComboBox* disabledComboBox = new NComboBox(this);
    disabledComboBox->addItems({"禁用项 1", "禁用项 2", "禁用项 3"});
    disabledComboBox->setEnabled(false);
    disabledComboBox->setMinimumWidth(200);
    stateLayout->addWidget(disabledComboBox);
    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    NComboBox* signalComboBox = new NComboBox(this);
    signalComboBox->addItems({"红色", "绿色", "蓝色", "黄色", "紫色"});
    signalComboBox->setMinimumWidth(200);
    NLabel* selectionLabel = new NLabel("当前选择: 红色", this);
    connect(signalComboBox, QOverload<int>::of(&NComboBox::currentIndexChanged), [selectionLabel, signalComboBox](int index) {
        selectionLabel->setText("当前选择: " + signalComboBox->currentText());
    });
    interactiveLayout->addWidget(signalComboBox);
    interactiveLayout->addWidget(selectionLabel);
    addSection("交互示例", interactiveLayout);
}
