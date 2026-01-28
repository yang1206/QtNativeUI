#include <QPainter>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NTimePicker.h>
#include "../private/ntimepicker_p.h"

Q_PROPERTY_CREATE_Q_CPP(NTimePicker, int, BorderRadius)

NTimePicker::NTimePicker(QWidget* parent) : QPushButton(parent), d_ptr(new NTimePickerPrivate()) {
    Q_D(NTimePicker);
    d->q_ptr             = this;
    d->_pBorderRadius    = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pTime            = QTime::currentTime();
    d->_pUse24HourFormat = true;
    d->_pShowSeconds     = false;

    setMouseTracking(true);
    setFixedSize(120, 32);
    setObjectName("NTimePicker");
    setStyleSheet("#NTimePicker{background-color:transparent;}");

    QFont font = this->font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    d->_timePickerContainer = new NTimePickerContainer(this);
    d->_timePickerContainer->resize(120, d->_pickerHeight + d->_timePickerContainer->getButtonAreaHeight());
    d->_timePickerContainer->hide();

    connect(this, &QPushButton::clicked, d, &NTimePickerPrivate::onTimePickerClicked);
    connect(d->_timePickerContainer,
            &NTimePickerContainer::confirmButtonClicked,
            d,
            &NTimePickerPrivate::onConfirmButtonClicked);
    connect(d->_timePickerContainer,
            &NTimePickerContainer::cancelButtonClicked,
            d,
            &NTimePickerPrivate::onCancelButtonClicked);

    d->_containerLayout = new QHBoxLayout(d->_timePickerContainer);
    d->_containerLayout->setSpacing(0);
    d->_containerLayout->setContentsMargins(0, 0, 0, 0);

    d->rebuildPickers();

    d->_themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

NTimePicker::~NTimePicker() {}

void NTimePicker::setTime(QTime Time) {
    Q_D(NTimePicker);
    if (!Time.isValid() || d->_pTime == Time) {
        return;
    }
    d->_pTime = Time;
    d->updatePickersFromTime();
    update();
    Q_EMIT pTimeChanged();
    Q_EMIT timeChanged(Time);
}

QTime NTimePicker::getTime() const {
    Q_D(const NTimePicker);
    if (d->_timePickerContainer->_pickerList.count() < 2) {
        return d->_pTime;
    }

    int hour   = d->_timePickerContainer->_pickerList[0]->getCurrentData().toInt();
    int minute = d->_timePickerContainer->_pickerList[1]->getCurrentData().toInt();
    int second = 0;

    if (d->_pShowSeconds && d->_timePickerContainer->_pickerList.count() >= 3) {
        second = d->_timePickerContainer->_pickerList[2]->getCurrentData().toInt();
    }

    if (!d->_pUse24HourFormat && hour == 12) {
        hour = 0;
    }

    return QTime(hour, minute, second);
}

void NTimePicker::setUse24HourFormat(bool Use24HourFormat) {
    Q_D(NTimePicker);
    if (d->_pUse24HourFormat == Use24HourFormat) {
        return;
    }
    d->_pUse24HourFormat = Use24HourFormat;
    d->rebuildPickers();
    update();
    Q_EMIT pUse24HourFormatChanged();
}

bool NTimePicker::getUse24HourFormat() const {
    Q_D(const NTimePicker);
    return d->_pUse24HourFormat;
}

void NTimePicker::setShowSeconds(bool ShowSeconds) {
    Q_D(NTimePicker);
    if (d->_pShowSeconds == ShowSeconds) {
        return;
    }
    d->_pShowSeconds = ShowSeconds;
    d->rebuildPickers();
    update();
    Q_EMIT pShowSecondsChanged();
}

bool NTimePicker::getShowSeconds() const {
    Q_D(const NTimePicker);
    return d->_pShowSeconds;
}

void NTimePicker::paintEvent([[maybe_unused]] QPaintEvent* event) {
    Q_D(NTimePicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect baseRect = rect();
    baseRect.adjust(1, 1, -1, -1);
    painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, d->_themeMode));
    painter.setBrush(underMouse() ? NThemeColor(NFluentColorKey::ControlFillColorSecondary, d->_themeMode)
                                  : NThemeColor(NFluentColorKey::ControlFillColorDefault, d->_themeMode));
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    int pickerXOffset = 0;
    for (int i = 0; i < d->_timePickerContainer->_pickerList.count(); i++) {
        auto picker = d->_timePickerContainer->_pickerList[i];
        painter.setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, d->_themeMode));
        int     pickerWidth = picker->width();
        QString text        = picker->getCurrentData();
        painter.drawText(QRect(pickerXOffset, baseRect.y(), pickerWidth, baseRect.height()), Qt::AlignCenter, text);
        pickerXOffset += pickerWidth;
        if (i != d->_timePickerContainer->_pickerList.count() - 1) {
            painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, d->_themeMode));
            painter.drawLine(pickerXOffset, 0, pickerXOffset, baseRect.bottom());
        }
    }
    painter.restore();
}
