#include "PivotPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NPivot.h>
#include <QtNativeUI/NStackedWidget.h>

PivotPage::PivotPage(QWidget* parent)
    : BasePage("Pivot 枢轴控件", "Pivot 提供水平导航标签，常用于内容分类切换。", parent) {

    QVBoxLayout* basicLayout = new QVBoxLayout();
    basicLayout->setSpacing(16);

    NPivot* basicPivot = new NPivot(this);
    basicPivot->addItem("首页");
    basicPivot->addItem("文档");
    basicPivot->addItem("设置");

    NStackedWidget* basicStack = new NStackedWidget(this);
    basicStack->setMinimumHeight(150);

    QWidget* page1 = new QWidget(basicStack);
    QVBoxLayout* page1Layout = new QVBoxLayout(page1);
    page1Layout->addWidget(new NLabel("首页内容", NLabelType::Title, page1));
    page1Layout->addStretch();

    QWidget* page2 = new QWidget(basicStack);
    QVBoxLayout* page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(new NLabel("文档内容", NLabelType::Title, page2));
    page2Layout->addStretch();

    QWidget* page3 = new QWidget(basicStack);
    QVBoxLayout* page3Layout = new QVBoxLayout(page3);
    page3Layout->addWidget(new NLabel("设置内容", NLabelType::Title, page3));
    page3Layout->addStretch();

    basicStack->addWidget(page1);
    basicStack->addWidget(page2);
    basicStack->addWidget(page3);

    connect(basicPivot, &NPivot::currentIndexChanged, basicStack, &NStackedWidget::setCurrentIndex);

    basicLayout->addWidget(basicPivot);
    basicLayout->addWidget(basicStack);

    addSection("基本 Pivot", basicLayout);

    QVBoxLayout* iconLayout = new QVBoxLayout();
    iconLayout->setSpacing(16);

    NPivot* iconPivot = new NPivot(this);
    iconPivot->addItem(NRegularIconType::Home24Regular, "首页", 20);
    iconPivot->addItem(NRegularIconType::Document24Regular, "文档", 20);
    iconPivot->addItem(NRegularIconType::Settings24Regular, "设置", 20);

    NStackedWidget* iconStack = new NStackedWidget(this);
    iconStack->setMinimumHeight(150);

    for (int i = 0; i < 3; ++i) {
        QWidget* page = new QWidget(iconStack);
        QVBoxLayout* pageLayout = new QVBoxLayout(page);
        pageLayout->addWidget(new NLabel(QString("页面 %1").arg(i + 1), NLabelType::Title, page));
        pageLayout->addStretch();
        iconStack->addWidget(page);
    }

    connect(iconPivot, &NPivot::currentIndexChanged, iconStack, &NStackedWidget::setCurrentIndex);

    iconLayout->addWidget(iconPivot);
    iconLayout->addWidget(iconStack);

    addSection("带图标的 Pivot", iconLayout);

    QVBoxLayout* badgeLayout = new QVBoxLayout();
    badgeLayout->setSpacing(16);

    NPivot* badgePivot = new NPivot(this);
    badgePivot->addItem("消息");
    badgePivot->addItem("通知");
    badgePivot->addItem("设置");

    badgePivot->setItemBadge(0, 5);
    badgePivot->setItemBadge(1, 12);

    NStackedWidget* badgeStack = new NStackedWidget(this);
    badgeStack->setMinimumHeight(150);

    for (int i = 0; i < 3; ++i) {
        QWidget* page = new QWidget(badgeStack);
        QVBoxLayout* pageLayout = new QVBoxLayout(page);
        pageLayout->addWidget(new NLabel(QString("内容 %1").arg(i + 1), NLabelType::Title, page));
        pageLayout->addStretch();
        badgeStack->addWidget(page);
    }

    connect(badgePivot, &NPivot::currentIndexChanged, badgeStack, &NStackedWidget::setCurrentIndex);

    badgeLayout->addWidget(badgePivot);
    badgeLayout->addWidget(badgeStack);

    addSection("带徽章的 Pivot", badgeLayout);

    QVBoxLayout* alignmentLayout = new QVBoxLayout();
    alignmentLayout->setSpacing(24);

    NPivot* leftPivot = new NPivot(this);
    leftPivot->setAlignment(NPivotType::Left);
    leftPivot->addItem("左对齐 1");
    leftPivot->addItem("左对齐 2");
    leftPivot->addItem("左对齐 3");

    NPivot* centerPivot = new NPivot(this);
    centerPivot->setAlignment(NPivotType::Center);
    centerPivot->addItem("居中 1");
    centerPivot->addItem("居中 2");
    centerPivot->addItem("居中 3");

    NPivot* rightPivot = new NPivot(this);
    rightPivot->setAlignment(NPivotType::Right);
    rightPivot->addItem("右对齐 1");
    rightPivot->addItem("右对齐 2");
    rightPivot->addItem("右对齐 3");

    alignmentLayout->addWidget(new QLabel("左对齐:", this));
    alignmentLayout->addWidget(leftPivot);
    alignmentLayout->addWidget(new QLabel("居中:", this));
    alignmentLayout->addWidget(centerPivot);
    alignmentLayout->addWidget(new QLabel("右对齐:", this));
    alignmentLayout->addWidget(rightPivot);

    addSection("不同对齐方式", alignmentLayout);
}
