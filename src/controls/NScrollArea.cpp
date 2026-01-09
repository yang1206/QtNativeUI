#include <QPainter>
#include <QResizeEvent>
#include <QtNativeUI/NScrollArea.h>
#include <QtNativeUI/NScrollBar.h>
#include "../private/nscrollarea_p.h"
#include "QtNativeUI/NTheme.h"

NScrollArea::NScrollArea(QWidget* parent) : QScrollArea(parent), d_ptr(new NScrollAreaPrivate()) {
    Q_D(NScrollArea);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NScrollArea");
    setFrameShape(QFrame::NoFrame);
    
    d->_originalHorizontalScrollBar = horizontalScrollBar();
    d->_originalVerticalScrollBar = verticalScrollBar();

    initScrollBars();

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NScrollArea);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

NScrollArea::~NScrollArea() {}

void NScrollArea::paintEvent(QPaintEvent* event) { 
    QScrollArea::paintEvent(event); 
}

void NScrollArea::resizeEvent(QResizeEvent* event) {
    Q_D(NScrollArea);
    QScrollArea::resizeEvent(event);
    d->updateContentRect();
}

void NScrollArea::setUseCustomScrollBars(bool use) {
    Q_D(NScrollArea);
    if (d->_useCustomScrollBars == use) {
        return;
    }
    
    d->_useCustomScrollBars = use;
    updateScrollBars();
}

bool NScrollArea::useCustomScrollBars() const {
    Q_D(const NScrollArea);
    return d->_useCustomScrollBars;
}

void NScrollArea::enableTransparentBackground() {
    Q_D(NScrollArea);
    d->_hasTransparentBackground = true;
    
    setStyleSheet("NScrollArea{border: none; background: transparent}");
    
    if (widget()) {
        widget()->setStyleSheet("QWidget{background: transparent}");
    }
}

void NScrollArea::setViewportMargins(int left, int top, int right, int bottom) {
    Q_D(NScrollArea);
    d->_viewportMargins = QMargins(left, top, right, bottom);
    QScrollArea::setViewportMargins(left, top, right, bottom);
    d->updateContentRect();
}

void NScrollArea::setViewportMargins(int margins) {
    setViewportMargins(margins, margins, margins, margins);
}

QRect NScrollArea::contentRect() const {
    Q_D(const NScrollArea);
    return d->_contentRect;
}

void NScrollArea::initScrollBars() {
    Q_D(NScrollArea);
    
    d->_horizontalScrollBar = new NScrollBar(Qt::Horizontal, this);
    
    d->_verticalScrollBar = new NScrollBar(Qt::Vertical, this);
    
    updateScrollBars();
}

void NScrollArea::updateScrollBars() {
    Q_D(NScrollArea);
    
    if (d->_useCustomScrollBars) {
        if (d->_horizontalScrollBar) {
            setHorizontalScrollBar(d->_horizontalScrollBar);
        }
        
        if (d->_verticalScrollBar) {
            setVerticalScrollBar(d->_verticalScrollBar);
        }
    } else {
        if (d->_originalHorizontalScrollBar) {
            setHorizontalScrollBar(d->_originalHorizontalScrollBar);
        }
        
        if (d->_originalVerticalScrollBar) {
            setVerticalScrollBar(d->_originalVerticalScrollBar);
        }
    }

    d->updateContentRect();
}