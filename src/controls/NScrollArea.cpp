#include <QPainter>
#include <QtNativeUI/NScrollArea.h>
#include "../private/nscrollarea_p.h"
#include "QtNativeUI/NTheme.h"

NScrollArea::NScrollArea(QWidget* parent) : QScrollArea(parent), d_ptr(new NScrollAreaPrivate()) {
    Q_D(NScrollArea);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NScrollArea");
    setFrameShape(QFrame::NoFrame);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NScrollArea);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

NScrollArea::~NScrollArea() {}

void NScrollArea::paintEvent(QPaintEvent* event) { QScrollArea::paintEvent(event); }