#include <QPainter>
#include <QResizeEvent>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include "../private/ntabwidget_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, LightSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, DarkSelectedBackground)

NTabWidget::NTabWidget(QWidget* parent) : QTabWidget(parent), d_ptr(new NTabWidgetPrivate()) { init(); }

NTabWidget::~NTabWidget() {}

void NTabWidget::init() {
    Q_D(NTabWidget);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NTabWidget");

    d->_pLightSelectedBackground = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkSelectedBackground  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);

    NTabBar* tabBar = new NTabBar(this);
    setTabBar(tabBar);

    if (QSysInfo::productType() == "macos") {
        setDocumentMode(true);
    }
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
    pal.setColor(QPalette::Window, d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground);
    setPalette(pal);
}

void NTabWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange ||
        event->type() == QEvent::PaletteChange) {
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