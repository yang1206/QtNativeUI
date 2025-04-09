#ifndef COLORS_H
#define COLORS_H

#include <QWidget>
#include <QtNativeUI/NTheme.h>

class NScrollArea;
class ColorBlock : public QWidget {
    Q_OBJECT
  public:
    explicit ColorBlock(const QString& name, const QColor& color, QWidget* parent = nullptr);
    void updateColor(const QColor& color);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    QString m_name;
    QColor  m_color;
};

class AccentColorBlock : public QWidget {
    Q_OBJECT
  public:
    explicit AccentColorBlock(const QString& name, const NAccentColor& color, QWidget* parent = nullptr);
    void updateColor(const NAccentColor& color);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    QString      m_name;
    NAccentColor m_color;
};

class ColorsExample : public QWidget {
    Q_OBJECT
  public:
    explicit ColorsExample(QWidget* parent = nullptr);

  private slots:
    void onThemeChanged(bool isDark);
    void onColorChanged(NFluentColorKey::Key key, const QColor& color);

  private:
    void     initUI();
    void     setupFluentColors(QWidget* section);
    void     setupAccentColors(QWidget* section);
    QWidget* createSection(const QString& title);

    NScrollArea*                            m_scrollArea;
    QMap<NFluentColorKey::Key, ColorBlock*> m_colorBlocks;
    QList<AccentColorBlock*>                m_accentBlocks;
};

#endif // COLORS_H