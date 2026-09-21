//
// Created by Yang1206 on 2025/12/29.
//

#include "nlistview_p.h"

#include <QAbstractItemView>
#include <QLabel>
#include <QListView>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NTheme.h"

NListViewPrivate::NListViewPrivate(QObject* parent) : QObject(parent) {}

NListViewPrivate::~NListViewPrivate() = default;

QColor NListViewPrivate::itemHoverColor() const { return isDark ? _pDarkItemHoverColor : _pLightItemHoverColor; }

QColor NListViewPrivate::itemSelectedColor() const {
    return isDark ? _pDarkItemSelectedColor : _pLightItemSelectedColor;
}

QColor NListViewPrivate::itemPressedColor() const { return isDark ? _pDarkItemPressedColor : _pLightItemPressedColor; }

QColor NListViewPrivate::textColor() const { return isDark ? _pDarkTextColor : _pLightTextColor; }

QColor NListViewPrivate::placeholderTextColor() const {
    return isDark ? _pDarkPlaceholderTextColor : _pLightPlaceholderTextColor;
}

QColor NListViewPrivate::sectionTextColor() const {
    return isDark ? _pDarkSectionTextColor : _pLightSectionTextColor;
}

QColor NListViewPrivate::backgroundColor() const {
    return isDark ? _pDarkBackgroundColor : _pLightBackgroundColor;
}

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
    } else {
        style->setBackgroundColor(_pLightBackgroundColor);
        style->setBorderColor(_pLightBorderColor);
    }
    style->setBorderRadius(_pBorderRadius);
    style->setItemHeight(_pItemHeight);
    style->setBorderVisible(_pBorderVisible);
    style->setBackgroundVisible(_pBackgroundVisible);
    Q_Q(NListView);
    q->update();
    if (q->viewport())
        q->viewport()->update();
}

void NListViewPrivate::applyReorderConfiguration() {
    Q_Q(NListView);
    q->setDragEnabled(false);
    q->setAcceptDrops(false);
    q->setDropIndicatorShown(false);
    q->setDragDropMode(QAbstractItemView::NoDragDrop);
}

void NListViewPrivate::updateHeaderFooterStyle() {
    if (!headerLabel && !footerLabel)
        return;
    const QColor textColor =
        NThemeColor(NFluentColorKey::TextFillColorSecondary, isDark ? NThemeType::Dark : NThemeType::Light);
    QFont font = headerLabel ? headerLabel->font() : footerLabel->font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
    font.setWeight(QFont::DemiBold);
    if (headerLabel) {
        headerLabel->setFont(font);
        headerLabel->setStyleSheet(QStringLiteral("background:transparent;color:%1;").arg(textColor.name()));
    }
    if (footerLabel) {
        footerLabel->setFont(font);
        footerLabel->setStyleSheet(QStringLiteral("background:transparent;color:%1;").arg(textColor.name()));
    }
}

void NListViewPrivate::layoutHeaderFooter() {
    Q_Q(NListView);
    constexpr int kSideMargin   = 4;
    constexpr int kHeaderHeight = 28;
    constexpr int kFooterHeight = 24;
    int           topMargin     = kSideMargin;
    int           bottomMargin  = kSideMargin;
    const int     innerWidth    = qMax(0, q->width() - kSideMargin * 2);
    if (headerLabel) {
        if (_pHeaderText.isEmpty()) {
            headerLabel->hide();
        } else {
            headerLabel->setText(_pHeaderText);
            headerLabel->setGeometry(kSideMargin, kSideMargin, innerWidth, kHeaderHeight);
            headerLabel->show();
            topMargin += kHeaderHeight;
        }
    }
    if (footerLabel) {
        if (_pFooterText.isEmpty()) {
            footerLabel->hide();
        } else {
            footerLabel->setText(_pFooterText);
            footerLabel->setGeometry(kSideMargin, q->height() - kSideMargin - kFooterHeight, innerWidth, kFooterHeight);
            footerLabel->show();
            bottomMargin += kFooterHeight;
        }
    }
    q->setViewportMargins(kSideMargin, topMargin, kSideMargin, bottomMargin);
}
