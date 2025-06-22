//
// Created by Yang1206 on 2025/4/6.
//

#include <QAbstractScrollArea>
#include <QEnterEvent>
#include <QStyleOptionSlider>
#include <QtNativeUI/NScrollBar.h>
#include "../private/nscrollbar_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightThumbColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkThumbColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightThumbHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkThumbHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightThumbPressColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkThumbPressColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightArrowColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkArrowColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightArrowHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkArrowHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, LightArrowPressColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, QColor, DarkArrowPressColor)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, int, ThumbThickness)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, int, ThumbNormalThickness)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, int, TrackThickness)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, int, ThumbCornerRadius)
Q_PROPERTY_CREATE_Q_CPP(NScrollBar, int, TrackCornerRadius)

NScrollBar::NScrollBar(QWidget* parent) : QScrollBar(parent), d_ptr(new NScrollBarPrivate()) {
    Q_D(NScrollBar);
    d->q_ptr = this;
    init();
}

NScrollBar::NScrollBar(Qt::Orientation orientation, QWidget* parent)
    : QScrollBar(orientation, parent), d_ptr(new NScrollBarPrivate()) {
    Q_D(NScrollBar);
    d->q_ptr = this;
    init();
}

NScrollBar::NScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent) : NScrollBar(parent) {
    Q_D(NScrollBar);
    d->q_ptr = this;
    init();
    if (!originScrollBar || !parent) {
        qCritical() << "Invalid origin or parent!";
        return;
    }
    d->_originScrollArea        = parent;
    Qt::Orientation orientation = originScrollBar->orientation();
    setOrientation(orientation);
    // 隐藏原始滚动条
    orientation == Qt::Horizontal ? parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff)
                                  : parent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    parent->installEventFilter(this);
    d->_originScrollBar = originScrollBar;
    d->_initAllConfig();
    // 连接信号
    connect(d->_originScrollBar, &QScrollBar::valueChanged, this, [=](int value) {
        d->_handleScrollBarValueChanged(this, value);
    });

    connect(this, &QScrollBar::valueChanged, this, [=](int value) {
        d->_handleScrollBarValueChanged(d->_originScrollBar, value);
    });

    connect(d->_originScrollBar, &QScrollBar::rangeChanged, this, [=](int min, int max) {
        d->_handleScrollBarRangeChanged(min, max);
    });
}

NScrollBar::~NScrollBar() {}

void NScrollBar::init() {
    Q_D(NScrollBar);

    // 初始化颜色属性
    d->_pLightTrackColor = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    d->_pDarkTrackColor  = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);

    // 半透明的滑块颜色
    d->_pLightThumbColor = QColor(NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light));
    d->_pLightThumbColor.setAlphaF(0.3);
    d->_pDarkThumbColor = QColor(NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark));
    d->_pDarkThumbColor.setAlphaF(0.3);

    // 悬浮和按下状态的不透明滑块颜色
    d->_pLightThumbHoverColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkThumbHoverColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);

    d->_pLightThumbPressColor = NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, NThemeType::Light);
    d->_pDarkThumbPressColor  = NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, NThemeType::Dark);
    // 箭头颜色
    d->_pLightArrowColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkArrowColor  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);

    d->_pLightArrowHoverColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkArrowHoverColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pLightArrowPressColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkArrowPressColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    // 初始化尺寸属性
    d->_pTrackThickness       = 12; // 轨道厚度
    d->_pThumbThickness       = 6;  // 悬浮/按下时的厚度
    d->_pThumbNormalThickness = 3;  // 正常状态下的厚度
    d->_pThumbCornerRadius    = 4;  // 滑块圆角
    d->_pTrackCornerRadius    = 6;  // 轨道圆角

    // 初始化样式
    d->initScrollBarStyle();

    // 监听主题变化
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NScrollBar);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        d->updateStyle();
    });
}

void NScrollBar::enterEvent(QEnterEvent* event) {
    Q_D(NScrollBar);
    d->isHovered = true;
    d->updateStyle();
    QScrollBar::enterEvent(event);
}

void NScrollBar::leaveEvent(QEvent* event) {
    Q_D(NScrollBar);
    d->isHovered = false;
    d->updateStyle();
    QScrollBar::leaveEvent(event);
}

void NScrollBar::mousePressEvent(QMouseEvent* event) {
    Q_D(NScrollBar);
    d->isPressed = true;
    d->updateStyle();
    QScrollBar::mousePressEvent(event);
}

void NScrollBar::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NScrollBar);
    d->isPressed = false;
    d->updateStyle();
    QScrollBar::mouseReleaseEvent(event);
}

void NScrollBar::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NScrollBar);

    // 获取鼠标所在位置的子控件
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QStyle::SubControl subControl = style()->hitTestComplexControl(QStyle::CC_ScrollBar, &opt, event->pos(), this);

    // 如果子控件发生变化，更新悬浮状态
    if (subControl != d->hoveredSubControl) {
        // 重置旧悬浮状态
        if (d->hoveredSubControl == QStyle::SC_ScrollBarAddLine ||
            d->hoveredSubControl == QStyle::SC_ScrollBarSubLine) {
            d->style->setLineHovered(d->hoveredSubControl, false);
        }

        // 设置新悬浮状态
        if (subControl == QStyle::SC_ScrollBarAddLine || subControl == QStyle::SC_ScrollBarSubLine) {
            d->style->setLineHovered(subControl, true);
        }

        d->hoveredSubControl = subControl;
        d->updateStyle();
    }

    QScrollBar::mouseMoveEvent(event);
}

void NScrollBar::changeEvent(QEvent* event) {
    QScrollBar::changeEvent(event);
    if (event->type() == QEvent::StyleChange) {
        Q_D(NScrollBar);
        d->updateStyle();
    }
}

bool NScrollBar::eventFilter(QObject* watched, QEvent* event) {
    Q_D(NScrollBar);
    switch (event->type()) {
        case QEvent::Show:
        case QEvent::Resize:
        case QEvent::LayoutRequest:
            d->_handleScrollBarGeometry();
            break;
        default:
            break;
    }
    return QScrollBar::eventFilter(watched, event);
}