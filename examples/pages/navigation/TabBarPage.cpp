#include "TabBarPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NStackedWidget.h>
#include <QtNativeUI/NTabBar.h>

TabBarPage::TabBarPage(QWidget* parent)
    : BasePage("TabBar 标签栏", "TabBar 提供标签栏控件，可以独立使用或与 StackedWidget 配合使用。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NTabBar* basicTabBar = new NTabBar(this);
    basicTabBar->addTab("首页");
    basicTabBar->addTab("文档");
    basicTabBar->addTab("设置");

    NStackedWidget* basicStack = new NStackedWidget(this);
    basicStack->setMinimumHeight(150);

    for (int i = 0; i < 3; ++i) {
        QWidget* page = new QWidget(basicStack);
        QVBoxLayout* pageLayout = new QVBoxLayout(page);
        pageLayout->addWidget(new NLabel(QString("页面 %1").arg(i + 1), NLabelType::Title, page));
        pageLayout->addStretch();
        basicStack->addWidget(page);
    }

    connect(basicTabBar, &NTabBar::currentChanged, basicStack, &NStackedWidget::setCurrentIndex);

    basicLayout->addWidget(basicTabBar);
    basicLayout->addWidget(basicStack);

    addSection("基本标签栏", basicLayout);

    QVBoxLayout* closableLayout = new QVBoxLayout();
    closableLayout->setSpacing(16);

    NTabBar* closableTabBar = new NTabBar(this);
    closableTabBar->setTabsClosable(true);
    closableTabBar->addTab("Tab 1");
    closableTabBar->addTab("Tab 2");
    closableTabBar->addTab("Tab 3");
    closableTabBar->addTab("Tab 4");

    NStackedWidget* closableStack = new NStackedWidget(this);
    closableStack->setMinimumHeight(150);

    for (int i = 0; i < 4; ++i) {
        QWidget* page = new QWidget(closableStack);
        QVBoxLayout* pageLayout = new QVBoxLayout(page);
        pageLayout->addWidget(new NLabel(QString("内容 %1").arg(i + 1), NLabelType::Title, page));
        pageLayout->addStretch();
        closableStack->addWidget(page);
    }

    connect(closableTabBar, &NTabBar::currentChanged, closableStack, &NStackedWidget::setCurrentIndex);
    connect(closableTabBar, &NTabBar::tabCloseRequested, [closableTabBar, closableStack](int index) {
        if (closableTabBar->count() > 1) {
            closableTabBar->removeTab(index);
            QWidget* widget = closableStack->widget(index);
            closableStack->removeWidget(widget);
            widget->deleteLater();
        }
    });

    closableLayout->addWidget(closableTabBar);
    closableLayout->addWidget(closableStack);

    addSection("可关闭的标签栏", closableLayout);

    QVBoxLayout* positionLayout = new QVBoxLayout();
    positionLayout->setSpacing(24);

    NTabBar* topTabBar = new NTabBar(this);
    topTabBar->setShape(QTabBar::RoundedNorth);
    topTabBar->addTab("Top 1");
    topTabBar->addTab("Top 2");
    topTabBar->addTab("Top 3");

    NTabBar* bottomTabBar = new NTabBar(this);
    bottomTabBar->setShape(QTabBar::RoundedSouth);
    bottomTabBar->addTab("Bottom 1");
    bottomTabBar->addTab("Bottom 2");
    bottomTabBar->addTab("Bottom 3");

    positionLayout->addWidget(new QLabel("顶部标签栏:", this));
    positionLayout->addWidget(topTabBar);
    positionLayout->addWidget(new QLabel("底部标签栏:", this));
    positionLayout->addWidget(bottomTabBar);

    addSection("不同位置", positionLayout);

    QVBoxLayout* interactiveLayout = new QVBoxLayout();
    interactiveLayout->setSpacing(16);

    NTabBar* interactiveTabBar = new NTabBar(this);
    interactiveTabBar->addTab("Tab 1");
    interactiveTabBar->addTab("Tab 2");
    interactiveTabBar->addTab("Tab 3");

    NStackedWidget* interactiveStack = new NStackedWidget(this);
    interactiveStack->setMinimumHeight(150);

    for (int i = 0; i < 3; ++i) {
        QWidget* page = new QWidget(interactiveStack);
        QVBoxLayout* pageLayout = new QVBoxLayout(page);
        pageLayout->addWidget(new NLabel(QString("页面 %1").arg(i + 1), NLabelType::Title, page));
        pageLayout->addStretch();
        interactiveStack->addWidget(page);
    }

    connect(interactiveTabBar, &NTabBar::currentChanged, interactiveStack, &NStackedWidget::setCurrentIndex);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(8);

    NPushButton* addTabButton = new NPushButton("添加标签", this);
    addTabButton->setFixedSize(100, 32);
    connect(addTabButton, &NPushButton::clicked, [interactiveTabBar, interactiveStack]() {
        int count = interactiveTabBar->count();
        interactiveTabBar->addTab(QString("New %1").arg(count + 1));
        
        QWidget* newPage = new QWidget(interactiveStack);
        QVBoxLayout* pageLayout = new QVBoxLayout(newPage);
        pageLayout->addWidget(new NLabel(QString("新页面 %1").arg(count + 1), NLabelType::Title, newPage));
        pageLayout->addStretch();
        interactiveStack->addWidget(newPage);
        
        interactiveTabBar->setCurrentIndex(count);
    });

    NPushButton* removeTabButton = new NPushButton("移除当前", this);
    removeTabButton->setFixedSize(100, 32);
    connect(removeTabButton, &NPushButton::clicked, [interactiveTabBar, interactiveStack]() {
        int currentIndex = interactiveTabBar->currentIndex();
        if (currentIndex >= 0 && interactiveTabBar->count() > 1) {
            interactiveTabBar->removeTab(currentIndex);
            QWidget* widget = interactiveStack->widget(currentIndex);
            interactiveStack->removeWidget(widget);
            widget->deleteLater();
        }
    });

    QLabel* currentLabel = new QLabel("当前标签: Tab 1", this);
    connect(interactiveTabBar, &NTabBar::currentChanged, [currentLabel, interactiveTabBar](int index) {
        currentLabel->setText(QString("当前标签: %1").arg(interactiveTabBar->tabText(index)));
    });

    controlLayout->addWidget(addTabButton);
    controlLayout->addWidget(removeTabButton);
    controlLayout->addWidget(currentLabel);
    controlLayout->addStretch();

    interactiveLayout->addWidget(interactiveTabBar);
    interactiveLayout->addWidget(interactiveStack);
    interactiveLayout->addLayout(controlLayout);

    addSection("交互示例", interactiveLayout);
}
