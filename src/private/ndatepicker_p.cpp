#include "ndatepicker_p.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NTheme.h>

NDatePickerContainer::NDatePickerContainer(QWidget* parent) : QWidget(parent) {
    _pAnimationPixOffsetY = 0;
    _pButtonAreaHeight    = 38;
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(6, 6, 6, 6 + _pButtonAreaHeight);
    setObjectName("NDatePickerContainer");
    setStyleSheet("#NDatePickerContainer{background-color:transparent}");

    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

NDatePickerContainer::~NDatePickerContainer() {}

void NDatePickerContainer::doPickerAnimation() {
    _handleSaveOrReset(true);
    if (!_animationPix.isNull()) {
        _animationPix = QPixmap();
    }
    _animationPix                       = this->grab(rect());
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

void NDatePickerContainer::mouseMoveEvent(QMouseEvent* event) {
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

void NDatePickerContainer::mouseReleaseEvent(QMouseEvent* event) {
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

void NDatePickerContainer::leaveEvent(QEvent* event) {
    _isConfirmButtonHover = false;
    _isCancelButtonHover  = false;
    update();
    QWidget::leaveEvent(event);
}

void NDatePickerContainer::hideEvent(QHideEvent* event) {
    if (_isConfirmButtonClicked) {
        _isConfirmButtonClicked = false;
    } else {
        _handleSaveOrReset(false);
    }
    QWidget::hideEvent(event);
}

void NDatePickerContainer::paintEvent([[maybe_unused]] QPaintEvent* event) {
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
                painter.drawLine(
                    pickerXOffset, foregroundRect.y(), pickerXOffset, foregroundRect.bottom() - _pButtonAreaHeight);
            }
        }

        painter.drawLine(foregroundRect.x(),
                         foregroundRect.bottom() - _pButtonAreaHeight,
                         foregroundRect.right(),
                         foregroundRect.bottom() - _pButtonAreaHeight);

        _confirmButtonRect = QRect(foregroundRect.x() + _buttonMargin,
                                   foregroundRect.bottom() - _pButtonAreaHeight + _buttonMargin,
                                   (foregroundRect.width() - 2 * _buttonMargin - _buttonSpacing) / 2,
                                   _pButtonAreaHeight - 2 * _buttonMargin + 1);
        _cancelButtonRect  = QRect(_confirmButtonRect.right() + _buttonSpacing,
                                  _confirmButtonRect.y(),
                                  _confirmButtonRect.width(),
                                  _confirmButtonRect.height());

        painter.setPen(Qt::NoPen);
        painter.setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
        if (_isConfirmButtonHover) {
            painter.drawRoundedRect(_confirmButtonRect, 5, 5);
        } else if (_isCancelButtonHover) {
            painter.drawRoundedRect(_cancelButtonRect, 5, 5);
        }

        QColor iconColor = (parentWidget() && parentWidget()->isEnabled())
                               ? NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode)
                               : NThemeColor(NFluentColorKey::TextFillColorDisabled, _themeMode);
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

void NDatePickerContainer::_handleSaveOrReset(bool isSave) {
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

NDatePickerPrivate::NDatePickerPrivate(QObject* parent) : QObject(parent) {}

NDatePickerPrivate::~NDatePickerPrivate() {}

void NDatePickerPrivate::onDatePickerClicked() {
    Q_Q(NDatePicker);
    QPoint targetPos(q->mapToGlobal(
        QPoint(-6, (q->height() - _datePickerContainer->height() + _datePickerContainer->getButtonAreaHeight()) / 2)));
    _datePickerContainer->show();
    _datePickerContainer->setGeometry(QRect(
        targetPos, QSize(_getPickerTotalWidth() + 12, _pickerHeight + _datePickerContainer->getButtonAreaHeight())));
    _datePickerContainer->doPickerAnimation();
}

void NDatePickerPrivate::onConfirmButtonClicked() {
    Q_Q(NDatePicker);
    QDate newDate = q->getDate();
    q->setDate(newDate);
}

void NDatePickerPrivate::onCancelButtonClicked() {}

void NDatePickerPrivate::onYearChanged() { updateDayPicker(); }

void NDatePickerPrivate::onMonthChanged() { updateDayPicker(); }

void NDatePickerPrivate::addPicker(const QStringList& itemList, bool isEnableLoop, int width) {
    Q_Q(NDatePicker);
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
    picker->setFixedWidth(width);
    _datePickerContainer->_pickerList.append(picker);
    _containerLayout->addWidget(picker);
    q->setFixedWidth(_getPickerTotalWidth());
}

void NDatePickerPrivate::updatePickersFromDate() {
    if (_yearPicker && _monthPicker && _dayPicker) {
        _yearPicker->setCurrentData(QString::number(_pDate.year()));
        _monthPicker->setCurrentData(_pLocale.monthName(_pDate.month(), QLocale::ShortFormat));
        _dayPicker->setCurrentData(QString::number(_pDate.day()));
    }
}

void NDatePickerPrivate::rebuildPickers() {
    Q_Q(NDatePicker);

    if (_yearPicker) {
        disconnect(_yearPicker, &NPicker::currentDataChanged, this, &NDatePickerPrivate::onYearChanged);
    }
    if (_monthPicker) {
        disconnect(_monthPicker, &NPicker::currentDataChanged, this, &NDatePickerPrivate::onMonthChanged);
    }

    while (!_datePickerContainer->_pickerList.isEmpty()) {
        NPicker* picker = _datePickerContainer->_pickerList.takeLast();
        _containerLayout->removeWidget(picker);
        delete picker;
    }
    _yearPicker  = nullptr;
    _monthPicker = nullptr;
    _dayPicker   = nullptr;

    QStringList years;
    for (int i = _pMinimumDate.year(); i <= _pMaximumDate.year(); i++) {
        years.append(QString::number(i));
    }
    addPicker(years, false, 80);
    if (_datePickerContainer->_pickerList.isEmpty()) {
        return;
    }
    _yearPicker = _datePickerContainer->_pickerList.last();
    connect(_yearPicker, &NPicker::currentDataChanged, this, &NDatePickerPrivate::onYearChanged);

    QStringList months;
    for (int i = 1; i <= 12; i++) {
        QString monthName = _pLocale.monthName(i, QLocale::ShortFormat);
        months.append(monthName);
    }

    int maxMonthLength = 0;
    for (const QString& monthName : months) {
        maxMonthLength = qMax(maxMonthLength, monthName.length());
    }

    int estimatedCharWidth = 10;
    int maxMonthWidth      = maxMonthLength * estimatedCharWidth;
    int monthPickerWidth   = qMax(60, qMin(maxMonthWidth + 20, 100));

    addPicker(months, true, monthPickerWidth);
    _monthPicker = _datePickerContainer->_pickerList.last();
    connect(_monthPicker, &NPicker::currentDataChanged, this, &NDatePickerPrivate::onMonthChanged);

    QStringList days;
    int         daysInMonth = _pDate.daysInMonth();
    for (int i = 1; i <= daysInMonth; i++) {
        days.append(QString::number(i));
    }
    addPicker(days, true, 60);
    _dayPicker = _datePickerContainer->_pickerList.last();

    updatePickersFromDate();
    _datePickerContainer->resize(_getPickerTotalWidth() + 12,
                                 _pickerHeight + _datePickerContainer->getButtonAreaHeight());
}

void NDatePickerPrivate::updateDayPicker() {
    if (!_yearPicker || !_monthPicker || !_dayPicker) {
        return;
    }

    int year = _yearPicker->getCurrentData().toInt();

    QString monthData = _monthPicker->getCurrentData();
    int     month     = 0;
    for (int i = 1; i <= 12; i++) {
        if (_pLocale.monthName(i, QLocale::ShortFormat) == monthData) {
            month = i;
            break;
        }
    }

    if (month == 0) {
        return;
    }

    QDate tempDate(year, month, 1);
    int   daysInMonth = tempDate.daysInMonth();

    int currentDay = _dayPicker->getCurrentData().toInt();

    QStringList days;
    for (int i = 1; i <= daysInMonth; i++) {
        days.append(QString::number(i));
    }

    _dayPicker->setItems(days);

    if (currentDay > daysInMonth) {
        _dayPicker->setCurrentData(QString::number(daysInMonth));
    } else {
        _dayPicker->setCurrentData(QString::number(currentDay));
    }
}

int NDatePickerPrivate::_getPickerTotalWidth() const {
    int totalWidth = 0;
    for (auto picker : _datePickerContainer->_pickerList) {
        totalWidth += picker->width();
    }
    return totalWidth;
}
