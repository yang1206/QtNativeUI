#include "menu.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QtNativeUI/NMenu.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NDropDownButton.h>
#include <QtNativeUI/NIcon.h>

MenuExample::MenuExample(QWidget* parent) : QWidget(parent) {
    initUI();
}

void MenuExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(24);

    // 标题
    QLabel* title = new QLabel("Menus & DropDownButtons");
    QFont titleFont = title->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    mainLayout->addWidget(title);

    // 基本菜单示例
    QWidget* basicSection = new QWidget;
    QVBoxLayout* basicLayout = new QVBoxLayout(basicSection);
    basicLayout->setSpacing(16);

    QLabel* basicTitle = new QLabel("Basic Menus");
    basicTitle->setFont(titleFont);
    basicLayout->addWidget(basicTitle);

    QHBoxLayout* basicButtonLayout = new QHBoxLayout;
    basicButtonLayout->setSpacing(16);

    // 1. 简单菜单按钮
    NPushButton* basicBtn = new NPushButton("Show Menu");
    NMenu* basicMenu = new NMenu(basicBtn);
    basicMenu->addAction("New File");
    basicMenu->addAction("Open...");
    basicMenu->addAction("Save");
    basicMenu->addSeparator();
    basicMenu->addAction("Exit");

    connect(basicBtn, &NPushButton::clicked, this, [basicBtn, basicMenu]() {
        QPoint pos = basicBtn->mapToGlobal(QPoint(0, basicBtn->height()));
        basicMenu->exec(pos);
    });

    // 2. 带图标的菜单
    NPushButton* iconBtn = new NPushButton("Menu with Icons");
    iconBtn->setFluentIcon(NRegularIconType::Settings16Regular);
    NMenu* iconMenu = new NMenu(iconBtn);
    
    QAction* newAction = iconMenu->addAction("New");
    newAction->setIcon(nIcon->fromRegular(NRegularIconType::Document16Regular));
    
    QAction* openAction = iconMenu->addAction("Open");
    openAction->setIcon(nIcon->fromRegular(NRegularIconType::FolderOpen16Regular));
    
    QAction* saveAction = iconMenu->addAction("Save");
    saveAction->setIcon(nIcon->fromRegular(NRegularIconType::Save16Regular));
    
    iconMenu->addSeparator();
    
    QAction* settingsAction = iconMenu->addAction("Settings");
    settingsAction->setIcon(nIcon->fromRegular(NRegularIconType::Settings16Regular));

    connect(iconBtn, &NPushButton::clicked, this, [iconBtn, iconMenu]() {
        QPoint pos = iconBtn->mapToGlobal(QPoint(0, iconBtn->height()));
        iconMenu->exec(pos);
    });

    basicButtonLayout->addWidget(basicBtn);
    basicButtonLayout->addWidget(iconBtn);
    basicButtonLayout->addStretch();
    basicLayout->addLayout(basicButtonLayout);

    // DropDownButton 示例
    QWidget* dropDownSection = new QWidget;
    QVBoxLayout* dropDownLayout = new QVBoxLayout(dropDownSection);
    dropDownLayout->setSpacing(16);

    QLabel* dropDownTitle = new QLabel("DropDown Buttons");
    dropDownTitle->setFont(titleFont);
    dropDownLayout->addWidget(dropDownTitle);

    QHBoxLayout* dropDownButtonLayout = new QHBoxLayout;
    dropDownButtonLayout->setSpacing(16);

    // 1. 基本下拉按钮
    NDropDownButton* basicDropDown = new NDropDownButton("Basic Dropdown");
    NMenu* dropMenu = new NMenu(basicDropDown);
    dropMenu->addAction("Option 1");
    dropMenu->addAction("Option 2");
    dropMenu->addAction("Option 3");
    basicDropDown->setMenu(dropMenu);

    // 2. 带图标的下拉按钮
    NDropDownButton* iconDropDown = new NDropDownButton("Settings");
    iconDropDown->setFluentIcon(NRegularIconType::Settings16Regular);
    NMenu* settingsMenu = new NMenu(iconDropDown);
    
    QAction* generalAction = settingsMenu->addAction("General");
    generalAction->setIcon(nIcon->fromRegular(NRegularIconType::Home16Regular));
    
    QAction* accountAction = settingsMenu->addAction("Account");
    accountAction->setIcon(nIcon->fromRegular(NRegularIconType::Person16Regular));
    
    settingsMenu->addSeparator();
    
    QAction* helpAction = settingsMenu->addAction("Help");
    helpAction->setIcon(nIcon->fromRegular(NRegularIconType::QuestionCircle16Regular));
    
    iconDropDown->setMenu(settingsMenu);

    // 3. 强调色下拉按钮
    NDropDownButton* accentDropDown = new NDropDownButton("Create New");
    accentDropDown->setButtonType(NPushButton::Accent);
    NMenu* createMenu = new NMenu(accentDropDown);
    createMenu->addAction("New File");
    createMenu->addAction("New Folder");
    createMenu->addAction("New Project");
    accentDropDown->setMenu(createMenu);

    dropDownButtonLayout->addWidget(basicDropDown);
    dropDownButtonLayout->addWidget(iconDropDown);
    dropDownButtonLayout->addWidget(accentDropDown);
    dropDownButtonLayout->addStretch();

    dropDownLayout->addLayout(dropDownButtonLayout);

    // 添加到主布局
    mainLayout->addWidget(basicSection);
    mainLayout->addWidget(dropDownSection);
    mainLayout->addStretch();

    // 设置最小宽度
    setMinimumWidth(600);
} 