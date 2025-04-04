//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NRADIOBUTTON_H
#define NRADIOBUTTON_H

#include <QRadioButton>
#include "stdafx.h"

class NRadioButtonPrivate;
class QTNATIVEUI_EXPORT NRadioButton : public QRadioButton {
    Q_OBJECT
    Q_Q_CREATE(NRadioButton)
    
    // 颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightOuterCircleDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkOuterCircleDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightOuterCircleCheckedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkOuterCircleCheckedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightOuterCircleBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkOuterCircleBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightInnerCircleColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkInnerCircleColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledColor)
    
    // 尺寸属性
    Q_PROPERTY_CREATE_Q_H(int, OuterCircleSize)
    Q_PROPERTY_CREATE_Q_H(int, OuterCircleBorderWidth)
    Q_PROPERTY_CREATE_Q_H(int, InnerCircleSize)
    Q_PROPERTY_CREATE_Q_H(int, TextPadding)

public:
    explicit NRadioButton(QWidget* parent = nullptr);
    NRadioButton(const QString& text, QWidget* parent = nullptr);
    ~NRadioButton();

protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    void init();
    void updateAccentColors();
    void drawRadioButton(QPainter* painter);
    void drawText(QPainter* painter);
};

#endif // NRADIOBUTTON_H
