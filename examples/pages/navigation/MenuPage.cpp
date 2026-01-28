#include "MenuPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NMenu.h>
#include <QtNativeUI/NPushButton.h>

MenuPage::MenuPage(QWidget* parent)
    : BasePage("Menu 菜单", "Menu 提供上下文菜单和下拉菜单功能，支持图标、快捷键和子菜单。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NPushButton* basicMenuButton = new NPushButton("基本菜单", this);
    basicMenuButton->setFixedSize(120, 40);

    NMenu* basicMenu = new NMenu(this);
    basicMenu->addItem("新建");
    basicMenu->addItem("打开");
    basicMenu->addItem("保存");
    basicMenu->addSeparator();
    basicMenu->addItem("退出");

    connect(basicMenuButton, &NPushButton::clicked, [basicMenu, basicMenuButton]() {
        basicMenu->exec(basicMenuButton->mapToGlobal(QPoint(0, basicMenuButton->height())));
    });

    basicLayout->addWidget(basicMenuButton);
    basicLayout->addStretch();

    addSection("基本菜单", basicLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);

    NPushButton* iconMenuButton = new NPushButton("带图标菜单", this);
    iconMenuButton->setFixedSize(120, 40);

    NMenu* iconMenu = new NMenu(this);
    iconMenu->addItem("新建", NRegularIconType::DocumentAdd16Regular);
    iconMenu->addItem("打开", NRegularIconType::FolderOpen16Regular);
    iconMenu->addItem("保存", NRegularIconType::Save16Regular);
    iconMenu->addSeparator();
    iconMenu->addItem("退出", NRegularIconType::DismissCircle16Regular);

    connect(iconMenuButton, &NPushButton::clicked, [iconMenu, iconMenuButton]() {
        iconMenu->exec(iconMenuButton->mapToGlobal(QPoint(0, iconMenuButton->height())));
    });

    iconLayout->addWidget(iconMenuButton);
    iconLayout->addStretch();

    addSection("带图标的菜单", iconLayout);

    QHBoxLayout* shortcutLayout = new QHBoxLayout();
    shortcutLayout->setSpacing(16);

    NPushButton* shortcutMenuButton = new NPushButton("带快捷键菜单", this);
    shortcutMenuButton->setFixedSize(140, 40);

    NMenu* shortcutMenu = new NMenu(this);
    shortcutMenu->addItem("新建", NRegularIconType::DocumentAdd16Regular, QKeySequence::New);
    shortcutMenu->addItem("打开", NRegularIconType::FolderOpen16Regular, QKeySequence::Open);
    shortcutMenu->addItem("保存", NRegularIconType::Save16Regular, QKeySequence::Save);
    shortcutMenu->addSeparator();
    shortcutMenu->addItem("退出", NRegularIconType::DismissCircle16Regular, QKeySequence::Quit);

    connect(shortcutMenuButton, &NPushButton::clicked, [shortcutMenu, shortcutMenuButton]() {
        shortcutMenu->exec(shortcutMenuButton->mapToGlobal(QPoint(0, shortcutMenuButton->height())));
    });

    shortcutLayout->addWidget(shortcutMenuButton);
    shortcutLayout->addStretch();

    addSection("带快捷键的菜单", shortcutLayout);

    QHBoxLayout* checkableLayout = new QHBoxLayout();
    checkableLayout->setSpacing(16);

    NPushButton* checkableMenuButton = new NPushButton("可选中菜单", this);
    checkableMenuButton->setFixedSize(120, 40);

    NMenu* checkableMenu = new NMenu(this);
    checkableMenu->addCheckableItem("选项 1", true);
    checkableMenu->addCheckableItem("选项 2", false);
    checkableMenu->addCheckableItem("选项 3", false);

    connect(checkableMenuButton, &NPushButton::clicked, [checkableMenu, checkableMenuButton]() {
        checkableMenu->exec(checkableMenuButton->mapToGlobal(QPoint(0, checkableMenuButton->height())));
    });

    checkableLayout->addWidget(checkableMenuButton);
    checkableLayout->addStretch();

    addSection("可选中的菜单项", checkableLayout);

    QHBoxLayout* submenuLayout = new QHBoxLayout();
    submenuLayout->setSpacing(16);

    NPushButton* submenuButton = new NPushButton("子菜单", this);
    submenuButton->setFixedSize(120, 40);

    NMenu* submenu = new NMenu(this);
    submenu->addItem("新建文件", NRegularIconType::DocumentAdd16Regular);
    
    NMenu* newSubmenu = submenu->addSubMenu("新建", NRegularIconType::Add16Regular);
    newSubmenu->addItem("文本文件");
    newSubmenu->addItem("文件夹");
    newSubmenu->addItem("项目");

    submenu->addItem("打开", NRegularIconType::FolderOpen16Regular);
    submenu->addSeparator();
    submenu->addItem("退出", NRegularIconType::DismissCircle16Regular);

    connect(submenuButton, &NPushButton::clicked, [submenu, submenuButton]() {
        submenu->exec(submenuButton->mapToGlobal(QPoint(0, submenuButton->height())));
    });

    submenuLayout->addWidget(submenuButton);
    submenuLayout->addStretch();

    addSection("带子菜单", submenuLayout);
}
