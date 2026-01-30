#include "AutoSuggestBoxPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NAutoSuggestBox.h>
#include <QtNativeUI/NLabel.h>

AutoSuggestBoxPage::AutoSuggestBoxPage(QWidget* parent)
    : BasePage("AutoSuggestBox 建议框", "AutoSuggestBox 提供输入建议功能，支持图标和自定义数据。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NAutoSuggestBox* basicSuggestBox = new NAutoSuggestBox(this);
    basicSuggestBox->setPlaceholderText("搜索...");
    basicSuggestBox->setMinimumWidth(300);
    basicSuggestBox->addSuggestion("Apple");
    basicSuggestBox->addSuggestion("Application");
    basicSuggestBox->addSuggestion("Banana");
    basicSuggestBox->addSuggestion("Cherry");
    basicSuggestBox->addSuggestion("Chocolate");
    NLabel* basicResultLabel = new NLabel("选择结果: 无", this);
    connect(basicSuggestBox, &NAutoSuggestBox::suggestionClicked, [=](QString text, QVariantMap data) {
        basicResultLabel->setText("选择结果: " + text);
    });
    QVBoxLayout* basicColumn = new QVBoxLayout();
    basicColumn->setSpacing(8);
    basicColumn->addWidget(basicSuggestBox);
    basicColumn->addWidget(basicResultLabel);
    basicLayout->addLayout(basicColumn);
    basicLayout->addStretch();
    addSection("基本建议框", basicLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);
    NAutoSuggestBox* iconSuggestBox = new NAutoSuggestBox(this);
    iconSuggestBox->setPlaceholderText("搜索文件...");
    iconSuggestBox->setMinimumWidth(300);
    iconSuggestBox->addSuggestion(NRegularIconType::Document16Regular, "文档.docx");
    iconSuggestBox->addSuggestion(NRegularIconType::SlideAdd16Regular, "演示.ppt");
    iconSuggestBox->addSuggestion(NRegularIconType::Image16Regular, "图片.jpg");
    iconSuggestBox->addSuggestion(NRegularIconType::Code16Regular, "代码.cpp");
    iconSuggestBox->addSuggestion(NRegularIconType::Table16Regular, "表格.xlsx");
    iconSuggestBox->addSuggestion(NRegularIconType::Video16Regular, "视频.mp4");
    NLabel* iconResultLabel = new NLabel("选择结果: 无", this);
    connect(iconSuggestBox, &NAutoSuggestBox::suggestionClicked, [=](QString text, QVariantMap data) {
        iconResultLabel->setText("选择结果: " + text);
    });
    QVBoxLayout* iconColumn = new QVBoxLayout();
    iconColumn->setSpacing(8);
    iconColumn->addWidget(iconSuggestBox);
    iconColumn->addWidget(iconResultLabel);
    iconLayout->addLayout(iconColumn);
    iconLayout->addStretch();
    addSection("带图标的建议框", iconLayout);

    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(16);
    NAutoSuggestBox* startsWithBox = new NAutoSuggestBox(this);
    startsWithBox->setPlaceholderText("以...开头");
    startsWithBox->setFilterMode(NAutoSuggestBox::FilterMode::StartsWith);
    startsWithBox->setMinimumWidth(300);
    startsWithBox->addSuggestion("Apple");
    startsWithBox->addSuggestion("Application");
    startsWithBox->addSuggestion("Banana");
    startsWithBox->addSuggestion("Cherry");
    QVBoxLayout* startsColumn = new QVBoxLayout();
    startsColumn->setSpacing(8);
    startsColumn->addWidget(startsWithBox);
    startsColumn->addWidget(new NLabel("StartsWith 模式", this));
    filterLayout->addLayout(startsColumn);
    NAutoSuggestBox* containsBox = new NAutoSuggestBox(this);
    containsBox->setPlaceholderText("包含...");
    containsBox->setFilterMode(NAutoSuggestBox::FilterMode::Contains);
    containsBox->setMinimumWidth(300);
    containsBox->addSuggestion("Apple");
    containsBox->addSuggestion("Application");
    containsBox->addSuggestion("Banana");
    containsBox->addSuggestion("Cherry");
    QVBoxLayout* containsColumn = new QVBoxLayout();
    containsColumn->setSpacing(8);
    containsColumn->addWidget(containsBox);
    containsColumn->addWidget(new NLabel("Contains 模式", this));
    filterLayout->addLayout(containsColumn);
    filterLayout->addStretch();
    addSection("过滤模式", filterLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(8);
    NAutoSuggestBox* interactiveBox = new NAutoSuggestBox(this);
    interactiveBox->setPlaceholderText("输入并选择或提交");
    interactiveBox->setMinimumWidth(300);
    interactiveBox->addSuggestion(NFilledIconType::Search16Filled, "搜索引擎");
    interactiveBox->addSuggestion(NFilledIconType::Globe16Filled, "网站");
    interactiveBox->addSuggestion(NFilledIconType::Book16Filled, "文档");
    NLabel* statusLabel = new NLabel("状态: 等待输入", this);
    connect(interactiveBox, &NAutoSuggestBox::suggestionClicked, [=](QString text, QVariantMap data) {
        statusLabel->setText("选择了建议: " + text);
    });
    connect(interactiveBox, &NAutoSuggestBox::querySubmitted, [=](const QString& query) {
        statusLabel->setText("提交了查询: " + query);
    });
    interactiveLayout->addWidget(interactiveBox);
    interactiveLayout->addWidget(statusLabel);
    addSection("交互示例", interactiveLayout);
}
