#include "DatePickerPage.h"
#include <QDate>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNativeUI/NDatePicker.h>
#include <QtNativeUI/NLabel.h>

DatePickerPage::DatePickerPage(QWidget* parent)
    : BasePage("DatePicker 日期选择器", "DatePicker 提供日期选择功能，支持日期范围限制和国际化。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);
    NDatePicker* basicDatePicker = new NDatePicker(this);
    basicDatePicker->setDate(QDate::currentDate());
    NLabel* basicResultLabel = new NLabel("选择日期: " + basicDatePicker->getDate().toString("yyyy-MM-dd"), this);
    connect(basicDatePicker, &NDatePicker::dateChanged, [basicResultLabel](const QDate& date) {
        basicResultLabel->setText("选择日期: " + date.toString("yyyy-MM-dd"));
    });
    basicLayout->addWidget(basicDatePicker);
    basicLayout->addWidget(basicResultLabel);
    basicLayout->addStretch();
    addSection("基本用法", basicLayout);

    QGridLayout* localeLayout = new QGridLayout();
    localeLayout->setSpacing(16);
    localeLayout->setColumnStretch(2, 1);

    struct LocaleDemo {
        QString     name;
        QLocale     locale;
        const char* flag;
    };

    QList<LocaleDemo> locales = {
        {"中文 (简体)", QLocale(QLocale::Chinese, QLocale::SimplifiedChineseScript, QLocale::China), "🇨🇳"},
        {"English (US)", QLocale(QLocale::English, QLocale::UnitedStates), "🇺🇸"},
        {"English (UK)", QLocale(QLocale::English, QLocale::UnitedKingdom), "🇬🇧"},
        {"Français", QLocale(QLocale::French, QLocale::France), "🇫🇷"},
        {"Deutsch", QLocale(QLocale::German, QLocale::Germany), "🇩🇪"},
        {"日本語", QLocale(QLocale::Japanese, QLocale::Japan), "🇯🇵"},
        {"한국어", QLocale(QLocale::Korean, QLocale::SouthKorea), "🇰🇷"},
        {"Español", QLocale(QLocale::Spanish, QLocale::Spain), "🇪🇸"},
        {"Italiano", QLocale(QLocale::Italian, QLocale::Italy), "🇮🇹"},
        {"Русский", QLocale(QLocale::Russian, QLocale::Russia), "🇷🇺"},
    };

    int row = 0;
    for (const auto& demo : locales) {
        NLabel* flagLabel = new NLabel(demo.flag, this);
        flagLabel->setFixedWidth(30);

        NLabel* nameLabel = new NLabel(demo.name, this);
        nameLabel->setFixedWidth(120);

        NDatePicker* localePicker = new NDatePicker(this);
        localePicker->setLocale(demo.locale);
        localePicker->setDate(QDate::currentDate());

        NLabel* resultLabel = new NLabel(localePicker->getDate().toString(demo.locale.dateFormat(QLocale::ShortFormat)), this);
        resultLabel->setMinimumWidth(150);
        connect(localePicker, &NDatePicker::dateChanged, [resultLabel, demo](const QDate& date) {
            resultLabel->setText(date.toString(demo.locale.dateFormat(QLocale::ShortFormat)));
        });

        localeLayout->addWidget(flagLabel, row, 0);
        localeLayout->addWidget(nameLabel, row, 1);
        localeLayout->addWidget(localePicker, row, 2);
        localeLayout->addWidget(resultLabel, row, 3);
        row++;
    }

    addSection("国际化支持", localeLayout);

    QHBoxLayout* rangeLayout = new QHBoxLayout();
    rangeLayout->setSpacing(16);
    NDatePicker* rangeDatePicker = new NDatePicker(this);
    rangeDatePicker->setMinimumDate(QDate::currentDate().addDays(-30));
    rangeDatePicker->setMaximumDate(QDate::currentDate().addDays(30));
    rangeDatePicker->setDate(QDate::currentDate());
    NLabel* rangeResultLabel = new NLabel("选择日期: " + rangeDatePicker->getDate().toString("yyyy-MM-dd"), this);
    NLabel* rangeInfoLabel   = new NLabel(QString("可选范围: %1 至 %2")
                                            .arg(rangeDatePicker->getMinimumDate().toString("yyyy-MM-dd"))
                                            .arg(rangeDatePicker->getMaximumDate().toString("yyyy-MM-dd")),
                                        this);
    connect(rangeDatePicker, &NDatePicker::dateChanged, [rangeResultLabel](const QDate& date) {
        rangeResultLabel->setText("选择日期: " + date.toString("yyyy-MM-dd"));
    });
    QVBoxLayout* rangeInfoLayout = new QVBoxLayout();
    rangeInfoLayout->setSpacing(8);
    rangeInfoLayout->addWidget(rangeResultLabel);
    rangeInfoLayout->addWidget(rangeInfoLabel);
    rangeLayout->addWidget(rangeDatePicker);
    rangeLayout->addLayout(rangeInfoLayout);
    rangeLayout->addStretch();
    addSection("日期范围限制", rangeLayout);

    QHBoxLayout* formatLayout = new QHBoxLayout();
    formatLayout->setSpacing(32);

    QVBoxLayout* format1Layout = new QVBoxLayout();
    NLabel*      format1Label  = new NLabel("yyyy-MM-dd", this);
    format1Label->setAlignment(Qt::AlignCenter);
    NDatePicker* format1Picker = new NDatePicker(this);
    format1Picker->setDate(QDate::currentDate());
    NLabel* format1Result = new NLabel(format1Picker->getDate().toString("yyyy-MM-dd"), this);
    format1Result->setAlignment(Qt::AlignCenter);
    connect(format1Picker, &NDatePicker::dateChanged, [format1Result](const QDate& date) {
        format1Result->setText(date.toString("yyyy-MM-dd"));
    });
    format1Layout->addWidget(format1Label);
    format1Layout->addWidget(format1Picker);
    format1Layout->addWidget(format1Result);

    QVBoxLayout* format2Layout = new QVBoxLayout();
    NLabel*      format2Label  = new NLabel("yyyy/MM/dd", this);
    format2Label->setAlignment(Qt::AlignCenter);
    NDatePicker* format2Picker = new NDatePicker(this);
    format2Picker->setDate(QDate::currentDate());
    NLabel* format2Result = new NLabel(format2Picker->getDate().toString("yyyy/MM/dd"), this);
    format2Result->setAlignment(Qt::AlignCenter);
    connect(format2Picker, &NDatePicker::dateChanged, [format2Result](const QDate& date) {
        format2Result->setText(date.toString("yyyy/MM/dd"));
    });
    format2Layout->addWidget(format2Label);
    format2Layout->addWidget(format2Picker);
    format2Layout->addWidget(format2Result);

    QVBoxLayout* format3Layout = new QVBoxLayout();
    NLabel*      format3Label  = new NLabel("dd/MM/yyyy", this);
    format3Label->setAlignment(Qt::AlignCenter);
    NDatePicker* format3Picker = new NDatePicker(this);
    format3Picker->setDate(QDate::currentDate());
    NLabel* format3Result = new NLabel(format3Picker->getDate().toString("dd/MM/yyyy"), this);
    format3Result->setAlignment(Qt::AlignCenter);
    connect(format3Picker, &NDatePicker::dateChanged, [format3Result](const QDate& date) {
        format3Result->setText(date.toString("dd/MM/yyyy"));
    });
    format3Layout->addWidget(format3Label);
    format3Layout->addWidget(format3Picker);
    format3Layout->addWidget(format3Result);

    formatLayout->addLayout(format1Layout);
    formatLayout->addLayout(format2Layout);
    formatLayout->addLayout(format3Layout);
    formatLayout->addStretch();
    addSection("不同日期格式", formatLayout);

    QHBoxLayout* stateLayout = new QHBoxLayout();
    stateLayout->setSpacing(16);
    QVBoxLayout* disabledLayout = new QVBoxLayout();
    NDatePicker* disabledPicker = new NDatePicker(this);
    disabledPicker->setDate(QDate::currentDate());
    disabledPicker->setEnabled(false);
    NLabel* disabledLabel = new NLabel("禁用状态", this);
    disabledLabel->setAlignment(Qt::AlignCenter);
    disabledLayout->addWidget(disabledPicker);
    disabledLayout->addWidget(disabledLabel);
    stateLayout->addLayout(disabledLayout);
    stateLayout->addStretch();
    addSection("不同状态", stateLayout);
}
