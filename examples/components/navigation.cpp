#include "navigation.h"

#include <QDateTime>
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

#include "QtNativeUI/NCheckBox.h"
#include "QtNativeUI/NLabel.h"
#include "QtNativeUI/NNavigationRouter.h"
#include "QtNativeUI/NNavigationView.h"
#include "QtNativeUI/NScrollArea.h"
#include "widgets/ExampleSection.h"

DemoPageComponent::DemoPageComponent(const QString& title, QWidget* parent)
    : NPageComponent(parent), m_blockNavigation(false) {
    setPageTitle(title);
    QVBoxLayout* layout = new QVBoxLayout(this);
    // 添加标题
    NLabel* titleLabel = new NLabel(title, NLabelType::Title);
    layout->addWidget(titleLabel);
    // 添加日志文本框
    m_logTextEdit = new NTextEdit(this);
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setMinimumHeight(150);
    layout->addWidget(m_logTextEdit);
    // 添加一些说明文本
    NLabel* infoLabel = new NLabel("此页面演示路由生命周期和参数传递", NLabelType::Body);
    layout->addWidget(infoLabel);
    layout->addStretch();
}

void DemoPageComponent::onRouteEnter(const QVariantMap& params) {
    QString log = "onRouteEnter 被调用\n";
    if (!params.isEmpty()) {
        log += "接收到参数:\n";
        for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
            log += QString("  %1: %2\n").arg(it.key()).arg(it.value().toString());
        }
    } else {
        log += "没有接收到参数\n";
    }
    m_logTextEdit->append(log);
}

void DemoPageComponent::onRouteLeave(QVariantMap& outParams) {
    QString log = "onRouteLeave 被调用\n";
    // 添加一些离开参数
    outParams["leaveTime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    outParams["fromPage"]  = pageTitle();
    log += "发送参数:\n";
    for (auto it = outParams.constBegin(); it != outParams.constEnd(); ++it) {
        log += QString("  %1: %2\n").arg(it.key()).arg(it.value().toString());
    }
    m_logTextEdit->append(log);
}

bool DemoPageComponent::beforeRouteEnter(const QVariantMap& params) {
    QString log = "beforeRouteEnter 被调用\n";
    if (!params.isEmpty()) {
        log += "检查参数:\n";
        for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
            log += QString("  %1: %2\n").arg(it.key()).arg(it.value().toString());
        }
    }
    // 检查是否有阻止进入的条件
    if (params.contains("blockEnter") && params["blockEnter"].toBool()) {
        log += "导航被阻止!\n";
        m_logTextEdit->append(log);
        return false;
    }
    log += "允许导航\n";
    m_logTextEdit->append(log);
    return true;
}

bool DemoPageComponent::beforeRouteLeave(QVariantMap& outParams) {
    QString log = "beforeRouteLeave 被调用\n";

    // 如果设置了阻止导航，则阻止离开
    if (m_blockNavigation) {
        log += "导航被阻止!\n";
        m_logTextEdit->append(log);
        return false;
    }

    log += "允许导航\n";
    m_logTextEdit->append(log);
    return true;
}

void DemoPageComponent::setBlockNavigation(bool block) {
    m_blockNavigation = block;
    m_logTextEdit->append(QString("导航阻止状态设置为: %1").arg(block ? "是" : "否"));
}

QString DemoPageComponent::getLogText() const { return m_logTextEdit->toPlainText(); }

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
    layout->setSpacing(16);

    // 创建一个说明标签
    NLabel* descriptionLabel =
        new NLabel("NNavigationBar 提供了完整的导航功能，支持路由系统、导航历史记录和页面切换。", NLabelType::Caption);
    descriptionLabel->setWordWrap(true);
    layout->addWidget(descriptionLabel);

    // 1. 基本导航栏示例
    {
        QWidget*     demoSection   = new QWidget;
        QHBoxLayout* sectionLayout = new QHBoxLayout(demoSection);
        sectionLayout->setContentsMargins(0, 0, 0, 16);

        // 创建导航栏
        NNavigationBar* navigationBar = new NNavigationBar(demoSection);
        navigationBar->setHeaderWidget(new NLabel("基本导航", NLabelType::Subtitle));
        navigationBar->setMinimumHeight(400);
        navigationBar->setSearchVisible(false);

        // 创建一些内容页面
        QWidget*     homePage   = new QWidget;
        QVBoxLayout* homeLayout = new QVBoxLayout(homePage);
        homeLayout->addWidget(new NLabel("首页", NLabelType::Title));
        homeLayout->addWidget(new NLabel("这是首页内容", NLabelType::Body));

        QWidget*     documentsPage   = new QWidget;
        QVBoxLayout* documentsLayout = new QVBoxLayout(documentsPage);
        documentsLayout->addWidget(new NLabel("文档", NLabelType::Title));
        documentsLayout->addWidget(new NLabel("这是文档页面内容", NLabelType::Body));

        QWidget*     settingsPage   = new QWidget;
        QVBoxLayout* settingsLayout = new QVBoxLayout(settingsPage);
        settingsLayout->addWidget(new NLabel("设置", NLabelType::Title));
        settingsLayout->addWidget(new NLabel("这是设置页面内容", NLabelType::Body));

        QWidget*     aboutPage   = new QWidget;
        QVBoxLayout* aboutLayout = new QVBoxLayout(aboutPage);
        aboutLayout->addWidget(new NLabel("关于", NLabelType::Title));
        aboutLayout->addWidget(new NLabel("这是关于页面内容", NLabelType::Body));

        QWidget*     profilePage   = new QWidget;
        QVBoxLayout* profileLayout = new QVBoxLayout(profilePage);
        profileLayout->addWidget(new NLabel("个人资料", NLabelType::Title));
        profileLayout->addWidget(new NLabel("这是个人资料页面内容", NLabelType::Body));

        // 创建内容显示区
        NStackedWidget* contentStack = new NStackedWidget(demoSection);
        contentStack->addWidget(homePage);
        contentStack->addWidget(documentsPage);
        contentStack->addWidget(settingsPage);
        contentStack->addWidget(aboutPage);
        contentStack->addWidget(profilePage);

        m_pageKeyMap.clear();

        connect(navigationBar,
                &NNavigationBar::displayModeChange,
                this,
                [](NNavigationType::NavigationDisplayMode mode) { qDebug() << "显示模式变更为:" << mode; });

        connect(navigationBar,
                &NNavigationBar::navigationNodeAdded,
                [this, contentStack](NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page) {
                    for (int i = 0; i < contentStack->count(); ++i) {
                        if (contentStack->widget(i) == page) {
                            m_pageKeyMap[nodeKey] = i;
                            qDebug() << "节点" << nodeKey << "映射到页面索引" << i;
                            break;
                        }
                    }
                });

        // 添加导航节点
        QString expanderKey;
        navigationBar->addExpanderNode("系统", expanderKey, NRegularIconType::Settings24Regular);

        // 添加页面节点
        navigationBar->addPageNode("首页", homePage, NRegularIconType::Home24Regular);
        navigationBar->addPageNode("文档", documentsPage, NRegularIconType::Document24Regular);
        navigationBar->addPageNode("设置", settingsPage, expanderKey, NRegularIconType::Settings24Regular);
        navigationBar->addPageNode("关于", aboutPage, NFilledIconType::Info24Filled);

        // 添加页脚节点
        QString footerKey;
        navigationBar->addFooterNode("个人资料", profilePage, footerKey, 0, NRegularIconType::Person24Regular);

        // 连接导航事件
        connect(navigationBar,
                &NNavigationBar::navigationNodeClicked,
                [this, contentStack](NNavigationType::NavigationNodeType nodeType, QString nodeKey) {
                    qDebug() << "导航节点被点击: " << nodeType << ", " << nodeKey;
                    if (m_pageKeyMap.contains(nodeKey)) {
                        contentStack->setCurrentIndex(m_pageKeyMap[nodeKey]);
                    }
                });

        sectionLayout->addWidget(navigationBar);
        sectionLayout->addWidget(contentStack, 1);
        layout->addWidget(demoSection);
    }

    // 2. 路由系统演示
    layout->addWidget(createRouterDemo());

    return container;
}

// 添加新方法来创建路由系统演示
// 添加新方法来创建路由系统演示
QWidget* NavigationExample::createRouterDemo() {
    QWidget*     demoSection   = new QWidget;
    QVBoxLayout* sectionLayout = new QVBoxLayout(demoSection);
    sectionLayout->setContentsMargins(0, 0, 0, 16);

    sectionLayout->addWidget(new NLabel("路由系统演示", NLabelType::Subtitle));
    QHBoxLayout* contentLayout = new QHBoxLayout();
    // 创建导航栏
    NNavigationBar* navigationBar = new NNavigationBar(demoSection);
    navigationBar->setHeaderWidget(new NLabel("路由系统", NLabelType::Subtitle));
    navigationBar->setMinimumHeight(500);
    navigationBar->setSearchVisible(true);
    // 创建页面组件
    m_homePage     = new DemoPageComponent("首页", demoSection);
    m_profilePage  = new DemoPageComponent("个人资料", demoSection);
    m_settingsPage = new DemoPageComponent("设置", demoSection);
    m_detailsPage  = new DemoPageComponent("详情页", demoSection);
    // 创建内容显示区
    NStackedWidget* contentStack = new NStackedWidget(demoSection);
    contentStack->addWidget(m_homePage);
    contentStack->addWidget(m_profilePage);
    contentStack->addWidget(m_settingsPage);
    contentStack->addWidget(m_detailsPage);
    // 创建路由键到页面索引的映射
    QMap<QString, int> pageIndexMap;
    // 添加导航节点
    QString expanderKey;
    navigationBar->addExpanderNode("系统", expanderKey, NRegularIconType::Settings24Regular);
    // 添加页面组件到导航栏
    navigationBar->addPageComponent(m_homePage, NRegularIconType::Home24Regular);
    navigationBar->addPageComponent(m_profilePage, NRegularIconType::Person24Regular);
    navigationBar->addPageComponent(m_settingsPage, expanderKey, NRegularIconType::Settings24Regular);
    navigationBar->addPageComponent(m_detailsPage, NRegularIconType::Document24Regular);
    // 存储路由键
    m_homeKey     = m_homePage->routeKey();
    m_profileKey  = m_profilePage->routeKey();
    m_settingsKey = m_settingsPage->routeKey();
    m_detailsKey  = m_detailsPage->routeKey();
    // 创建路由键到页面索引的映射
    pageIndexMap[m_homeKey]     = 0;
    pageIndexMap[m_profileKey]  = 1;
    pageIndexMap[m_settingsKey] = 2;
    pageIndexMap[m_detailsKey]  = 3;
    // 获取路由器实例
    NNavigationRouter* router = NNavigationRouter::getInstance();
    // 监听路由变化，切换对应的页面
    connect(router,
            &NNavigationRouter::routeChanged,
            [contentStack, pageIndexMap](const QString& pageKey, const QVariantMap& params) {
                Q_UNUSED(params);
                if (pageIndexMap.contains(pageKey)) {
                    contentStack->setCurrentIndex(pageIndexMap[pageKey]);
                }
            });
    // 监听路由返回，切换对应的页面
    connect(
        router,
        &NNavigationRouter::routeBack,
        [contentStack, pageIndexMap](const QString& fromPageKey, const QString& toPageKey, const QVariantMap& params) {
            Q_UNUSED(fromPageKey);
            Q_UNUSED(params);
            if (pageIndexMap.contains(toPageKey)) {
                contentStack->setCurrentIndex(pageIndexMap[toPageKey]);
            }
        });
    // 创建控制面板
    QWidget*     controlPanel  = new QWidget(demoSection);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);
    // 添加导航控制按钮
    QGroupBox*   navControlGroup  = new QGroupBox("导航控制", controlPanel);
    QVBoxLayout* navControlLayout = new QVBoxLayout(navControlGroup);
    // 导航到首页
    NPushButton* navToHomeBtn = new NPushButton("导航到首页", navControlGroup);
    connect(navToHomeBtn, &NPushButton::clicked, [this, router]() {
        QVariantMap params;
        params["source"] = "导航按钮";
        params["time"]   = QDateTime::currentDateTime().toString("hh:mm:ss");
        router->navigateTo(m_homeKey, params);
    });
    // 导航到个人资料
    NPushButton* navToProfileBtn = new NPushButton("导航到个人资料", navControlGroup);
    connect(navToProfileBtn, &NPushButton::clicked, [this, router]() {
        QVariantMap params;
        params["source"] = "导航按钮";
        params["time"]   = QDateTime::currentDateTime().toString("hh:mm:ss");
        params["userId"] = "user123";
        router->navigateTo(m_profileKey, params);
    });
    // 导航到设置
    NPushButton* navToSettingsBtn = new NPushButton("导航到设置", navControlGroup);
    connect(navToSettingsBtn, &NPushButton::clicked, [this, router]() {
        QVariantMap params;
        params["source"]  = "导航按钮";
        params["time"]    = QDateTime::currentDateTime().toString("hh:mm:ss");
        params["section"] = "general";
        router->navigateTo(m_settingsKey, params);
    });
    // 导航到详情页
    NPushButton* navToDetailsBtn = new NPushButton("导航到详情页", navControlGroup);
    connect(navToDetailsBtn, &NPushButton::clicked, [this, router]() {
        QVariantMap params;
        params["source"]   = "导航按钮";
        params["time"]     = QDateTime::currentDateTime().toString("hh:mm:ss");
        params["itemId"]   = "item456";
        params["itemName"] = "示例项目";
        router->navigateTo(m_detailsKey, params);
    });
    // 返回按钮
    NPushButton* navBackBtn = new NPushButton("返回上一页", navControlGroup);
    connect(navBackBtn, &NPushButton::clicked, [router]() {
        QVariantMap params;
        params["backReason"] = "用户点击返回按钮";
        router->navigateBack(params);
    });
    navControlLayout->addWidget(navToHomeBtn);
    navControlLayout->addWidget(navToProfileBtn);
    navControlLayout->addWidget(navToSettingsBtn);
    navControlLayout->addWidget(navToDetailsBtn);
    navControlLayout->addWidget(navBackBtn);
    // 添加路由守卫控制
    QGroupBox*   guardControlGroup  = new QGroupBox("路由守卫控制", controlPanel);
    QVBoxLayout* guardControlLayout = new QVBoxLayout(guardControlGroup);
    // 阻止离开设置
    NCheckBox* blockLeaveCheckBox = new NCheckBox("阻止离开当前页面", guardControlGroup);
    connect(blockLeaveCheckBox, &NCheckBox::toggled, [this](bool checked) {
        // 获取当前页面并设置阻止状态
        QString            currentKey  = NNavigationRouter::getInstance()->currentRouteKey();
        DemoPageComponent* currentPage = nullptr;
        if (currentKey == m_homeKey)
            currentPage = m_homePage;
        else if (currentKey == m_profileKey)
            currentPage = m_profilePage;
        else if (currentKey == m_settingsKey)
            currentPage = m_settingsPage;
        else if (currentKey == m_detailsKey)
            currentPage = m_detailsPage;
        if (currentPage) {
            currentPage->setBlockNavigation(checked);
        }
    });
    // 阻止进入详情页
    NCheckBox* blockEnterDetailsCheckBox = new NCheckBox("阻止进入详情页", guardControlGroup);
    connect(blockEnterDetailsCheckBox, &NCheckBox::toggled, [this, navToDetailsBtn, router](bool checked) {
        // 修改导航参数，添加阻止标志
        disconnect(navToDetailsBtn, &NPushButton::clicked, nullptr, nullptr);
        connect(navToDetailsBtn, &NPushButton::clicked, [this, router, checked]() {
            QVariantMap params;
            params["source"]     = "导航按钮";
            params["time"]       = QDateTime::currentDateTime().toString("hh:mm:ss");
            params["itemId"]     = "item456";
            params["blockEnter"] = checked;
            router->navigateTo(m_detailsKey, params);
        });
    });
    guardControlLayout->addWidget(blockLeaveCheckBox);
    guardControlLayout->addWidget(blockEnterDetailsCheckBox);
    // 添加历史记录显示
    QGroupBox*   historyGroup    = new QGroupBox("导航历史", controlPanel);
    QVBoxLayout* historyLayout   = new QVBoxLayout(historyGroup);
    QTextEdit*   historyTextEdit = new QTextEdit(historyGroup);
    historyTextEdit->setReadOnly(true);
    historyTextEdit->setMaximumHeight(100);
    // 清除历史按钮
    NPushButton* clearHistoryBtn = new NPushButton("清除历史记录", historyGroup);
    connect(clearHistoryBtn, &NPushButton::clicked, [router, historyTextEdit]() {
        router->clearHistory();
        historyTextEdit->setText("历史记录已清除");
    });
    historyLayout->addWidget(historyTextEdit);
    historyLayout->addWidget(clearHistoryBtn);
    // 更新历史记录显示
    auto updateHistoryDisplay = [router, historyTextEdit]() {
        QString historyText = "当前路由: " + router->currentRouteKey() + "\n";
        historyText += "历史记录数: " + QString::number(router->historyCount()) + "\n";
        // 这里可以添加更多历史信息
        historyTextEdit->setText(historyText);
    };
    // 连接路由信号
    connect(router,
            &NNavigationRouter::routeChanged,
            [updateHistoryDisplay](const QString& pageKey, const QVariantMap& params) {
                qDebug() << "路由变化到:" << pageKey << "参数:" << params;
                updateHistoryDisplay();
            });
    connect(router,
            &NNavigationRouter::routeBack,
            [updateHistoryDisplay](const QString& fromPageKey, const QString& toPageKey, const QVariantMap& params) {
                qDebug() << "路由返回从:" << fromPageKey << "到:" << toPageKey << "参数:" << params;
                updateHistoryDisplay();
            });
    connect(router, &NNavigationRouter::navigationStateChanged, [updateHistoryDisplay](bool hasHistory) {
        qDebug() << "导航状态变化, 有历史记录:" << hasHistory;
        updateHistoryDisplay();
    });
    // 初始化历史显示
    updateHistoryDisplay();
    // 添加到控制面板
    controlLayout->addWidget(navControlGroup);
    controlLayout->addWidget(guardControlGroup);
    controlLayout->addWidget(historyGroup);
    controlLayout->addStretch();

    // 添加到主布局
    contentLayout->addWidget(navigationBar);
    contentLayout->addWidget(contentStack, 2);
    contentLayout->addWidget(controlPanel, 1);

    sectionLayout->addLayout(contentLayout);

    return demoSection;
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