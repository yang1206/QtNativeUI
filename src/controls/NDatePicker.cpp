#include <QPainter>
#include <QtNativeUI/NDatePicker.h>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NTheme.h>
#include "../private/ndatepicker_p.h"

Q_PROPERTY_CREATE_Q_CPP(NDatePicker, int, BorderRadius)

NDatePicker::NDatePicker(QWidget* parent) : QPushButton(parent), d_ptr(new NDatePickerPrivate()) {
    Q_D(NDatePicker);
    d->q_ptr          = this;
    d->_pBorderRadius = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pDate         = QDate::currentDate();
    d->_pMinimumDate  = QDate(1900, 1, 1);
    d->_pMaximumDate  = QDate(9999, 12, 31);
    d->_pLocale       = QLocale::system();

    setMouseTracking(true);
    setFixedSize(240, 32);
    setObjectName("NDatePicker");
    setStyleSheet("#NDatePicker{background-color:transparent;}");

    QFont font = this->font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    d->_datePickerContainer = new NDatePickerContainer(this);
    d->_datePickerContainer->resize(240, d->_pickerHeight + d->_datePickerContainer->getButtonAreaHeight());
    d->_datePickerContainer->hide();

    connect(this, &QPushButton::clicked, d, &NDatePickerPrivate::onDatePickerClicked);
    connect(d->_datePickerContainer,
            &NDatePickerContainer::confirmButtonClicked,
            d,
            &NDatePickerPrivate::onConfirmButtonClicked);
    connect(d->_datePickerContainer,
            &NDatePickerContainer::cancelButtonClicked,
            d,
            &NDatePickerPrivate::onCancelButtonClicked);

    d->_containerLayout = new QHBoxLayout(d->_datePickerContainer);
    d->_containerLayout->setSpacing(0);
    d->_containerLayout->setContentsMargins(0, 0, 0, 0);

    d->rebuildPickers();

    d->_themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

NDatePicker::~NDatePicker() {}

void NDatePicker::setDate(QDate Date) {
    Q_D(NDatePicker);
    if (!Date.isValid() || d->_pDate == Date) {
        return;
    }
    if (Date < d->_pMinimumDate) {
        Date = d->_pMinimumDate;
    }
    if (Date > d->_pMaximumDate) {
        Date = d->_pMaximumDate;
    }
    d->_pDate = Date;
    d->updatePickersFromDate();
    update();
    Q_EMIT pDateChanged();
    Q_EMIT dateChanged(Date);
}

QDate NDatePicker::getDate() const {
    Q_D(const NDatePicker);
    if (d->_datePickerContainer->_pickerList.count() < 3) {
        return d->_pDate;
    }

    int year  = d->_yearPicker->getCurrentData().toInt();
    int month = 0;
    int day   = d->_dayPicker->getCurrentData().toInt();

    QString monthData = d->_monthPicker->getCurrentData();
    for (int i = 1; i <= 12; i++) {
        if (d->_pLocale.monthName(i, QLocale::ShortFormat) == monthData) {
            month = i;
            break;
        }
    }

    if (month == 0) {
        return d->_pDate;
    }

    QDate date(year, month, day);
    if (!date.isValid()) {
        return d->_pDate;
    }

    return date;
}

void NDatePicker::setMinimumDate(QDate MinimumDate) {
    Q_D(NDatePicker);
    if (!MinimumDate.isValid() || d->_pMinimumDate == MinimumDate) {
        return;
    }
    d->_pMinimumDate = MinimumDate;
    if (d->_pDate < d->_pMinimumDate) {
        setDate(d->_pMinimumDate);
    }
    d->rebuildPickers();
    Q_EMIT pMinimumDateChanged();
}

QDate NDatePicker::getMinimumDate() const {
    Q_D(const NDatePicker);
    return d->_pMinimumDate;
}

void NDatePicker::setMaximumDate(QDate MaximumDate) {
    Q_D(NDatePicker);
    if (!MaximumDate.isValid() || d->_pMaximumDate == MaximumDate) {
        return;
    }
    d->_pMaximumDate = MaximumDate;
    if (d->_pDate > d->_pMaximumDate) {
        setDate(d->_pMaximumDate);
    }
    d->rebuildPickers();
    Q_EMIT pMaximumDateChanged();
}

QDate NDatePicker::getMaximumDate() const {
    Q_D(const NDatePicker);
    return d->_pMaximumDate;
}

void NDatePicker::paintEvent([[maybe_unused]] QPaintEvent* event) {
    Q_D(NDatePicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect baseRect = rect();
    baseRect.adjust(1, 1, -1, -1);
    
    QColor bgColor;
    if (!isEnabled()) {
        bgColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
    } else if (underMouse()) {
        bgColor = NThemeColor(NFluentColorKey::ControlFillColorSecondary, d->_themeMode);
    } else {
        bgColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, d->_themeMode);
    }
    
    painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, d->_themeMode));
    painter.setBrush(bgColor);
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    QColor textColor = isEnabled() ? NThemeColor(NFluentColorKey::TextFillColorPrimary, d->_themeMode)
                                   : NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);

    int pickerXOffset = 0;
    for (int i = 0; i < d->_datePickerContainer->_pickerList.count(); i++) {
        auto picker = d->_datePickerContainer->_pickerList[i];
        painter.setPen(textColor);
        int     pickerWidth = picker->width();
        QString text        = picker->getCurrentData();
        painter.drawText(QRect(pickerXOffset, baseRect.y(), pickerWidth, baseRect.height()), Qt::AlignCenter, text);
        pickerXOffset += pickerWidth;
        if (i != d->_datePickerContainer->_pickerList.count() - 1) {
            painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, d->_themeMode));
            painter.drawLine(pickerXOffset, 0, pickerXOffset, baseRect.bottom());
        }
    }
    painter.restore();
}

void NDatePicker::setLocale(QLocale Locale) {
    Q_D(NDatePicker);
    if (d->_pLocale == Locale) {
        return;
    }
    d->_pLocale = Locale;
    d->rebuildPickers();
    update();
    Q_EMIT pLocaleChanged();
    Q_EMIT localeChanged(Locale);
}

QLocale NDatePicker::getLocale() const {
    Q_D(const NDatePicker);
    return d->_pLocale;
}
