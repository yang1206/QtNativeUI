#include "CalendarDatePickerPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NCalendarDatePicker.h>
#include <QtNativeUI/NCalendarWidget.h>
#include <QtNativeUI/NLabel.h>

CalendarDatePickerPage::CalendarDatePickerPage(QWidget* parent)
    : BasePage("CalendarDatePicker 日期选择器",
               "CalendarDatePicker 提供下拉式日期选择器，支持单日期、多日期和日期范围选择。",
               parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NCalendarDatePicker* basicPicker = new NCalendarDatePicker(this);
    basicPicker->setMinimumWidth(250);

    QLabel* basicSelectedLabel = new QLabel("选择的日期: 无", this);
    connect(basicPicker, &NCalendarDatePicker::dateSelected, [basicSelectedLabel](QDate date) {
        basicSelectedLabel->setText("选择的日期: " + date.toString("yyyy-MM-dd"));
    });

    basicLayout->addWidget(basicPicker);
    basicLayout->addWidget(basicSelectedLabel);
    basicLayout->addStretch();

    addSection("基本日期选择器", basicLayout);

    QHBoxLayout* presetLayout = new QHBoxLayout();
    presetLayout->setSpacing(16);

    NCalendarDatePicker* presetPicker = new NCalendarDatePicker(this);
    presetPicker->setSelectedDate(QDate::currentDate());
    presetPicker->setMinimumWidth(250);

    QLabel* presetLabel = new QLabel("预设为今天", this);

    presetLayout->addWidget(presetPicker);
    presetLayout->addWidget(presetLabel);
    presetLayout->addStretch();

    addSection("预设日期", presetLayout);

    QHBoxLayout* formatLayout = new QHBoxLayout();
    formatLayout->setSpacing(16);

    NCalendarDatePicker* formatPicker1 = new NCalendarDatePicker(this);
    formatPicker1->setSelectedDate(QDate::currentDate());
    formatPicker1->setDateFormat("yyyy年MM月dd日");
    formatPicker1->setMinimumWidth(200);

    NCalendarDatePicker* formatPicker2 = new NCalendarDatePicker(this);
    formatPicker2->setSelectedDate(QDate::currentDate());
    formatPicker2->setDateFormat("MM/dd/yyyy");
    formatPicker2->setMinimumWidth(200);

    NCalendarDatePicker* formatPicker3 = new NCalendarDatePicker(this);
    formatPicker3->setSelectedDate(QDate::currentDate());
    formatPicker3->setDateFormat("dd.MM.yyyy");
    formatPicker3->setMinimumWidth(200);

    formatLayout->addWidget(formatPicker1);
    formatLayout->addWidget(formatPicker2);
    formatLayout->addWidget(formatPicker3);
    formatLayout->addStretch();

    addSection("自定义日期格式", formatLayout);

    QHBoxLayout* modeLayout = new QHBoxLayout();
    modeLayout->setSpacing(32);

    QVBoxLayout* singleLayout    = new QVBoxLayout();
    QLabel*      singleModeLabel = new QLabel("单日期选择", this);
    singleModeLabel->setAlignment(Qt::AlignCenter);
    NCalendarDatePicker* singlePicker = new NCalendarDatePicker(this);
    singlePicker->setDateSelectionMode(NCalendarWidget::SingleDate);
    singlePicker->setMinimumWidth(200);
    QLabel* singleSelectedLabel = new QLabel("选择的日期: 无", this);
    connect(singlePicker, &NCalendarDatePicker::dateSelected, [singleSelectedLabel](QDate date) {
        singleSelectedLabel->setText("选择的日期: " + date.toString("yyyy-MM-dd"));
    });
    singleLayout->addWidget(singleModeLabel);
    singleLayout->addWidget(singlePicker);
    singleLayout->addWidget(singleSelectedLabel);

    QVBoxLayout* multiLayout    = new QVBoxLayout();
    QLabel*      multiModeLabel = new QLabel("多日期选择", this);
    multiModeLabel->setAlignment(Qt::AlignCenter);
    NCalendarDatePicker* multiPicker = new NCalendarDatePicker(this);
    multiPicker->setDateSelectionMode(NCalendarWidget::MultipleDate);
    multiPicker->setMinimumWidth(200);
    QLabel* multiSelectedLabel = new QLabel("选择的日期: 无", this);
    connect(multiPicker, &NCalendarDatePicker::selectedDatesChanged, [multiSelectedLabel](const QList<QDate>& dates) {
        if (dates.isEmpty()) {
            multiSelectedLabel->setText("选择的日期: 无");
            return;
        }
        QString dateString = "选择的日期:\n";
        for (const QDate& date : dates) {
            dateString += date.toString("yyyy-MM-dd") + "\n";
        }
        multiSelectedLabel->setText(dateString);
    });
    multiLayout->addWidget(multiModeLabel);
    multiLayout->addWidget(multiPicker);
    multiLayout->addWidget(multiSelectedLabel);

    QVBoxLayout* rangeLayout    = new QVBoxLayout();
    QLabel*      rangeModeLabel = new QLabel("日期范围选择", this);
    rangeModeLabel->setAlignment(Qt::AlignCenter);
    NCalendarDatePicker* rangePicker = new NCalendarDatePicker(this);
    rangePicker->setDateSelectionMode(NCalendarWidget::DateRange);
    rangePicker->setMinimumWidth(200);
    QLabel* rangeSelectedLabel = new QLabel("选择的日期范围: 无", this);
    connect(rangePicker, &NCalendarDatePicker::selectedDateRangeChanged, [rangeSelectedLabel](const QPair<QDate, QDate>& range) {
        if (range.first.isValid() && range.second.isValid()) {
            rangeSelectedLabel->setText("选择的日期范围:\n" + range.first.toString("yyyy-MM-dd") + " 至\n" +
                                        range.second.toString("yyyy-MM-dd"));
        } else {
            rangeSelectedLabel->setText("选择的日期范围: 无");
        }
    });
    rangeLayout->addWidget(rangeModeLabel);
    rangeLayout->addWidget(rangePicker);
    rangeLayout->addWidget(rangeSelectedLabel);

    modeLayout->addLayout(singleLayout);
    modeLayout->addLayout(multiLayout);
    modeLayout->addLayout(rangeLayout);
    modeLayout->addStretch();

    addSection("不同选择模式", modeLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(32);

    QVBoxLayout* disabledLayout = new QVBoxLayout();
    NCalendarDatePicker* disabledPicker = new NCalendarDatePicker(this);
    disabledPicker->setSelectedDate(QDate::currentDate());
    disabledPicker->setEnabled(false);
    disabledPicker->setMinimumWidth(180);
    QLabel* disabledLabel = new QLabel("禁用状态", this);
    disabledLabel->setAlignment(Qt::AlignCenter);
    disabledLayout->addWidget(disabledPicker);
    disabledLayout->addWidget(disabledLabel);

    QVBoxLayout* placeholderLayout = new QVBoxLayout();
    NCalendarDatePicker* placeholderPicker = new NCalendarDatePicker(this);
    placeholderPicker->setPlaceholderText("请选择一个日期");
    placeholderPicker->setMinimumWidth(180);
    QLabel* placeholderLabel = new QLabel("自定义占位符", this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLayout->addWidget(placeholderPicker);
    placeholderLayout->addWidget(placeholderLabel);

    stateLayout->addLayout(disabledLayout);
    stateLayout->addLayout(placeholderLayout);
    stateLayout->addStretch();

    addSection("不同状态", stateLayout);

    QHBoxLayout* limitLayout = new QHBoxLayout();
    limitLayout->setSpacing(16);

    NCalendarDatePicker* limitedPicker = new NCalendarDatePicker(this);
    limitedPicker->setMinimumDate(QDate::currentDate().addDays(-10));
    limitedPicker->setMaximumDate(QDate::currentDate().addDays(10));
    limitedPicker->setMinimumWidth(250);

    QLabel* rangeInfoLabel = new QLabel(QString("限制范围: %1 至 %2")
                                            .arg(QDate::currentDate().addDays(-10).toString("yyyy-MM-dd"),
                                                 QDate::currentDate().addDays(10).toString("yyyy-MM-dd")),
                                        this);

    limitLayout->addWidget(limitedPicker);
    limitLayout->addWidget(rangeInfoLabel);
    limitLayout->addStretch();

    addSection("日期范围限制", limitLayout);
}
