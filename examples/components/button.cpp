//
// Created by admin on 2025/3/10.
//

#include "button.h"

#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QMenu>
#include <QScrollArea>
#include <QStyleHints>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NToggleButton.h>
#include "QtNativeUI/NHyperlinkButton.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NRadioButton.h"
#include "QtNativeUI/NTheme.h"
#include "widgets/ExampleSection.h"

ButtonExample::ButtonExample(QWidget* parent) : QWidget(parent) { initUI(); }

void ButtonExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建内容容器
    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // 添加各个示例区域
    contentLayout->addWidget(new ExampleSection("PushButton", createPushButtons()));
    contentLayout->addWidget(new ExampleSection("ToggleButton", createToggleButtons()));
    contentLayout->addWidget(new ExampleSection("HyperlinkButton", createHyperlinkButtons()));
    contentLayout->addWidget(new ExampleSection("RadioButton", createRadioButtons()));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
    setMinimumWidth(600);
}

QWidget* ButtonExample::createPushButtons() {
    QWidget*     container = new QWidget;
    QHBoxLayout* layout    = new QHBoxLayout(container);
    layout->setSpacing(16);

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

    layout->addWidget(themeBtn);
    layout->addWidget(normalBtn);
    layout->addWidget(disabledBtn);
    layout->addWidget(settingsBtn);
    layout->addWidget(searchBtn);
    layout->addWidget(addBtn);
    layout->addWidget(customBtn);
    layout->addStretch();

    return container;
}

QWidget* ButtonExample::createToggleButtons() {
    QWidget*     container = new QWidget;
    QHBoxLayout* layout    = new QHBoxLayout(container);
    layout->setSpacing(16);
    // 1. 标准图标按钮
    NToggleButton* toggleBtn = new NToggleButton("Toggle");
    toggleBtn->setFixedSize(120, 40);
    toggleBtn->setFluentIcon(NRegularIconType::Checkbox124Regular, 24);
    toggleBtn->setChecked(true);

    // 2. 只有图标的按钮
    NToggleButton* toggleBtn2 = new NToggleButton;
    toggleBtn2->setFluentIcon(NRegularIconType::Checkbox124Regular, 24);
    toggleBtn2->setFixedSize(40, 40);

    // 3. 禁用状态
    NToggleButton* toggleBtn3 = new NToggleButton;
    toggleBtn3->setFluentIcon(NRegularIconType::Checkbox124Regular, 24);
    toggleBtn3->setFixedSize(40, 40);
    toggleBtn3->setDisabled(true);

    layout->addWidget(toggleBtn);
    layout->addWidget(toggleBtn2);
    layout->addWidget(toggleBtn3);
    layout->addStretch();
    return container;
}

QWidget* ButtonExample::createHyperlinkButtons() {
    QWidget*     container = new QWidget;
    QHBoxLayout* layout    = new QHBoxLayout(container);
    layout->setSpacing(16);
    NHyperlinkButton* hyperlinkBtn = new NHyperlinkButton("Visit Website");
    hyperlinkBtn->setUrl("https://www.qt.io/");

    NHyperlinkButton* hyperlinkBtn2 = new NHyperlinkButton("disabled");
    hyperlinkBtn2->setDisabled(true);

    layout->addWidget(hyperlinkBtn);
    layout->addWidget(hyperlinkBtn2);
    layout->addStretch();
    return container;
}

QWidget* ButtonExample::createRadioButtons() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 1. 单选按钮组
    QButtonGroup* radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(true);
    NRadioButton* radioBtn1 = new NRadioButton("Option 1");
    NRadioButton* radioBtn2 = new NRadioButton("Option 2");
    NRadioButton* radioBtn3 = new NRadioButton("Option 3");

    radioGroup->addButton(radioBtn1);
    radioGroup->addButton(radioBtn2);
    radioGroup->addButton(radioBtn3);

    layout->addWidget(radioBtn1);
    layout->addWidget(radioBtn2);
    layout->addWidget(radioBtn3);
    return container;
}