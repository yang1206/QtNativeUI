#include "ntableview_p.h"

#include <QApplication>
#include <QFont>
#include <QHeaderView>

#include "QtNativeUI/NTheme.h"

NTableViewPrivate::NTableViewPrivate(QObject* parent) : QObject(parent) {}

NTableViewPrivate::~NTableViewPrivate() = default;

QColor NTableViewPrivate::backgroundColor() const { return isDark ? _pDarkBackgroundColor : _pLightBackgroundColor; }

QColor NTableViewPrivate::headerBackgroundColor() const {
    return isDark ? _pDarkHeaderBackgroundColor : _pLightHeaderBackgroundColor;
}

QColor NTableViewPrivate::headerHoverColor() const { return isDark ? _pDarkHeaderHoverColor : _pLightHeaderHoverColor; }

QColor NTableViewPrivate::headerPressedColor() const {
    return isDark ? _pDarkHeaderPressedColor : _pLightHeaderPressedColor;
}

QColor NTableViewPrivate::itemHoverColor() const { return isDark ? _pDarkItemHoverColor : _pLightItemHoverColor; }

QColor NTableViewPrivate::itemSelectedColor() const {
    return isDark ? _pDarkItemSelectedColor : _pLightItemSelectedColor;
}

QColor NTableViewPrivate::textColor() const { return isDark ? _pDarkTextColor : _pLightTextColor; }

QColor NTableViewPrivate::headerTextColor() const { return isDark ? _pDarkHeaderTextColor : _pLightHeaderTextColor; }

QColor NTableViewPrivate::placeholderTextColor() const {
    return isDark ? _pDarkPlaceholderTextColor : _pLightPlaceholderTextColor;
}

QColor NTableViewPrivate::dividerColor() const { return isDark ? _pDarkDividerColor : _pLightDividerColor; }

QColor NTableViewPrivate::borderColor() const { return isDark ? _pDarkBorderColor : _pLightBorderColor; }

void NTableViewPrivate::updatePalette() {
    Q_Q(NTableView);
    if (!q)
        return;
    QFont bodyFont = QApplication::font();
    bodyFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
    q->setFont(bodyFont);
    QFont headerFont = bodyFont;
    headerFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
    headerFont.setWeight(static_cast<QFont::Weight>(NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt()));
    if (q->horizontalHeader())
        q->horizontalHeader()->setFont(headerFont);
    if (q->verticalHeader())
        q->verticalHeader()->setFont(headerFont);
    if (q->horizontalHeader()) {
        q->horizontalHeader()->setFixedHeight(_pHeaderHeight);
        q->horizontalHeader()->viewport()->update();
    }
    if (q->verticalHeader())
        q->verticalHeader()->setDefaultSectionSize(_pRowHeight);
    if (borderOverlay)
        borderOverlay->update();
    if (chromeUnderlay)
        chromeUnderlay->update();
    if (q->viewport())
        q->viewport()->update();
}
