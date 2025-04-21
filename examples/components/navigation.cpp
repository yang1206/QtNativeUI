#include "navigation.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include <QtNativeUI/NToolButton.h>

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

    contentLayout->addWidget(new ExampleSection("TabBar", createTabBars()));

    // 添加TabWidget示例
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