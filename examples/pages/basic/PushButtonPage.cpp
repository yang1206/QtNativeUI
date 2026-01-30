#include "PushButtonPage.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleHints>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTheme.h>

PushButtonPage::PushButtonPage(QWidget* parent)
    : BasePage("PushButton 按钮", "PushButton 是最基础的按钮控件，支持文本、图标和多种样式。", parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(24);

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NPushButton* standardBtn = new NPushButton("Standard");
    standardBtn->setFixedSize(120, 40);

    NPushButton* accentBtn = new NPushButton("Accent");
    accentBtn->setFixedSize(120, 40);
    accentBtn->setButtonType(NPushButton::Accent);

    NPushButton* disabledBtn = new NPushButton("Disabled");
    disabledBtn->setFixedSize(120, 40);
    disabledBtn->setEnabled(false);

    basicLayout->addWidget(standardBtn);
    basicLayout->addWidget(accentBtn);
    basicLayout->addWidget(disabledBtn);
    basicLayout->addStretch();

    mainLayout->addWidget(new QLabel("基本按钮类型："));
    mainLayout->addLayout(basicLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);

    NPushButton* iconTextBtn = new NPushButton("Settings");
    iconTextBtn->setFixedSize(120, 40);
    iconTextBtn->setFluentIcon(NRegularIconType::Settings24Regular, 20);

    NPushButton* iconOnlyBtn = new NPushButton();
    iconOnlyBtn->setFixedSize(40, 40);
    iconOnlyBtn->setFluentIcon(NRegularIconType::Search24Regular, 24);

    NPushButton* accentIconBtn = new NPushButton("Add");
    accentIconBtn->setFixedSize(120, 40);
    accentIconBtn->setButtonType(NPushButton::Accent);
    accentIconBtn->setFluentIcon(NFilledIconType::Add20Filled, 20);

    NPushButton* colorIconBtn = new NPushButton();
    colorIconBtn->setFixedSize(48, 48);
    colorIconBtn->setFluentIcon(NRegularIconType::Heart24Regular, 30, Qt::red);

    iconLayout->addWidget(iconTextBtn);
    iconLayout->addWidget(iconOnlyBtn);
    iconLayout->addWidget(accentIconBtn);
    iconLayout->addWidget(colorIconBtn);
    iconLayout->addStretch();

    mainLayout->addWidget(new QLabel("带图标的按钮："));
    mainLayout->addLayout(iconLayout);

    QHBoxLayout* customAccentLayout = new QHBoxLayout();
    customAccentLayout->setSpacing(16);

    NPushButton* customAccent1 = new NPushButton("Green");
    customAccent1->setFixedSize(120, 40);
    customAccent1->setAccentColor(QColor(76, 175, 80));

    NPushButton* customAccent2 = new NPushButton("Purple");
    customAccent2->setFixedSize(120, 40);
    customAccent2->setAccentColor(QColor(156, 39, 176));

    NPushButton* customAccent3 = new NPushButton("Orange");
    customAccent3->setFixedSize(120, 40);
    customAccent3->setAccentColor(QColor(255, 152, 0));

    customAccentLayout->addWidget(customAccent1);
    customAccentLayout->addWidget(customAccent2);
    customAccentLayout->addWidget(customAccent3);
    customAccentLayout->addStretch();

    mainLayout->addWidget(new QLabel("自定义强调色："));
    mainLayout->addLayout(customAccentLayout);

    QHBoxLayout* transparentLayout = new QHBoxLayout();
    transparentLayout->setSpacing(16);

    NPushButton* transparentBtn = new NPushButton("Transparent");
    transparentBtn->setFixedSize(120, 40);
    transparentBtn->setTransparentBackground(true);

    NPushButton* transparentIconBtn = new NPushButton();
    transparentIconBtn->setFixedSize(40, 40);
    transparentIconBtn->setTransparentBackground(true);
    transparentIconBtn->setFluentIcon(NRegularIconType::MoreHorizontal24Regular, 24);

    transparentLayout->addWidget(transparentBtn);
    transparentLayout->addWidget(transparentIconBtn);
    transparentLayout->addStretch();

    mainLayout->addWidget(new QLabel("透明背景按钮："));
    mainLayout->addLayout(transparentLayout);

    QHBoxLayout* interactiveLayout = new QHBoxLayout();
    interactiveLayout->setSpacing(16);

    NPushButton* themeBtn = new NPushButton("Toggle Theme");
    themeBtn->setFixedSize(140, 40);
    themeBtn->setFluentIcon(NRegularIconType::WeatherMoon24Regular, 20);
    connect(themeBtn, &NPushButton::clicked, this, [this]() {
        nTheme->setThemeMode(qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark ? NThemeType::ThemeMode::Light
                                                                                        : NThemeType::ThemeMode::Dark);
    });

    NPushButton* changeIconBtn = new NPushButton("Change Icon");
    changeIconBtn->setFixedSize(140, 40);
    changeIconBtn->setFluentIcon(NRegularIconType::Play24Regular, 20);
    connect(changeIconBtn, &NPushButton::clicked, this, [changeIconBtn]() {
        static bool playing = false;
        playing             = !playing;
        if (playing) {
            changeIconBtn->setFluentIcon(NRegularIconType::Pause24Regular, 20);
            changeIconBtn->setText("Pause");
        } else {
            changeIconBtn->setFluentIcon(NRegularIconType::Play24Regular, 20);
            changeIconBtn->setText("Play");
        }
    });

    interactiveLayout->addWidget(themeBtn);
    interactiveLayout->addWidget(changeIconBtn);
    interactiveLayout->addStretch();

    mainLayout->addWidget(new QLabel("交互示例："));
    mainLayout->addLayout(interactiveLayout);

    addSection("PushButton 完整示例", mainLayout);
}
