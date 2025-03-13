#ifndef QTNATIVEUI_NPUSHBUTTON_H
#define QTNATIVEUI_NPUSHBUTTON_H

#include <QPropertyAnimation>
#include <QPushButton>

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
    explicit NPushButton(QWidget* parent = nullptr);
    explicit NPushButton(QString text, QWidget* parent = nullptr);
    ~NPushButton();

    void   setLightTextColor(QColor color);
    QColor getLightTextColor() const;

    void   setDarkTextColor(QColor color);
    QColor getDarkTextColor() const;

    void setShowBorder(bool show);
    bool showBorder() const;

  protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    void         paintEvent(QPaintEvent* event) override;

    // 模块化绘制函数
    void drawBackground(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawText(QPainter* painter);
};

#endif // QTNATIVEUI_NPUSHBUTTON_H