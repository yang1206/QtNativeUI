#include "navigation.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NNavigationBar.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NStackedWidget.h>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include <QtNativeUI/NToolButton.h>
#include "QtNativeUI/NLabel.h"
#include "QtNativeUI/NNavigationView.h"
#include "QtNativeUI/NScrollArea.h"
#include "widgets/ExampleSection.h"

NavigationExample::NavigationExample(QWidget* parent) : QWidget(parent) { initUI(); }

NavigationExample::~NavigationExample() {}

void NavigationExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    // 创建滚动区域
    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建内容容器
    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    contentLayout->addWidget(new ExampleSection("NavigationView", createNavigationViews()));
    contentLayout->addWidget(new ExampleSection("NavigationBar", createNavigationBars()));
    contentLayout->addWidget(new ExampleSection("TabBar", createTabBars()));
    contentLayout->addWidget(new ExampleSection("TabWidget", createTabWidgets()));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
    setMinimumWidth(600);
}

QWidget* NavigationExample::createTabBars() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 创建一个普通的NTabBar
    auto normalTabBar = new NTabBar(this);
    normalTabBar->addTab("Home");
    normalTabBar->addTab("Documents");
    normalTabBar->addTab("Settings");

    // 创建一个带图标的NTabBar
    auto iconTabBar = new NTabBar(this);
    iconTabBar->setIconSize(QSize(16, 16));
    iconTabBar->addTab(nIcon->fromRegular(NRegularIconType::Home24Regular), "Home");
    iconTabBar->addTab(nIcon->fromRegular(NRegularIconType::Document24Regular), "Documents");
    iconTabBar->addTab(nIcon->fromRegular(NRegularIconType::Settings24Regular), "Settings");

    // 创建一个带关闭按钮的NTabBar
    auto closableTabBar = new NTabBar(this);
    closableTabBar->setTabsClosable(true);
    closableTabBar->setMovable(true);
    closableTabBar->addTab("Tab 1");

    // 添加一个按钮来动态添加标签
    auto addTabButton = new NPushButton("Add Tab", this);
    connect(addTabButton, &NPushButton::clicked, this, [closableTabBar]() {
        int newIndex = closableTabBar->count() + 1;
        closableTabBar->addTab(QString("Tab %1").arg(newIndex));
    });

    // 将组件添加到布局中
    layout->addWidget(new QLabel("Standard Tab Bar:"));
    layout->addWidget(normalTabBar);
    layout->addWidget(new QLabel("Tab Bar with Icons:"));
    layout->addWidget(iconTabBar);
    layout->addWidget(new QLabel("Closable Tab Bar:"));
    layout->addWidget(closableTabBar);
    layout->addWidget(addTabButton);

    // 在视觉效果上分隔不同示例
    layout->addWidget(new QLabel(""));

    auto horizontalLayout = new QHBoxLayout();
    auto verticalTabBar   = new NTabBar(this);
    verticalTabBar->setShape(QTabBar::RoundedWest);
    verticalTabBar->addTab("Tab 1");
    verticalTabBar->addTab("Tab 2");
    verticalTabBar->addTab("Tab 3");
    verticalTabBar->setTabsClosable(true);

    horizontalLayout->addWidget(verticalTabBar);

    auto verticalTabBar2 = new NTabBar(this);
    verticalTabBar2->setShape(QTabBar::RoundedEast);
    verticalTabBar2->addTab("Tab 1");
    verticalTabBar2->addTab("Tab 2");
    verticalTabBar2->addTab("Tab 3");
    verticalTabBar2->setTabsClosable(true);
    verticalTabBar2->setMovable(true);
    horizontalLayout->addWidget(verticalTabBar2);

    layout->addLayout(horizontalLayout);

    // 添加到主布局
    return container;
}

QWidget* NavigationExample::createTabWidgets() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 1. 创建带图标的标准TabWidget (North position)
    {
        QWidget*     section       = new QWidget;
        QVBoxLayout* sectionLayout = new QVBoxLayout(section);
        sectionLayout->setContentsMargins(0, 0, 0, 16);

        sectionLayout->addWidget(new QLabel("Standard TabWidget (North position):"));

        // 创建TabWidget
        NTabWidget* tabWidget = new NTabWidget(section);
        tabWidget->setMinimumHeight(200);

        // 创建第一个标签页
        QWidget*     homeTab    = new QWidget;
        QVBoxLayout* homeLayout = new QVBoxLayout(homeTab);
        homeLayout->addWidget(new QLabel("Home content here"));

        // 创建第二个标签页
        QWidget*     docsTab    = new QWidget;
        QVBoxLayout* docsLayout = new QVBoxLayout(docsTab);
        docsLayout->addWidget(new QLabel("Documents content here"));

        // 创建第三个标签页
        QWidget*     settingsTab    = new QWidget;
        QVBoxLayout* settingsLayout = new QVBoxLayout(settingsTab);
        settingsLayout->addWidget(new QLabel("Settings content here"));

        // 添加标签页到TabWidget
        tabWidget->addTab(homeTab, nIcon->fromRegular(NRegularIconType::Home24Regular), "Home");
        tabWidget->addTab(docsTab, nIcon->fromRegular(NRegularIconType::Document24Regular), "Documents");
        tabWidget->addTab(settingsTab, nIcon->fromRegular(NRegularIconType::Settings24Regular), "Settings");

        sectionLayout->addWidget(tabWidget);
        layout->addWidget(section);
    }

    // 2. 创建可关闭标签的TabWidget (South position)
    {
        QWidget*     section       = new QWidget;
        QVBoxLayout* sectionLayout = new QVBoxLayout(section);
        sectionLayout->setContentsMargins(0, 0, 0, 16);

        sectionLayout->addWidget(new QLabel("Closable TabWidget (South position):"));

        // 创建TabWidget
        NTabWidget* tabWidget = new NTabWidget(section);
        tabWidget->setTabPosition(NTabWidget::South);
        tabWidget->setTabsClosable(true);
        tabWidget->setMinimumHeight(200);

        // 添加几个标签页
        for (int i = 1; i <= 3; ++i) {
            QWidget*     tab       = new QWidget;
            QVBoxLayout* tabLayout = new QVBoxLayout(tab);
            tabLayout->addWidget(new QLabel(QString("Content for Tab %1").arg(i)));
            QString tipText = QString("Detailed information for Tab %1").arg(i);
            tabWidget->addTab(tab, QString("Tab %1").arg(i));
            tabWidget->setTabToolTip(i - 1, tipText);
        }

        // 添加一个按钮来动态添加标签
        QWidget*     controlPanel  = new QWidget;
        QHBoxLayout* controlLayout = new QHBoxLayout(controlPanel);
        controlLayout->setContentsMargins(0, 8, 0, 0);

        NPushButton* addTabButton = new NPushButton("Add Tab", controlPanel);
        connect(addTabButton, &NPushButton::clicked, tabWidget, [tabWidget]() {
            int          newIndex  = tabWidget->count() + 1;
            QWidget*     newTab    = new QWidget;
            QVBoxLayout* tabLayout = new QVBoxLayout(newTab);
            tabLayout->addWidget(new QLabel(QString("Content for Tab %1").arg(newIndex)));

            tabWidget->addTab(newTab, QString("Tab %1").arg(newIndex));
            QString tipText = QString("Detailed information for Tab %1").arg(newIndex);
            tabWidget->setTabToolTip(newIndex - 1, tipText);
            tabWidget->setCurrentIndex(tabWidget->count() - 1);
        });

        controlLayout->addWidget(addTabButton);
        controlLayout->addStretch();

        // 响应关闭标签请求
        connect(tabWidget, &NTabWidget::tabCloseRequested, tabWidget, [tabWidget](int index) {
            qDebug() << index;
            QWidget* widget = tabWidget->widget(index);
            tabWidget->removeTab(index);
            delete widget;
        });

        sectionLayout->addWidget(tabWidget);
        sectionLayout->addWidget(controlPanel);
        layout->addWidget(section);
    }

    // 3. 创建水平和垂直方向的TabWidget
    {
        QWidget*     section       = new QWidget;
        QHBoxLayout* sectionLayout = new QHBoxLayout(section);
        sectionLayout->setContentsMargins(0, 0, 0, 0);

        // 左侧TabWidget (West position)
        QWidget*     leftContainer = new QWidget;
        QVBoxLayout* leftLayout    = new QVBoxLayout(leftContainer);
        leftLayout->setContentsMargins(0, 0, 8, 0);

        leftLayout->addWidget(new QLabel("West position:"));

        NTabWidget* westTabWidget = new NTabWidget(leftContainer);
        westTabWidget->setTabPosition(NTabWidget::West);
        westTabWidget->setMinimumHeight(200);
        westTabWidget->setMinimumWidth(200);

        // 添加标签页
        for (int i = 1; i <= 3; ++i) {
            QWidget*     tab       = new QWidget;
            QVBoxLayout* tabLayout = new QVBoxLayout(tab);
            tabLayout->addWidget(new QLabel(QString("Content %1").arg(i)));

            westTabWidget->addTab(tab, QString("Tab %1").arg(i));
        }

        leftLayout->addWidget(westTabWidget);
        sectionLayout->addWidget(leftContainer, 1);

        // 右侧TabWidget (East position)
        QWidget*     rightContainer = new QWidget;
        QVBoxLayout* rightLayout    = new QVBoxLayout(rightContainer);
        rightLayout->setContentsMargins(8, 0, 0, 0);

        rightLayout->addWidget(new QLabel("East position:"));

        NTabWidget* eastTabWidget = new NTabWidget(rightContainer);
        eastTabWidget->setTabPosition(NTabWidget::East);
        eastTabWidget->setTabsClosable(true);
        eastTabWidget->setMinimumHeight(200);
        eastTabWidget->setMinimumWidth(200);

        // 添加标签页
        for (int i = 1; i <= 3; ++i) {
            QWidget*     tab       = new QWidget;
            QVBoxLayout* tabLayout = new QVBoxLayout(tab);
            tabLayout->addWidget(new QLabel(QString("Content %1").arg(i)));

            eastTabWidget->addTab(tab, QString("Tab %1").arg(i));
        }

        connect(eastTabWidget, &NTabWidget::tabCloseRequested, eastTabWidget, [eastTabWidget](int index) {
            if (eastTabWidget->count() > 1) { // 保留至少一个标签
                QWidget* widget = eastTabWidget->widget(index);
                eastTabWidget->removeTab(index);
                delete widget;
            }
        });

        rightLayout->addWidget(eastTabWidget);
        sectionLayout->addWidget(rightContainer, 1);

        layout->addWidget(section);
    }

    return container;
}

QWidget* NavigationExample::createNavigationBars() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    // layout->setSpacing(16);

    // 1. 基本导航栏示例
    {
        QWidget*     demoSection   = new QWidget;
        QHBoxLayout* sectionLayout = new QHBoxLayout(demoSection);
        // sectionLayout->setContentsMargins(0, 0, 0, 16);

        // 创建导航栏
        NNavigationBar* navigationBar = new NNavigationBar(demoSection);
        navigationBar->setHeaderWidget(new NLabel("头部组件", NLabelType::Subtitle));
        navigationBar->setMinimumHeight(400);
        navigationBar->setSearchVisible(false);

        // 创建一些内容页面
        QWidget*     homePage   = new QWidget;
        QVBoxLayout* homeLayout = new QVBoxLayout(homePage);
        homeLayout->addWidget(new QLabel("Home Page Content"));

        QWidget*     documentsPage   = new QWidget;
        QVBoxLayout* documentsLayout = new QVBoxLayout(documentsPage);
        documentsLayout->addWidget(new QLabel("Documents Page Content"));

        QWidget*     settingsPage   = new QWidget;
        QVBoxLayout* settingsLayout = new QVBoxLayout(settingsPage);
        settingsLayout->addWidget(new QLabel("Settings Page Content"));

        QWidget*     aboutPage   = new QWidget;
        QVBoxLayout* aboutLayout = new QVBoxLayout(aboutPage);
        aboutLayout->addWidget(new QLabel("About Page Content"));

        QWidget*     profilePage   = new QWidget;
        QVBoxLayout* profileLayout = new QVBoxLayout(profilePage);
        profileLayout->addWidget(new QLabel("Profile Page Content"));

        // 创建内容显示区
        NStackedWidget* contentStack = new NStackedWidget(demoSection);
        contentStack->addWidget(homePage);
        contentStack->addWidget(documentsPage);
        contentStack->addWidget(settingsPage);
        contentStack->addWidget(profilePage);
        contentStack->addWidget(aboutPage);

        m_pageKeyMap.clear();

        connect(navigationBar,
                &NNavigationBar::displayModeChange,
                this,
                [](NNavigationType::NavigationDisplayMode mode) { qDebug() << "displayModeChange:" << mode; });

        connect(navigationBar,
                &NNavigationBar::navigationNodeAdded,
                [this, contentStack](NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page) {
                    for (int i = 0; i < contentStack->count(); ++i) {
                        if (contentStack->widget(i) == page) {
                            m_pageKeyMap[nodeKey] = i;
                            qDebug() << "Mapped node" << nodeKey << "to page index" << i;
                            break;
                        }
                    }
                });

        // 添加导航节点
        QString expanderKey;
        navigationBar->addExpanderNode("System", expanderKey, NRegularIconType::Settings24Regular);

        // 添加页面节点
        navigationBar->addPageNode("Home", homePage, NRegularIconType::Home24Regular);
        navigationBar->addPageNode("Documents", documentsPage, NRegularIconType::Document24Regular);
        navigationBar->addPageNode("Settings", settingsPage, expanderKey, NRegularIconType::Settings24Regular);
        navigationBar->addPageNode("About", aboutPage, NFilledIconType::Info24Filled);

        // 添加页脚节点
        QString footerKey;
        navigationBar->addFooterNode("Profile", profilePage, footerKey, 0, NRegularIconType::Person24Regular);

        // 连接导航事件，使用映射表切换页面
        connect(navigationBar,
                &NNavigationBar::navigationNodeClicked,
                [this, contentStack](NNavigationType::NavigationNodeType nodeType, QString nodeKey) {
                    qDebug() << "Navigation node clicked: " << nodeType << ", " << nodeKey;
                    if (m_pageKeyMap.contains(nodeKey)) {
                        contentStack->setCurrentIndex(m_pageKeyMap[nodeKey]);
                    }
                });

        sectionLayout->addWidget(navigationBar);
        sectionLayout->addWidget(contentStack, 1);
        layout->addWidget(demoSection);
    }

    return container;
}

QWidget* NavigationExample::createNavigationViews() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 创建一个说明标签
    NLabel* descriptionLabel = new NLabel(
        "NNavigationView 组合了 NNavigationBar 和 NStackedWidget，提供了完整的导航和页面切换功能，并支持过渡动画。",
        NLabelType::Caption);
    descriptionLabel->setWordWrap(true);
    layout->addWidget(descriptionLabel);

    // 1. 创建基本的 NavigationView 示例
    {
        QWidget*     demoSection   = new QWidget;
        QVBoxLayout* sectionLayout = new QVBoxLayout(demoSection);
        sectionLayout->setContentsMargins(0, 0, 0, 16);

        sectionLayout->addWidget(new NLabel("基本 NavigationView 示例", NLabelType::Subtitle));

        // 创建 NavigationView
        NNavigationView* navigationView = new NNavigationView(demoSection);
        navigationView->setMinimumHeight(400);
        navigationView->setPageTransitionType(NNavigationType::SlideVertical);
        navigationView->setSearchVisible(false);

        // 设置头部组件
        navigationView->setHeaderWidget(new NLabel("导航示例", NLabelType::Subtitle));

        // 创建一些内容页面
        QWidget*     homePage   = new QWidget;
        QVBoxLayout* homeLayout = new QVBoxLayout(homePage);
        homeLayout->addWidget(new NLabel("首页内容", NLabelType::Title));
        homeLayout->addWidget(new NLabel("这是首页的详细内容展示区域", NLabelType::Body));

        QWidget*     documentsPage   = new QWidget;
        QVBoxLayout* documentsLayout = new QVBoxLayout(documentsPage);
        documentsLayout->addWidget(new NLabel("文档页面", NLabelType::Title));
        documentsLayout->addWidget(new NLabel("这里展示文档相关内容", NLabelType::Body));

        QWidget*     settingsPage   = new QWidget;
        QVBoxLayout* settingsLayout = new QVBoxLayout(settingsPage);
        settingsLayout->addWidget(new NLabel("设置页面", NLabelType::Title));
        settingsLayout->addWidget(new NLabel("系统设置选项将在此处显示", NLabelType::Body));

        QWidget*     profilePage   = new QWidget;
        QVBoxLayout* profileLayout = new QVBoxLayout(profilePage);
        profileLayout->addWidget(new NLabel("个人资料", NLabelType::Title));
        profileLayout->addWidget(new NLabel("用户个人资料信息", NLabelType::Body));

        // 添加导航节点
        QString expanderKey;
        navigationView->addExpanderNode("系统", expanderKey, NRegularIconType::Settings24Regular);

        // 添加页面节点 - 使用 Regular 图标
        navigationView->addPageNode("首页", homePage, NRegularIconType::Home24Regular);
        navigationView->addPageNode("文档", documentsPage, NRegularIconType::Document24Regular);
        navigationView->addPageNode("设置", settingsPage, expanderKey, NRegularIconType::Settings24Regular);

        // 添加页脚节点
        QString footerKey;
        navigationView->addFooterNode("个人资料", profilePage, footerKey, 0, NRegularIconType::Person24Regular);

        // 连接信号
        connect(navigationView,
                &NNavigationView::navigationNodeClicked,
                [](NNavigationType::NavigationNodeType nodeType, QString nodeKey) {
                    qDebug() << "节点被点击:" << nodeType << nodeKey;
                });

        connect(navigationView, &NNavigationView::currentChanged, [](int index) {
            qDebug() << "当前页面索引变更为:" << index;
        });

        sectionLayout->addWidget(navigationView);
        layout->addWidget(demoSection);
    }

    // 2. 创建使用 Filled 图标的 NavigationView 示例
    {
        QWidget*     demoSection   = new QWidget;
        QVBoxLayout* sectionLayout = new QVBoxLayout(demoSection);
        sectionLayout->setContentsMargins(0, 0, 0, 16);

        sectionLayout->addWidget(new NLabel("使用 Filled 图标的 NavigationView", NLabelType::Subtitle));

        // 创建 NavigationView
        NNavigationView* navigationView = new NNavigationView(demoSection);
        navigationView->setMinimumHeight(400);
        navigationView->setDisplayMode(NNavigationType::Compact); // 设置为紧凑模式

        // 设置头部组件
        navigationView->setHeaderWidget(new NLabel("Filled 图标", NLabelType::Subtitle));

        // 创建内容页面
        QWidget*     dashboardPage   = new QWidget;
        QVBoxLayout* dashboardLayout = new QVBoxLayout(dashboardPage);
        dashboardLayout->addWidget(new NLabel("仪表盘", NLabelType::Title));
        dashboardLayout->addWidget(new NLabel("数据概览和统计信息", NLabelType::Body));

        QWidget*     mailPage   = new QWidget;
        QVBoxLayout* mailLayout = new QVBoxLayout(mailPage);
        mailLayout->addWidget(new NLabel("邮件", NLabelType::Title));
        mailLayout->addWidget(new NLabel("收件箱和邮件管理", NLabelType::Body));

        QWidget*     calendarPage   = new QWidget;
        QVBoxLayout* calendarLayout = new QVBoxLayout(calendarPage);
        calendarLayout->addWidget(new NLabel("日历", NLabelType::Title));
        calendarLayout->addWidget(new NLabel("日程和事件管理", NLabelType::Body));

        QWidget*     helpPage   = new QWidget;
        QVBoxLayout* helpLayout = new QVBoxLayout(helpPage);
        helpLayout->addWidget(new NLabel("帮助", NLabelType::Title));
        helpLayout->addWidget(new NLabel("帮助文档和支持信息", NLabelType::Body));

        // 添加页面节点 - 使用 Filled 图标
        navigationView->addPageNode("仪表盘", dashboardPage, NFilledIconType::Dishwasher20Filled);
        navigationView->addPageNode("邮件", mailPage, NFilledIconType::Mail24Filled);
        navigationView->addPageNode("日历", calendarPage, NFilledIconType::Calendar24Filled);

        // 添加页脚节点
        QString helpKey;
        navigationView->addFooterNode("帮助", helpPage, helpKey, 0, NFilledIconType::QuestionCircle24Filled);

        // 设置页面切换动画时长
        navigationView->setPageTransitionDuration(400);

        sectionLayout->addWidget(navigationView);
        layout->addWidget(demoSection);
    }

    // 3. 添加控制面板示例
    {
        QWidget*     controlSection = new QWidget;
        QVBoxLayout* controlLayout  = new QVBoxLayout(controlSection);
        controlLayout->setContentsMargins(0, 0, 0, 0);

        controlLayout->addWidget(new NLabel("NavigationView 控制示例", NLabelType::Subtitle));

        // 创建 NavigationView
        NNavigationView* navigationView = new NNavigationView(controlSection);
        navigationView->setMinimumHeight(300);

        // 创建一些简单页面
        QWidget*     page1   = new QWidget;
        QVBoxLayout* layout1 = new QVBoxLayout(page1);
        layout1->addWidget(new NLabel("页面 1", NLabelType::Title));

        QWidget*     page2   = new QWidget;
        QVBoxLayout* layout2 = new QVBoxLayout(page2);
        layout2->addWidget(new NLabel("页面 2", NLabelType::Title));

        QWidget*     page3   = new QWidget;
        QVBoxLayout* layout3 = new QVBoxLayout(page3);
        layout3->addWidget(new NLabel("页面 3", NLabelType::Title));

        // 添加页面
        QString key1, key2, key3;
        navigationView->addPageNode("页面 1", page1, NRegularIconType::NumberCircle116Regular);
        navigationView->addPageNode("页面 2", page2, NRegularIconType::NumberCircle216Regular);
        navigationView->addPageNode("页面 3", page3, NRegularIconType::NumberCircle316Regular);

        // 创建控制按钮
        QWidget*     controlPanel = new QWidget;
        QHBoxLayout* buttonLayout = new QHBoxLayout(controlPanel);
        buttonLayout->setContentsMargins(0, 16, 0, 0);

        // 显示模式控制
        NLabel*      modeLabel  = new NLabel("显示模式:", NLabelType::Body);
        NPushButton* minimalBtn = new NPushButton("最小化", controlPanel);
        NPushButton* compactBtn = new NPushButton("紧凑", controlPanel);
        NPushButton* maximalBtn = new NPushButton("最大化", controlPanel);
        NPushButton* autoBtn    = new NPushButton("自动", controlPanel);

        connect(minimalBtn, &NPushButton::clicked, [navigationView]() {
            navigationView->setDisplayMode(NNavigationType::Minimal);
        });

        connect(compactBtn, &NPushButton::clicked, [navigationView]() {
            navigationView->setDisplayMode(NNavigationType::Compact);
        });

        connect(maximalBtn, &NPushButton::clicked, [navigationView]() {
            navigationView->setDisplayMode(NNavigationType::Maximal);
        });

        connect(autoBtn, &NPushButton::clicked, [navigationView]() {
            navigationView->setDisplayMode(NNavigationType::Auto);
        });

        buttonLayout->addWidget(modeLabel);
        buttonLayout->addWidget(minimalBtn);
        buttonLayout->addWidget(compactBtn);
        buttonLayout->addWidget(maximalBtn);
        buttonLayout->addWidget(autoBtn);
        buttonLayout->addStretch();

        // 添加到布局
        controlLayout->addWidget(navigationView);
        controlLayout->addWidget(controlPanel);

        layout->addWidget(controlSection);
    }

    return container;
}