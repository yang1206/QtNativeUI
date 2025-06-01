#include "nmaskwidget_p.h"

#include <QEasingCurve>
#include <QPainter>
#include <QPropertyAnimation>
#include "QtNativeUI/NTheme.h"

NMaskWidget::NMaskWidget(QWidget* parent) : QWidget(parent) {
    setObjectName("NMaskWidget");
    setStyleSheet("#NMaskWidget{background-color:transparent;}");
    _pMaskAlpha = 0;
}

NMaskWidget::~NMaskWidget() {
}

void NMaskWidget::doMaskAnimation(int endValue) {
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pMaskAlpha");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        if (endValue == 0) {
            setVisible(false);
        }
    });
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(_pMaskAlpha);
    opacityAnimation->setEndValue(endValue);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NMaskWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0x00, 0x00, 0x00, _pMaskAlpha));
    painter.drawRect(rect());
    painter.restore();
}