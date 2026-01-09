//
// Created by Yang1206 on 2025/12/29.
//

#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NScrollBar.h>
#include <QtNativeUI/NTheme.h>
#include "../private/nlistview_p.h"

Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, ItemHeight)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, ItemBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, BorderRadius)

NListView::NListView(QWidget* parent) : QListView(parent), d_ptr(new NListViewPrivate()) {
    Q_D(NListView);
    d->q_ptr = this;
    init();
}

NListView::~NListView() {}

void NListView::init() {
    Q_D(NListView);

    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::LayerFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::LayerFillColorDefault, NThemeType::Dark);

    d->_pLightItemHoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHoverColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);

    d->_pLightItemSelectedColor = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkItemSelectedColor  = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);

    d->_pLightItemPressedColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemPressedColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pLightBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    d->_pItemHeight       = 36;
    d->_pItemBorderRadius = 4;
    d->_pBorderRadius     = 8;

    d->isDark    = nTheme->isDarkMode();
    d->themeMode = nTheme->themeMode();
    d->initStyle();

    setMouseTracking(true);
    setViewportMargins(4, 4, 4, 4);
    setVerticalScrollBar(new NScrollBar(this));
    setHorizontalScrollBar(new NScrollBar(this));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NListView);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        d->updateStyle();
    });
}
