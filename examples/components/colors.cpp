#include "colors.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtNativeUI/NColor.h>
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NTheme.h>

// 颜色项实现
ColorItem::ColorItem(const QString& name, const QColor& color, QWidget* parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // 颜色预览
    m_colorPreview = new QLabel(this);
    m_colorPreview->setFixedSize(40, 40);
    m_colorPreview->setAutoFillBackground(true);

    // 名称标签
    m_nameLabel = new QLabel(name, this);
    m_nameLabel->setMinimumWidth(200);

    // 值标签
    m_valueLabel = new QLabel(this);

    layout->addWidget(m_colorPreview);
    layout->addWidget(m_nameLabel);
    layout->addWidget(m_valueLabel);
    layout->addStretch();

    updateColor(color);
}

void ColorItem::updateColor(const QColor& color) {
    // 更新颜色预览
    QPalette pal = m_colorPreview->palette();
    pal.setColor(QPalette::Window, color);
    m_colorPreview->setPalette(pal);

    // 更新值标签
    QString colorText;
    if (color.alpha() < 255) {
        colorText =
            QString("RGBA(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    } else {
        colorText = QString("RGB(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    }
    m_valueLabel->setText(colorText);
}

// 颜色展示组件实现
ColorsExample::ColorsExample(QWidget* parent) : QWidget(parent) {
    m_theme = NTheme::instance();

    initUI();
    populateColors();
    populateAccentColors();

    // 连接主题变化信号
    connect(m_theme, &NTheme::darkModeChanged, this, &ColorsExample::onThemeChanged);
    connect(m_theme, &NTheme::colorChanged, this, &ColorsExample::onColorChanged);
    connect(m_theme, &NTheme::accentColorChanged, this, &ColorsExample::onAccentColorChanged);
}

ColorsExample::~ColorsExample() {}

void ColorsExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 主题控制区域
    QHBoxLayout* controlLayout = new QHBoxLayout();

    QLabel*    themeLabel = new QLabel("主题模式:", this);
    QComboBox* themeCombo = new QComboBox(this);
    themeCombo->addItem("系统", static_cast<int>(NTheme::ThemeMode::System));
    themeCombo->addItem("亮色", static_cast<int>(NTheme::ThemeMode::Light));
    themeCombo->addItem("暗色", static_cast<int>(NTheme::ThemeMode::Dark));
    themeCombo->setCurrentIndex(static_cast<int>(m_theme->themeMode()));

    connect(themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        m_theme->setThemeMode(static_cast<NTheme::ThemeMode>(index));
    });

    QLabel*    accentLabel = new QLabel("强调色:", this);
    QComboBox* accentCombo = new QComboBox(this);
    accentCombo->addItem("蓝色", static_cast<int>(NColors::blue));
    accentCombo->addItem("紫色", static_cast<int>(NColors::purple));
    accentCombo->addItem("粉色", static_cast<int>(NColors::pink));
    accentCombo->addItem("红色", static_cast<int>(NColors::red));
    accentCombo->addItem("橙色", static_cast<int>(NColors::orange));
    accentCombo->addItem("黄色", static_cast<int>(NColors::yellow));
    accentCombo->addItem("绿色", static_cast<int>(NColors::green));
    accentCombo->addItem("青色", static_cast<int>(NColors::teal));
    accentCombo->setCurrentIndex(static_cast<int>(m_theme->accentColor()));

    connect(accentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        m_theme->setAccentColor(static_cast<NAccentColor>(index));
    });

    controlLayout->addWidget(themeLabel);
    controlLayout->addWidget(themeCombo);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(accentLabel);
    controlLayout->addWidget(accentCombo);
    controlLayout->addStretch();

    // 颜色展示区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_colorContainer = new QWidget(m_scrollArea);
    m_colorsLayout   = new QGridLayout(m_colorContainer);
    m_colorsLayout->setContentsMargins(10, 10, 10, 10);
    m_colorsLayout->setSpacing(10);

    m_scrollArea->setWidget(m_colorContainer);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(m_scrollArea);
}

void ColorsExample::populateColors() {
    // 添加标题
    QLabel* titleLabel = new QLabel("Fluent Design 系统颜色", this);
    QFont   titleFont  = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    m_colorsLayout->addWidget(titleLabel, 0, 0, 1, 3);

    // 添加颜色项
    int row = 1;
    int col = 0;

    // 添加文本颜色
    QLabel* textColorsLabel = new QLabel("文本颜色", this);
    QFont   sectionFont     = textColorsLabel->font();
    sectionFont.setPointSize(14);
    sectionFont.setBold(true);
    textColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(textColorsLabel, row++, 0, 1, 3);

    QStringList textColors = {NFluentColorConstants::TextFillColorPrimary,
                              NFluentColorConstants::TextFillColorSecondary,
                              NFluentColorConstants::TextFillColorTertiary,
                              NFluentColorConstants::TextFillColorDisabled,
                              NFluentColorConstants::TextOnAccentFillColorPrimary,
                              NFluentColorConstants::TextOnAccentFillColorSecondary};

    for (const QString& key : textColors) {
        ColorItem* item   = new ColorItem(key, m_theme->getColor(key));
        m_colorItems[key] = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }

    // 添加控件填充颜色
    col = 0;
    if (col != 0) {
        row++;
        col = 0;
    }

    QLabel* controlColorsLabel = new QLabel("控件填充颜色", this);
    controlColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(controlColorsLabel, row++, 0, 1, 3);

    QStringList controlColors = {NFluentColorConstants::ControlFillColorDefault,
                                 NFluentColorConstants::ControlFillColorSecondary,
                                 NFluentColorConstants::ControlFillColorTertiary,
                                 NFluentColorConstants::ControlFillColorDisabled,
                                 NFluentColorConstants::ControlFillColorTransparent,
                                 NFluentColorConstants::ControlFillColorInputActive,
                                 NFluentColorConstants::ControlStrongFillColorDefault,
                                 NFluentColorConstants::ControlStrongFillColorDisabled,
                                 NFluentColorConstants::ControlSolidFillColorDefault};

    for (const QString& key : controlColors) {
        ColorItem* item   = new ColorItem(key, m_theme->getColor(key));
        m_colorItems[key] = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }

    // 添加背景颜色
    if (col != 0) {
        row++;
        col = 0;
    }

    QLabel* bgColorsLabel = new QLabel("背景颜色", this);
    bgColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(bgColorsLabel, row++, 0, 1, 3);

    QStringList bgColors = {NFluentColorConstants::SolidBackgroundFillColorBase,
                            NFluentColorConstants::SolidBackgroundFillColorBaseAlt,
                            NFluentColorConstants::SolidBackgroundFillColorSecondary,
                            NFluentColorConstants::SolidBackgroundFillColorTertiary,
                            NFluentColorConstants::SolidBackgroundFillColorQuarternary};

    for (const QString& key : bgColors) {
        ColorItem* item   = new ColorItem(key, m_theme->getColor(key));
        m_colorItems[key] = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }

    // 添加边框颜色
    if (col != 0) {
        row++;
        col = 0;
    }

    QLabel* borderColorsLabel = new QLabel("边框颜色", this);
    borderColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(borderColorsLabel, row++, 0, 1, 3);

    QStringList borderColors = {NFluentColorConstants::ControlStrokeColorDefault,
                                NFluentColorConstants::ControlStrokeColorSecondary,
                                NFluentColorConstants::ControlStrokeColorOnAccentDefault,
                                NFluentColorConstants::ControlStrokeColorOnAccentSecondary,
                                NFluentColorConstants::ControlStrokeColorOnAccentTertiary,
                                NFluentColorConstants::ControlStrokeColorOnAccentDisabled};

    for (const QString& key : borderColors) {
        ColorItem* item   = new ColorItem(key, m_theme->getColor(key));
        m_colorItems[key] = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
}

void ColorsExample::populateAccentColors() {
    // 添加强调色部分
    int row = m_colorsLayout->rowCount();
    int col = 0;

    QLabel* accentColorsLabel = new QLabel("强调色系统", this);
    QFont   sectionFont       = accentColorsLabel->font();
    sectionFont.setPointSize(16);
    sectionFont.setBold(true);
    accentColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(accentColorsLabel, row++, 0, 1, 3);

    // 获取当前强调色
    NAccentColor currentAccent = m_theme->accentColor();

    // 添加强调色色调
    QStringList accentShades = {"Darkest", "Darker", "Dark", "Normal", "Light", "Lighter", "Lightest"};

    for (int i = 0; i < accentShades.size(); i++) {
        QString name = QString("Accent %1").arg(accentShades[i]);
        QColor  color;

        switch (i) {
            case 0:
                color = NColors::getAccentColor(currentAccent, NColors::Darkest);
                break;
            case 1:
                color = NColors::getAccentColor(currentAccent, NColors::Darker);
                break;
            case 2:
                color = NColors::getAccentColor(currentAccent, NColors::Dark);
                break;
            case 3:
                color = NColors::getAccentColor(currentAccent, NColors::Normal);
                break;
            case 4:
                color = NColors::getAccentColor(currentAccent, NColors::Light);
                break;
            case 5:
                color = NColors::getAccentColor(currentAccent, NColors::Lighter);
                break;
            case 6:
                color = NColors::getAccentColor(currentAccent, NColors::Lightest);
                break;
        }

        ColorItem* item          = new ColorItem(name, color);
        m_accentColorItems[name] = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }

    // 添加所有强调色
    if (col != 0) {
        row++;
        col = 0;
    }

    QLabel* allAccentsLabel = new QLabel("所有强调色", this);
    allAccentsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(allAccentsLabel, row++, 0, 1, 3);

    QStringList accentNames = {"Blue", "Purple", "Pink", "Red", "Orange", "Yellow", "Green", "Teal"};

    for (int i = 0; i < accentNames.size(); i++) {
        QString name  = accentNames[i];
        QColor  color = NColors::getAccentColor(static_cast<NAccentColor>(i), NColors::Normal);

        ColorItem* item          = new ColorItem(name, color);
        m_accentColorItems[name] = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }

    // 添加一个空白区域
    m_colorsLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), row, 0);
}

void ColorsExample::onThemeChanged(bool isDark) {
    // 主题变化时，所有颜色都会通过 colorChanged 信号更新
    // 这里可以添加额外的主题相关逻辑
}

void ColorsExample::onColorChanged(const QString& key, const QColor& color) {
    // 更新对应的颜色项
    if (m_colorItems.contains(key)) {
        m_colorItems[key]->updateColor(color);
    }
}

void ColorsExample::onAccentColorChanged(NAccentColor accentColor) {
    // 更新强调色展示
    QStringList accentShades = {"Accent Darkest",
                                "Accent Darker",
                                "Accent Dark",
                                "Accent Normal",
                                "Accent Light",
                                "Accent Lighter",
                                "Accent Lightest"};

    for (int i = 0; i < accentShades.size(); i++) {
        QString name = accentShades[i];
        QColor  color;

        switch (i) {
            case 0:
                color = NColors::getAccentColor(accentColor, NColors::Darkest);
                break;
            case 1:
                color = NColors::getAccentColor(accentColor, NColors::Darker);
                break;
            case 2:
                color = NColors::getAccentColor(accentColor, NColors::Dark);
                break;
            case 3:
                color = NColors::getAccentColor(accentColor, NColors::Normal);
                break;
            case 4:
                color = NColors::getAccentColor(accentColor, NColors::Light);
                break;
            case 5:
                color = NColors::getAccentColor(accentColor, NColors::Lighter);
                break;
            case 6:
                color = NColors::getAccentColor(accentColor, NColors::Lightest);
                break;
        }

        if (m_accentColorItems.contains(name)) {
            m_accentColorItems[name]->updateColor(color);
        }
    }
}