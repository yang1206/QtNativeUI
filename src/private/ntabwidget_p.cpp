#include "ntabwidget_p.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtNativeUI/NTabWidget.h>
#include <QtNativeUI/NTabBar.h>
#include "QtNativeUI/NTheme.h"

NTabWidgetPrivate::NTabWidgetPrivate(QObject* parent) : QObject(parent) {
    // 初始化
}

NTabWidgetPrivate::~NTabWidgetPrivate() {
    // 清理资源
}

void NTabWidgetPrivate::updateLayout() {
    if (!q_ptr || !_tabBar || !_stack)
        return;

    // 移除现有布局
    if (q_ptr->layout()) {
        delete q_ptr->layout();
    }

    // 根据标签位置创建布局
    QBoxLayout* mainLayout = nullptr;
    
    switch (_tabPosition) {
    case Qt::TopPosition:
        mainLayout = new QVBoxLayout(q_ptr);
        mainLayout->addWidget(_tabBar);
        mainLayout->addWidget(_stack);
        break;
    case Qt::BottomPosition:
        mainLayout = new QVBoxLayout(q_ptr);
        mainLayout->addWidget(_stack);
        mainLayout->addWidget(_tabBar);
        break;
    case Qt::LeftPosition:
        mainLayout = new QHBoxLayout(q_ptr);
        mainLayout->addWidget(_tabBar);
        mainLayout->addWidget(_stack);
        break;
    case Qt::RightPosition:
        mainLayout = new QHBoxLayout(q_ptr);
        mainLayout->addWidget(_stack);
        mainLayout->addWidget(_tabBar);
        break;
    }
    
    if (mainLayout) {
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        q_ptr->setLayout(mainLayout);
    }
}

// 私有方法实现
// ...