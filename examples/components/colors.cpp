#include "colors.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NTheme.h>
#include <QPushButton>
#include <QComboBox>

// 颜色项实现
ColorItem::ColorItem(const QString& name, const QColor& color, QWidget* parent)
    : QWidget(parent)
{
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

void ColorItem::updateColor(const QColor& color)
{
    // 更新颜色预览
    QPalette pal = m_colorPreview->palette();
    pal.setColor(QPalette::Window, color);
    m_colorPreview->setPalette(pal);
    
    // 更新值标签
    QString colorText;
    if (color.alpha() < 255) {
        colorText = QString("RGBA(%1, %2, %3, %4)")
                    .arg(color.red())
                    .arg(color.green())
                    .arg(color.blue())
                    .arg(color.alpha());
    } else {
        colorText = QString("RGB(%1, %2, %3)")
                    .arg(color.red())
                    .arg(color.green())
                    .arg(color.blue());
    }
    m_valueLabel->setText(colorText);
}

// 强调色项实现
AccentColorItem::AccentColorItem(const QString& name, const NAccentColor& accentColor, QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    
    // 名称标签
    m_nameLabel = new QLabel(name, this);
    QFont nameFont = m_nameLabel->font();
    nameFont.setBold(true);
    m_nameLabel->setFont(nameFont);
    
    mainLayout->addWidget(m_nameLabel);
    
    // 创建色调展示区域
    QHBoxLayout* swatchLayout = new QHBoxLayout();
    swatchLayout->setSpacing(2);
    
    QStringList swatchNames = {"darkest", "darker", "dark", "normal", "light", "lighter", "lightest"};
    
    for (const QString& swatchName : swatchNames) {
        QLabel* swatch = new QLabel(this);
        swatch->setFixedSize(30, 30);
        swatch->setAutoFillBackground(true);
        swatch->setToolTip(swatchName);
        
        // 设置边框
        swatch->setFrameShape(QFrame::Box);
        swatch->setFrameShadow(QFrame::Plain);
        swatch->setLineWidth(1);
        
        m_swatchLabels[swatchName] = swatch;
        swatchLayout->addWidget(swatch);
    }
    
    mainLayout->addLayout(swatchLayout);
    
    // 更新颜色
    updateAccentColor(accentColor);
}

void AccentColorItem::updateAccentColor(const NAccentColor& accentColor)
{
    // 更新所有色调
    QStringList swatchNames = {"darkest", "darker", "dark", "normal", "light", "lighter", "lightest"};
    
    for (const QString& swatchName : swatchNames) {
        QLabel* swatch = m_swatchLabels[swatchName];
        QColor color = accentColor[swatchName];
        
        QPalette pal = swatch->palette();
        pal.setColor(QPalette::Window, color);
        swatch->setPalette(pal);
    }
}

// 颜色展示组件实现
ColorsExample::ColorsExample(QWidget* parent)
    : QWidget(parent)
{
    m_theme = NTheme::instance();
    
    initUI();
    populateColors();
    populateAccentColors();
    
    // 连接主题变化信号
    connect(m_theme, &NTheme::darkModeChanged, this, &ColorsExample::onThemeChanged);
    connect(m_theme, &NTheme::colorChanged, this, &ColorsExample::onColorChanged);
    connect(m_theme, &NTheme::accentColorChanged, this, &ColorsExample::onAccentColorChanged);
}

ColorsExample::~ColorsExample()
{
}

void ColorsExample::initUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // 主题控制区域
    QHBoxLayout* controlLayout = new QHBoxLayout();
    
    QLabel* themeLabel = new QLabel("主题模式:", this);
    QComboBox* themeCombo = new QComboBox(this);
    themeCombo->addItem("系统", static_cast<int>(NTheme::ThemeMode::System));
    themeCombo->addItem("亮色", static_cast<int>(NTheme::ThemeMode::Light));
    themeCombo->addItem("暗色", static_cast<int>(NTheme::ThemeMode::Dark));
    themeCombo->setCurrentIndex(static_cast<int>(m_theme->themeMode()));
    
    connect(themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            [this](int index) {
                m_theme->setThemeMode(static_cast<NTheme::ThemeMode>(index));
            });
    
    QLabel* accentLabel = new QLabel("强调色:", this);
    QComboBox* accentCombo = new QComboBox(this);
    
    // 添加预定义的强调色
    accentCombo->addItem("蓝色", "blue");
    accentCombo->addItem("红色", "red");
    accentCombo->addItem("绿色", "green");
    accentCombo->addItem("黄色", "yellow");
    accentCombo->addItem("橙色", "orange");
    accentCombo->addItem("紫色", "purple");
    accentCombo->addItem("品红", "magenta");
    accentCombo->addItem("青色", "teal");
    
    // 设置当前强调色
    accentCombo->setCurrentIndex(0); // 默认蓝色
    
    connect(accentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            [this, accentCombo](int index) {
                QString colorName = accentCombo->itemData(index).toString();
                if (colorName == "blue") m_theme->setAccentColor(NColors::blue);
                else if (colorName == "red") m_theme->setAccentColor(NColors::red);
                else if (colorName == "green") m_theme->setAccentColor(NColors::green);
                else if (colorName == "yellow") m_theme->setAccentColor(NColors::yellow);
                else if (colorName == "orange") m_theme->setAccentColor(NColors::orange);
                else if (colorName == "purple") m_theme->setAccentColor(NColors::purple);
                else if (colorName == "magenta") m_theme->setAccentColor(NColors::magenta);
                else if (colorName == "teal") m_theme->setAccentColor(NColors::teal);
            });
    
    controlLayout->addWidget(themeLabel);
    controlLayout->addWidget(themeCombo);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(accentLabel);
    controlLayout->addWidget(accentCombo);
    controlLayout->addStretch();
    
    mainLayout->addLayout(controlLayout);
    
    // 创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    
    m_colorContainer = new QWidget(m_scrollArea);
    m_colorsLayout = new QGridLayout(m_colorContainer);
    
    m_scrollArea->setWidget(m_colorContainer);
    mainLayout->addWidget(m_scrollArea);
}

void ColorsExample::populateColors()
{
    int row = 0;
    int col = 0;
    
    // 添加标题
    QLabel* titleLabel = new QLabel("Fluent UI 颜色系统", this);
    QFont titleFont = titleFont.defaultFamily();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    m_colorsLayout->addWidget(titleLabel, row++, 0, 1, 3);
    
    // 添加文本颜色
    QFont sectionFont = sectionFont.defaultFamily();
    sectionFont.setBold(true);
    sectionFont.setPointSize(12);
    
    QLabel* textColorsLabel = new QLabel("文本颜色", this);
    textColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(textColorsLabel, row++, 0, 1, 3);
    
    QStringList textColors = {
        NFluentColorConstants::TextFillColorPrimary,
        NFluentColorConstants::TextFillColorSecondary,
        NFluentColorConstants::TextFillColorTertiary,
        NFluentColorConstants::TextFillColorDisabled,
        NFluentColorConstants::TextFillColorInverse
    };
    
    for (const QString& key : textColors) {
        ColorItem* item = new ColorItem(key, m_theme->getColor(key));
        m_colorItems[key] = item;
        m_colorsLayout->addWidget(item, row, col);
        
        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
    
    // 添加控件颜色
    if (col != 0) {
        row++;
        col = 0;
    }
    
    QLabel* controlColorsLabel = new QLabel("控件颜色", this);
    controlColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(controlColorsLabel, row++, 0, 1, 3);
    
    QStringList controlColors = {
        NFluentColorConstants::ControlFillColorDefault,
        NFluentColorConstants::ControlFillColorSecondary,
        NFluentColorConstants::ControlFillColorTertiary,
        NFluentColorConstants::ControlFillColorDisabled,
        NFluentColorConstants::ControlFillColorTransparent,
        NFluentColorConstants::ControlFillColorInputActive,
        NFluentColorConstants::ControlStrongFillColorDefault,
        NFluentColorConstants::ControlStrongFillColorDisabled,
        NFluentColorConstants::ControlSolidFillColorDefault
    };
    
    for (const QString& key : controlColors) {
        ColorItem* item = new ColorItem(key, m_theme->getColor(key));
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
    
    QStringList bgColors = {
        NFluentColorConstants::SolidBackgroundFillColorBase,
        NFluentColorConstants::SolidBackgroundFillColorBaseAlt,
        NFluentColorConstants::SolidBackgroundFillColorSecondary,
        NFluentColorConstants::SolidBackgroundFillColorTertiary,
        NFluentColorConstants::SolidBackgroundFillColorQuarternary
    };
    
    for (const QString& key : bgColors) {
        ColorItem* item = new ColorItem(key, m_theme->getColor(key));
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
    
    QStringList borderColors = {
        NFluentColorConstants::ControlStrokeColorDefault,
        NFluentColorConstants::ControlStrokeColorSecondary,
        NFluentColorConstants::ControlStrokeColorOnAccentDefault,
        NFluentColorConstants::ControlStrokeColorOnAccentSecondary,
        NFluentColorConstants::ControlStrokeColorOnAccentTertiary,
        NFluentColorConstants::ControlStrokeColorOnAccentDisabled
    };
    
    for (const QString& key : borderColors) {
        ColorItem* item = new ColorItem(key, m_theme->getColor(key));
        m_colorItems[key] = item;
        m_colorsLayout->addWidget(item, row, col);
        
        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
}

void ColorsExample::populateAccentColors()
{
    int row = m_colorsLayout->rowCount();
    int col = 0;
    
    // 添加强调色标题
    QFont sectionFont = sectionFont.defaultFamily();
    sectionFont.setBold(true);
    sectionFont.setPointSize(12);
    
    QLabel* accentColorsLabel = new QLabel("强调色系统", this);
    accentColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(accentColorsLabel, row++, 0, 1, 3);
    
    // 添加当前强调色
    AccentColorItem* currentAccentItem = new AccentColorItem("当前强调色", m_theme->accentColor());
    m_accentColorItems["current"] = currentAccentItem;
    m_colorsLayout->addWidget(currentAccentItem, row++, 0, 1, 3);
    
    // 添加预定义的强调色
    struct NamedAccentColor {
        QString name;
        NAccentColor color;
    };
    
    QList<NamedAccentColor> namedColors = {
        {"蓝色 (Blue)", NColors::blue},
        {"红色 (Red)", NColors::red},
        {"绿色 (Green)", NColors::green},
        {"黄色 (Yellow)", NColors::yellow},
        {"橙色 (Orange)", NColors::orange},
        {"紫色 (Purple)", NColors::purple},
        {"品红 (Magenta)", NColors::magenta},
        {"青色 (Teal)", NColors::teal}
    };
    
    for (const auto& namedColor : namedColors) {
        AccentColorItem* item = new AccentColorItem(namedColor.name, namedColor.color);
        m_accentColorItems[namedColor.name] = item;
        m_colorsLayout->addWidget(item, row++, 0, 1, 3);
    }
    
    // 添加一个空白区域
    m_colorsLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), row, 0);
}

void ColorsExample::onThemeChanged(bool isDark)
{
    // 主题变化时，所有颜色都会通过 colorChanged 信号更新
    // 这里可以添加额外的主题相关逻辑
}

void ColorsExample::onColorChanged(const QString& key, const QColor& color)
{
    // 更新对应的颜色项
    if (m_colorItems.contains(key)) {
        m_colorItems[key]->updateColor(color);
    }
}

void ColorsExample::onAccentColorChanged(const NAccentColor& color)
{
    // 更新当前强调色
    if (m_accentColorItems.contains("current")) {
        m_accentColorItems["current"]->updateAccentColor(color);
    }
} 