#include "npivot_p.h"
#include "QtNativeUI/NTheme.h"

NPivotPrivate::NPivotPrivate(QObject* parent) : QObject(parent) {}

NPivotPrivate::~NPivotPrivate() {
    if (_posAnimation) {
        _posAnimation->stop();
    }
    if (_widthAnimation) {
        _widthAnimation->stop();
    }
}

QColor NPivotPrivate::getCurrentColor(const QColor& lightColor, const QColor& darkColor) const {
    return _isDark ? darkColor : lightColor;
}

int NPivotPrivate::rightWidgetsTotalWidth() const {
    int total = 0;
    for (const auto& rw : _rightWidgets) {
        if (rw.widget) {
            total += rw.widget->sizeHint().width() + rw.spacing;
        }
    }
    return total;
}
