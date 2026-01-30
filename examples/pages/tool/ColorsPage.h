#ifndef COLORSPAGE_H
#define COLORSPAGE_H

#include "../BasePage.h"
#include <QtNativeUI/NTheme.h>

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

class ColorsPage : public BasePage {
    Q_OBJECT
  public:
    explicit ColorsPage(QWidget* parent = nullptr);

  private slots:
    void onThemeChanged();

  private:
    QMap<NFluentColorKey::Key, ColorBlock*> m_colorBlocks;
    QList<AccentColorBlock*>                m_accentBlocks;
};

#endif // COLORSPAGE_H
