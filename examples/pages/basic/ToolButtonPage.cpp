#include "./ToolButtonPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NToolButton.h>

ToolButtonPage::ToolButtonPage(QWidget* parent)
    : BasePage("ToolButton 工具按钮", "ToolButton 支持多种图标和文本布局方式，适合工具栏使用。", parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(24);

    QHBoxLayout* styleLayout = new QHBoxLayout();
    styleLayout->setSpacing(16);

    NToolButton* iconOnlyBtn = new NToolButton();
    iconOnlyBtn->setFixedSize(40, 40);
    iconOnlyBtn->setFluentIcon(NRegularIconType::Settings24Regular, 24);
    iconOnlyBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    NToolButton* textOnlyBtn = new NToolButton("设置");
    textOnlyBtn->setFixedSize(80, 40);
    textOnlyBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);

    NToolButton* textBesideBtn = new NToolButton("设置");
    textBesideBtn->setFixedSize(120, 40);
    textBesideBtn->setFluentIcon(NRegularIconType::Settings24Regular, 20);
    textBesideBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    NToolButton* textUnderBtn = new NToolButton("设置");
    textUnderBtn->setFixedSize(60, 60);
    textUnderBtn->setFluentIcon(NRegularIconType::Settings24Regular, 24);
    textUnderBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    styleLayout->addWidget(iconOnlyBtn);
    styleLayout->addWidget(textOnlyBtn);
    styleLayout->addWidget(textBesideBtn);
    styleLayout->addWidget(textUnderBtn);
    styleLayout->addStretch();

    mainLayout->addWidget(new QLabel("不同布局样式："));
    mainLayout->addLayout(styleLayout);

    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->setSpacing(16);

    NToolButton* standardBtn = new NToolButton("标准");
    standardBtn->setFixedSize(120, 40);
    standardBtn->setFluentIcon(NRegularIconType::Document24Regular, 20);
    standardBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    NToolButton* accentBtn = new NToolButton("强调");
    accentBtn->setFixedSize(120, 40);
    accentBtn->setFluentIcon(NFilledIconType::Document24Filled, 20);
    accentBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    accentBtn->setButtonType(NToolButton::Accent);

    NToolButton* customAccentBtn = new NToolButton("自定义");
    customAccentBtn->setFixedSize(120, 40);
    customAccentBtn->setFluentIcon(NFilledIconType::Document24Filled, 20);
    customAccentBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    customAccentBtn->setAccentColor(QColor(76, 175, 80));

    NToolButton* disabledBtn = new NToolButton("禁用");
    disabledBtn->setFixedSize(120, 40);
    disabledBtn->setFluentIcon(NRegularIconType::NoteAdd16Regular, 16);
    disabledBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    disabledBtn->setEnabled(false);

    typeLayout->addWidget(standardBtn);
    typeLayout->addWidget(accentBtn);
    typeLayout->addWidget(customAccentBtn);
    typeLayout->addWidget(disabledBtn);
    typeLayout->addStretch();

    mainLayout->addWidget(new QLabel("不同按钮类型："));
    mainLayout->addLayout(typeLayout);

    QHBoxLayout* transparentLayout = new QHBoxLayout();
    transparentLayout->setSpacing(16);

    NToolButton* transparentBtn = new NToolButton();
    transparentBtn->setFixedSize(40, 40);
    transparentBtn->setFluentIcon(NRegularIconType::MoreHorizontal24Regular, 24);
    transparentBtn->setTransparentBackground(true);

    NToolButton* transparentTextBtn = new NToolButton("透明");
    transparentTextBtn->setFixedSize(80, 40);
    transparentTextBtn->setTransparentBackground(true);

    transparentLayout->addWidget(transparentBtn);
    transparentLayout->addWidget(transparentTextBtn);
    transparentLayout->addStretch();

    mainLayout->addWidget(new QLabel("透明背景："));
    mainLayout->addLayout(transparentLayout);

    addSection("ToolButton 完整示例", mainLayout);
}
