#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionTab>
#include <QtNativeUI/NTabBar.h>
#include "../private/ntabbar_p.h"
#include "../private/ntabbarstyle.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NTabBar, int, BorderRadius)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemSeparator)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemSeparator)

NTabBar::NTabBar(QWidget* parent) : QTabBar(parent), d_ptr(new NTabBarPrivate()) { init(); }

NTabBar::~NTabBar() {}

void NTabBar::init() {
    Q_D(NTabBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NTabBar");
    setStyleSheet("#NTabBar{background-color:transparent;}");

    d->_pLightItemHeaderBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundSelected =
        NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundSelected =
        NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundPointerOver =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundPointerOver =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundPressed =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundPressed =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundDisabled =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundDisabled =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);

    d->_pLightItemHeaderForeground            = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderForeground             = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundSelected    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundSelected     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundPointerOver = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundPointerOver  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundPressed     = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundPressed      = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundDisabled    = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkItemHeaderForegroundDisabled     = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightItemIconForeground            = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemIconForeground             = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemIconForegroundSelected    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkItemIconForegroundSelected     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightItemIconForegroundPointerOver = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemIconForegroundPointerOver  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemIconForegroundPressed     = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkItemIconForegroundPressed      = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightItemIconForegroundDisabled    = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkItemIconForegroundDisabled     = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightItemSeparator = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkItemSeparator  = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

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