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
    NPushButton* basicBtn = new NPushButton("Show Menu");
    basicBtn->setFixedSize(120, 40);
    NMenu* basicMenu = new NMenu(basicBtn);
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
    iconBtn->setFixedSize(180, 40);
    iconBtn->setFluentIcon(NRegularIconType::Settings16Regular, 24);
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

    // 添加高级菜单示例
    QWidget*     advancedSection = new QWidget;
    QVBoxLayout* advancedLayout  = new QVBoxLayout(advancedSection);
    advancedLayout->setSpacing(16);

    QLabel* advancedTitle = new QLabel("Advanced Menus");
    advancedTitle->setFont(titleFont);
    advancedLayout->addWidget(advancedTitle);

    QHBoxLayout* advancedButtonLayout = new QHBoxLayout;
    advancedButtonLayout->setSpacing(16);

    // 1. 带快捷键的菜单
    NPushButton* shortcutBtn = new NPushButton("Menu with Shortcuts");
    shortcutBtn->setFixedSize(180, 40);

    shortcutBtn->setFluentIcon(NRegularIconType::Keyboard16Regular, 24);
    NMenu* shortcutMenu = new NMenu(shortcutBtn);

    shortcutMenu->addItem("New", NRegularIconType::Document16Regular, QKeySequence::New);
    shortcutMenu->addItem("Open", NRegularIconType::FolderOpen16Regular, QKeySequence::Open);
    shortcutMenu->addItem("Save", NRegularIconType::Save16Regular, QKeySequence::Save);
    shortcutMenu->addSeparator();
    shortcutMenu->addItem("Cut", NRegularIconType::Cut16Regular, QKeySequence::Cut);
    shortcutMenu->addItem("Copy", NRegularIconType::Copy16Regular, QKeySequence::Copy);
    shortcutMenu->addItem("Paste", NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);

    connect(shortcutBtn, &NPushButton::clicked, this, [shortcutBtn, shortcutMenu]() {
        QPoint pos = shortcutBtn->mapToGlobal(QPoint(0, shortcutBtn->height()));
        shortcutMenu->exec(pos);
    });

    // 2. 可勾选菜单
    NPushButton* checkableBtn = new NPushButton("Checkable Menu");
    checkableBtn->setFixedSize(180, 40);

    checkableBtn->setFluentIcon(NRegularIconType::CheckboxChecked16Regular, 24);
    NMenu* checkableMenu = new NMenu(checkableBtn);

    QAction* boldAction   = checkableMenu->addCheckableItem("Bold", NRegularIconType::TextBold16Regular, false);
    QAction* italicAction = checkableMenu->addCheckableItem("Italic", NRegularIconType::TextItalic16Regular, true);
    QAction* underlineAction =
        checkableMenu->addCheckableItem("Underline", NRegularIconType::TextUnderline16Regular, false);

    checkableMenu->addSeparator();

    // 添加子菜单示例
    NMenu*   viewMenu        = checkableMenu->addSubMenu("View Options", NRegularIconType::Eye16Regular);
    QAction* statusBarAction = viewMenu->addCheckableItem("Status Bar", true);
    QAction* toolbarAction   = viewMenu->addCheckableItem("Toolbar", true);
    QAction* sidebarAction   = viewMenu->addCheckableItem("Sidebar", false);

    // 设置工具提示
    checkableMenu->setItemToolTip(boldAction, "Make text bold (Ctrl+B)");
    checkableMenu->setItemToolTip(italicAction, "Make text italic (Ctrl+I)");
    checkableMenu->setItemToolTip(underlineAction, "Make text underlined (Ctrl+U)");

    // 连接信号
    // checkableMenu->connectItem(boldAction, [this]() { qDebug() << "Bold toggled"; });

    connect(checkableBtn, &NPushButton::clicked, this, [checkableBtn, checkableMenu]() {
        QPoint pos = checkableBtn->mapToGlobal(QPoint(0, checkableBtn->height()));
        checkableMenu->exec(pos);
    });

    // 3. 禁用项菜单示例
    NPushButton* disabledItemBtn = new NPushButton("Menu with Disabled Items");
    disabledItemBtn->setFixedSize(240, 40);

    disabledItemBtn->setFluentIcon(NRegularIconType::Prohibited16Regular, 24);
    NMenu* disabledItemMenu = new NMenu(disabledItemBtn);

    disabledItemMenu->addItem("Enabled Item 1");
    QAction* disabledAction = disabledItemMenu->addItem("Disabled Item", NRegularIconType::Warning16Regular);
    disabledItemMenu->setItemEnabled(disabledAction, false);
    disabledItemMenu->addItem("Enabled Item 2");

    connect(disabledItemBtn, &NPushButton::clicked, this, [disabledItemBtn, disabledItemMenu]() {
        QPoint pos = disabledItemBtn->mapToGlobal(QPoint(0, disabledItemBtn->height()));
        disabledItemMenu->exec(pos);
    });

    advancedButtonLayout->addWidget(shortcutBtn);
    advancedButtonLayout->addWidget(checkableBtn);
    advancedButtonLayout->addWidget(disabledItemBtn);
    advancedButtonLayout->addStretch();
    advancedLayout->addLayout(advancedButtonLayout);

    // 添加到主布局
    mainLayout->addWidget(basicSection);
    mainLayout->addWidget(advancedSection);

    // 添加下拉按钮示例区域
    QWidget*     dropdownSection = new QWidget;
    QVBoxLayout* dropdownLayout  = new QVBoxLayout(dropdownSection);
    dropdownLayout->setSpacing(16);

    QLabel* dropdownTitle = new QLabel("下拉按钮示例");
    dropdownTitle->setFont(titleFont);
    dropdownLayout->addWidget(dropdownTitle);

    QHBoxLayout* dropdownButtonLayout = new QHBoxLayout;
    dropdownButtonLayout->setSpacing(16);

    // 1. 基本下拉按钮
    NDropDownButton* basicDropdown = new NDropDownButton("基本下拉按钮");
    basicDropdown->setFixedSize(140, 40);

    NMenu* basicDropdownMenu = new NMenu(basicDropdown);
    basicDropdownMenu->addItem("选项 1");
    basicDropdownMenu->addItem("选项 2");
    basicDropdownMenu->addItem("选项 3");
    basicDropdownMenu->addSeparator();
    basicDropdownMenu->addItem("更多选项");

    basicDropdown->setMenu(basicDropdownMenu);

    // 2. 带图标的下拉按钮
    NDropDownButton* iconDropdown = new NDropDownButton();
    iconDropdown->setFixedSize(70, 40);
    iconDropdown->setFluentIcon(NRegularIconType::Settings16Regular, 24);

    NMenu* iconDropdownMenu = new NMenu(iconDropdown);
    iconDropdownMenu->addItem("新建", NRegularIconType::Document16Regular);
    iconDropdownMenu->addItem("打开", NRegularIconType::FolderOpen16Regular);
    iconDropdownMenu->addItem("保存", NRegularIconType::Save16Regular);
    iconDropdownMenu->addSeparator();
    iconDropdownMenu->addItem("设置", NRegularIconType::Settings16Regular);

    iconDropdown->setMenu(iconDropdownMenu);

    // 3. 带子菜单的下拉按钮
    NDropDownButton* submenuDropdown = new NDropDownButton("带子菜单的下拉按钮");
    submenuDropdown->setFixedSize(180, 40);

    NMenu* submenuDropdownMenu = new NMenu(submenuDropdown);
    submenuDropdownMenu->addItem("主选项 1");
    submenuDropdownMenu->addItem("主选项 2");

    NMenu* subMenu = submenuDropdownMenu->addSubMenu("子菜单", NRegularIconType::FolderOpen16Regular);
    subMenu->addItem("子选项 1");
    subMenu->addItem("子选项 2");
    subMenu->addItem("子选项 3");

    submenuDropdownMenu->addSeparator();
    submenuDropdownMenu->addCheckableItem("启用选项", true);

    submenuDropdown->setMenu(submenuDropdownMenu);

    // 4. 不显示箭头的下拉按钮
    NDropDownButton* noArrowDropdown = new NDropDownButton("无箭头下拉按钮");
    noArrowDropdown->setFixedSize(150, 40);
    noArrowDropdown->setShowArrow(false);

    NMenu* noArrowMenu = new NMenu(noArrowDropdown);
    noArrowMenu->addItem("项目 1");
    noArrowMenu->addItem("项目 2");
    noArrowMenu->addItem("项目 3");

    noArrowDropdown->setMenu(noArrowMenu);

    // 添加到布局
    dropdownButtonLayout->addWidget(basicDropdown);
    dropdownButtonLayout->addWidget(iconDropdown);
    dropdownButtonLayout->addWidget(submenuDropdown);
    dropdownButtonLayout->addWidget(noArrowDropdown);
    dropdownButtonLayout->addStretch();

    dropdownLayout->addLayout(dropdownButtonLayout);

    // 添加到主布局
    mainLayout->addWidget(dropdownSection);
    mainLayout->addStretch();
}