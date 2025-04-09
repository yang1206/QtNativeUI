#include <QPainter>
#include <QtNativeUI/NProgressBar.h>
#include "../private/nprogressbar_p.h"
#include "QtNativeUI/NTheme.h"

NProgressBar::NProgressBar(QWidget* parent) : QProgressBar(parent), d_ptr(new NProgressBarPrivate()) {
    Q_D(NProgressBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NProgressBar");

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NProgressBar);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

NProgressBar::~NProgressBar() {}

void NProgressBar::paintEvent(QPaintEvent* event) { QProgressBar::paintEvent(event); }
