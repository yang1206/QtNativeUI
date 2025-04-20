#include "ntabwidget_p.h"
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include "QtNativeUI/NTheme.h"

NTabWidgetPrivate::NTabWidgetPrivate(QObject* parent) : QObject(parent) {}

NTabWidgetPrivate::~NTabWidgetPrivate() {}

void NTabWidgetPrivate::updateLayout() {
    if (!q_ptr || !_tabBar || !_stack)
        return;

    if (q_ptr->layout()) {
        delete q_ptr->layout();
    }
    
    // 移除样式表，我们使用调色板
    _stack->setStyleSheet(""); 
    
    // 确保堆栈小部件能够显示背景色
    _stack->setAutoFillBackground(true);
    
    switch (_pTabPosition) {
        case NTabWidget::North: {
            QVBoxLayout* layout = new QVBoxLayout(q_ptr);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            layout->addWidget(_tabBar);
            layout->addWidget(_stack);
            break;
        }
        case NTabWidget::South: {
            QVBoxLayout* layout = new QVBoxLayout(q_ptr);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            layout->addWidget(_stack);
            layout->addWidget(_tabBar);
            break;
        }
        case NTabWidget::West: {
            QHBoxLayout* layout = new QHBoxLayout(q_ptr);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            layout->addWidget(_tabBar);
            layout->addWidget(_stack);
            break;
        }
        case NTabWidget::East: {
            QHBoxLayout* layout = new QHBoxLayout(q_ptr);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            layout->addWidget(_stack);
            layout->addWidget(_tabBar);
            break;
        }
    }

    _tabBar->setShape(positionToShape(static_cast<NTabWidget::TabPosition>(_pTabPosition)));
}

QTabBar::Shape NTabWidgetPrivate::positionToShape(NTabWidget::TabPosition pos) const {
    switch (pos) {
        case NTabWidget::North:
            return QTabBar::RoundedNorth;
        case NTabWidget::South:
            return QTabBar::RoundedSouth;
        case NTabWidget::West:
            return QTabBar::RoundedWest;
        case NTabWidget::East:
            return QTabBar::RoundedEast;
        default:
            return QTabBar::RoundedNorth;
    }
}
