#ifndef QTNATIVEUI_NTOGGLEBUTTON_H
#define QTNATIVEUI_NTOGGLEBUTTON_H

#include <QWidget>
#include "NIconEnums.h"
#include "stdafx.h"

class NToggleButtonPrivate;
class QTNATIVEUI_EXPORT NToggleButton : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NToggleButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QString, Text)

  public:
    explicit NToggleButton(QWidget* parent = nullptr);
    explicit NToggleButton(const QString& text, QWidget* parent = nullptr);
    ~NToggleButton();

    // 选中状态
    void setChecked(bool checked);
    bool isChecked() const;
    void toggle();

    // 图标设置
    void  setFluentIcon(NRegularIconType::Icon icon, int size = 16, const QColor& color = QColor());
    void  setFluentIcon(NFilledIconType::Icon icon, int size = 16, const QColor& color = QColor());
    QIcon icon() const;
    void  setIconSize(const QSize& size);
    QSize iconSize() const;

  signals:
    void clicked(bool checked = false);
    void toggled(bool checked);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool event(QEvent* event) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void drawBackground(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawIcon(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void init();
    void updateAccentColors();
    void updateFluentIcon();
};

#endif // QTNATIVEUI_NTOGGLEBUTTON_H