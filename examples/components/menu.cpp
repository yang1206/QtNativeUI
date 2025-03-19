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
    shortcutBtn->setFluentIcon(NRegularIconType::Keyboard16Regular);
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
    checkableBtn->setFluentIcon(NRegularIconType::CheckboxChecked16Regular);
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
    disabledItemBtn->setFluentIcon(NRegularIconType::Prohibited16Regular);
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
    mainLayout->addStretch();

    // 设置最小宽度
    setMinimumWidth(800);
}