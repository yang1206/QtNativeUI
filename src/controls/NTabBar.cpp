#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionTab>
#include <QtNativeUI/NTabBar.h>
#include "../private/ntabbar_p.h"
#include "../private/ntabbarstyle.h"
#include "QtNativeUI/NTheme.h"

// 属性实现
Q_PROPERTY_CREATE_Q_CPP(NTabBar, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, bool, OverlayMode)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, SelectedTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, UnselectedTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, HoverTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DisabledTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, SelectedBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, HoverBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, BorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, SeparatorColor)

NTabBar::NTabBar(QWidget* parent) : QTabBar(parent), d_ptr(new NTabBarPrivate()) { init(); }

NTabBar::~NTabBar() {}

void NTabBar::init() {
    Q_D(NTabBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NTabBar");
    setStyleSheet("#NTabBar{background-color:transparent;}");

    // 优化颜色定义
    d->_pSelectedTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pSelectedTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pUnselectedTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pUnselectedTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);

    d->_pHoverTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pHoverTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pDisabledTextColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDisabledTextColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    // 亮色主题使用SolidBackgroundFillColorTertiary作为选中背景
    // 暗色主题使用SolidBackgroundFillColorQuarternary作为选中背景
    d->_pSelectedBackgroundColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pSelectedBackgroundColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorQuarternary, NThemeType::Dark);

    // 使用SubtleFillColorSecondary作为悬停背景色，保持一致性
    d->_pHoverBackgroundColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pHoverBackgroundColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);

    d->_pBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    d->_pSeparatorColor = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pSeparatorColor = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pOverlayMode  = false;

    // 设置标签属性
    setTabsClosable(true);
    setMovable(true);
    setAcceptDrops(true);

    // 应用自定义样式
    setStyle(new NTabBarStyle(style()));

    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NTabBar);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

int NTabBar::hoverIndex() const {
    Q_D(const NTabBar);
    return d->_hoverIndex;
}

void NTabBar::mousePressEvent(QMouseEvent* event) {
    Q_D(NTabBar);
    QTabBar::mousePressEvent(event);
    d->_lastDragPix = grab(tabRect(currentIndex()));
    Q_EMIT tabBarPressed(currentIndex());
}

void NTabBar::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NTabBar);

    // 检测是否应该开始拖拽
    QRect moveRect = rect();
    moveRect.adjust(0, -height(), 0, height());
    QPoint currentPos = event->pos();

    if (currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y()) {
        QDrag* drag = new QDrag(this);
        drag->setPixmap(d->_lastDragPix);
        qreal  pixRatio  = d->_lastDragPix.devicePixelRatioF();
        QPoint pixCenter = d->_lastDragPix.rect().center();
        drag->setHotSpot(pixCenter / pixRatio);

        QMimeData* data = new QMimeData;
        data->setProperty("DragType", "NTabBarDrag");
        data->setProperty("TabIndex", currentIndex());
        data->setProperty("TabIcon", tabIcon(currentIndex()));
        data->setProperty("TabText", tabText(currentIndex()));
        data->setProperty("NTabBarObject", QVariant::fromValue(this));
        data->setProperty("QDragObject", QVariant::fromValue(drag));

        drag->setMimeData(data);
        Q_EMIT tabDragStarted(drag);
    }

    // 更新悬停索引
    int oldHoverIndex = d->_hoverIndex;
    d->_hoverIndex    = tabAt(event->pos());

    if (oldHoverIndex != d->_hoverIndex) {
        update();
    }

    QTabBar::mouseMoveEvent(event);
}

void NTabBar::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->property("DragType").toString() == "NTabBarDrag") {
        event->acceptProposedAction();
    }
    QTabBar::dragEnterEvent(event);
}

void NTabBar::dropEvent(QDropEvent* event) {
    if (event->mimeData()->property("NTabBarObject").value<NTabBar*>() != this) {
        event->accept();
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif

        Q_EMIT tabDropped(data);
    }
    QTabBar::dropEvent(event);
}

void NTabBar::leaveEvent(QEvent* event) {
    Q_D(NTabBar);

    if (d->_hoverIndex != -1) {
        d->_hoverIndex = -1;
        update();
    }

    QTabBar::leaveEvent(event);
}

void NTabBar::changeEvent(QEvent* event) {
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange ||
        event->type() == QEvent::PaletteChange) {
        update();
    }

    QTabBar::changeEvent(event);
}