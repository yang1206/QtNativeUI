#include "ColorsPage.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
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

    QRect colorRect = rect().adjusted(4, 4, -4, -4);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    painter.drawRoundedRect(colorRect, 8, 8);

    int    brightness = (m_color.red() * 299 + m_color.green() * 587 + m_color.blue() * 114) / 1000;
    QColor textColor  = brightness > 128 ? Qt::black : Qt::white;

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

    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(nTheme->getColor(NFluentColorKey::TextFillColorPrimary));
    painter.drawText(
        QRect(contentRect.x(), contentRect.y(), contentRect.width(), 20), Qt::AlignLeft | Qt::AlignVCenter, m_name);

    int blockHeight = 30;
    int spacing     = 4;
    int y           = contentRect.y() + 30;
    int blockWidth  = (contentRect.width() - spacing * 6) / 7;

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

ColorsPage::ColorsPage(QWidget* parent)
    : BasePage("Colors 颜色", "展示 QtNativeUI 的颜色系统，包括 Fluent UI 颜色和强调色。", parent) {

    QGridLayout* fluentGrid = new QGridLayout();
    fluentGrid->setSpacing(8);

    int row     = 0;
    int col     = 0;
    int maxCols = 3;

    for (int i = 0; i < NFluentColorKey::Count; ++i) {
        auto    key   = static_cast<NFluentColorKey::Key>(i);
        QString name  = fluentColorKeyToString(key);
        QColor  color = nTheme->getColor(key);

        ColorBlock* block  = new ColorBlock(name, color);
        m_colorBlocks[key] = block;
        fluentGrid->addWidget(block, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    addSection("Fluent UI 颜色", fluentGrid);

    QGridLayout* accentGrid = new QGridLayout();
    accentGrid->setSpacing(16);

    QList<QPair<QString, NAccentColorType::Type>> accentColors = {{"Yellow", NAccentColorType::Yellow},
                                                                  {"Orange", NAccentColorType::Orange},
                                                                  {"Red", NAccentColorType::Red},
                                                                  {"Magenta", NAccentColorType::Magenta},
                                                                  {"Purple", NAccentColorType::Purple},
                                                                  {"Blue", NAccentColorType::Blue},
                                                                  {"Teal", NAccentColorType::Teal},
                                                                  {"Green", NAccentColorType::Green}};

    row     = 0;
    col     = 0;
    maxCols = 2;

    for (const auto& pair : accentColors) {
        AccentColorBlock* block = new AccentColorBlock(pair.first, nTheme->getAccentColor(pair.second));
        m_accentBlocks.append(block);
        accentGrid->addWidget(block, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    addSection("强调色", accentGrid);

    connect(nTheme, &NTheme::darkModeChanged, this, &ColorsPage::onThemeChanged);
    connect(nTheme, &NTheme::themeModeChanged, this, &ColorsPage::onThemeChanged);
}

void ColorsPage::onThemeChanged() {
    for (auto it = m_colorBlocks.begin(); it != m_colorBlocks.end(); ++it) {
        it.value()->updateColor(nTheme->getColor(it.key()));
    }
    for (auto block : m_accentBlocks) {
        block->update();
    }
}