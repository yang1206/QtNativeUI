#include "QtNativeUI/NOverlay.h"

#include <QMouseEvent>
#include <QPainter>

#include "../private/noverlay_p.h"

NOverlay::NOverlay(QWidget* parent) : QWidget(parent), d_ptr(new NOverlayPrivate()) {
    Q_D(NOverlay);
    d->q_ptr = this;
    d->init();
}

NOverlay::NOverlay(NOverlayPrivate& dd, NOverlay* parent) : QWidget(parent), d_ptr(&dd) {}

NOverlay::~NOverlay() {}

Q_PROPERTY_CREATE_Q_CPP(NOverlay, QColor, LightOverlayColor)
Q_PROPERTY_CREATE_Q_CPP(NOverlay, QColor, DarkOverlayColor)
Q_PROPERTY_CREATE_Q_CPP(NOverlay, int, Opacity)
Q_PROPERTY_CREATE_Q_CPP(NOverlay, bool, ClickToClose)
Q_PROPERTY_CREATE_Q_CPP(NOverlay, int, AnimationDuration)

void NOverlay::setContentWidget(QWidget* widget) {
    Q_D(NOverlay);

    if (d->_contentWidget) {
        d->_contentWidget->setParent(nullptr);
    }

    d->_contentWidget = widget;

    if (widget) {
        widget->setParent(this);
        d->updateContentPosition();
    }
}

QWidget* NOverlay::contentWidget() const {
    Q_D(const NOverlay);
    return d->_contentWidget;
}

void NOverlay::showOverlay() {
    Q_D(NOverlay);

    if (parentWidget()) {
        move(0, 0);
        resize(parentWidget()->size());
    }

    setVisible(true);
    raise();
    d->doAnimation(d->_pOpacity);
}

void NOverlay::showOverlay(QWidget* parent) {
    if (parent) {
        setParent(parent);
    }
    showOverlay();
}

void NOverlay::hideOverlay() {
    Q_D(NOverlay);
    d->doAnimation(0);
}

bool NOverlay::isAnimating() const {
    Q_D(const NOverlay);
    return d->_isAnimating;
}

void NOverlay::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    Q_D(NOverlay);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QColor color = d->currentOverlayColor();
    color.setAlpha(d->_pCurrentOpacity);
    painter.setBrush(color);
    painter.drawRect(rect());
}

void NOverlay::mousePressEvent(QMouseEvent* event) {
    Q_D(NOverlay);

    if (event->button() == Qt::LeftButton) {
        Q_EMIT clicked();

        if (d->_pClickToClose) {
            close();
        }
    }

    QWidget::mousePressEvent(event);
}

void NOverlay::resizeEvent(QResizeEvent* event) {
    Q_D(NOverlay);
    QWidget::resizeEvent(event);
    d->updateContentPosition();
}
void NOverlay::showLoading(const QString& text) {
    Q_D(NOverlay);

    if (d->_loadingWidget) {
        d->_loadingWidget->deleteLater();
    }

    d->_loadingWidget = d->createLoadingWidget(text);
    setContentWidget(d->_loadingWidget);

    setClickToClose(false);
    showOverlay();
}

void NOverlay::showLoading(QWidget* parent, const QString& text) {
    if (parent) {
        setParent(parent);
    }
    showLoading(text);
}

void NOverlay::close() {
    Q_D(NOverlay);
    if (d->_pAnimationDuration > 0) {
        d->_closeAfterAnimation = true;
        d->doAnimation(0);
    } else {
        QWidget::close();
    }
}

void NOverlay::hideLoading() {
    Q_D(NOverlay);
    
    if (d->_loadingWidget) {
        d->_loadingWidget->deleteLater();
        d->_loadingWidget = nullptr;
    }
    
    hideOverlay();
}