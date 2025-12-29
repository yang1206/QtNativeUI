#include "ngroupbox_p.h"
#include "QtNativeUI/NTheme.h"

NGroupBoxPrivate::NGroupBoxPrivate(QObject* parent) : QObject(parent) {
    _themeMode = nTheme->themeMode();
    _isDark    = nTheme->isDarkMode();
}

NGroupBoxPrivate::~NGroupBoxPrivate() {}

QRect NGroupBoxPrivate::calculateTitleRect(const QRect& rect) const {
    return QRect(
        rect.x() + _pContentMargin, rect.y() + _pContentMargin, rect.width() - 2 * _pContentMargin, _pTitleHeight);
}

QRect NGroupBoxPrivate::calculateContentRect(const QRect& rect) const {
    int contentY = _pContentMargin + _pTitleHeight + _pContentMargin;
    return QRect(rect.x() + _pContentMargin,
                 rect.y() + contentY,
                 rect.width() - 2 * _pContentMargin,
                 rect.height() - contentY - _pContentMargin);
}

QRect NGroupBoxPrivate::calculateCollapseIndicatorRect(const QRect& titleRect) const {
    int indicatorSize = _pCollapseIndicatorSize;
    int rightMargin   = _pContentMargin;
    return QRect(titleRect.right() - rightMargin - indicatorSize,
                 titleRect.y() + (titleRect.height() - indicatorSize) / 2,
                 indicatorSize,
                 indicatorSize);
}