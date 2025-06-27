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

    d->_pLightSelectedBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkSelectedBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);

    d->_pLightBorderColor = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

    d->_pBorderWidth = 1;

    NTabBar* tabBar = new NTabBar(this);
    setTabBar(tabBar);
    if (QSysInfo::productType() == "macos") {
        setDocumentMode(true);
    }
    setAutoFillBackground(false);

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

    setPalette(pal);
    for (int i = 0; i < count(); ++i) {
        QWidget* widget = this->widget(i);
        if (widget) {
            widget->setAutoFillBackground(true);
            widget->setPalette(pal);
        }
    }

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
    QColor   bgColor      = d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground;
    QColor   borderColor  = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
    int      borderRadius = d->_pBorderRadius;
    int      borderWidth  = d->_pBorderWidth;
    QRect    contentRect  = rect();
    QTabBar* tb           = tabBar();
    QRect    tabBarRect   = tb->geometry();

    QPainterPath path;

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

    path.addRoundedRect(contentRect, borderRadius, borderRadius);

    painter.fillPath(path, bgColor);

    if (borderWidth > 0) {
        QPen pen(borderColor, borderWidth);
        painter.setPen(pen);
        painter.drawPath(path);
    }
}

void NTabWidget::resizeEvent(QResizeEvent* event) {
    QTabWidget::resizeEvent(event);
    adjustContentMargins();
}

void NTabWidget::tabInserted(int index) {
    QTabWidget::tabInserted(index);

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
    Q_D(NTabWidget);
    int margin = d->_pBorderRadius / 2;

    QWidget* container = this->findChild<QWidget*>("qt_tabwidget_stackedwidget");
    if (container) {
        container->setContentsMargins(margin, margin, margin, margin);
    }
}

void NTabWidget::showEvent(QShowEvent* event) {
    QTabWidget::showEvent(event);

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