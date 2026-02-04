#include "ngroupbox_p.h"
#include "QtNativeUI/NTheme.h"

NGroupBoxPrivate::NGroupBoxPrivate(QObject* parent) : QObject(parent) {
    _themeMode = nTheme->themeMode();
    _isDark    = nTheme->isDarkMode();
}

NGroupBoxPrivate::~NGroupBoxPrivate() {}

QRect NGroupBoxPrivate::calculateTitleRect(const QRect& rect) const {
    return QRect(rect.x(), rect.y(), rect.width(), _pTitleHeight);
}

QRect NGroupBoxPrivate::calculateContentRect(const QRect& rect) const {
    int contentY = _pTitleHeight + 4 + _pContentMargin;
    return QRect(rect.x() + _pContentMargin,
                 rect.y() + contentY,
                 rect.width() - 2 * _pContentMargin,
                 rect.height() - contentY - _pContentMargin);
}