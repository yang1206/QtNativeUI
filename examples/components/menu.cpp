#include "menu.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NDropDownButton.h>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NMenu.h>
#include <QtNativeUI/NPushButton.h>

MenuExample::MenuExample(QWidget* parent) : QWidget(parent) { initUI(); }

void MenuExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(24);

    // 标题
    QLabel* title     = new QLabel("Menus & DropDownButtons");
    QFont   titleFont = title->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    mainLayout->addWidget(title);

    // 基本菜单示例
    QWidget*     basicSection = new QWidget;
    QVBoxLayout* basicLayout  = new QVBoxLayout(basicSection);
    basicLayout->setSpacing(16);

    QLabel* basicTitle = new QLabel("Basic Menus");
    basicTitle->setFont(titleFont);
    basicLayout->addWidget(basicTitle);

    QHBoxLayout* basicButtonLayout = new QHBoxLayout;
    basicButtonLayout->setSpacing(16);

    // 1. 简单菜单按钮
    NPushButton* basicBtn  = new NPushButton("Show Menu");
    NMenu*       basicMenu = new NMenu(basicBtn);
    basicMenu->addItem("New File");
    basicMenu->addItem("Open...");
    basicMenu->addItem("Save");
    basicMenu->addSeparator();
    basicMenu->addItem("Exit");

    connect(basicBtn, &NPushButton::clicked, this, [basicBtn, basicMenu]() {
        QPoint pos = basicBtn->mapToGlobal(QPoint(0, basicBtn->height()));
        basicMenu->exec(pos);
    });

    // 2. 带图标的菜单
    NPushButton* iconBtn = new NPushButton("Menu with Icons");
    iconBtn->setFluentIcon(NRegularIconType::Settings16Regular);
    NMenu* iconMenu = new NMenu(iconBtn);

    iconMenu->addItem("New", NRegularIconType::Document16Regular);
    iconMenu->addItem("Open", NRegularIconType::FolderOpen16Regular);
    iconMenu->addItem("Save", NRegularIconType::Save16Regular);
    iconMenu->addSeparator();
    iconMenu->addItem("Settings", NRegularIconType::Settings16Regular);

    connect(iconBtn, &NPushButton::clicked, this, [iconBtn, iconMenu]() {
        QPoint pos = iconBtn->mapToGlobal(QPoint(0, iconBtn->height()));
        iconMenu->exec(pos);
    });

    // 子菜单示例
    NMenu* settingsMenu = iconMenu->addSubMenu("More Settings", NRegularIconType::Settings16Regular);
    settingsMenu->addItem("General", NRegularIconType::Home16Regular);
    settingsMenu->addItem("Account", NRegularIconType::Person16Regular);
    settingsMenu->addSeparator();
    settingsMenu->addItem("Help", NRegularIconType::QuestionCircle16Regular);

    basicButtonLayout->addWidget(basicBtn);
    basicButtonLayout->addWidget(iconBtn);
    basicButtonLayout->addStretch();
    basicLayout->addLayout(basicButtonLayout);

    // 添加到主布局
    mainLayout->addWidget(basicSection);
    mainLayout->addStretch();

    // 设置最小宽度
    setMinimumWidth(600);
}