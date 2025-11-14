#include "nrangeslider_p.h"
#include <QtNativeUI/NToolTip.h>
#include "QtNativeUI/NRangeSlider.h"

NRangeSliderPrivate::NRangeSliderPrivate(QObject* parent) : QObject(parent) {
    _lowerThumbAnimation = new QPropertyAnimation(this, "lowerThumbScale");
    _lowerThumbAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _lowerThumbAnimation->setDuration(150);

    _upperThumbAnimation = new QPropertyAnimation(this, "upperThumbScale");
    _upperThumbAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _upperThumbAnimation->setDuration(150);
}

NRangeSliderPrivate::~NRangeSliderPrivate() {
    // 清理tooltip
    if (_lowerTooltip) {
        _lowerTooltip->deleteLater();
        _lowerTooltip = nullptr;
    }
    if (_upperTooltip) {
        _upperTooltip->deleteLater();
        _upperTooltip = nullptr;
    }
}

void NRangeSliderPrivate::startThumbAnimation(DragHandle handle, qreal startScale, qreal endScale) {
    if (handle == Lower) {
        _lowerThumbAnimation->stop();
        _lowerThumbAnimation->setStartValue(startScale);
        _lowerThumbAnimation->setEndValue(endScale);

        disconnect(_lowerThumbAnimation, nullptr, q_ptr, nullptr);

        connect(_lowerThumbAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
            _lowerThumbScale = value.toReal();
            q_ptr->update();
        });

        connect(_lowerThumbAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
            disconnect(_lowerThumbAnimation, nullptr, q_ptr, nullptr);
        });

        _lowerThumbAnimation->start();
    } else if (handle == Upper) {
        _upperThumbAnimation->stop();
        _upperThumbAnimation->setStartValue(startScale);
        _upperThumbAnimation->setEndValue(endScale);

        disconnect(_upperThumbAnimation, nullptr, q_ptr, nullptr);

        QObject::connect(_upperThumbAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
            _upperThumbScale = value.toReal();
            q_ptr->update();
        });

        QObject::connect(_upperThumbAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
            QObject::disconnect(_upperThumbAnimation, nullptr, q_ptr, nullptr);
        });

        _upperThumbAnimation->start();
    }
}

void NRangeSliderPrivate::updateTooltip(DragHandle handle) {
    if (!_showTooltip)
        return;

    Q_Q(NRangeSlider);

    NToolTip** tooltip   = (handle == Lower) ? &_lowerTooltip : &_upperTooltip;
    int        value     = (handle == Lower) ? _pLowerValue : _pUpperValue;
    QRect      thumbRect = (handle == Lower) ? q->lowerHandleRect() : q->upperHandleRect();

    // 懒加载创建tooltip
    if (!*tooltip) {
        *tooltip = new NToolTip("", q->window());
        (*tooltip)->setDuration(-1);
    }

    // 更新内容
    QString text = _tooltipFormatter ? _tooltipFormatter(value) : QString::number(value);
    (*tooltip)->setText(text);

    // 计算位置
    QPoint thumbCenter = q->mapToGlobal(thumbRect.center());
    (*tooltip)->adjustSize();

    QPoint tooltipPos;
    if (_orientation == Qt::Horizontal) {
        // 水平滑块：tooltip在上方居中
        tooltipPos = QPoint(thumbCenter.x() - (*tooltip)->width() / 2,
                            thumbCenter.y() - thumbRect.height() / 2 - (*tooltip)->height() - 5);
    } else {
        // 垂直滑块：tooltip在右侧居中
        tooltipPos = QPoint(thumbCenter.x() + thumbRect.width() / 2 + 5, thumbCenter.y() - (*tooltip)->height() / 2);
    }

    (*tooltip)->move(tooltipPos);

    // 显示tooltip
    (*tooltip)->show();
}

void NRangeSliderPrivate::hideTooltips() {
    if (_lowerTooltip && _lowerTooltip->isVisible()) {
        _lowerTooltip->hide();
    }
    if (_upperTooltip && _upperTooltip->isVisible()) {
        _upperTooltip->hide();
    }
}
