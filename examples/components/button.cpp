//
// Created by admin on 2025/3/10.
//

#include "button.h"

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QStyleHints>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NToggleButton.h>
#include "QtNativeUI/NDropDownButton.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

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
        nTheme->setThemeMode(qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark ? NThemeType::ThemeMode::Light
                                                                                        : NThemeType::ThemeMode::Dark);
    });
    NPushButton* normalBtn   = new NPushButton("Standard");
    NPushButton* disabledBtn = new NPushButton("Disabled");
    disabledBtn->setDisabled(true);

    standardButtonLayout->addWidget(themeBtn);
    standardButtonLayout->addWidget(normalBtn);
    standardButtonLayout->addWidget(disabledBtn);
    standardButtonLayout->addStretch();

    standardLayout->addLayout(standardButtonLayout);

    QLabel* toggleButtonTitle = new QLabel("Toggle Buttons");
    QFont   toggleTitleFont   = toggleButtonTitle->font();
    toggleTitleFont.setPointSize(16);
    toggleTitleFont.setBold(true);
    toggleButtonTitle->setFont(toggleTitleFont);
    standardLayout->addWidget(toggleButtonTitle);

    QHBoxLayout* toggleButtonLayout = new QHBoxLayout;
    toggleButtonLayout->setSpacing(16);

    // 1. 标准图标按钮
    NToggleButton* toggleBtn = new NToggleButton("Toggle");
    toggleBtn->setFixedSize(120, 40);
    toggleBtn->setFluentIcon(NRegularIconType::Checkbox124Regular, 24);
    toggleBtn->setChecked(true);

    // 2. 只有图标的按钮
    NToggleButton* toggleBtn2 = new NToggleButton;
    toggleBtn2->setFluentIcon(NRegularIconType::Checkbox124Regular, 24);

    // 3. 禁用状态
    NToggleButton* toggleBtn3 = new NToggleButton;
    toggleBtn3->setFluentIcon(NRegularIconType::Checkbox124Regular, 24);
    toggleBtn3->setDisabled(true);

    toggleButtonLayout->addWidget(toggleBtn);
    toggleButtonLayout->addWidget(toggleBtn2);
    toggleButtonLayout->addWidget(toggleBtn3);
    toggleButtonLayout->addStretch();

    standardLayout->addLayout(toggleButtonLayout);

    QLabel* iconButtonTitle = new QLabel("Icon Buttons");
    iconButtonTitle->setFont(titleFont);
    standardLayout->addWidget(iconButtonTitle);

    QHBoxLayout* iconButtonLayout = new QHBoxLayout;
    iconButtonLayout->setSpacing(16);

    // 1. 标准图标按钮
    NPushButton* settingsBtn = new NPushButton("Settings");
    settingsBtn->setFixedSize(120, 40);
    settingsBtn->setFluentIcon(NRegularIconType::SettingsChat16Regular, 24);

    // 2. 只有图标的按钮
    NPushButton* searchBtn = new NPushButton;
    searchBtn->setFixedSize(40, 40);
    searchBtn->setFluentIcon(NRegularIconType::Search24Regular, 24);

    // 3. 带颜色的图标按钮
    NPushButton* addBtn = new NPushButton("Add");
    addBtn->setFixedSize(120, 40);
    addBtn->setButtonType(NPushButton::Accent);
    addBtn->setFluentIcon(NFilledIconType::Add20Filled, 24, Qt::white);

    // 4. 自定义大小和颜色的图标按钮
    NPushButton* customBtn = new NPushButton;
    customBtn->setFixedSize(48, 48);
    customBtn->setFluentIcon(NRegularIconType::Headphones24Regular, 30, Qt::red);

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
    QGridLayout* gridLayout = new QGridLayout(flowWidget);
    gridLayout->setSpacing(16);
    gridLayout->setAlignment(Qt::AlignLeft);

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
    int columnCount = 4;
    for (int i = 0; i < accentTypes.size(); ++i) {
        const auto&                  info = accentTypes[i];
        const QString&               name = info.name;
        const NAccentColorType::Type type = info.type;
        NPushButton*                 btn  = new NPushButton(name);
        btn->setFixedWidth(120);
        btn->setButtonType(NPushButton::Accent);
        btn->setAccentColor(NColors::getAccentColor(type));
        gridLayout->addWidget(btn, i / columnCount, i % columnCount, Qt::AlignCenter);
    }

    QHBoxLayout* otherButtons = new QHBoxLayout();
    QWidget*     otherBtns    = new QWidget(flowWidget);
    otherBtns->setLayout(otherButtons);

    NPushButton* accentBtn = new NPushButton("Accent");
    accentBtn->setButtonType(NPushButton::Accent);
    otherButtons->addWidget(accentBtn);
    // 添加一个禁用状态的强调色按钮示例
    NPushButton* disabledAccentBtn = new NPushButton("Disabled Accent");
    disabledAccentBtn->setButtonType(NPushButton::Accent);
    disabledAccentBtn->setAccentColor(NColors::getAccentColor(NAccentColorType::Blue));
    disabledAccentBtn->setDisabled(true);
    otherButtons->addWidget(disabledBtn);
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
    otherButtons->addWidget(customAccentBtn);

    accentLayout->addWidget(flowWidget);
    accentLayout->addWidget(otherBtns);

    mainLayout->addWidget(accentSection);
    // 添加底部间距
    mainLayout->addStretch();

    // 设置最小宽度以确保有足够空间进行换行
    setMinimumWidth(600);
}
