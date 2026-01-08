#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QMenuBar>
#include <QStyle>
#include <QtNativeUI/NNavigationView.h>
#include <QtNativeUI/NWindowBar.h>
#include "components/button.h"
#include "components/colors.h"
#include "components/controls.h"
#include "components/dialogs.h"
#include "components/icon.h"
#include "components/menu.h"
#include "components/navigation.h"
#include "components/view.h"

MainWindow::MainWindow(QWidget* parent) : NMainWindow(parent) {
    setupMenuBar();

    m_navigationView = new NNavigationView(this);
    setCentralWidget(m_navigationView);

    auto button = new ButtonExample(this);
    button->setMainWindow(this);
    m_navigationView->addPageNode("Button", button, NRegularIconType::Button16Regular);
    m_navigationView->addPageNode("Controls", new ControlsExample(this), NRegularIconType::Accessibility16Regular);
    m_navigationView->addPageNode("Menus", new MenuExample(this), NRegularIconType::MoreHorizontal16Regular);
    m_navigationView->addPageNode("Dialogs", new DialogsExample(this), NRegularIconType::Flag16Regular);
    m_navigationView->addPageNode("Navigation", new NavigationExample(this), NRegularIconType::Router20Regular);
    m_navigationView->addPageNode("Views", new ViewExample(this), NRegularIconType::List16Regular);
    m_navigationView->addPageNode("Colors", new ColorsExample(this), NRegularIconType::Color16Regular);
    m_navigationView->addPageNode("Icons", new IconExample(this), NRegularIconType::Icons20Regular);

    setWindowTitle("QtNativeUI by Yang1206");
    setWindowIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    resize(1200, 700);

    setBackdropType(MicaAlt);
}

void MainWindow::setupMenuBar() {
    auto menuBar = new QMenuBar(this);
    menuBar->setNativeMenuBar(true);

    // File Menu
    auto fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&New", this, &MainWindow::onNewFile, QKeySequence::New);
    fileMenu->addAction("&Open", this, &MainWindow::onOpenFile, QKeySequence::Open);
    fileMenu->addAction("&Save", this, &MainWindow::onSaveFile, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence::Quit);

    // View Menu
    auto viewMenu    = menuBar->addMenu("&View");
    auto themeAction = viewMenu->addAction("Toggle &Theme");
    connect(themeAction, &QAction::triggered, this, [this]() {
        // Trigger theme button
        if (auto themeBtn = systemButton(Theme)) {
            themeBtn->click();
        }
    });

    viewMenu->addSeparator();
    auto backdropMenu  = viewMenu->addMenu("&Backdrop Effect");
    auto noneAction    = backdropMenu->addAction("&None");
    auto blurAction    = backdropMenu->addAction("&Blur");
    auto acrylicAction = backdropMenu->addAction("&Acrylic");
    auto micaAction    = backdropMenu->addAction("&Mica");
    auto micaAltAction = backdropMenu->addAction("Mica &Alt");

    connect(noneAction, &QAction::triggered, [this]() { setBackdropType(None); });
    connect(blurAction, &QAction::triggered, [this]() { setBackdropType(Blur); });
    connect(acrylicAction, &QAction::triggered, [this]() { setBackdropType(Acrylic); });
    connect(micaAction, &QAction::triggered, [this]() { setBackdropType(Mica); });
    connect(micaAltAction, &QAction::triggered, [this]() { setBackdropType(MicaAlt); });

    // Window Menu
    auto windowMenu = menuBar->addMenu("&Window");
    auto pinAction  = windowMenu->addAction("Stay on &Top");
    pinAction->setCheckable(true);
    connect(pinAction, &QAction::triggered, this, [this](bool checked) {
        if (auto pinBtn = systemButton(Pin)) {
            pinBtn->click();
        }
    });

    windowMenu->addSeparator();
    windowMenu->addAction("&Minimize", this, &QWidget::showMinimized, QKeySequence("Ctrl+M"));
    windowMenu->addAction("&Maximize", this, [this]() {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });

    // Help Menu
    auto helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("&About", this, &MainWindow::onAbout);
    helpMenu->addAction("About &Qt", qApp, &QApplication::aboutQt);

    setMenuBar(menuBar);
}

void MainWindow::onNewFile() {
    // Placeholder for new file functionality
}

void MainWindow::onOpenFile() {
    // Placeholder for open file functionality
}

void MainWindow::onSaveFile() {
    // Placeholder for save file functionality
}

void MainWindow::onAbout() {
    // Placeholder for about dialog
}
