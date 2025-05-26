#include "QtNativeUI/NCalendarDatePicker.h"
#include <QEvent>
#include <QResizeEvent>
#include "../private/ncalendardatepicker_p.h"

NCalendarDatePicker::NCalendarDatePicker(QWidget* parent)
    : QWidget(parent), d_ptr(new NCalendarDatePickerPrivate(this)) {
    Q_D(NCalendarDatePicker);
    d->initUi();
    setFocusPolicy(Qt::StrongFocus);
}

NCalendarDatePicker::~NCalendarDatePicker() {}

void NCalendarDatePicker::setSelectedDate(QDate date) {
    Q_D(NCalendarDatePicker);
    if (d->_pSelectedDate != date && date.isValid()) {
        d->_pSelectedDate = date;
        d->updateDisplayText();
        emit pSelectedDateChanged();
        emit dateSelected(date);
    }
}

QDate NCalendarDatePicker::getSelectedDate() const {
    Q_D(const NCalendarDatePicker);
    return d->_pSelectedDate;
}

void NCalendarDatePicker::setMinimumDate(QDate date) {
    Q_D(NCalendarDatePicker);
    if (d->_pMinimumDate != date && date.isValid()) {
        d->_pMinimumDate = date;
        emit pMinimumDateChanged();
    }
}

QDate NCalendarDatePicker::getMinimumDate() const {
    Q_D(const NCalendarDatePicker);
    return d->_pMinimumDate;
}

void NCalendarDatePicker::setMaximumDate(QDate date) {
    Q_D(NCalendarDatePicker);
    if (d->_pMaximumDate != date && date.isValid()) {
        d->_pMaximumDate = date;
        emit pMaximumDateChanged();
    }
}

QDate NCalendarDatePicker::getMaximumDate() const {
    Q_D(const NCalendarDatePicker);
    return d->_pMaximumDate;
}

void NCalendarDatePicker::setPlaceholderText(QString text) {
    Q_D(NCalendarDatePicker);
    if (d->_pPlaceholderText != text) {
        d->_pPlaceholderText = text;
        d->updateDisplayText();
        emit pPlaceholderTextChanged();
    }
}

QString NCalendarDatePicker::getPlaceholderText() const {
    Q_D(const NCalendarDatePicker);
    return d->_pPlaceholderText;
}

void NCalendarDatePicker::setDateFormat(QString format) {
    Q_D(NCalendarDatePicker);
    if (d->_pDateFormat != format) {
        d->_pDateFormat = format;
        d->updateDisplayText();
        emit pDateFormatChanged();
    }
}

QString NCalendarDatePicker::getDateFormat() const {
    Q_D(const NCalendarDatePicker);
    return d->_pDateFormat;
}

void NCalendarDatePicker::setBorderRadius(int radius) {
    Q_D(NCalendarDatePicker);
    if (d->_pBorderRadius != radius) {
        d->_pBorderRadius = radius;
        d->button->setBorderRadius(radius);
        emit pBorderRadiusChanged();
    }
}

int NCalendarDatePicker::getBorderRadius() const {
    Q_D(const NCalendarDatePicker);
    return d->_pBorderRadius;
}

void NCalendarDatePicker::setLocale(const QLocale& locale) {
    Q_D(NCalendarDatePicker);
    if (d->_locale != locale) {
        d->_locale = locale;

        d->updateDisplayText();
        update();
    }
}

QLocale NCalendarDatePicker::locale() const {
    Q_D(const NCalendarDatePicker);
    return d->_locale;
}

void NCalendarDatePicker::setDateSelectionMode(NCalendarWidget::DateSelectionMode mode) {
    Q_D(NCalendarDatePicker);
    if (d->_selectionMode != mode) {
        d->_selectionMode = mode;

        emit dateSelectionModeChanged(mode);
    }
}

NCalendarWidget::DateSelectionMode NCalendarDatePicker::dateSelectionMode() const {
    Q_D(const NCalendarDatePicker);
    return d->_selectionMode;
}

void NCalendarDatePicker::setSelectedDates(const QList<QDate>& dates) {
    Q_D(NCalendarDatePicker);
    d->_selectedDates = dates;

    if (!dates.isEmpty()) {
        d->_pSelectedDate = dates.first();
        d->updateDisplayText();
        emit pSelectedDateChanged();
    }

    emit selectedDatesChanged(dates);
}

QList<QDate> NCalendarDatePicker::selectedDates() const {
    Q_D(const NCalendarDatePicker);
    return d->_selectedDates;
}

void NCalendarDatePicker::setDateRange(const QDate& startDate, const QDate& endDate) {
    Q_D(NCalendarDatePicker);
    QPair<QDate, QDate> range(startDate, endDate);
    d->_selectedDateRange = range;

    if (startDate.isValid()) {
        d->_pSelectedDate = startDate;
        d->updateDisplayText();
        emit pSelectedDateChanged();
    }

    emit selectedDateRangeChanged(range);
}

QPair<QDate, QDate> NCalendarDatePicker::selectedDateRange() const {
    Q_D(const NCalendarDatePicker);
    return d->_selectedDateRange;
}

void NCalendarDatePicker::paintEvent(QPaintEvent* event) { QWidget::paintEvent(event); }

void NCalendarDatePicker::resizeEvent(QResizeEvent* event) {
    Q_D(NCalendarDatePicker);
    d->button->resize(size());
    d->contentWidget->resize(size());
    QWidget::resizeEvent(event);
}

void NCalendarDatePicker::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        Q_D(NCalendarDatePicker);
        if (!d->_pSelectedDate.isValid()) {
            d->_pPlaceholderText = tr("Pick a date");
            d->updateDisplayText();
        }
    }
    QWidget::changeEvent(event);
}

bool NCalendarDatePicker::eventFilter(QObject* watched, QEvent* event) { return QWidget::eventFilter(watched, event); }