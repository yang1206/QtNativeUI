#include <QEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QtNativeUI/NCalendarWidget.h>

#include "../private/ncalendardelegate.h"
#include "../private/ncalendarheader.h"
#include "../private/ncalendarmodel.h"
#include "../private/ncalendarwidget_p.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"
#include "nbaselistview.h"

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, int, BorderRadius)

NCalendarWidget::NCalendarWidget(QWidget* parent) : QWidget{parent}, d_ptr(new NCalendarWidgetPrivate()) { init(); }

NCalendarWidget::~NCalendarWidget() {}

void NCalendarWidget::init() {
    Q_D(NCalendarWidget);
    setFixedSize(305, 340);
    setObjectName("NCalendarWidget");
    d->q_ptr          = this;
    d->_pBorderRadius = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();

    // 日历标题
    d->_calendarTitleView = new NBaseListView(this);
    d->_calendarTitleView->setFlow(NBaseListView::LeftToRight);
    d->_calendarTitleView->setViewMode(NBaseListView::IconMode);
    d->_calendarTitleView->setResizeMode(NBaseListView::Adjust);
    d->_calendarTitleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarTitleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarTitleView->setModel(new NCalendarTitleModel(this));
    d->_calendarTitleView->setItemDelegate(new NCalendarTitleDelegate(this));
    d->_calendarTitleView->setFixedHeight(30);

    // 日历内容
    d->_calendarView = new NBaseListView(this);
    d->_calendarView->setFlow(NBaseListView::LeftToRight);
    d->_calendarView->setViewMode(NBaseListView::IconMode);
    d->_calendarView->setResizeMode(NBaseListView::Adjust);
    d->_calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setSelectionMode(QAbstractItemView::NoSelection);
    d->_calendarModel = new NCalendarModel(this);
    d->_calendarView->setModel(d->_calendarModel);
    d->_calendarDelegate = new NCalendarDelegate(d->_calendarModel, this);
    d->_calendarView->setItemDelegate(d->_calendarDelegate);
    connect(d->_calendarView, &QListView::clicked, d, &NCalendarWidgetPrivate::onCalendarViewClicked);

    // 模式切换按钮
    d->_modeSwitchButton = new NPushButton(this);
    d->_modeSwitchButton->setText("1924年1月");
    QFont switchButtonFont = d->_modeSwitchButton->font();
    switchButtonFont.setWeight(QFont::Bold);
    d->_modeSwitchButton->setFont(switchButtonFont);
    d->_modeSwitchButton->setTransparentBackground(true);
    d->_modeSwitchButton->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d->_modeSwitchButton->setContentsMargins(10, 0, 0, 0);
    d->_modeSwitchButton->setFixedHeight(36);
    d->_modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(d->_modeSwitchButton, &NPushButton::clicked, d, &NCalendarWidgetPrivate::onSwitchButtonClicked);
    connect(
        d->_calendarView->verticalScrollBar(), &QScrollBar::valueChanged, d, [=]() { d->_updateSwitchButtonText(); });

    d->_upButton = new NPushButton(this);
    d->_upButton->setTransparentBackground(true);
    d->_upButton->setFixedSize(36, 36);
    d->_upButton->setFluentIcon(NFilledIconType::CaretUp16Filled);
    connect(d->_upButton, &NPushButton::clicked, d, &NCalendarWidgetPrivate::onUpButtonClicked);

    d->_downButton = new NPushButton(this);
    d->_downButton->setTransparentBackground(true);
    d->_downButton->setFixedSize(36, 36);
    d->_downButton->setFluentIcon(NFilledIconType::CaretDown16Filled);
    connect(d->_downButton, &NPushButton::clicked, d, &NCalendarWidgetPrivate::onDownButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(5, 5, 10, 0);
    buttonLayout->addWidget(d->_modeSwitchButton);
    buttonLayout->addWidget(d->_upButton);
    buttonLayout->addWidget(d->_downButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 0, 0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(d->_calendarTitleView);
    mainLayout->addWidget(d->_calendarView);

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [this, d](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
    d->_locale         = QLocale::system();
    int firstDayOfWeek = d->_locale.firstDayOfWeek();
    d->_calendarModel->setFirstDayOfWeek(firstDayOfWeek);
    d->_selectionMode = SingleDate;
    d->_selectedDates.clear();
    d->_selectedDateRange = QPair<QDate, QDate>(QDate(), QDate());
    setVisible(true);
    QDate currentDate     = QDate::currentDate();
    d->_lastSelectedYear  = currentDate.year();
    d->_lastSelectedMonth = currentDate.month();
    d->_scrollToDate(currentDate);
}

void NCalendarWidget::setSelectedDate(QDate selectedDate) {
    Q_D(NCalendarWidget);
    if (!selectedDate.isValid() || selectedDate.daysTo(d->_calendarModel->getMaximumDate()) < 0 ||
        selectedDate.daysTo(d->_calendarModel->getMinimumDate()) > 0) {
        return;
    }
    d->_pSelectedDate = selectedDate;
    d->_calendarView->selectionModel()->setCurrentIndex(d->_calendarModel->getIndexFromDate(selectedDate),
                                                        QItemSelectionModel::Select);
    Q_EMIT pSelectedDateChanged();
}

QDate NCalendarWidget::getSelectedDate() const {
    Q_D(const NCalendarWidget);
    return d->_pSelectedDate;
    // return d->_calendarModel->getDateFromIndex(d->_calendarView->selectionModel()->currentIndex());
}

void NCalendarWidget::setMinimumDate(QDate minimudate) {
    Q_D(NCalendarWidget);
    if (!minimudate.isValid() || minimudate.daysTo(d->_calendarModel->getMaximumDate()) < 0) {
        return;
    }
    d->_calendarModel->setMaximumDate(minimudate);
    Q_EMIT pMinimumDateChanged();
}

QDate NCalendarWidget::getMinimumDate() const {
    Q_D(const NCalendarWidget);
    return d->_calendarModel->getMinimumDate();
}

void NCalendarWidget::setMaximumDate(QDate maximumDate) {
    Q_D(NCalendarWidget);
    if (!maximumDate.isValid() || maximumDate.daysTo(d->_calendarModel->getMinimumDate()) > 0) {
        return;
    }
    d->_calendarModel->setMaximumDate(maximumDate);
    Q_EMIT pMaximumDateChanged();
}

QDate NCalendarWidget::getMaximumDate() const {
    Q_D(const NCalendarWidget);
    return d->_calendarModel->getMaximumDate();
}

void NCalendarWidget::setLocale(const QLocale& locale) {
    Q_D(NCalendarWidget);
    if (d->_locale != locale) {
        d->_locale = locale;
        d->_calendarModel->setFirstDayOfWeek(locale.firstDayOfWeek());

        d->_calendarModel->setLocale(locale);
        d->_updateSwitchButtonText();

        if (NCalendarTitleModel* titleModel = qobject_cast<NCalendarTitleModel*>(d->_calendarTitleView->model())) {
            titleModel->setFirstDayOfWeek(locale.firstDayOfWeek());
            titleModel->setLocale(locale);
        }

        d->_calendarModel->invalidate();
        update();

        emit localeChanged(locale);
    }
}

void NCalendarWidget::setDateSelectionMode(DateSelectionMode mode) {
    Q_D(NCalendarWidget);
    if (d->_selectionMode != mode) {
        d->_selectionMode = mode;
        if (mode == SingleDate) {
            d->_selectedDates.clear();
            d->_selectedDateRange = QPair<QDate, QDate>(QDate(), QDate());
        } else if (mode == MultipleDate) {
            d->_selectedDateRange = QPair<QDate, QDate>(QDate(), QDate());
            if (d->_pSelectedDate.isValid() && !d->_selectedDates.contains(d->_pSelectedDate)) {
                d->_selectedDates.append(d->_pSelectedDate);
            }
        } else if (mode == DateRange) {
            d->_selectedDates.clear();
            if (d->_pSelectedDate.isValid()) {
                d->_selectedDateRange.first  = d->_pSelectedDate;
                d->_selectedDateRange.second = d->_pSelectedDate;
            }
        }
        d->updateDateSelection();
        update();
        emit dateSelectionModeChanged(mode);
    }
}

NCalendarWidget::DateSelectionMode NCalendarWidget::dateSelectionMode() const {
    Q_D(const NCalendarWidget);
    return d->_selectionMode;
}

void NCalendarWidget::setSelectedDates(const QList<QDate>& dates) {
    Q_D(NCalendarWidget);
    setDateSelectionMode(MultipleDate);
    QList<QDate> validDates;
    for (const QDate& date : dates) {
        if (date.isValid() && date >= d->_calendarModel->getMinimumDate() &&
            date <= d->_calendarModel->getMaximumDate()) {
            validDates.append(date);
        }
    }
    if (d->_selectedDates != validDates) {
        d->_selectedDates = validDates;
        if (!validDates.isEmpty()) {
            d->_pSelectedDate = validDates.first();
            emit pSelectedDateChanged();
        }
        d->updateDateSelection();
        update();
        emit selectedDatesChanged(validDates);
    }
}

QList<QDate> NCalendarWidget::selectedDates() const {
    Q_D(const NCalendarWidget);
    return d->_selectedDates;
}

// 日期范围相关方法
void NCalendarWidget::setDateRange(const QDate& startDate, const QDate& endDate) {
    Q_D(NCalendarWidget);
    setDateSelectionMode(DateRange);
    QDate validStartDate = startDate;
    QDate validEndDate   = endDate;
    if (!validStartDate.isValid() || validStartDate < d->_calendarModel->getMinimumDate()) {
        validStartDate = d->_calendarModel->getMinimumDate();
    }
    if (!validEndDate.isValid() || validEndDate > d->_calendarModel->getMaximumDate()) {
        validEndDate = d->_calendarModel->getMaximumDate();
    }
    if (validStartDate > validEndDate) {
        std::swap(validStartDate, validEndDate);
    }
    QPair<QDate, QDate> newRange(validStartDate, validEndDate);
    if (d->_selectedDateRange != newRange) {
        d->_selectedDateRange = newRange;

        d->_pSelectedDate = validStartDate;
        emit pSelectedDateChanged();

        d->updateDateSelection();
        update();

        emit selectedDateRangeChanged(newRange);
    }
}

QPair<QDate, QDate> NCalendarWidget::selectedDateRange() const {
    Q_D(const NCalendarWidget);
    return d->_selectedDateRange;
}

QLocale NCalendarWidget::locale() const {
    Q_D(const NCalendarWidget);
    return d->_locale;
}

void NCalendarWidget::paintEvent(QPaintEvent* event) {
    Q_D(NCalendarWidget);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect baseRect = rect();
    baseRect.adjust(d->_borderWidth, d->_borderWidth, -d->_borderWidth, -d->_borderWidth);
    painter.setPen(Qt::NoPen);

    QColor bgColor = d->_isDark ? NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Dark)
                                : NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Light);
    painter.setBrush(bgColor);

    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    if (!d->_isSwitchAnimationFinished) {
        painter.save();
        QRect pixRect = QRect(baseRect.x(), d->_borderWidth + 45, baseRect.width(), baseRect.height() - 45);
        painter.setOpacity(d->_pPixOpacity);
        painter.translate(pixRect.center());
        painter.scale(d->_pZoomRatio, d->_pZoomRatio);
        painter.translate(-pixRect.center());
        painter.drawPixmap(pixRect, d->_isDrawNewPix ? d->_newCalendarViewPix : d->_oldCalendarViewPix);
        painter.restore();
    }

    QColor borderColor = d->_isDark ? NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark)
                                    : NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    painter.setPen(QPen(borderColor, d->_borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.drawLine(QPointF(baseRect.x(), 45), QPointF(baseRect.right(), 45));

    painter.restore();
}