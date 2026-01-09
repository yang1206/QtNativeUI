#include "ncalendardatepicker_p.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QTimer>
#include "QtNativeUI/NIcon.h"

#include "QtNativeUI/NFlyout.h"

NCalendarDatePickerPrivate::NCalendarDatePickerPrivate(NCalendarDatePicker* q)
    : QObject(q), q_ptr(q), button(nullptr), contentWidget(nullptr), dateLabel(nullptr), iconLabel(nullptr) {
    _pBorderRadius    = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    _themeMode        = nTheme->themeMode();
    _isDark           = nTheme->isDarkMode();
    _pSelectedDate    = QDate();
    _pMinimumDate     = QDate(1601, 1, 1);
    _pMaximumDate     = QDate(9999, 12, 31);
    _pPlaceholderText = tr("Pick a date");
    _pDateFormat      = "yyyy-MM-dd";
    _locale           = QLocale::system();
    _isAccentStyle    = false;
    _selectionMode    = NCalendarWidget::SingleDate;
}

NCalendarDatePickerPrivate::~NCalendarDatePickerPrivate() {}

void NCalendarDatePickerPrivate::initUi() {
    q_ptr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    button = new NPushButton(q_ptr);
    button->setObjectName("NCalendarDatePickerButton");
    button->setTransparentBackground(false);
    button->setBorderRadius(_pBorderRadius);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    contentWidget = new QWidget(q_ptr);
    contentWidget->setObjectName("NCalendarDatePickerContent");
    contentWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    dateLabel = new QLabel(contentWidget);
    dateLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    iconLabel = new QLabel(contentWidget);
    iconLabel->setFixedSize(16, 16);
    QIcon calendarIcon = nIcon->fromRegular(NRegularIconType::CalendarLtr12Regular, 16);
    iconLabel->setPixmap(calendarIcon.pixmap(16, 16));

    QHBoxLayout* layout = new QHBoxLayout(contentWidget);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(NSpacingToken(NDesignTokenKey::SpacingM).toInt());
    layout->addWidget(dateLabel, 1);
    layout->addStretch();
    layout->addWidget(iconLabel);

    QHBoxLayout* mainLayout = new QHBoxLayout(q_ptr);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(button);

    contentWidget->raise();
    contentWidget->resize(q_ptr->size());

    updateDisplayText();

    QObject::connect(nTheme, &NTheme::themeModeChanged, [this](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        _isDark    = nTheme->isDarkMode();
        updateDisplayText();
        q_ptr->update();
    });

    QObject::connect(button, &NPushButton::clicked, this, &NCalendarDatePickerPrivate::showCalendarFlyout);
}

void NCalendarDatePickerPrivate::updateDisplayText() {
    QString displayText = _pPlaceholderText;

    if (_selectionMode == NCalendarWidget::SingleDate && _pSelectedDate.isValid()) {
        displayText = _locale.toString(_pSelectedDate, _pDateFormat);
    } else if (_selectionMode == NCalendarWidget::MultipleDate && !_selectedDates.isEmpty()) {
        if (_selectedDates.size() == 1) {
            displayText = _locale.toString(_selectedDates.first(), _pDateFormat);
        } else {
            displayText = QString("%1 (%2)")
                              .arg(_locale.toString(_selectedDates.first(), _pDateFormat))
                              .arg(_selectedDates.size());
        }
    } else if (_selectionMode == NCalendarWidget::DateRange && _selectedDateRange.first.isValid() &&
               _selectedDateRange.second.isValid()) {
        displayText = QString("%1 - %2")
                          .arg(_locale.toString(_selectedDateRange.first, _pDateFormat))
                          .arg(_locale.toString(_selectedDateRange.second, _pDateFormat));
    }

    dateLabel->setText(displayText);

    QColor textColor;
    if (_isDark) {
        textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    } else {
        textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    }

    QString colorStyle = QString("color: %1;").arg(textColor.name());
    dateLabel->setStyleSheet(colorStyle);

    QIcon calendarIcon = nIcon->fromRegular(NRegularIconType::CalendarLtr12Regular, 16, textColor);
    iconLabel->setPixmap(calendarIcon.pixmap(16, 16));
}

void NCalendarDatePickerPrivate::showCalendarFlyout() {
    if (currentFlyout) {
        return;
    }

    NCalendarWidget* calendarWidget = new NCalendarWidget(q_ptr);

    calendarWidget->setLocale(_locale);
    calendarWidget->setDateSelectionMode(_selectionMode);


    calendarWidget->setMinimumDate(_pMinimumDate);
    calendarWidget->setMaximumDate(_pMaximumDate);


    switch (_selectionMode) {
        case NCalendarWidget::SingleDate:
            if (_pSelectedDate.isValid()) {
                calendarWidget->setSelectedDate(_pSelectedDate);
            }
            break;
        case NCalendarWidget::MultipleDate:
            if (!_selectedDates.isEmpty()) {
                calendarWidget->setSelectedDates(_selectedDates);
            }
            break;
        case NCalendarWidget::DateRange:
            if (_selectedDateRange.first.isValid() && _selectedDateRange.second.isValid()) {
                calendarWidget->setDateRange(_selectedDateRange.first, _selectedDateRange.second);
            }
            break;
    }

    QWidget* content = new QWidget();
    content->setFixedSize(305, 340);
    content->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(calendarWidget);

    NFlyout* flyout = NFlyout::createWithContent(content, button, q_ptr);
    flyout->setContentsMargins(0, 0, 0, 0);
    flyout->setPlacement(Qt::BottomEdge);


    currentFlyout = flyout;


    QObject::connect(calendarWidget, &NCalendarWidget::clicked, this, &NCalendarDatePickerPrivate::handleDateSelection);

    QObject::connect(calendarWidget,
                     &NCalendarWidget::selectedDatesChanged,
                     this,
                     &NCalendarDatePickerPrivate::handleMultiDateSelection);

    QObject::connect(calendarWidget,
                     &NCalendarWidget::selectedDateRangeChanged,
                     this,
                     &NCalendarDatePickerPrivate::handleDateRangeSelection);


    QObject::connect(flyout, &NFlyout::opened, q_ptr, &NCalendarDatePicker::popupOpened);
    QObject::connect(flyout, &NFlyout::closed, this, [this]() {

        currentFlyout = nullptr;
        q_ptr->emit popupClosed();
    });


    flyout->showAt(button);
}

void NCalendarDatePickerPrivate::handleDateSelection(const QDate& date) {
    if (date.isValid()) {
        if (_selectionMode == NCalendarWidget::SingleDate) {
            _pSelectedDate = date;
            updateDisplayText();
            q_ptr->emit pSelectedDateChanged();
            q_ptr->emit dateSelected(date);
            currentFlyout->fadeOut();
        }
    }
}

void NCalendarDatePickerPrivate::handleMultiDateSelection(const QList<QDate>& dates) {
    if (_selectionMode == NCalendarWidget::MultipleDate) {
        _selectedDates = dates;
        if (!dates.isEmpty()) {
            _pSelectedDate = dates.first();
            updateDisplayText();
            q_ptr->emit pSelectedDateChanged();
        }
        q_ptr->emit selectedDatesChanged(dates);
    }
}

void NCalendarDatePickerPrivate::handleDateRangeSelection(const QPair<QDate, QDate>& range) {
    if (_selectionMode == NCalendarWidget::DateRange) {
        _selectedDateRange = range;
        if (range.first.isValid()) {
            _pSelectedDate = range.first;
            updateDisplayText();
            q_ptr->emit pSelectedDateChanged();
        }
        q_ptr->emit selectedDateRangeChanged(range);
    }
}