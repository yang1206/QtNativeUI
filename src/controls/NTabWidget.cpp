#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include "../private/ntabwidget_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, LightSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, DarkSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, int, BorderWidth)

NTabWidget::NTabWidget(QWidget* parent) : QTabWidget(parent), d_ptr(new NTabWidgetPrivate()) { init(); }

NTabWidget::~NTabWidget() {}

void NTabWidget::init() {
    Q_D(NTabWidget);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NTabWidget");

    // 设置背景颜色
    d->_pLightSelectedBackground = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkSelectedBackground  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);

    // 设置边框颜色
    d->_pLightBorderColor = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);

    // 设置边框圆角 - 使用默认的圆角值
    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

    // 设置边框宽度
    d->_pBorderWidth = 1;

    // 创建和设置NTabBar
    NTabBar* tabBar = new NTabBar(this);
    setTabBar(tabBar);
    // Mac特殊处理
    if (QSysInfo::productType() == "macos") {
        setDocumentMode(true);
    }

    // 允许自定义绘制
    setAutoFillBackground(false);

    // 设置内部小部件的样式
    updatePalette();

    NTabBar* nTabBar = qobject_cast<NTabBar*>(this->tabBar());
    if (nTabBar) {
        bool currentClosable = tabsClosable();
        if (currentClosable) {
            nTabBar->setTabsClosable(currentClosable);
        }

        connect(nTabBar, &QTabBar::tabCloseRequested, this, &QTabWidget::tabCloseRequested);
    }

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NTabWidget);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updatePalette();
        update();
    });
}

void NTabWidget::updatePalette() {
    Q_D(NTabWidget);

    QPalette pal = palette();

    QColor bgColor = d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground;
    pal.setColor(QPalette::Window, bgColor);
    pal.setColor(QPalette::Base, bgColor);

    QColor textColor = d->_isDark ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark)
                                  : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    pal.setColor(QPalette::WindowText, textColor);
    pal.setColor(QPalette::Text, textColor);

    // 应用调色板
    setPalette(pal);

    // 更新内容区域背景
    for (int i = 0; i < count(); ++i) {
        QWidget* widget = this->widget(i);
        if (widget) {
            widget->setAutoFillBackground(true);
            widget->setPalette(pal);
        }
    }

    // 更新页面容器控件的调色板
    QWidget* container = this->findChild<QWidget*>("qt_tabwidget_stackedwidget");
    if (container) {
        container->setAutoFillBackground(true);
        container->setPalette(pal);
    }
}

void NTabWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    Q_D(NTabWidget);
    QColor bgColor      = d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground;
    QColor borderColor  = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
    int    borderRadius = d->_pBorderRadius;
    int    borderWidth  = d->_pBorderWidth;

    // 计算内容区域
    QRect    contentRect = rect();
    QTabBar* tb          = tabBar();
    QRect    tabBarRect  = tb->geometry();

    // 创建一个用于绘制的路径
    QPainterPath path;

    // 处理不同的标签位置
    switch (tabPosition()) {
        case QTabWidget::North:
            contentRect.setTop(tabBarRect.bottom() + 1);
            break;
        case QTabWidget::South:
            contentRect.setBottom(tabBarRect.top() - 1);
            break;
        case QTabWidget::East:
            contentRect.setRight(tabBarRect.left() - 1);
            break;
        case QTabWidget::West:
            contentRect.setLeft(tabBarRect.right() + 1);
            break;
    }

    // 创建具有圆角的路径
    path.addRoundedRect(contentRect, borderRadius, borderRadius);

    // 绘制背景
    painter.fillPath(path, bgColor);

    // 设置边框画笔
    if (borderWidth > 0) {
        QPen pen(borderColor, borderWidth);
        painter.setPen(pen);
        painter.drawPath(path);
    }

    // 不调用基类的绘制方法，因为我们完全自定义绘制
    // QTabWidget::paintEvent(event);
}

void NTabWidget::resizeEvent(QResizeEvent* event) {
    QTabWidget::resizeEvent(event);

    // 确保标签栏和内容区域之间有正确的空间
    adjustContentMargins();
}

void NTabWidget::tabInserted(int index) {
    QTabWidget::tabInserted(index);

    // 当添加新标签页时，确保它继承正确的调色板
    QWidget* widget = this->widget(index);
    if (widget) {
        Q_D(NTabWidget);
        QColor bgColor = d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground;

        QPalette pal = widget->palette();
        pal.setColor(QPalette::Window, bgColor);
        pal.setColor(QPalette::Base, bgColor);

        widget->setAutoFillBackground(true);
        widget->setPalette(pal);
    }

    adjustContentMargins();
}

void NTabWidget::adjustContentMargins() {
    // 根据边框圆角调整内容区域的边距
    Q_D(NTabWidget);
    int margin = d->_pBorderRadius / 2;

    // 找到内容区域的容器控件
    QWidget* container = this->findChild<QWidget*>("qt_tabwidget_stackedwidget");
    if (container) {
        container->setContentsMargins(margin, margin, margin, margin);
    }
}

void NTabWidget::showEvent(QShowEvent* event) {
    QTabWidget::showEvent(event);

    // 当控件首次显示时调整边距
    adjustContentMargins();
}

void NTabWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange ||
        event->type() == QEvent::PaletteChange) {
        updatePalette();
        update();
    }
    QTabWidget::changeEvent(event);
}

void NTabWidget::setTabsClosable(bool closable) {
    NTabBar* nTabBar = qobject_cast<NTabBar*>(QTabWidget::tabBar());
    if (nTabBar) {
        nTabBar->setTabsClosable(closable);
    }
}

bool NTabWidget::tabsClosable() const {
    NTabBar* nTabBar = qobject_cast<NTabBar*>(QTabWidget::tabBar());
    if (nTabBar) {
        return nTabBar->tabsClosable();
    }
    return QTabWidget::tabsClosable();
}

NTabBar* NTabWidget::tabBar() const { return static_cast<NTabBar*>(QTabWidget::tabBar()); }