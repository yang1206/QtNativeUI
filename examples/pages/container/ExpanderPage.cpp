#include "ExpanderPage.h"
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NExpander.h>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NSpinBox.h>
#include <QtNativeUI/NToggleSwitch.h>

ExpanderPage::ExpanderPage(QWidget* parent)
    : BasePage("Expander 折叠面板", "Expander 用于显示可展开和折叠的内容区域。", parent) {
    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NExpander* basicExpander = new NExpander("基本信息", this);

    basicExpander->contentWidget()->layout()->addWidget(new NLineEdit("姓名"));
    basicExpander->contentWidget()->layout()->addWidget(new NLineEdit("邮箱"));
    basicExpander->contentWidget()->layout()->addWidget(new NLineEdit("电话"));

    basicLayout->addWidget(basicExpander);

    addSection("基本用法", basicLayout);

    QVBoxLayout* iconLayout = new QVBoxLayout();
    iconLayout->setSpacing(16);

    NExpander* settingsExpander = new NExpander("设置选项", this);
    settingsExpander->setHeaderIcon(NRegularIconType::Settings24Regular, 20);

    settingsExpander->contentWidget()->layout()->addWidget(new NToggleSwitch("启用通知"));
    settingsExpander->contentWidget()->layout()->addWidget(new NToggleSwitch("自动更新"));
    settingsExpander->contentWidget()->layout()->addWidget(new NToggleSwitch("深色模式"));

    NExpander* securityExpander = new NExpander("安全设置", this);
    securityExpander->setHeaderIcon(NFilledIconType::Shield24Filled, 20);

    securityExpander->contentWidget()->layout()->addWidget(new NCheckBox("启用防火墙"));
    securityExpander->contentWidget()->layout()->addWidget(new NCheckBox("自动扫描"));
    securityExpander->contentWidget()->layout()->addWidget(new NCheckBox("实时保护"));

    iconLayout->addWidget(settingsExpander);
    iconLayout->addWidget(securityExpander);

    addSection("带图标的标题", iconLayout);

    QVBoxLayout* nestedLayout = new QVBoxLayout();
    nestedLayout->setSpacing(16);

    NExpander* parentExpander = new NExpander("用户配置", this);
    parentExpander->setHeaderIcon(NRegularIconType::Person24Regular, 20);

    parentExpander->contentWidget()->layout()->addWidget(new NLabel("个人信息"));
    parentExpander->contentWidget()->layout()->addWidget(new NLineEdit("用户名"));
    parentExpander->contentWidget()->layout()->addWidget(new NLineEdit("显示名称"));
    parentExpander->contentWidget()->layout()->addWidget(new NLineEdit("邮箱地址"));

    parentExpander->contentWidget()->layout()->addWidget(new NLabel("权限设置"));
    parentExpander->contentWidget()->layout()->addWidget(new NCheckBox("管理员权限"));
    parentExpander->contentWidget()->layout()->addWidget(new NCheckBox("读取权限"));
    parentExpander->contentWidget()->layout()->addWidget(new NCheckBox("写入权限"));

    nestedLayout->addWidget(parentExpander);

    addSection("嵌套内容", nestedLayout);

    QVBoxLayout* multipleLayout = new QVBoxLayout();
    multipleLayout->setSpacing(16);

    NExpander* expander1 = new NExpander("第一个面板", this);
    expander1->setHeaderIcon(NRegularIconType::Document24Regular, 20);
    expander1->contentWidget()->layout()->addWidget(new NLabel("这是第一个面板的内容"));
    expander1->contentWidget()->layout()->addWidget(new NPushButton("操作按钮"));

    NExpander* expander2 = new NExpander("第二个面板", this);
    expander2->setHeaderIcon(NRegularIconType::Folder24Regular, 20);
    expander2->contentWidget()->layout()->addWidget(new NLabel("这是第二个面板的内容"));
    expander2->contentWidget()->layout()->addWidget(new NLineEdit("输入框"));

    NExpander* expander3 = new NExpander("第三个面板", this);
    expander3->setHeaderIcon(NRegularIconType::Image24Regular, 20);
    expander3->contentWidget()->layout()->addWidget(new NLabel("这是第三个面板的内容"));
    expander3->contentWidget()->layout()->addWidget(new NCheckBox("选项 1"));
    expander3->contentWidget()->layout()->addWidget(new NCheckBox("选项 2"));

    multipleLayout->addWidget(expander1);
    multipleLayout->addWidget(expander2);
    multipleLayout->addWidget(expander3);

    addSection("多个折叠面板", multipleLayout);

    QVBoxLayout* stateLayout = new QVBoxLayout();
    stateLayout->setSpacing(16);

    NExpander* normalExpander = new NExpander("正常状态", this);
    normalExpander->contentWidget()->layout()->addWidget(new NLabel("这是正常状态的面板"));
    normalExpander->contentWidget()->layout()->addWidget(new NPushButton("按钮"));

    NExpander* disabledExpander = new NExpander("禁用状态", this);
    disabledExpander->setEnabled(false);
    disabledExpander->contentWidget()->layout()->addWidget(new NLabel("这是禁用状态的面板"));
    disabledExpander->contentWidget()->layout()->addWidget(new NPushButton("按钮"));

    stateLayout->addWidget(normalExpander);
    stateLayout->addWidget(disabledExpander);

    addSection("不同状态", stateLayout);

    QVBoxLayout* customLayout = new QVBoxLayout();
    customLayout->setSpacing(16);

    NExpander* customExpander = new NExpander("自定义样式", this);
    customExpander->setBorderRadius(16);
    customExpander->setHeaderHeight(56);
    customExpander->setContentPadding(24);

    customExpander->contentWidget()->layout()->addWidget(new NLabel("这个面板使用了自定义的圆角、标题高度和内边距"));
    customExpander->contentWidget()->layout()->addWidget(new NLineEdit("输入框"));
    customExpander->contentWidget()->layout()->addWidget(new NSpinBox());

    customLayout->addWidget(customExpander);

    addSection("自定义样式", customLayout);

    QVBoxLayout* eventLayout = new QVBoxLayout();
    eventLayout->setSpacing(16);

    NExpander* eventExpander = new NExpander("展开/折叠事件", this);
    eventExpander->contentWidget()->layout()->addWidget(new NLabel("点击标题栏可以展开或折叠"));
    eventExpander->contentWidget()->layout()->addWidget(new NCheckBox("选项 1"));
    eventExpander->contentWidget()->layout()->addWidget(new NCheckBox("选项 2"));

    NLabel* statusLabel = new NLabel("状态: 已折叠", this);
    connect(eventExpander, &NExpander::expandedChanged, [statusLabel](bool expanded) {
        statusLabel->setText(expanded ? "状态: 已展开" : "状态: 已折叠");
    });

    eventLayout->addWidget(eventExpander);
    eventLayout->addWidget(statusLabel);

    addSection("展开/折叠事件", eventLayout);
}
