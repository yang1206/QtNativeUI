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
    closableTabBar->addTab("Tab 1");
    closableTabBar->addTab("Tab 2");
    closableTabBar->addTab("Tab 3");

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

    // 创建一个水平排列的TabBar示例
    auto horizontalLayout = new QHBoxLayout();
    auto verticalTabBar   = new NTabBar(this);
    verticalTabBar->setShape(QTabBar::RoundedWest);
    verticalTabBar->addTab("Tab 1");
    verticalTabBar->addTab("Tab 2");
    verticalTabBar->addTab("Tab 3");
    horizontalLayout->addWidget(verticalTabBar);

    auto verticalTabBar2 = new NTabBar(this);
    verticalTabBar2->setShape(QTabBar::RoundedEast);
    verticalTabBar2->addTab("Tab 1");
    verticalTabBar2->addTab("Tab 2");
    verticalTabBar2->addTab("Tab 3");
    horizontalLayout->addWidget(verticalTabBar2);

    layout->addLayout(horizontalLayout);

    // 添加到主布局
    return container;
}