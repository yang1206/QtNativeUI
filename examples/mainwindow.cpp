#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QStyle>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NMenuBar.h>
#include <QtNativeUI/NNavigationView.h>
#include <QtNativeUI/NWindowBar.h>
#include "pages/basic/DropDownButtonPage.h"
#include "pages/basic/HyperlinkButtonPage.h"
#include "pages/basic/LabelPage.h"
#include "pages/basic/PushButtonPage.h"
#include "pages/basic/ToggleButtonPage.h"
#include "pages/basic/ToolButtonPage.h"
#include "pages/container/ExpanderPage.h"
#include "pages/container/GroupBoxPage.h"
#include "pages/container/OverlayPage.h"
#include "pages/container/ScrollAreaPage.h"
#include "pages/container/StackedWidgetPage.h"
#include "pages/datetime/CalendarDatePickerPage.h"
#include "pages/datetime/CalendarWidgetPage.h"
#include "pages/datetime/DatePickerPage.h"
#include "pages/datetime/TimePickerPage.h"
#include "pages/feedback/ContentDialogPage.h"
#include "pages/feedback/FlyoutPage.h"
#include "pages/feedback/InfoBarPage.h"
#include "pages/feedback/ProgressBarPage.h"
#include "pages/feedback/ProgressRingPage.h"
#include "pages/feedback/ToolTipPage.h"
#include "pages/input/AutoSuggestBoxPage.h"
#include "pages/input/CheckBoxPage.h"
#include "pages/input/ComboBoxPage.h"
#include "pages/input/DoubleSpinBoxPage.h"
#include "pages/input/LineEditPage.h"
#include "pages/input/PickerPage.h"
#include "pages/input/PlainTextEditPage.h"
#include "pages/input/RadioButtonPage.h"
#include "pages/input/SliderPage.h"
#include "pages/input/SpinBoxPage.h"
#include "pages/input/TextEditPage.h"
#include "pages/input/ToggleSwitchPage.h"
#include "pages/list/ListViewPage.h"
#include "pages/list/TreeViewPage.h"
#include "pages/navigation/MenuPage.h"
#include "pages/navigation/NavigationViewPage.h"
#include "pages/navigation/PivotPage.h"
#include "pages/navigation/TabBarPage.h"
#include "pages/navigation/TabWidgetPage.h"
#include "pages/settings/SettingsPage.h"
#include "pages/tool/ColorsPage.h"
#include "pages/tool/IconsPage.h"
#include "pages/window/DialogPage.h"
#include "pages/window/MainWindowPage.h"
#include "pages/window/ScrollBarPage.h"
#include "pages/window/WindowBarPage.h"

MainWindow::MainWindow(QWidget* parent) : NMainWindow(parent) {
    setupMenuBar();

    m_navigationView = new NNavigationView(this);
    setCentralWidget(m_navigationView);

    QString basicGroup;
    m_navigationView->addExpanderNode("基础组件", basicGroup, NRegularIconType::Apps16Regular);
    m_navigationView->addPageNode("Label", new LabelPage(this), basicGroup, NRegularIconType::None);
    m_navigationView->addPageNode("PushButton", new PushButtonPage(this), basicGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ToolButton", new ToolButtonPage(this), basicGroup, NRegularIconType::None);
    m_navigationView->addPageNode("HyperlinkButton", new HyperlinkButtonPage(this), basicGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ToggleButton", new ToggleButtonPage(this), basicGroup, NRegularIconType::None);
    m_navigationView->addPageNode("DropDownButton", new DropDownButtonPage(this), basicGroup, NRegularIconType::None);

    QString inputGroup;
    m_navigationView->addExpanderNode("输入组件", inputGroup, NRegularIconType::Edit16Regular);
    m_navigationView->addPageNode("LineEdit", new LineEditPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("TextEdit", new TextEditPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("PlainTextEdit", new PlainTextEditPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("AutoSuggestBox", new AutoSuggestBoxPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ComboBox", new ComboBoxPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("SpinBox", new SpinBoxPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("DoubleSpinBox", new DoubleSpinBoxPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("CheckBox", new CheckBoxPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("RadioButton", new RadioButtonPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ToggleSwitch", new ToggleSwitchPage(this), inputGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Slider", new SliderPage(this), inputGroup, NRegularIconType::None);

    QString selectionGroup;
    m_navigationView->addExpanderNode("选择组件", selectionGroup, NRegularIconType::CheckboxChecked16Regular);
    m_navigationView->addPageNode("CheckBox", new CheckBoxPage(this), selectionGroup, NRegularIconType::None);
    m_navigationView->addPageNode("RadioButton", new RadioButtonPage(this), selectionGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ToggleSwitch", new ToggleSwitchPage(this), selectionGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Slider", new SliderPage(this), selectionGroup, NRegularIconType::None);

    QString datetimeGroup;
    m_navigationView->addExpanderNode("日期时间", datetimeGroup, NRegularIconType::Calendar16Regular);
    m_navigationView->addPageNode("CalendarWidget", new CalendarWidgetPage(this), datetimeGroup, NRegularIconType::None);
    m_navigationView->addPageNode("CalendarDatePicker", new CalendarDatePickerPage(this), datetimeGroup, NRegularIconType::None);
    m_navigationView->addPageNode("DatePicker", new DatePickerPage(this), datetimeGroup, NRegularIconType::None);
    m_navigationView->addPageNode("TimePicker", new TimePickerPage(this), datetimeGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Picker", new PickerPage(this), datetimeGroup, NRegularIconType::None);

    QString containerGroup;
    m_navigationView->addExpanderNode("布局容器", containerGroup, NRegularIconType::Box16Regular);
    m_navigationView->addPageNode("GroupBox", new GroupBoxPage(this), containerGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Expander", new ExpanderPage(this), containerGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ScrollArea", new ScrollAreaPage(this), containerGroup, NRegularIconType::None);
    m_navigationView->addPageNode("StackedWidget", new StackedWidgetPage(this), containerGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Overlay", new OverlayPage(this), containerGroup, NRegularIconType::None);

    QString navigationGroup;
    m_navigationView->addExpanderNode("导航组件", navigationGroup, NRegularIconType::Navigation16Regular);
    m_navigationView->addPageNode("NavigationView", new NavigationViewPage(this), navigationGroup, NRegularIconType::None);
    m_navigationView->addPageNode("TabWidget", new TabWidgetPage(this), navigationGroup, NRegularIconType::None);
    m_navigationView->addPageNode("TabBar", new TabBarPage(this), navigationGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Pivot", new PivotPage(this), navigationGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Menu", new MenuPage(this), navigationGroup, NRegularIconType::None);

    QString feedbackGroup;
    m_navigationView->addExpanderNode("反馈组件", feedbackGroup, NRegularIconType::Alert16Regular);
    m_navigationView->addPageNode("ContentDialog", new ContentDialogPage(this), feedbackGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Flyout", new FlyoutPage(this), feedbackGroup, NRegularIconType::None);
    m_navigationView->addPageNode("InfoBar", new InfoBarPage(this), feedbackGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ToolTip", new ToolTipPage(this), feedbackGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ProgressBar", new ProgressBarPage(this), feedbackGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ProgressRing", new ProgressRingPage(this), feedbackGroup, NRegularIconType::None);

    QString listGroup;
    m_navigationView->addExpanderNode("列表组件", listGroup, NRegularIconType::List16Regular);
    m_navigationView->addPageNode("ListView", new ListViewPage(this), listGroup, NRegularIconType::None);
    m_navigationView->addPageNode("TreeView", new TreeViewPage(this), listGroup, NRegularIconType::None);

    QString windowGroup;
    m_navigationView->addExpanderNode("窗口组件", windowGroup, NRegularIconType::Window16Regular);
    m_navigationView->addPageNode("Dialog", new DialogPage(this), windowGroup, NRegularIconType::None);
    m_navigationView->addPageNode("MainWindow", new MainWindowPage(this), windowGroup, NRegularIconType::None);
    m_navigationView->addPageNode("WindowBar", new WindowBarPage(this), windowGroup, NRegularIconType::None);
    m_navigationView->addPageNode("ScrollBar", new ScrollBarPage(this), windowGroup, NRegularIconType::None);

    QString toolsGroup;
    m_navigationView->addExpanderNode("工具", toolsGroup, NRegularIconType::Wrench16Regular);
    m_navigationView->addPageNode("Colors", new ColorsPage(this), toolsGroup, NRegularIconType::None);
    m_navigationView->addPageNode("Icons", new IconsPage(this), toolsGroup, NRegularIconType::None);

    QString settingsKey;
    m_navigationView->addFooterNode("Settings", new SettingsPage(this), settingsKey, 0, NRegularIconType::Settings16Regular);

    setWindowTitle("QtNativeUI by Yang1206");
    setWindowIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    resize(1200, 700);

    setWindowEffect(MicaAlt);
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
    auto backdropMenu = viewMenu->addSubMenu("Window Effect", NRegularIconType::Window16Regular);
    auto noneAction   = backdropMenu->addItem("None");
    auto blurAction   = backdropMenu->addItem("Blur");
#ifdef Q_OS_WIN
    auto acrylicAction = backdropMenu->addItem("Acrylic");
    auto micaAction    = backdropMenu->addItem("Mica");
    auto micaAltAction = backdropMenu->addItem("Mica Alt");
#endif

    connect(noneAction, &QAction::triggered, [this]() { setWindowEffect(None); });
    connect(blurAction, &QAction::triggered, [this]() { setWindowEffect(Blur); });
#ifdef Q_OS_WIN
    connect(acrylicAction, &QAction::triggered, [this]() { setWindowEffect(Acrylic); });
    connect(micaAction, &QAction::triggered, [this]() { setWindowEffect(Mica); });
    connect(micaAltAction, &QAction::triggered, [this]() { setWindowEffect(MicaAlt); });
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
