#include "GroupBoxPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NGroupBox.h>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NSpinBox.h>
#include <QtNativeUI/NToggleSwitch.h>

GroupBoxPage::GroupBoxPage(QWidget* parent)
    : BasePage("GroupBox 分组框", "GroupBox 用于将相关控件分组显示，支持多种样式和可折叠功能。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NGroupBox* basicGroupBox = new NGroupBox("基本信息", this);
    basicGroupBox->setMinimumWidth(400);

    QVBoxLayout* basicContentLayout = new QVBoxLayout(basicGroupBox);
    basicContentLayout->addWidget(new NLineEdit("姓名"));
    basicContentLayout->addWidget(new NLineEdit("邮箱"));
    basicContentLayout->addWidget(new NLineEdit("电话"));

    basicLayout->addWidget(basicGroupBox);

    addSection("基本分组框", basicLayout);

    QHBoxLayout* styleLayout = new QHBoxLayout();
    styleLayout->setSpacing(16);

    NGroupBox* standardGroupBox = new NGroupBox("标准样式", this);
    standardGroupBox->setGroupBoxStyle(NGroupBox::Standard);
    standardGroupBox->setMinimumWidth(180);
    QVBoxLayout* standardLayout = new QVBoxLayout(standardGroupBox);
    standardLayout->addWidget(new NCheckBox("选项 1"));
    standardLayout->addWidget(new NCheckBox("选项 2"));

    NGroupBox* cardGroupBox = new NGroupBox("卡片样式", this);
    cardGroupBox->setGroupBoxStyle(NGroupBox::Card);
    cardGroupBox->setMinimumWidth(180);
    QVBoxLayout* cardLayout = new QVBoxLayout(cardGroupBox);
    cardLayout->addWidget(new NCheckBox("选项 A"));
    cardLayout->addWidget(new NCheckBox("选项 B"));

    NGroupBox* outlinedGroupBox = new NGroupBox("轮廓样式", this);
    outlinedGroupBox->setGroupBoxStyle(NGroupBox::Outlined);
    outlinedGroupBox->setMinimumWidth(180);
    QVBoxLayout* outlinedLayout = new QVBoxLayout(outlinedGroupBox);
    outlinedLayout->addWidget(new NCheckBox("选项 X"));
    outlinedLayout->addWidget(new NCheckBox("选项 Y"));

    styleLayout->addWidget(standardGroupBox);
    styleLayout->addWidget(cardGroupBox);
    styleLayout->addWidget(outlinedGroupBox);
    styleLayout->addStretch();

    addSection("不同样式", styleLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);

    NGroupBox* settingsGroupBox = new NGroupBox("设置", this);
    settingsGroupBox->setTitleIcon(NRegularIconType::Settings24Regular, 20);
    settingsGroupBox->setMinimumWidth(200);
    QVBoxLayout* settingsLayout = new QVBoxLayout(settingsGroupBox);
    settingsLayout->addWidget(new NToggleSwitch("启用通知"));
    settingsLayout->addWidget(new NToggleSwitch("自动更新"));

    NGroupBox* securityGroupBox = new NGroupBox("安全", this);
    securityGroupBox->setTitleIcon(NFilledIconType::Shield24Filled, 20);
    securityGroupBox->setMinimumWidth(200);
    QVBoxLayout* securityLayout = new QVBoxLayout(securityGroupBox);
    securityLayout->addWidget(new NCheckBox("启用防火墙"));
    securityLayout->addWidget(new NCheckBox("自动扫描"));

    iconLayout->addWidget(settingsGroupBox);
    iconLayout->addWidget(securityGroupBox);
    iconLayout->addStretch();

    addSection("带图标的分组框", iconLayout);

    QVBoxLayout* collapsibleLayout = new QVBoxLayout();
    collapsibleLayout->setSpacing(16);

    NGroupBox* collapsibleGroupBox = new NGroupBox("高级设置", this);
    collapsibleGroupBox->setTitleIcon(NRegularIconType::Options24Regular, 18);
    collapsibleGroupBox->setCollapsible(true);
    collapsibleGroupBox->setMinimumWidth(400);

    QVBoxLayout* collapsibleContentLayout = new QVBoxLayout(collapsibleGroupBox);
    collapsibleContentLayout->addWidget(new NLineEdit("服务器地址"));
    collapsibleContentLayout->addWidget(new NSpinBox());
    collapsibleContentLayout->addWidget(new NCheckBox("启用SSL"));
    collapsibleContentLayout->addWidget(new NCheckBox("启用压缩"));

    QLabel* statusLabel = new QLabel("状态: 展开", this);
    connect(collapsibleGroupBox, &NGroupBox::collapsedChanged, [statusLabel](bool collapsed) {
        statusLabel->setText(collapsed ? "状态: 折叠" : "状态: 展开");
    });

    collapsibleLayout->addWidget(collapsibleGroupBox);
    collapsibleLayout->addWidget(statusLabel);

    addSection("可折叠的分组框", collapsibleLayout);

    QVBoxLayout* nestedLayout = new QVBoxLayout();
    nestedLayout->setSpacing(16);

    NGroupBox* parentGroupBox = new NGroupBox("用户配置", this);
    parentGroupBox->setMinimumWidth(450);

    QVBoxLayout* parentLayout = new QVBoxLayout(parentGroupBox);

    NGroupBox* personalGroupBox = new NGroupBox("个人信息", parentGroupBox);
    personalGroupBox->setGroupBoxStyle(NGroupBox::Outlined);
    personalGroupBox->setTitleIcon(NRegularIconType::Person24Regular, 16);
    QVBoxLayout* personalLayout = new QVBoxLayout(personalGroupBox);
    personalLayout->addWidget(new NLineEdit("用户名"));
    personalLayout->addWidget(new NLineEdit("显示名称"));
    parentLayout->addWidget(personalGroupBox);

    NGroupBox* permissionGroupBox = new NGroupBox("权限设置", parentGroupBox);
    permissionGroupBox->setGroupBoxStyle(NGroupBox::Outlined);
    permissionGroupBox->setTitleIcon(NRegularIconType::Key24Regular, 16);
    permissionGroupBox->setCollapsible(true);
    QVBoxLayout* permissionLayout = new QVBoxLayout(permissionGroupBox);
    permissionLayout->addWidget(new NCheckBox("管理员权限"));
    permissionLayout->addWidget(new NCheckBox("读取权限"));
    permissionLayout->addWidget(new NCheckBox("写入权限"));
    parentLayout->addWidget(permissionGroupBox);

    nestedLayout->addWidget(parentGroupBox);

    addSection("嵌套分组框", nestedLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);

    NGroupBox* normalGroupBox = new NGroupBox("正常状态", this);
    normalGroupBox->setMinimumWidth(150);
    QVBoxLayout* normalLayout = new QVBoxLayout(normalGroupBox);
    normalLayout->addWidget(new NPushButton("按钮"));

    NGroupBox* disabledGroupBox = new NGroupBox("禁用状态", this);
    disabledGroupBox->setEnabled(false);
    disabledGroupBox->setMinimumWidth(150);
    QVBoxLayout* disabledLayout = new QVBoxLayout(disabledGroupBox);
    disabledLayout->addWidget(new NPushButton("按钮"));

    NGroupBox* noBorderGroupBox = new NGroupBox("无边框", this);
    noBorderGroupBox->setShowBorder(false);
    noBorderGroupBox->setMinimumWidth(150);
    QVBoxLayout* noBorderLayout = new QVBoxLayout(noBorderGroupBox);
    noBorderLayout->addWidget(new NPushButton("按钮"));

    stateLayout->addWidget(normalGroupBox);
    stateLayout->addWidget(disabledGroupBox);
    stateLayout->addWidget(noBorderGroupBox);
    stateLayout->addStretch();

    addSection("不同状态", stateLayout);

    QHBoxLayout* customLayout = new QHBoxLayout();
    customLayout->setSpacing(16);

    NGroupBox* customMarginGroupBox = new NGroupBox("大边距", this);
    customMarginGroupBox->setContentMargin(20);
    customMarginGroupBox->setTitleHeight(40);
    customMarginGroupBox->setMinimumWidth(200);
    QVBoxLayout* customMarginLayout = new QVBoxLayout(customMarginGroupBox);
    customMarginLayout->addWidget(new NCheckBox("选项 1"));
    customMarginLayout->addWidget(new NCheckBox("选项 2"));

    NGroupBox* customIconGroupBox = new NGroupBox("自定义折叠图标", this);
    customIconGroupBox->setCollapsible(true);
    customIconGroupBox->setExpandedIcon(NRegularIconType::ChevronUp16Regular);
    customIconGroupBox->setCollapsedIcon(NRegularIconType::Add16Regular);
    customIconGroupBox->setCollapseIndicatorSize(18);
    customIconGroupBox->setMinimumWidth(200);
    QVBoxLayout* customIconLayout = new QVBoxLayout(customIconGroupBox);
    customIconLayout->addWidget(new NLineEdit("内容"));
    customIconLayout->addWidget(new NCheckBox("选项"));

    customLayout->addWidget(customMarginGroupBox);
    customLayout->addWidget(customIconGroupBox);
    customLayout->addStretch();

    addSection("自定义边距和折叠图标", customLayout);
}
