//
// Created by admin on 2025/3/10.
//

#include "button.h"

#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QStyleHints>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NToggleButton.h>
#include "QtNativeUI/NHyperlinkButton.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NMainWindow.h"
#include "QtNativeUI/NRadioButton.h"
#include "QtNativeUI/NScrollArea.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolButton.h"
#include "widgets/ExampleSection.h"

ButtonExample::ButtonExample(QWidget* parent) : QWidget(parent) { initUI(); }

void ButtonExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建滚动区域
    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    // 创建内容容器
    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // 添加各个示例区域
    contentLayout->addWidget(new ExampleSection("PushButton", createPushButtons()));
    contentLayout->addWidget(new ExampleSection("ToggleButton", createToggleButtons()));
    contentLayout->addWidget(new ExampleSection("ToolButton", createToolButtons()));
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

QWidget* ButtonExample::createToolButtons() {
    QWidget*     container  = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setSpacing(20);

    // 图标位置示例
    QWidget*     iconPositionContainer = new QWidget;
    QHBoxLayout* iconPositionLayout    = new QHBoxLayout(iconPositionContainer);
    iconPositionLayout->setSpacing(16);

    // 只有图标
    NToolButton* iconOnlyBtn = new NToolButton();
    iconOnlyBtn->setToolTip("只有图标");
    iconOnlyBtn->setFixedSize(40, 40);
    iconOnlyBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    iconOnlyBtn->setFluentIcon(NRegularIconType::Settings24Regular, 24);
    iconOnlyBtn->setTransparentBackground(true);

    // 只有文本
    NToolButton* textOnlyBtn = new NToolButton();
    textOnlyBtn->setToolTip("只有文本");
    textOnlyBtn->setFixedSize(80, 40);
    textOnlyBtn->setText("设置");
    textOnlyBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);

    // 文本在图标旁边
    NToolButton* textBesideBtn = new NToolButton();
    textBesideBtn->setToolTip("文本在图标旁边");
    textBesideBtn->setFixedSize(120, 40);
    textBesideBtn->setText("设置");
    textBesideBtn->setFluentIcon(NRegularIconType::Settings24Regular, 20);
    textBesideBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 文本在图标下方
    NToolButton* textUnderBtn = new NToolButton();
    textUnderBtn->setToolTip("文本在图标下方");
    textUnderBtn->setFixedSize(60, 60);
    textUnderBtn->setText("设置");
    textUnderBtn->setFluentIcon(NRegularIconType::Settings24Regular, 24);
    textUnderBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 禁用状态
    NToolButton* disabledBtn = new NToolButton();
    disabledBtn->setToolTip("禁用状态");

    disabledBtn->setFixedSize(80, 40);
    disabledBtn->setText("禁用");
    disabledBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    disabledBtn->setDisabled(true);

    iconPositionLayout->addWidget(iconOnlyBtn);
    iconPositionLayout->addWidget(textOnlyBtn);
    iconPositionLayout->addWidget(textBesideBtn);
    iconPositionLayout->addWidget(textUnderBtn);
    iconPositionLayout->addWidget(disabledBtn);
    iconPositionLayout->addStretch();

    // 按钮类型示例
    QWidget*     buttonTypeContainer = new QWidget;
    QHBoxLayout* buttonTypeLayout    = new QHBoxLayout(buttonTypeContainer);
    buttonTypeLayout->setSpacing(16);

    // 标准按钮
    NToolButton* standardBtn = new NToolButton();
    standardBtn->setToolTip("标准按钮");
    standardBtn->setFixedSize(120, 40);
    standardBtn->setText("标准");
    standardBtn->setFluentIcon(NRegularIconType::Document24Regular, 20);
    standardBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 强调色按钮
    NToolButton* accentBtn = new NToolButton();
    accentBtn->setToolTip("强调色按钮");
    accentBtn->setFixedSize(120, 40);
    accentBtn->setText("强调");
    accentBtn->setFluentIcon(NFilledIconType::Document24Filled, 20);
    accentBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    accentBtn->setButtonType(NToolButton::Accent);

    // 自定义强调色按钮
    NToolButton* customAccentBtn = new NToolButton();
    customAccentBtn->setToolTip("自定义强调色");
    customAccentBtn->setFixedSize(120, 40);
    customAccentBtn->setText("自定义");
    customAccentBtn->setFluentIcon(NFilledIconType::Document24Filled, 20);
    customAccentBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    customAccentBtn->setButtonType(NToolButton::Accent);
    customAccentBtn->setAccentColor(QColor(76, 175, 80)); // 绿色

    buttonTypeLayout->addWidget(standardBtn);
    buttonTypeLayout->addWidget(accentBtn);
    buttonTypeLayout->addWidget(customAccentBtn);
    buttonTypeLayout->addStretch();

    // 状态示例
    QWidget*     stateContainer = new QWidget;
    QHBoxLayout* stateLayout    = new QHBoxLayout(stateContainer);
    stateLayout->setSpacing(16);

    // 添加各个部分到主布局
    mainLayout->addWidget(new QLabel("不同图标位置样式："));
    mainLayout->addWidget(iconPositionContainer);
    mainLayout->addWidget(new QLabel("不同按钮类型："));
    mainLayout->addWidget(buttonTypeContainer);

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

#ifdef Q_OS_WIN
    // 2. 窗口效果切换单选按钮组
    QLabel* windowEffectLabel = new QLabel("切换窗口效果：");
    windowEffectLabel->setStyleSheet("font-weight: bold; margin-top: 16px;");
    layout->addWidget(windowEffectLabel);

    QButtonGroup* effectGroup = new QButtonGroup(this);
    effectGroup->setExclusive(true);

    NRadioButton* noneBtn    = new NRadioButton("无效果");
    NRadioButton* micaBtn    = new NRadioButton("Mica (Windows 11)");
    NRadioButton* micaAltBtn = new NRadioButton("MicaAlt (Windows 11 22H2+)");
    NRadioButton* acrylicBtn = new NRadioButton("亚克力效果");
    NRadioButton* dwmblurBtn = new NRadioButton("DWM模糊效果 (Windows 8)");

    effectGroup->addButton(noneBtn, NMainWindow::None);
    effectGroup->addButton(micaBtn, NMainWindow::Mica);
    effectGroup->addButton(micaAltBtn, NMainWindow::MicaAlt);
    effectGroup->addButton(acrylicBtn, NMainWindow::Acrylic);
    effectGroup->addButton(dwmblurBtn, NMainWindow::DWMBlur);

    // 连接信号槽，当选择改变时切换窗口效果
    connect(
        effectGroup,
        QOverload<int>::of(&QButtonGroup::idClicked),
        this,
        [this](int id) {
            if (m_mainWindow) {
                m_mainWindow->setBackdropEffect(static_cast<NMainWindow::BackdropType>(id));
            }
        },
        Qt::QueuedConnection);

    // 根据当前窗口效果设置选中状态
    if (m_mainWindow) {
        int              currentEffect = m_mainWindow->backdropEffect();
        QAbstractButton* button        = effectGroup->button(currentEffect);
        if (button) {
            button->setChecked(true);
        }
    }

    layout->addWidget(noneBtn);
    layout->addWidget(micaBtn);
    layout->addWidget(micaAltBtn);
    layout->addWidget(acrylicBtn);
    layout->addWidget(dwmblurBtn);
#endif
    return container;
}