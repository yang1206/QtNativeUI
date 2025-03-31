#include "mainwindow.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "components/button.h"
#include "components/checkbox.h"
#include "components/colors.h"
#include "components/icon.h"
#include "components/lineedit.h"
#include "components/menu.h"
#include "components/toggleswitch.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    auto btn = new ButtonExample(this);
    m_tabWidget->addTab(btn, "Button");

    auto menu = new MenuExample(this);
    m_tabWidget->addTab(menu, "Menu");

    auto toggleSwitch = new ToggleSwitchExample(this);
    m_tabWidget->addTab(toggleSwitch, "ToggleSwitch");

    auto checkBox = new CheckBoxExample(this);
    m_tabWidget->addTab(checkBox, "CheckBox");

    // 添加颜色展示页面
    auto colors = new ColorsExample(this);
    m_tabWidget->addTab(colors, "Colors");

    // 添加图标展示页面
    auto icons = new IconExample(this);
    m_tabWidget->addTab(icons, "Icons");

    // 添加输入框展示页面
    auto lineEdit = new LineEditExample(this);
    m_tabWidget->addTab(lineEdit, "LineEdit");

    // 设置窗口属性
    setWindowTitle("QtNativeUI by Yang1206");
    resize(900, 700);
}