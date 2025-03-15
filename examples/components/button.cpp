//
// Created by admin on 2025/3/10.
//

#include "button.h"

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QStyleHints>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>

#include "QtNativeUI/NIcon.h"

ButtonExample::ButtonExample(QWidget* parent) : QWidget(parent) { initUI(); }

void ButtonExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(24);

    // 标准按钮展示区
    QWidget*     standardSection = new QWidget;
    QVBoxLayout* standardLayout  = new QVBoxLayout(standardSection);
    standardLayout->setSpacing(16);

    QLabel* standardTitle = new QLabel("Standard Buttons");
    QFont   titleFont     = standardTitle->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    standardTitle->setFont(titleFont);
    standardLayout->addWidget(standardTitle);

    QHBoxLayout* standardButtonLayout = new QHBoxLayout;
    standardButtonLayout->setSpacing(16);

    // 添加标准按钮
    NPushButton* themeBtn = new NPushButton("Change Theme");
    themeBtn->setFixedSize(120, 40);
    connect(themeBtn, &NPushButton::clicked, this, [this]() {
        qApp->styleHints()->setColorScheme(qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark
                                               ? Qt::ColorScheme::Light
                                               : Qt::ColorScheme::Dark);
    });
    NPushButton* normalBtn   = new NPushButton("Standard");
    NPushButton* disabledBtn = new NPushButton("Disabled");
    disabledBtn->setDisabled(true);

    standardButtonLayout->addWidget(themeBtn);
    standardButtonLayout->addWidget(normalBtn);
    standardButtonLayout->addWidget(disabledBtn);
    standardButtonLayout->addStretch();

    standardLayout->addLayout(standardButtonLayout);

    QLabel* iconButtonTitle = new QLabel("Icon Buttons");
    iconButtonTitle->setFont(titleFont);
    standardLayout->addWidget(iconButtonTitle);

    QHBoxLayout* iconButtonLayout = new QHBoxLayout;
    iconButtonLayout->setSpacing(16);

    // 1. 标准图标按钮
    NPushButton* settingsBtn = new NPushButton("Settings");
    settingsBtn->setFixedSize(120, 40);
    settingsBtn->setIcon(nIcon->fromRegular(NRegularIconType::Settings24Regular, 30));

    // 2. 只有图标的按钮
    NPushButton* searchBtn = new NPushButton;
    searchBtn->setFixedSize(40, 40);
    searchBtn->setIcon(nIcon->fromFilled(NFilledIconType::Search20Filled, 30));

    // 3. 带颜色的图标按钮
    NPushButton* addBtn = new NPushButton("Add");
    addBtn->setFixedSize(120, 40);
    addBtn->setButtonType(NPushButton::Accent);
    addBtn->setIcon(nIcon->fromFilled(NFilledIconType::Add20Filled, 30, Qt::white));

    // 4. 自定义大小和颜色的图标按钮
    NPushButton* customBtn = new NPushButton;
    customBtn->setFixedSize(48, 48);
    customBtn->setIcon(nIcon->fromRegular(NRegularIconType::Headphones24Regular, 48, Qt::red));

    iconButtonLayout->addWidget(settingsBtn);
    iconButtonLayout->addWidget(searchBtn);
    iconButtonLayout->addWidget(addBtn);
    iconButtonLayout->addWidget(customBtn);
    iconButtonLayout->addStretch();

    standardLayout->addLayout(iconButtonLayout);

    mainLayout->addWidget(standardSection);

    // 强调色按钮展示区
    QWidget*     accentSection = new QWidget;
    QVBoxLayout* accentLayout  = new QVBoxLayout(accentSection);
    accentLayout->setSpacing(16);

    QLabel* accentTitle = new QLabel("Accent Buttons");
    accentTitle->setFont(titleFont);
    accentLayout->addWidget(accentTitle);

    // 使用流式布局展示强调色按钮
    QWidget*     flowWidget = new QWidget;
    QGridLayout* flowLayout = new QGridLayout(flowWidget);
    flowLayout->setSpacing(16);
    flowLayout->setAlignment(Qt::AlignLeft);

    // 预定义的强调色类型
    struct AccentButtonInfo {
        QString                name;
        NAccentColorType::Type type;
    };

    QList<AccentButtonInfo> accentTypes = {{"Yellow", NAccentColorType::Yellow},
                                           {"Orange", NAccentColorType::Orange},
                                           {"Red", NAccentColorType::Red},
                                           {"Magenta", NAccentColorType::Magenta},
                                           {"Purple", NAccentColorType::Purple},
                                           {"Blue", NAccentColorType::Blue},
                                           {"Teal", NAccentColorType::Teal},
                                           {"Green", NAccentColorType::Green}};

    // 添加所有预定义的强调色按钮
    for (const auto& info : accentTypes) {
        NPushButton* btn = new NPushButton(info.name);
        btn->setFixedWidth(120);
        btn->setButtonType(NPushButton::Accent);
        btn->setAccentColor(NColors::getAccentColor(info.type));
        flowLayout->addWidget(btn);
    }

    NPushButton* accentBtn = new NPushButton("Accent");
    accentBtn->setButtonType(NPushButton::Accent);
    flowLayout->addWidget(accentBtn);

    // 添加一个禁用状态的强调色按钮示例
    NPushButton* disabledAccentBtn = new NPushButton("Disabled Accent");
    disabledAccentBtn->setButtonType(NPushButton::Accent);
    disabledAccentBtn->setAccentColor(NColors::getAccentColor(NAccentColorType::Blue));
    disabledAccentBtn->setDisabled(true);
    flowLayout->addWidget(disabledAccentBtn);

    // 添加自定义强调色按钮
    NPushButton* customAccentBtn = new NPushButton("Custom Accent");
    customAccentBtn->setButtonType(NPushButton::Accent);

    // 创建自定义强调色
    QColor       customColor(82, 196, 26); // 一个自定义的绿色
    NAccentColor customAccent = NAccentColor::fromColor(customColor,
                                                        0.38, // darkest
                                                        0.30, // darker
                                                        0.15, // dark
                                                        0.15, // light
                                                        0.30, // lighter
                                                        0.38  // lightest
    );

    customAccentBtn->setAccentColor(customAccent);
    flowLayout->addWidget(customAccentBtn);
    accentLayout->addWidget(flowWidget);
    mainLayout->addWidget(accentSection);

    // 添加底部间距
    mainLayout->addStretch();

    // 设置最小宽度以确保有足够空间进行换行
    setMinimumWidth(600);
}
