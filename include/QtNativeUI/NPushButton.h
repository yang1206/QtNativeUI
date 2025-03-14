#ifndef QTNATIVEUI_NPUSHBUTTON_H
#define QTNATIVEUI_NPUSHBUTTON_H

#include <QPropertyAnimation>
#include <QPushButton>

#include "NColor.h"
#include "stdafx.h"

class NPushButtonPrivate;
class QTNATIVEUI_EXPORT NPushButton : public QPushButton {
    Q_OBJECT
    Q_Q_CREATE(NPushButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)

  public:
    enum ButtonType {
        Standard, // 标准按钮
        Accent    // 强调色按钮
    };
    Q_ENUM(ButtonType)

    explicit NPushButton(QWidget* parent = nullptr);
    explicit NPushButton(QString text, QWidget* parent = nullptr);
    ~NPushButton();

    void       setButtonType(ButtonType type);
    ButtonType buttonType() const;

    void   setLightTextColor(QColor color);
    QColor getLightTextColor() const;

    void   setDarkTextColor(QColor color);
    QColor getDarkTextColor() const;

    void setAccentColor(const NAccentColor& color);
    void setAccentColor(const QColor& color);

  protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

    void drawBackground(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void updateAccentColors();
    void resetAccentColor();
};

#endif // QTNATIVEUI_NPUSHBUTTON_H