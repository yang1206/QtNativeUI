#ifndef COLORS_H
#define COLORS_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include <QtNativeUI/NTheme.h>

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

class ColorsExample : public QWidget {
    Q_OBJECT
public:
    explicit ColorsExample(QWidget* parent = nullptr);
    ~ColorsExample();

private slots:
    void onThemeChanged(bool isDark);
    void onColorChanged(const QString& key, const QColor& color);

private:
    void initUI();
    void populateColors();
    
    QScrollArea* m_scrollArea;
    QWidget* m_colorContainer;
    QGridLayout* m_colorsLayout;
    QMap<QString, ColorItem*> m_colorItems;
    NTheme* m_theme;
};

#endif // COLORS_H 