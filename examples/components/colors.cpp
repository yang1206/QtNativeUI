#include "colors.h"

#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QtNativeUI/NTheme.h>

ColorBlock::ColorBlock(const QString& name, const QColor& color, QWidget* parent)
    : QWidget(parent), m_name(name), m_color(color) {
    setMinimumSize(180, 60);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void ColorBlock::updateColor(const QColor& color) {
    m_color = color;
    update();
}

void ColorBlock::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制圆角矩形
    QRect colorRect = rect().adjusted(4, 4, -4, -4);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    painter.drawRoundedRect(colorRect, 8, 8);

    // 根据背景色亮度选择文字颜色
    int    brightness = (m_color.red() * 299 + m_color.green() * 587 + m_color.blue() * 114) / 1000;
    QColor textColor  = brightness > 128 ? Qt::black : Qt::white;

    // 绘制文字
    painter.setPen(textColor);
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    painter.drawText(colorRect, Qt::AlignCenter, m_name);
}

AccentColorBlock::AccentColorBlock(const QString& name, const NAccentColor& color, QWidget* parent)
    : QWidget(parent), m_name(name), m_color(color) {
    setMinimumSize(180, 120);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void AccentColorBlock::updateColor(const NAccentColor& color) {
    m_color = color;
    update();
}

void AccentColorBlock::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect contentRect = rect().adjusted(4, 4, -4, -4);

    // 绘制标题
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(nTheme->getColor(NFluentColorKey::TextFillColorPrimary));
    painter.drawText(
        QRect(contentRect.x(), contentRect.y(), contentRect.width(), 20), Qt::AlignLeft | Qt::AlignVCenter, m_name);

    // 计算色块大小和间距
    int blockHeight = 30;
    int spacing     = 4;
    int y           = contentRect.y() + 30;
    int blockWidth  = (contentRect.width() - spacing * 6) / 7;

    // 绘制色调块
    QList<QPair<QString, QColor>> shades = {{"Darkest", m_color.darkest()},
                                            {"Darker", m_color.darker()},
                                            {"Dark", m_color.dark()},
                                            {"Normal", m_color.normal()},
                                            {"Light", m_color.light()},
                                            {"Lighter", m_color.lighter()},
                                            {"Lightest", m_color.lightest()}};

    int x = contentRect.x();
    for (const auto& shade : shades) {
        QRect blockRect(x, y, blockWidth, blockHeight);
        painter.setPen(Qt::NoPen);
        painter.setBrush(shade.second);
        painter.drawRoundedRect(blockRect, 4, 4);
        x += blockWidth + spacing;
    }
}

ColorsExample::ColorsExample(QWidget* parent) : QWidget(parent) {
    initUI();

    // 连接主题变化信号
    connect(nTheme, &NTheme::darkModeChanged, this, &ColorsExample::onThemeChanged);
    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        for (auto it = m_colorBlocks.begin(); it != m_colorBlocks.end(); ++it) {
            it.value()->updateColor(nTheme->getColor(it.key()));
        }
    });
}

void ColorsExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget* contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(32);

    // 添加 Fluent 颜色部分
    QWidget* fluentSection = createSection("Fluent UI Colors");
    fluentSection->setObjectName("FluentUIColors"); // 设置对象名
    setupFluentColors(fluentSection); // 直接传递部件
    contentLayout->addWidget(fluentSection);

    // 添加强调色部分
    QWidget* accentSection = createSection("Accent Colors");
    accentSection->setObjectName("AccentColors"); // 设置对象名
    setupAccentColors(accentSection); // 直接传递部件
    contentLayout->addWidget(accentSection);

    contentLayout->addStretch();
    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
}

// 修改函数签名，直接接收部件参数
// 修改 setupFluentColors 方法
void ColorsExample::setupFluentColors(QWidget* section) {
    // 删除现有布局中的所有项目
    QLayout* oldLayout = section->layout();
    QVBoxLayout* sectionLayout = qobject_cast<QVBoxLayout*>(oldLayout);
    
    // 创建网格布局
    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(8);
    grid->setContentsMargins(0, 16, 0, 0);

    int row = 0;
    int col = 0;
    int maxCols = 3;

    // 创建所有 Fluent 颜色块
    for (int i = 0; i < NFluentColorKey::Count; ++i) {
        auto key = static_cast<NFluentColorKey::Key>(i);
        QString name = fluentColorKeyToString(key);
        QColor color = nTheme->getColor(key);

        ColorBlock* block = new ColorBlock(name, color, section);
        m_colorBlocks[key] = block;
        grid->addWidget(block, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    // 将网格布局添加到部分布局中
    sectionLayout->addLayout(grid);
}

// 修改 setupAccentColors 方法
void ColorsExample::setupAccentColors(QWidget* section) {
    // 删除现有布局中的所有项目
    QLayout* oldLayout = section->layout();
    QVBoxLayout* sectionLayout = qobject_cast<QVBoxLayout*>(oldLayout);
    
    // 创建网格布局
    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(16);
    grid->setContentsMargins(0, 16, 0, 0);

    QList<QPair<QString, NAccentColorType::Type>> accentColors = {
        {"Yellow", NAccentColorType::Yellow},
        {"Orange", NAccentColorType::Orange},
        {"Red", NAccentColorType::Red},
        {"Magenta", NAccentColorType::Magenta},
        {"Purple", NAccentColorType::Purple},
        {"Blue", NAccentColorType::Blue},
        {"Teal", NAccentColorType::Teal},
        {"Green", NAccentColorType::Green}
    };

    int row = 0;
    int col = 0;
    int maxCols = 2;

    for (const auto& pair : accentColors) {
        AccentColorBlock* block = new AccentColorBlock(
            pair.first, 
            nTheme->getAccentColor(pair.second),
            section
        );
        m_accentBlocks.append(block);
        grid->addWidget(block, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    // 将网格布局添加到部分布局中
    sectionLayout->addLayout(grid);
}

QWidget* ColorsExample::createSection(const QString& title) {
    QWidget*     section = new QWidget;
    QVBoxLayout* layout  = new QVBoxLayout(section);
    layout->setSpacing(16);

    QLabel* titleLabel = new QLabel(title, section);
    QFont   titleFont  = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    return section;
}

void ColorsExample::onThemeChanged(bool isDark) {
    for (auto it = m_colorBlocks.begin(); it != m_colorBlocks.end(); ++it) {
        it.value()->updateColor(nTheme->getColor(it.key()));
    }
}

void ColorsExample::onColorChanged(NFluentColorKey::Key key, const QColor& color) {
    if (m_colorBlocks.contains(key)) {
        m_colorBlocks[key]->updateColor(color);
    }
}