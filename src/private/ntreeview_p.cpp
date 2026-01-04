//
// Created by Yang1206 on 2025/12/30.
//

#include "ntreeview_p.h"
#include "QtNativeUI/NTheme.h"

NTreeViewPrivate::NTreeViewPrivate(QObject* parent) : QObject(parent) {}

NTreeViewPrivate::~NTreeViewPrivate() {}

void NTreeViewPrivate::initStyle() {
    Q_Q(NTreeView);
    style = new NTreeViewStyle();
    q->setStyle(style);
    updateStyle();
}

void NTreeViewPrivate::updateStyle() {
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
    style->setIndentation(_pIndentation);

    Q_Q(NTreeView);
    q->update();
}
