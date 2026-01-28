#include "CalendarWidgetPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NCalendarWidget.h>
#include <QtNativeUI/NComboBox.h>
#include <QtNativeUI/NGroupBox.h>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>

CalendarWidgetPage::CalendarWidgetPage(QWidget* parent)
    : BasePage("CalendarWidget 日历控件", "CalendarWidget 提供日历视图，支持单日期、多日期和日期范围选择。", parent) {

    QHBoxLayout* singleLayout = new QHBoxLayout();
    singleLayout->setSpacing(24);

    NCalendarWidget* singleCalendar = new NCalendarWidget(this);
    singleCalendar->setDateSelectionMode(NCalendarWidget::SingleDate);
    singleCalendar->setFixedSize(350, 320);

    QLabel* selectedDateLabel = new QLabel("选择的日期：无", this);
    connect(singleCalendar, &NCalendarWidget::clicked, [selectedDateLabel](QDate date) {
        selectedDateLabel->setText("选择的日期：" + date.toString("yyyy-MM-dd"));
    });

    QVBoxLayout* singleInfoLayout = new QVBoxLayout();
    singleInfoLayout->addWidget(selectedDateLabel);
    singleInfoLayout->addStretch();

    singleLayout->addWidget(singleCalendar);
    singleLayout->addLayout(singleInfoLayout);
    singleLayout->addStretch();

    addSection("单日期选择模式", singleLayout);

    QHBoxLayout* multipleLayout = new QHBoxLayout();
    multipleLayout->setSpacing(24);

    NCalendarWidget* multipleCalendar = new NCalendarWidget(this);
    multipleCalendar->setDateSelectionMode(NCalendarWidget::MultipleDate);
    multipleCalendar->setFixedSize(350, 320);

    QLabel* multipleDatesLabel = new QLabel("选择的日期：无", this);
    connect(multipleCalendar, &NCalendarWidget::selectedDatesChanged, [multipleDatesLabel](const QList<QDate>& dates) {
        if (dates.isEmpty()) {
            multipleDatesLabel->setText("选择的日期：无");
            return;
        }
        QString dateStr = "选择的日期：\n";
        for (const QDate& date : dates) {
            dateStr += date.toString("yyyy-MM-dd") + "\n";
        }
        multipleDatesLabel->setText(dateStr);
    });

    QVBoxLayout* multipleInfoLayout = new QVBoxLayout();
    multipleInfoLayout->addWidget(multipleDatesLabel);
    multipleInfoLayout->addStretch();

    multipleLayout->addWidget(multipleCalendar);
    multipleLayout->addLayout(multipleInfoLayout);
    multipleLayout->addStretch();

    addSection("多日期选择模式", multipleLayout);

    QHBoxLayout* rangeLayout = new QHBoxLayout();
    rangeLayout->setSpacing(24);

    NCalendarWidget* rangeCalendar = new NCalendarWidget(this);
    rangeCalendar->setDateSelectionMode(NCalendarWidget::DateRange);
    rangeCalendar->setFixedSize(350, 320);

    QLabel* rangeDatesLabel = new QLabel("选择的日期范围：无", this);
    connect(rangeCalendar, &NCalendarWidget::selectedDateRangeChanged, [rangeDatesLabel](const QPair<QDate, QDate>& range) {
        if (range.first.isValid() && range.second.isValid()) {
            rangeDatesLabel->setText("选择的日期范围：\n" + range.first.toString("yyyy-MM-dd") + " 至 " +
                                     range.second.toString("yyyy-MM-dd"));
        } else if (range.first.isValid()) {
            rangeDatesLabel->setText("选择的日期：" + range.first.toString("yyyy-MM-dd"));
        } else {
            rangeDatesLabel->setText("选择的日期范围：无");
        }
    });

    QVBoxLayout* rangeInfoLayout = new QVBoxLayout();
    rangeInfoLayout->addWidget(rangeDatesLabel);
    rangeInfoLayout->addStretch();

    rangeLayout->addWidget(rangeCalendar);
    rangeLayout->addLayout(rangeInfoLayout);
    rangeLayout->addStretch();

    addSection("日期范围选择模式", rangeLayout);

    NGroupBox*   controlsGroup  = new NGroupBox("日历控制", this);
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);

    QHBoxLayout* dateRangeLayout = new QHBoxLayout();
    QLabel*      minDateLabel    = new QLabel("最小日期:", this);
    QLabel*      maxDateLabel    = new QLabel("最大日期:", this);

    NLineEdit* minDateEdit = new NLineEdit(this);
    minDateEdit->setPlaceholderText("YYYY-MM-DD");
    minDateEdit->setMinimumWidth(120);

    NLineEdit* maxDateEdit = new NLineEdit(this);
    maxDateEdit->setPlaceholderText("YYYY-MM-DD");
    maxDateEdit->setMinimumWidth(120);

    NPushButton* setRangeButton = new NPushButton("设置日期范围", this);
    setRangeButton->setFixedSize(120, 32);

    connect(setRangeButton, &NPushButton::clicked, [=]() {
        QDate minDate = QDate::fromString(minDateEdit->text(), "yyyy-MM-dd");
        QDate maxDate = QDate::fromString(maxDateEdit->text(), "yyyy-MM-dd");

        if (minDate.isValid() && maxDate.isValid()) {
            singleCalendar->setMinimumDate(minDate);
            singleCalendar->setMaximumDate(maxDate);
            multipleCalendar->setMinimumDate(minDate);
            multipleCalendar->setMaximumDate(maxDate);
            rangeCalendar->setMinimumDate(minDate);
            rangeCalendar->setMaximumDate(maxDate);
        }
    });

    dateRangeLayout->addWidget(minDateLabel);
    dateRangeLayout->addWidget(minDateEdit);
    dateRangeLayout->addWidget(maxDateLabel);
    dateRangeLayout->addWidget(maxDateEdit);
    dateRangeLayout->addWidget(setRangeButton);
    dateRangeLayout->addStretch();

    controlsLayout->addLayout(dateRangeLayout);

    QHBoxLayout* localeLayout = new QHBoxLayout();
    QLabel*      localeLabel  = new QLabel("设置语言:", this);

    NComboBox* localeCombo = new NComboBox(this);
    localeCombo->addItem("中文", QLocale::Chinese);
    localeCombo->addItem("英文", QLocale::English);
    localeCombo->addItem("法文", QLocale::French);
    localeCombo->addItem("德文", QLocale::German);
    localeCombo->addItem("日文", QLocale::Japanese);
    localeCombo->setMinimumWidth(120);

    NPushButton* setLocaleButton = new NPushButton("应用", this);
    setLocaleButton->setFixedSize(80, 32);

    connect(setLocaleButton, &NPushButton::clicked, [=]() {
        QLocale::Language lang = static_cast<QLocale::Language>(localeCombo->currentData().toInt());
        QLocale           locale(lang);

        singleCalendar->setLocale(locale);
        multipleCalendar->setLocale(locale);
        rangeCalendar->setLocale(locale);
    });

    localeLayout->addWidget(localeLabel);
    localeLayout->addWidget(localeCombo);
    localeLayout->addWidget(setLocaleButton);
    localeLayout->addStretch();

    controlsLayout->addLayout(localeLayout);

    addSection("交互控制", controlsGroup);
}
