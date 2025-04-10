#include "ntabwidget_p.h"
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
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
}
