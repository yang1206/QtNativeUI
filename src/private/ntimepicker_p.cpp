#include "ntimepicker_p.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NTheme.h>

NTimePickerContainer::NTimePickerContainer(QWidget* parent) : QWidget(parent) {
    _pAnimationPixOffsetY = 0;
    _pButtonAreaHeight    = 38;
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(6, 6, 6, 6 + _pButtonAreaHeight);
    setObjectName("NTimePickerContainer");
    setStyleSheet("#NTimePickerContainer{background-color:transparent}");

    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

NTimePickerContainer::~NTimePickerContainer() {}

void NTimePickerContainer::doPickerAnimation() {
    _handleSaveOrReset(true);
    if (!_animationPix.isNull()) {
        _animationPix = QPixmap();
    }
    _animationPix = this->grab(rect());
    QPropertyAnimation* offsetAnimation = new QPropertyAnimation(this, "pAnimationPixOffsetY");
    connect(offsetAnimation, &QPropertyAnimation::finished, this, [=]() {
        _animationPix = QPixmap();
        update();
    });
    connect(offsetAnimation, &QPropertyAnimation::valueChanged, this, [=]([[maybe_unused]] const QVariant& value) {
        update();
    });
    offsetAnimation->setEasingCurve(QEasingCurve::OutCubic);
    offsetAnimation->setDuration(175);
    offsetAnimation->setStartValue(70);
    offsetAnimation->setEndValue(0);
    offsetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NTimePickerContainer::mouseMoveEvent(QMouseEvent* event) {
    auto currentPos = event->pos();
    if (_confirmButtonRect.contains(currentPos)) {
        _isConfirmButtonHover = true;
        _isCancelButtonHover  = false;
        update();
    } else if (_cancelButtonRect.contains(currentPos)) {
        _isCancelButtonHover  = true;
        _isConfirmButtonHover = false;
        update();
    } else {
        if (_isCancelButtonHover || _isConfirmButtonHover) {
            _isCancelButtonHover  = false;
            _isConfirmButtonHover = false;
            update();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void NTimePickerContainer::mouseReleaseEvent(QMouseEvent* event) {
    auto currentPos = event->pos();
    if (_confirmButtonRect.contains(currentPos)) {
        _isConfirmButtonClicked = true;
        Q_EMIT confirmButtonClicked();
        hide();
    } else if (_cancelButtonRect.contains(currentPos)) {
        _handleSaveOrReset(false);
        Q_EMIT cancelButtonClicked();
        hide();
    }
    QWidget::mouseReleaseEvent(event);
}

void NTimePickerContainer::leaveEvent(QEvent* event) {
    _isConfirmButtonHover = false;
    _isCancelButtonHover  = false;
    update();
    QWidget::leaveEvent(event);
}

void NTimePickerContainer::hideEvent(QHideEvent* event) {
    if (_isConfirmButtonClicked) {
        _isConfirmButtonClicked = false;
    } else {
        _handleSaveOrReset(false);
    }
    QWidget::hideEvent(event);
}

void NTimePickerContainer::paintEvent([[maybe_unused]] QPaintEvent* event) {
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    if (!_animationPix.isNull()) {
        painter.setClipRect(QRect(0, _pAnimationPixOffsetY, width(), height() - 2 * _pAnimationPixOffsetY));
        painter.drawPixmap(rect(), _animationPix);
    } else {
        painter.setPen(NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, _themeMode));
        painter.setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, _themeMode));
        QRect foregroundRect(6, 6, rect().width() - 12, rect().height() - 12);
        painter.drawRoundedRect(foregroundRect, 8, 8);

        painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, _themeMode));
        int pickerXOffset = 6;
        for (int i = 0; i < _pickerList.count(); i++) {
            pickerXOffset += _pickerList[i]->width();
            if (i != _pickerList.count() - 1) {
                painter.drawLine(pickerXOffset, foregroundRect.y(), pickerXOffset, foregroundRect.bottom() - _pButtonAreaHeight);
            }
        }

        painter.drawLine(foregroundRect.x(), foregroundRect.bottom() - _pButtonAreaHeight, foregroundRect.right(), foregroundRect.bottom() - _pButtonAreaHeight);

        _confirmButtonRect = QRect(foregroundRect.x() + _buttonMargin,
                                   foregroundRect.bottom() - _pButtonAreaHeight + _buttonMargin,
                                   (foregroundRect.width() - 2 * _buttonMargin - _buttonSpacing) / 2,
                                   _pButtonAreaHeight - 2 * _buttonMargin + 1);
        _cancelButtonRect  = QRect(_confirmButtonRect.right() + _buttonSpacing, _confirmButtonRect.y(), _confirmButtonRect.width(), _confirmButtonRect.height());

        painter.setPen(Qt::NoPen);
        painter.setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
        if (_isConfirmButtonHover) {
            painter.drawRoundedRect(_confirmButtonRect, 5, 5);
        } else if (_isCancelButtonHover) {
            painter.drawRoundedRect(_cancelButtonRect, 5, 5);
        }

        QColor iconColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
        int    iconSize  = 16;

        QIcon confirmIcon = nIcon->fromRegular(NRegularIconType::Checkmark16Regular, iconSize, iconColor);
        QRect confirmIconRect(_confirmButtonRect.center().x() - iconSize / 2,
                              _confirmButtonRect.center().y() - iconSize / 2,
                              iconSize,
                              iconSize);
        confirmIcon.paint(&painter, confirmIconRect);

        QIcon cancelIcon = nIcon->fromRegular(NRegularIconType::Dismiss16Regular, iconSize, iconColor);
        QRect cancelIconRect(_cancelButtonRect.center().x() - iconSize / 2,
                             _cancelButtonRect.center().y() - iconSize / 2,
                             iconSize,
                             iconSize);
        cancelIcon.paint(&painter, cancelIconRect);
    }
    painter.restore();
}

void NTimePickerContainer::_handleSaveOrReset(bool isSave) {
    if (isSave) {
        _historyIndexList.clear();
        for (auto picker : _pickerList) {
            _historyIndexList.append(picker->getCurrentIndex());
        }
    } else {
        for (int i = 0; i < _pickerList.count(); i++) {
            auto picker = _pickerList[i];
            picker->setCurrentIndex(_historyIndexList[i]);
        }
    }
}

NTimePickerPrivate::NTimePickerPrivate(QObject* parent) : QObject(parent) {}

NTimePickerPrivate::~NTimePickerPrivate() {}

void NTimePickerPrivate::onTimePickerClicked() {
    Q_Q(NTimePicker);
    QPoint targetPos(q->mapToGlobal(QPoint(-6, (q->height() - _timePickerContainer->height() + _timePickerContainer->getButtonAreaHeight()) / 2)));
    _timePickerContainer->show();
    _timePickerContainer->setGeometry(QRect(targetPos, QSize(_getPickerTotalWidth() + 12, _pickerHeight + _timePickerContainer->getButtonAreaHeight())));
    _timePickerContainer->doPickerAnimation();
}

void NTimePickerPrivate::onConfirmButtonClicked() {
    Q_Q(NTimePicker);
    QTime newTime = q->getTime();
    q->setTime(newTime);
}

void NTimePickerPrivate::onCancelButtonClicked() {
}

void NTimePickerPrivate::addPicker(const QStringList& itemList, bool isEnableLoop) {
    Q_Q(NTimePicker);
    if (itemList.isEmpty()) {
        return;
    }
    NPicker* picker = new NPicker(q);
    picker->setContainer(true);
    picker->setItems(itemList);
    picker->setVisibleItemCount(7);
    picker->setLoopEnabled(isEnableLoop);
    picker->setItemHeight(35);
    picker->setFixedHeight(_pickerHeight);
    picker->setFixedWidth(60);
    _timePickerContainer->_pickerList.append(picker);
    _containerLayout->addWidget(picker);
    q->setFixedWidth(_getPickerTotalWidth());
}

void NTimePickerPrivate::updatePickersFromTime() {
    if (_timePickerContainer->_pickerList.count() >= 2) {
        int hour = _pTime.hour();
        if (!_pUse24HourFormat && hour > 12) {
            hour -= 12;
        }
        if (!_pUse24HourFormat && hour == 0) {
            hour = 12;
        }
        _timePickerContainer->_pickerList[0]->setCurrentData(QString::number(hour).rightJustified(2, '0'));
        _timePickerContainer->_pickerList[1]->setCurrentData(QString::number(_pTime.minute()).rightJustified(2, '0'));

        if (_pShowSeconds && _timePickerContainer->_pickerList.count() >= 3) {
            _timePickerContainer->_pickerList[2]->setCurrentData(QString::number(_pTime.second()).rightJustified(2, '0'));
        }
    }
}

void NTimePickerPrivate::rebuildPickers() {
    Q_Q(NTimePicker);
    for (auto picker : _timePickerContainer->_pickerList) {
        _containerLayout->removeWidget(picker);
        picker->deleteLater();
    }
    _timePickerContainer->_pickerList.clear();

    QStringList hours;
    int         startHour = _pUse24HourFormat ? 0 : 1;
    int         maxHour   = _pUse24HourFormat ? 24 : 13;
    for (int i = startHour; i < maxHour; i++) {
        hours.append(QString::number(i).rightJustified(2, '0'));
    }
    addPicker(hours, true);

    QStringList minutes;
    for (int i = 0; i < 60; i++) {
        minutes.append(QString::number(i).rightJustified(2, '0'));
    }
    addPicker(minutes, true);

    if (_pShowSeconds) {
        QStringList seconds;
        for (int i = 0; i < 60; i++) {
            seconds.append(QString::number(i).rightJustified(2, '0'));
        }
        addPicker(seconds, true);
    }

    updatePickersFromTime();
    _timePickerContainer->resize(_getPickerTotalWidth() + 12, _pickerHeight + _timePickerContainer->getButtonAreaHeight());
}

int NTimePickerPrivate::_getPickerTotalWidth() const {
    int totalWidth = 0;
    for (auto picker : _timePickerContainer->_pickerList) {
        totalWidth += picker->width();
    }
    return totalWidth;
}
