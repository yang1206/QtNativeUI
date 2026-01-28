#include "StackedWidgetPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NStackedWidget.h>

StackedWidgetPage::StackedWidgetPage(QWidget* parent)
    : BasePage("StackedWidget 堆叠控件", "StackedWidget 用于在同一区域显示多个页面，一次只显示一个页面，支持页面切换动画。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NStackedWidget* stackedWidget = new NStackedWidget(this);
    stackedWidget->setMinimumHeight(200);
    stackedWidget->setMinimumWidth(400);

    QWidget* page1 = new QWidget(stackedWidget);
    QVBoxLayout* page1Layout = new QVBoxLayout(page1);
    page1Layout->addWidget(new NLabel("这是第一页", NLabelType::Title, page1));
    page1Layout->addWidget(new NLabel("页面内容 1", NLabelType::Body, page1));
    page1Layout->addStretch();

    QWidget* page2 = new QWidget(stackedWidget);
    QVBoxLayout* page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(new NLabel("这是第二页", NLabelType::Title, page2));
    page2Layout->addWidget(new NLabel("页面内容 2", NLabelType::Body, page2));
    page2Layout->addStretch();

    QWidget* page3 = new QWidget(stackedWidget);
    QVBoxLayout* page3Layout = new QVBoxLayout(page3);
    page3Layout->addWidget(new NLabel("这是第三页", NLabelType::Title, page3));
    page3Layout->addWidget(new NLabel("页面内容 3", NLabelType::Body, page3));
    page3Layout->addStretch();

    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);
    stackedWidget->addWidget(page3);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(8);

    NPushButton* prevButton = new NPushButton("上一页", this);
    prevButton->setFixedSize(80, 32);
    connect(prevButton, &NPushButton::clicked, [stackedWidget]() {
        int currentIndex = stackedWidget->currentIndex();
        if (currentIndex > 0) {
            stackedWidget->setCurrentIndex(currentIndex - 1);
        }
    });

    NPushButton* nextButton = new NPushButton("下一页", this);
    nextButton->setFixedSize(80, 32);
    connect(nextButton, &NPushButton::clicked, [stackedWidget]() {
        int currentIndex = stackedWidget->currentIndex();
        if (currentIndex < stackedWidget->count() - 1) {
            stackedWidget->setCurrentIndex(currentIndex + 1);
        }
    });

    QLabel* pageLabel = new QLabel("当前页: 1 / 3", this);
    connect(stackedWidget, &NStackedWidget::currentChanged, [pageLabel](int index) {
        pageLabel->setText(QString("当前页: %1 / 3").arg(index + 1));
    });

    controlLayout->addWidget(prevButton);
    controlLayout->addWidget(nextButton);
    controlLayout->addWidget(pageLabel);
    controlLayout->addStretch();

    basicLayout->addWidget(stackedWidget);
    basicLayout->addLayout(controlLayout);

    addSection("基本堆叠控件", basicLayout);

    QVBoxLayout* animationLayout = new QVBoxLayout();
    animationLayout->setSpacing(16);

    NStackedWidget* animatedWidget = new NStackedWidget(this);
    animatedWidget->setMinimumHeight(200);
    animatedWidget->setMinimumWidth(400);

    QWidget* animPage1 = new QWidget(animatedWidget);
    QVBoxLayout* animPage1Layout = new QVBoxLayout(animPage1);
    animPage1Layout->addWidget(new NLabel("动画页面 1", NLabelType::Title, animPage1));
    animPage1Layout->addStretch();

    QWidget* animPage2 = new QWidget(animatedWidget);
    QVBoxLayout* animPage2Layout = new QVBoxLayout(animPage2);
    animPage2Layout->addWidget(new NLabel("动画页面 2", NLabelType::Title, animPage2));
    animPage2Layout->addStretch();

    QWidget* animPage3 = new QWidget(animatedWidget);
    QVBoxLayout* animPage3Layout = new QVBoxLayout(animPage3);
    animPage3Layout->addWidget(new NLabel("动画页面 3", NLabelType::Title, animPage3));
    animPage3Layout->addStretch();

    animatedWidget->addWidget(animPage1);
    animatedWidget->addWidget(animPage2);
    animatedWidget->addWidget(animPage3);

    QHBoxLayout* animControlLayout = new QHBoxLayout();
    animControlLayout->setSpacing(8);

    NPushButton* popupButton = new NPushButton("弹出过渡", this);
    popupButton->setFixedSize(100, 32);
    connect(popupButton, &NPushButton::clicked, [animatedWidget]() {
        int nextIndex = (animatedWidget->currentIndex() + 1) % animatedWidget->count();
        animatedWidget->doPageSwitch(NNavigationType::PopupTransition, nextIndex, false, 300);
    });

    NPushButton* scaleButton = new NPushButton("缩放过渡", this);
    scaleButton->setFixedSize(100, 32);
    connect(scaleButton, &NPushButton::clicked, [animatedWidget]() {
        int nextIndex = (animatedWidget->currentIndex() + 1) % animatedWidget->count();
        animatedWidget->doPageSwitch(NNavigationType::ScaleTransition, nextIndex, false, 300);
    });

    NPushButton* flipButton = new NPushButton("翻转过渡", this);
    flipButton->setFixedSize(100, 32);
    connect(flipButton, &NPushButton::clicked, [animatedWidget]() {
        int nextIndex = (animatedWidget->currentIndex() + 1) % animatedWidget->count();
        animatedWidget->doPageSwitch(NNavigationType::FlipTransition, nextIndex, false, 300);
    });

    QLabel* animPageLabel = new QLabel("当前页: 1 / 3", this);
    connect(animatedWidget, &NStackedWidget::currentChanged, [animPageLabel](int index) {
        animPageLabel->setText(QString("当前页: %1 / 3").arg(index + 1));
    });

    animControlLayout->addWidget(popupButton);
    animControlLayout->addWidget(scaleButton);
    animControlLayout->addWidget(flipButton);
    animControlLayout->addWidget(animPageLabel);
    animControlLayout->addStretch();

    animationLayout->addWidget(animatedWidget);
    animationLayout->addLayout(animControlLayout);

    addSection("带动画的页面切换", animationLayout);
}
