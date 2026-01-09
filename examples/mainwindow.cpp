#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QStyle>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NMenuBar.h>
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

#ifdef Q_OS_WINDOWS
    setupTitleBarWidgets();
#endif
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
    auto menuBar = new NMenuBar(this);

    // File Menu
    auto fileMenu   = menuBar->addMenu("File");
    auto newAction  = fileMenu->addItem("New", NRegularIconType::DocumentAdd16Regular, QKeySequence::New);
    auto openAction = fileMenu->addItem("Open", NRegularIconType::FolderOpen16Regular, QKeySequence::Open);
    auto saveAction = fileMenu->addItem("Save", NRegularIconType::Save16Regular, QKeySequence::Save);
    fileMenu->addSeparator();
    auto exitAction = fileMenu->addItem("Exit", NRegularIconType::DismissCircle16Regular, QKeySequence::Quit);

    connect(newAction, &QAction::triggered, this, &MainWindow::onNewFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // View Menu
    auto viewMenu    = menuBar->addMenu("View");
    auto themeAction = viewMenu->addItem("Toggle Theme", NRegularIconType::WeatherMoon16Regular);
    connect(themeAction, &QAction::triggered, this, [this]() {
        nTheme->isDarkMode() ? nTheme->setThemeMode(NThemeType::ThemeMode::Light)
                             : nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
    });

    viewMenu->addSeparator();
    auto backdropMenu = viewMenu->addSubMenu("Backdrop Effect", NRegularIconType::Window16Regular);
    auto noneAction   = backdropMenu->addItem("None");
    auto blurAction   = backdropMenu->addItem("Blur");
#ifdef Q_OS_WIN
    auto acrylicAction = backdropMenu->addItem("Acrylic");
    auto micaAction    = backdropMenu->addItem("Mica");
    auto micaAltAction = backdropMenu->addItem("Mica Alt");
#endif

    connect(noneAction, &QAction::triggered, [this]() { setBackdropType(None); });
    connect(blurAction, &QAction::triggered, [this]() { setBackdropType(Blur); });
#ifdef Q_OS_WIN
    connect(acrylicAction, &QAction::triggered, [this]() { setBackdropType(Acrylic); });
    connect(micaAction, &QAction::triggered, [this]() { setBackdropType(Mica); });
    connect(micaAltAction, &QAction::triggered, [this]() { setBackdropType(MicaAlt); });
#endif

    // Window Menu
    auto windowMenu = menuBar->addMenu("Window");
    auto pinAction  = windowMenu->addCheckableItem("Stay on Top", NRegularIconType::Pin16Regular, false);
    connect(pinAction, &QAction::triggered, this, [this](bool checked) {
        if (auto pinBtn = systemButton(Pin)) {
            pinBtn->click();
        }
    });

    windowMenu->addSeparator();
    auto minimizeAction = windowMenu->addItem("Minimize", NRegularIconType::Subtract16Regular, QKeySequence("Ctrl+M"));
    auto maximizeAction = windowMenu->addItem("Maximize", NRegularIconType::Maximize16Regular);

    connect(minimizeAction, &QAction::triggered, this, &QWidget::showMinimized);
    connect(maximizeAction, &QAction::triggered, this, [this]() {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });

    // Help Menu
    auto helpMenu      = menuBar->addMenu("Help");
    auto aboutAction   = helpMenu->addItem("About", NRegularIconType::Info16Regular);
    auto aboutQtAction = helpMenu->addItem("About Qt");

    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

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

void MainWindow::setupTitleBarWidgets() {
    // Add a search box to the title bar
    auto searchBox = new NLineEdit(this);
    searchBox->setPlaceholderText("Search...");

    setHitTestVisible(searchBox, true);

    // Add the search box to the window bar (between menu and system buttons)
    windowBar()->addWidget(searchBox);

    // Connect search functionality
    connect(searchBox, &NLineEdit::textChanged, this, [](const QString& text) {
        // Handle search logic here
        qDebug() << "Search:" << text;
    });
}
