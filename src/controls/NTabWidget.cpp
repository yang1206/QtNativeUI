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

    // 如果需要，添加额外样式
    if (QSysInfo::productType() == "macos") {
        // 添加macOS特定的样式覆盖
        setDocumentMode(true); // 这在macOS上会禁用某些特定样式
    }
    updatePalette();

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