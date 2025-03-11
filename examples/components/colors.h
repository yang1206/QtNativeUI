#ifndef COLORS_H
#define COLORS_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NColor.h>

class ColorItem : public QWidget {
    Q_OBJECT
public:
    explicit ColorItem(const QString& name, const QColor& color, QWidget* parent = nullptr);
    void updateColor(const QColor& color);

private:
    QLabel* m_colorPreview;
    QLabel* m_nameLabel;
    QLabel* m_valueLabel;
};

class AccentColorItem : public QWidget {
    Q_OBJECT
public:
    explicit AccentColorItem(const QString& name, const NAccentColor& accentColor, QWidget* parent = nullptr);
    void updateAccentColor(const NAccentColor& accentColor);

private:
    QLabel* m_nameLabel;
    QMap<QString, QLabel*> m_colorPreviews;
    QMap<QString, QLabel*> m_valueLabels;
};

class ColorsExample : public QWidget {
    Q_OBJECT
public:
    explicit ColorsExample(QWidget* parent = nullptr);
    ~ColorsExample() override;

private slots:
    void onThemeChanged(bool isDark);
    void onColorChanged(const QString& key, const QColor& color);
    void onAccentColorChanged(const NAccentColor& color);

private:
    void initUI();
    void populateColors();
    void populateAccentColors();
    
    QScrollArea* m_scrollArea;
    QWidget* m_colorContainer;
    QGridLayout* m_colorsLayout;
    QMap<QString, ColorItem*> m_colorItems;
    QMap<QString, AccentColorItem*> m_accentColorItems;
    NTheme* m_theme;
};

#endif // COLORS_H 