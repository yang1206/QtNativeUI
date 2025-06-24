#include "mainwindow.h"
#include <QtNativeUI/NNavigationView.h>
#include "components/button.h"
#include "components/colors.h"
#include "components/controls.h"
#include "components/dialogs.h"
#include "components/icon.h"
#include "components/menu.h"
#include "components/navigation.h"

MainWindow::MainWindow(QWidget* parent) : NMainWindow(parent) {
    m_navigationView = new NNavigationView(this);
    setCentralWidget(m_navigationView);
    auto button = new ButtonExample(this);
    button->setMainWindow(this);
    m_navigationView->addPageNode("Button", button, NRegularIconType::Button16Regular);
    m_navigationView->addPageNode("Controls", new ControlsExample(this), NRegularIconType::Accessibility16Regular);
    m_navigationView->addPageNode("Menus", new MenuExample(this), NRegularIconType::MoreHorizontal16Regular);
    m_navigationView->addPageNode("Dialogs", new DialogsExample(this), NRegularIconType::Flag16Regular);
    m_navigationView->addPageNode("Navigation", new NavigationExample(this), NRegularIconType::Router20Regular);
    m_navigationView->addPageNode("Colors", new ColorsExample(this), NRegularIconType::Color16Regular);
    m_navigationView->addPageNode("Icons", new IconExample(this), NRegularIconType::Icons20Regular);

    // 设置窗口属性
    setWindowTitle("QtNativeUI by Yang1206");
    resize(1200, 700);
}
