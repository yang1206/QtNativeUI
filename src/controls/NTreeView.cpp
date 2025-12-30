//
// Created by Yang1206 on 2025/12/30.
//

#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NScrollBar.h>
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NTreeView.h>
#include "../private/ntreeview_p.h"

Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkBorderColor)

void NTreeView::setItemHeight(int ItemHeight) {
    Q_D(NTreeView);
    d->_pItemHeight = ItemHeight;
    d->updateStyle();
    doItemsLayout();
    Q_EMIT pItemHeightChanged();
}
int NTreeView::getItemHeight() const { return d_ptr->_pItemHeight; }

void NTreeView::setItemBorderRadius(int ItemBorderRadius) {
    Q_D(NTreeView);
    d->_pItemBorderRadius = ItemBorderRadius;
    d->updateStyle();
    Q_EMIT pItemBorderRadiusChanged();
}
int NTreeView::getItemBorderRadius() const { return d_ptr->_pItemBorderRadius; }

void NTreeView::setBorderRadius(int BorderRadius) {
    Q_D(NTreeView);
    d->_pBorderRadius = BorderRadius;
    d->updateStyle();
    Q_EMIT pBorderRadiusChanged();
}
int NTreeView::getBorderRadius() const { return d_ptr->_pBorderRadius; }

void NTreeView::setIndentation(int Indentation) {
    Q_D(NTreeView);
    d->_pIndentation = Indentation;
    QTreeView::setIndentation(Indentation);
    d->updateStyle();
    Q_EMIT pIndentationChanged();
}
int NTreeView::getIndentation() const { return d_ptr->_pIndentation; }

NTreeView::NTreeView(QWidget* parent) : QTreeView(parent), d_ptr(new NTreeViewPrivate()) {
    Q_D(NTreeView);
    d->q_ptr = this;
    init();
}

NTreeView::~NTreeView() {}

void NTreeView::init() {
    Q_D(NTreeView);

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

    d->_pItemHeight       = 32;
    d->_pItemBorderRadius = 4;
    d->_pBorderRadius     = 8;
    d->_pIndentation      = 20;

    d->isDark    = nTheme->isDarkMode();
    d->themeMode = nTheme->themeMode();
    d->initStyle();

    setMouseTracking(true);
    setViewportMargins(4, 4, 4, 4);
    setIndentation(d->_pIndentation);
    setVerticalScrollBar(new NScrollBar(this));
    setHorizontalScrollBar(new NScrollBar(this));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setHeaderHidden(true);
    setAnimated(true);
    setExpandsOnDoubleClick(true);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NTreeView);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        d->updateStyle();
    });
}
