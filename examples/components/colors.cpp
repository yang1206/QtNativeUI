#include "colors.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtNativeUI/NTheme.h>

// 颜色项实现
ColorItem::ColorItem(NFluentColorKey::Key key, const QString& name, const QColor& color, QWidget* parent)
    : QWidget(parent), m_key(key) {
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

// 强调色项实现
AccentColorItem::AccentColorItem(const QString& name, const NAccentColor& accentColor, QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 10, 5, 10);

    // 名称标签
    m_nameLabel    = new QLabel(name, this);
    QFont nameFont = m_nameLabel->font();
    nameFont.setBold(true);
    m_nameLabel->setFont(nameFont);
    mainLayout->addWidget(m_nameLabel);

    // 色调预览
    QGridLayout* swatchLayout = new QGridLayout();
    swatchLayout->setSpacing(8);
    mainLayout->addLayout(swatchLayout);

    // 添加各个色调
    QStringList shades = {"darkest", "darker", "dark", "normal", "light", "lighter", "lightest"};
    int         col    = 0;

    for (const QString& shade : shades) {
        QColor color;
        if (shade == "darkest")
            color = accentColor.darkest();
        else if (shade == "darker")
            color = accentColor.darker();
        else if (shade == "dark")
            color = accentColor.dark();
        else if (shade == "normal")
            color = accentColor.normal();
        else if (shade == "light")
            color = accentColor.light();
        else if (shade == "lighter")
            color = accentColor.lighter();
        else if (shade == "lightest")
            color = accentColor.lightest();

        // 颜色预览
        QLabel* colorPreview = new QLabel(this);
        colorPreview->setFixedSize(30, 30);
        colorPreview->setAutoFillBackground(true);

        QPalette pal = colorPreview->palette();
        pal.setColor(QPalette::Window, color);
        colorPreview->setPalette(pal);

        // 颜色值标签
        QLabel* valueLabel =
            new QLabel(QString("RGB(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()), this);
        valueLabel->setFixedWidth(100);

        swatchLayout->addWidget(colorPreview, 0, col);
        swatchLayout->addWidget(new QLabel(shade, this), 1, col);
        swatchLayout->addWidget(valueLabel, 2, col);

        m_colorPreviews[shade] = colorPreview;
        m_valueLabels[shade]   = valueLabel;

        col++;
    }
}

void AccentColorItem::updateAccentColor(const NAccentColor& accentColor) {
    // 更新各个色调预览
    QStringList shades = {"darkest", "darker", "dark", "normal", "light", "lighter", "lightest"};

    for (const QString& shade : shades) {
        QColor color;
        if (shade == "darkest")
            color = accentColor.darkest();
        else if (shade == "darker")
            color = accentColor.darker();
        else if (shade == "dark")
            color = accentColor.dark();
        else if (shade == "normal")
            color = accentColor.normal();
        else if (shade == "light")
            color = accentColor.light();
        else if (shade == "lighter")
            color = accentColor.lighter();
        else if (shade == "lightest")
            color = accentColor.lightest();

        QPalette pal = m_colorPreviews[shade]->palette();
        pal.setColor(QPalette::Window, color);
        m_colorPreviews[shade]->setPalette(pal);

        m_valueLabels[shade]->setText(QString("RGB(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    }
}

// 颜色展示组件实现
ColorsExample::ColorsExample(QWidget* parent)
    : QWidget(parent), m_scrollArea(nullptr), m_colorContainer(nullptr), m_colorsLayout(nullptr), m_theme(nullptr) {
    m_theme = NTheme::getInstance();

    initUI();
    populateColors();
    populateAccentColors();

    // 连接主题变化信号
    connect(m_theme, &NTheme::darkModeChanged, this, &ColorsExample::onThemeChanged);
    connect(m_theme, &NTheme::themeModeChanged, this, [this]() {
        // 主题模式变化时更新所有颜色
        for (auto it = m_colorItems.begin(); it != m_colorItems.end(); ++it) {
            it.value()->updateColor(m_theme->getColor(it.key()));
        }
    });
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
    themeCombo->addItem("亮色", static_cast<int>(NThemeType::ThemeMode::Light));
    themeCombo->addItem("暗色", static_cast<int>(NThemeType::ThemeMode::Dark));
    themeCombo->addItem("系统", static_cast<int>(NThemeType::ThemeMode::System));

    themeCombo->setCurrentIndex(static_cast<int>(m_theme->themeMode()));

    connect(themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        m_theme->setThemeMode(static_cast<NThemeType::ThemeMode>(index));
    });

    QLabel*    accentLabel = new QLabel("强调色:", this);
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

    connect(accentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, accentCombo](int index) {
        QString colorName = accentCombo->itemData(index).toString();
        if (colorName == "blue")
            m_theme->setAccentColor(NColors::blue);
        else if (colorName == "red")
            m_theme->setAccentColor(NColors::red);
        else if (colorName == "green")
            m_theme->setAccentColor(NColors::green);
        else if (colorName == "yellow")
            m_theme->setAccentColor(NColors::yellow);
        else if (colorName == "orange")
            m_theme->setAccentColor(NColors::orange);
        else if (colorName == "purple")
            m_theme->setAccentColor(NColors::purple);
        else if (colorName == "magenta")
            m_theme->setAccentColor(NColors::magenta);
        else if (colorName == "teal")
            m_theme->setAccentColor(NColors::teal);
    });

    controlLayout->addWidget(themeLabel);
    controlLayout->addWidget(themeCombo);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(accentLabel);
    controlLayout->addWidget(accentCombo);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    // 确保创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_colorContainer = new QWidget(m_scrollArea);
    m_colorsLayout   = new QGridLayout(m_colorContainer);

    m_scrollArea->setWidget(m_colorContainer);
    mainLayout->addWidget(m_scrollArea);
}

void ColorsExample::populateColors() {
    int row = 0;
    int col = 0;

    // 添加标题
    QLabel* titleLabel = new QLabel("Fluent UI 颜色系统", this);
    QFont   titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    m_colorsLayout->addWidget(titleLabel, row++, 0, 1, 3);

    // 添加文本颜色
    QFont sectionFont;
    sectionFont.setPointSize(12);
    sectionFont.setBold(true);

    QLabel* textColorsLabel = new QLabel("文本颜色", this);
    textColorsLabel->setFont(sectionFont);
    m_colorsLayout->addWidget(textColorsLabel, row++, 0, 1, 3);

    // 定义文本颜色键数组
    NFluentColorKey::Key textColors[] = {NFluentColorKey::TextFillColorPrimary,
                                         NFluentColorKey::TextFillColorSecondary,
                                         NFluentColorKey::TextFillColorTertiary,
                                         NFluentColorKey::TextFillColorDisabled,
                                         NFluentColorKey::TextFillColorInverse};

    for (NFluentColorKey::Key key : textColors) {
        // 获取颜色键的字符串表示
        QString keyName = fluentColorKeyToString(key);
        // 创建颜色项
        ColorItem* item   = new ColorItem(key, keyName, m_theme->getColor(key));
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

    // 定义控件颜色键数组
    NFluentColorKey::Key controlColors[] = {NFluentColorKey::ControlFillColorDefault,
                                            NFluentColorKey::ControlFillColorSecondary,
                                            NFluentColorKey::ControlFillColorTertiary,
                                            NFluentColorKey::ControlFillColorDisabled,
                                            NFluentColorKey::ControlFillColorTransparent,
                                            NFluentColorKey::ControlFillColorInputActive,
                                            NFluentColorKey::ControlStrongFillColorDefault,
                                            NFluentColorKey::ControlStrongFillColorDisabled,
                                            NFluentColorKey::ControlSolidFillColorDefault};

    for (NFluentColorKey::Key key : controlColors) {
        QString    keyName = fluentColorKeyToString(key);
        ColorItem* item    = new ColorItem(key, keyName, m_theme->getColor(key));
        m_colorItems[key]  = item;
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

    // 定义背景颜色键数组
    NFluentColorKey::Key bgColors[] = {NFluentColorKey::SolidBackgroundFillColorBase,
                                       NFluentColorKey::SolidBackgroundFillColorBaseAlt,
                                       NFluentColorKey::SolidBackgroundFillColorSecondary,
                                       NFluentColorKey::SolidBackgroundFillColorTertiary,
                                       NFluentColorKey::SolidBackgroundFillColorQuarternary};

    for (NFluentColorKey::Key key : bgColors) {
        QString    keyName = fluentColorKeyToString(key);
        ColorItem* item    = new ColorItem(key, keyName, m_theme->getColor(key));
        m_colorItems[key]  = item;
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

    // 定义边框颜色键数组
    NFluentColorKey::Key borderColors[] = {NFluentColorKey::ControlStrokeColorDefault,
                                           NFluentColorKey::ControlStrokeColorSecondary,
                                           NFluentColorKey::ControlStrokeColorOnAccentDefault,
                                           NFluentColorKey::ControlStrokeColorOnAccentSecondary,
                                           NFluentColorKey::ControlStrokeColorOnAccentTertiary,
                                           NFluentColorKey::ControlStrokeColorOnAccentDisabled};

    for (NFluentColorKey::Key key : borderColors) {
        QString    keyName = fluentColorKeyToString(key);
        ColorItem* item    = new ColorItem(key, keyName, m_theme->getColor(key));
        m_colorItems[key]  = item;
        m_colorsLayout->addWidget(item, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
}

void ColorsExample::populateAccentColors() {
    // 添加当前强调色
    QWidget*     accentSection = new QWidget(m_colorContainer);
    QVBoxLayout* accentLayout  = new QVBoxLayout(accentSection);

    QLabel* sectionTitle = new QLabel(tr("当前强调色"), accentSection);
    QFont   titleFont    = sectionTitle->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    sectionTitle->setFont(titleFont);

    accentLayout->addWidget(sectionTitle);

    // 添加当前强调色预览
    AccentColorItem* currentAccentItem = new AccentColorItem("Current Accent", m_theme->accentColor(), accentSection);
    accentLayout->addWidget(currentAccentItem);
    m_accentColorItems["current"] = currentAccentItem;

    // 添加到主布局
    m_colorsLayout->addWidget(accentSection, m_colorsLayout->rowCount(), 0, 1, 2);
}

void ColorsExample::onThemeChanged(bool isDark) {
    qDebug() << isDark;
    // 主题变化时，所有颜色都会通过单独更新
    // 这里可以添加额外的主题相关逻辑
}

void ColorsExample::onColorChanged(NFluentColorKey::Key key, const QColor& color) {
    // 更新对应的颜色项
    if (m_colorItems.contains(key)) {
        m_colorItems[key]->updateColor(color);
    }
}

void ColorsExample::onAccentColorChanged(const NAccentColor& color) {
    // 更新当前强调色
    if (m_accentColorItems.contains("current")) {
        m_accentColorItems["current"]->updateAccentColor(color);
    }
}