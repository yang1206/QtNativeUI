#include "noverlay_p.h"

#include <QLabel>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "QtNativeUI/NOverlay.h"
#include "QtNativeUI/NProgressRing.h"
#include "QtNativeUI/NTheme.h"

NOverlayPrivate::NOverlayPrivate(QObject* parent) : QObject(parent) {}

NOverlayPrivate::~NOverlayPrivate() {}

void NOverlayPrivate::init() {
    Q_Q(NOverlay);

    _pLightOverlayColor = QColor(0, 0, 0);
    _pDarkOverlayColor  = QColor(0, 0, 0);
    _pOpacity           = 150;
    _pClickToClose      = true;
    _pAnimationDuration = 250;
    _pCurrentOpacity    = 0;

    q->setAttribute(Qt::WA_TranslucentBackground);
    q->setVisible(false);
}

void NOverlayPrivate::doAnimation(int endValue) {
    _isAnimating = true;
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pCurrentOpacity");
    connect(animation, &QPropertyAnimation::valueChanged, this, [this]() {
        if (q_ptr) {
            q_ptr->update();
        }
    });
    connect(animation, &QPropertyAnimation::finished, this, [this, endValue]() {
        _isAnimating = false;
        if (endValue == 0 && q_ptr) {
            q_ptr->setVisible(false);
            if (_closeAfterAnimation) {
                _closeAfterAnimation = false;
                q_ptr->close();
            }
        }
    });
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->setDuration(_pAnimationDuration);
    animation->setStartValue(_pCurrentOpacity);
    animation->setEndValue(endValue);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NOverlayPrivate::updateContentPosition() {
    Q_Q(NOverlay);

    if (_contentWidget) {
        QSize contentSize = _contentWidget->sizeHint();
        if (!contentSize.isValid()) {
            contentSize = _contentWidget->size();
        }
        int x = (q->width() - contentSize.width()) / 2;
        int y = (q->height() - contentSize.height()) / 2;
        _contentWidget->setGeometry(x, y, contentSize.width(), contentSize.height());
    }
}

QColor NOverlayPrivate::currentOverlayColor() const {
    return nTheme->isDarkMode() ? _pDarkOverlayColor : _pLightOverlayColor;
}
QWidget* NOverlayPrivate::createLoadingWidget(const QString& text) {
    Q_Q(NOverlay);

    QWidget* widget = new QWidget(q);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setAlignment(Qt::AlignCenter);

    NProgressRing* ring = new NProgressRing(widget);
    ring->setFixedSize(48, 48);
    ring->setIndeterminate(true);

    QLabel* label = new QLabel(text, widget);
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(ring, 0, Qt::AlignCenter);
    layout->addWidget(label);

    return widget;
}