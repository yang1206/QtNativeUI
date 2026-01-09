//
// Created by Yang1206 on 2025/12/29.
//

#include "nlistview_p.h"
#include "QtNativeUI/NTheme.h"

NListViewPrivate::NListViewPrivate(QObject* parent) : QObject(parent) {}

NListViewPrivate::~NListViewPrivate() {}

void NListViewPrivate::initStyle() {
    Q_Q(NListView);
    style = new NListViewStyle();
    q->setStyle(style);
    updateStyle();
}

void NListViewPrivate::updateStyle() {
    if (!style)
        return;

    if (isDark) {
        style->setBackgroundColor(_pDarkBackgroundColor);
        style->setBorderColor(_pDarkBorderColor);
        style->setItemHoverColor(_pDarkItemHoverColor);
        style->setItemSelectedColor(_pDarkItemSelectedColor);
        style->setItemPressedColor(_pDarkItemPressedColor);
        style->setTextColor(_pDarkTextColor);
    } else {
        style->setBackgroundColor(_pLightBackgroundColor);
        style->setBorderColor(_pLightBorderColor);
        style->setItemHoverColor(_pLightItemHoverColor);
        style->setItemSelectedColor(_pLightItemSelectedColor);
        style->setItemPressedColor(_pLightItemPressedColor);
        style->setTextColor(_pLightTextColor);
    }

    NAccentColor accentColor = nTheme->accentColor();
    style->setAccentColor(accentColor.normal());
    style->setBorderRadius(_pBorderRadius);
    style->setItemHeight(_pItemHeight);
    style->setItemBorderRadius(_pItemBorderRadius);

    Q_Q(NListView);
    q->update();
}
