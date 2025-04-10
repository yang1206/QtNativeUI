#include <QPainter>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QtNativeUI/NTabWidget.h>
#include <QtNativeUI/NTabBar.h>
#include "../private/ntabwidget_p.h"
#include "QtNativeUI/NTheme.h"

// 属性实现
// ...

NTabWidget::NTabWidget(QWidget* parent) : QWidget(parent), d_ptr(new NTabWidgetPrivate()) {
    init();
}

NTabWidget::~NTabWidget() {}

void NTabWidget::init() {
    Q_D(NTabWidget);
    d->q_ptr = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark = nTheme->isDarkMode();

    setObjectName("NTabWidget");

    // 初始化颜色和属性
    // ...

    // 创建和配置组件
    d->_tabBar = new NTabBar(this);
    d->_stack = new QStackedWidget(this);
    
    // 设置布局
    d->updateLayout();
    
    // 连接信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NTabWidget);
        d->_themeMode = themeMode;
        d->_isDark = nTheme->isDarkMode();
        update();
    });
    
    // 连接其他信号
    // ...
}

// 公共方法实现
// ...

// 事件处理实现
void NTabWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制逻辑
    // ...
}

void NTabWidget::resizeEvent(QResizeEvent* event) {
    // 尺寸变化事件处理
    QWidget::resizeEvent(event);
    
    // 更新布局
    Q_D(NTabWidget);
    d->updateLayout();
}

void NTabWidget::showEvent(QShowEvent* event) {
    // 显示事件处理
    QWidget::showEvent(event);
}

void NTabWidget::changeEvent(QEvent* event) {
    // 状态变化事件处理
    QWidget::changeEvent(event);
}