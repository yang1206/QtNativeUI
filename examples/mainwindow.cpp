#include "mainwindow.h"
#include <QVBoxLayout>
#include <QtNativeUI/NTabWidget.h>
#include "components/button.h"
#include "components/colors.h"
#include "components/controls.h"
#include "components/dialogs.h"
#include "components/icon.h"
#include "components/menu.h"
#include "components/navigation.h"

MainWindow::MainWindow(QWidget* parent) : NMainWindow(parent) {
    m_tabWidget = new NTabWidget(this);
    setCentralWidget(m_tabWidget);
    m_tabWidget->setMovable(true);
    auto btn = new ButtonExample(this);
    btn->setMainWindow(this);
    m_tabWidget->addTab(btn, "Button");

    auto menu = new MenuExample(this);
    m_tabWidget->addTab(menu, "Menu");

    auto dialogs = new DialogsExample(this);
    m_tabWidget->addTab(dialogs, "Dialogs");

    auto controls = new ControlsExample(this);
    m_tabWidget->addTab(controls, "Controls");

    auto navigation = new NavigationExample(this);
    m_tabWidget->addTab(navigation, "Navigation");

    // 添加颜色展示页面
    auto colors = new ColorsExample(this);
    m_tabWidget->addTab(colors, "Colors");

    // 添加图标展示页面
    auto icons = new IconExample(this);
    m_tabWidget->addTab(icons, "Icons");

    // 设置窗口属性
    setWindowTitle("QtNativeUI by Yang1206");
    resize(900, 700);
}
