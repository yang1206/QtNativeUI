#include "TabWidgetPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTabWidget.h>

TabWidgetPage::TabWidgetPage(QWidget* parent)
    : BasePage("TabWidget 标签页控件", "TabWidget 提供标签页导航，支持多个页面的切换。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NTabWidget* basicTabWidget = new NTabWidget(this);
    basicTabWidget->setMinimumHeight(200);

    QWidget* tab1 = new QWidget(basicTabWidget);
    QVBoxLayout* tab1Layout = new QVBoxLayout(tab1);
    tab1Layout->addWidget(new NLabel("这是第一个标签页", NLabelType::Title, tab1));
    tab1Layout->addWidget(new NLabel("标签页内容 1", NLabelType::Body, tab1));
    tab1Layout->addStretch();

    QWidget* tab2 = new QWidget(basicTabWidget);
    QVBoxLayout* tab2Layout = new QVBoxLayout(tab2);
    tab2Layout->addWidget(new NLabel("这是第二个标签页", NLabelType::Title, tab2));
    tab2Layout->addWidget(new NLabel("标签页内容 2", NLabelType::Body, tab2));
    tab2Layout->addStretch();

    QWidget* tab3 = new QWidget(basicTabWidget);
    QVBoxLayout* tab3Layout = new QVBoxLayout(tab3);
    tab3Layout->addWidget(new NLabel("这是第三个标签页", NLabelType::Title, tab3));
    tab3Layout->addWidget(new NLabel("标签页内容 3", NLabelType::Body, tab3));
    tab3Layout->addStretch();

    basicTabWidget->addTab(tab1, "首页");
    basicTabWidget->addTab(tab2, "文档");
    basicTabWidget->addTab(tab3, "设置");

    basicLayout->addWidget(basicTabWidget);

    addSection("基本标签页", basicLayout);

    QVBoxLayout* closableLayout = new QVBoxLayout();
    closableLayout->setSpacing(16);

    NTabWidget* closableTabWidget = new NTabWidget(this);
    closableTabWidget->setMinimumHeight(200);
    closableTabWidget->setTabsClosable(true);

    for (int i = 1; i <= 4; ++i) {
        QWidget* tab = new QWidget(closableTabWidget);
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);
        tabLayout->addWidget(new NLabel(QString("标签页 %1").arg(i), NLabelType::Title, tab));
        tabLayout->addStretch();
        closableTabWidget->addTab(tab, QString("Tab %1").arg(i));
    }

    closableLayout->addWidget(closableTabWidget);

    addSection("可关闭的标签页", closableLayout);

    QVBoxLayout* positionLayout = new QVBoxLayout();
    positionLayout->setSpacing(16);

    NTabWidget* topTabWidget = new NTabWidget(this);
    topTabWidget->setMinimumHeight(150);
    topTabWidget->setTabPosition(QTabWidget::North);

    for (int i = 1; i <= 3; ++i) {
        QWidget* tab = new QWidget(topTabWidget);
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);
        tabLayout->addWidget(new NLabel(QString("顶部标签 %1").arg(i), NLabelType::Body, tab));
        tabLayout->addStretch();
        topTabWidget->addTab(tab, QString("Tab %1").arg(i));
    }

    NTabWidget* bottomTabWidget = new NTabWidget(this);
    bottomTabWidget->setMinimumHeight(150);
    bottomTabWidget->setTabPosition(QTabWidget::South);

    for (int i = 1; i <= 3; ++i) {
        QWidget* tab = new QWidget(bottomTabWidget);
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);
        tabLayout->addWidget(new NLabel(QString("底部标签 %1").arg(i), NLabelType::Body, tab));
        tabLayout->addStretch();
        bottomTabWidget->addTab(tab, QString("Tab %1").arg(i));
    }

    positionLayout->addWidget(new QLabel("标签在顶部:", this));
    positionLayout->addWidget(topTabWidget);
    positionLayout->addWidget(new QLabel("标签在底部:", this));
    positionLayout->addWidget(bottomTabWidget);

    addSection("不同位置", positionLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(16);

    NTabWidget* interactiveTabWidget = new NTabWidget(this);
    interactiveTabWidget->setMinimumHeight(200);

    for (int i = 1; i <= 3; ++i) {
        QWidget* tab = new QWidget(interactiveTabWidget);
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);
        tabLayout->addWidget(new NLabel(QString("页面 %1").arg(i), NLabelType::Title, tab));
        tabLayout->addStretch();
        interactiveTabWidget->addTab(tab, QString("Tab %1").arg(i));
    }

    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(8);

    NPushButton* addTabButton = new NPushButton("添加标签", this);
    addTabButton->setFixedSize(100, 32);
    connect(addTabButton, &NPushButton::clicked, [interactiveTabWidget]() {
        int count = interactiveTabWidget->count();
        QWidget* newTab = new QWidget(interactiveTabWidget);
        QVBoxLayout* tabLayout = new QVBoxLayout(newTab);
        tabLayout->addWidget(new NLabel(QString("新标签 %1").arg(count + 1), NLabelType::Title, newTab));
        tabLayout->addStretch();
        interactiveTabWidget->addTab(newTab, QString("New %1").arg(count + 1));
        interactiveTabWidget->setCurrentIndex(count);
    });

    NPushButton* removeTabButton = new NPushButton("移除当前", this);
    removeTabButton->setFixedSize(100, 32);
    connect(removeTabButton, &NPushButton::clicked, [interactiveTabWidget]() {
        int currentIndex = interactiveTabWidget->currentIndex();
        if (currentIndex >= 0 && interactiveTabWidget->count() > 1) {
            interactiveTabWidget->removeTab(currentIndex);
        }
    });

    QLabel* currentLabel = new QLabel("当前标签: Tab 1", this);
    connect(interactiveTabWidget, &NTabWidget::currentChanged, [currentLabel, interactiveTabWidget](int index) {
        currentLabel->setText(QString("当前标签: %1").arg(interactiveTabWidget->tabText(index)));
    });

    controlLayout->addWidget(addTabButton);
    controlLayout->addWidget(removeTabButton);
    controlLayout->addWidget(currentLabel);
    controlLayout->addStretch();

    interactiveLayout->addWidget(interactiveTabWidget);
    interactiveLayout->addLayout(controlLayout);

    addSection("交互示例", interactiveLayout);
}
