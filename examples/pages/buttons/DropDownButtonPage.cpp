#include "DropDownButtonPage.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QtNativeUI/NDropDownButton.h>
#include <QtNativeUI/NMenu.h>

DropDownButtonPage::DropDownButtonPage(QWidget* parent)
    : BasePage("DropDownButton 下拉按钮", "DropDownButton 提供下拉菜单功能，可以包含多个操作。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    
    NDropDownButton* basicButton = new NDropDownButton("操作", this);
    NMenu* basicMenu = new NMenu(this);
    auto action1 = basicMenu->addItem("选项 1");
    auto action2 = basicMenu->addItem("选项 2");
    auto action3 = basicMenu->addItem("选项 3");
    basicButton->setMenu(basicMenu);
    
    connect(action1, &QAction::triggered, []() { qDebug() << "选项 1"; });
    connect(action2, &QAction::triggered, []() { qDebug() << "选项 2"; });
    connect(action3, &QAction::triggered, []() { qDebug() << "选项 3"; });
    
    basicLayout->addWidget(basicButton);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);
    
    NDropDownButton* iconButton = new NDropDownButton("文件", this);
    iconButton->setFluentIcon(NRegularIconType::Document16Regular);
    
    NMenu* fileMenu = new NMenu(this);
    auto newAction = fileMenu->addItem("新建", NRegularIconType::DocumentAdd16Regular);
    auto openAction = fileMenu->addItem("打开", NRegularIconType::FolderOpen16Regular);
    auto saveAction = fileMenu->addItem("保存", NRegularIconType::Save16Regular);
    iconButton->setMenu(fileMenu);
    
    connect(newAction, &QAction::triggered, []() { qDebug() << "新建"; });
    connect(openAction, &QAction::triggered, []() { qDebug() << "打开"; });
    connect(saveAction, &QAction::triggered, []() { qDebug() << "保存"; });
    
    iconLayout->addWidget(iconButton);
    iconLayout->addStretch();
    addSection("带图标", iconLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    
    NDropDownButton* disabledButton = new NDropDownButton("禁用按钮", this);
    NMenu* disabledMenu = new NMenu(this);
    disabledMenu->addItem("选项 A");
    disabledMenu->addItem("选项 B");
    disabledButton->setMenu(disabledMenu);
    disabledButton->setEnabled(false);
    
    stateLayout->addWidget(disabledButton);
    stateLayout->addStretch();
    addSection("禁用状态", stateLayout);
}
