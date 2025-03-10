#ifndef QTNATIVEUI_NPUSHBUTTON_P_H
#define QTNATIVEUI_NPUSHBUTTON_P_H

#include <QtNativeUI/NPushButton.h>
#include <QColor>

namespace QtNativeUI {

class NPushButton::NPushButtonPrivate
{
public:
    NPushButtonPrivate(NPushButton* q);
    ~NPushButtonPrivate();
    
    // 按钮状态
    enum class ButtonState {
        Normal,
        Hover,
        Pressed,
        Disabled
    };
    
    // 当前状态
    ButtonState _buttonState;
    NPushButton::NPushButtonStyle _buttonStyle;
    
    // 外观属性
    qreal _cornerRadius;
    bool _elevationEnabled;
    QColor _backgroundColor;
    QColor _textColor;
    QColor _borderColor;
    
    // 状态颜色
    QColor normalBackgroundColor;
    QColor hoverBackgroundColor;
    QColor pressedBackgroundColor;
    QColor disabledBackgroundColor;
    
    QColor normalTextColor;
    QColor hoverTextColor;
    QColor pressedTextColor;
    QColor disabledTextColor;
    
    QColor normalBorderColor;
    QColor hoverBorderColor;
    QColor pressedBorderColor;
    QColor disabledBorderColor;
    
    // 计算当前状态下的颜色
    QColor currentBackgroundColor() const;
    QColor currentTextColor() const;
    QColor currentBorderColor() const;
    
    // 根据按钮样式初始化颜色
    void initStyleColors();
    
    // 主题变化处理
    void handleThemeChanged();
    
    // 连接主题信号
    void connectThemeSignals();
    
    // 使用宏声明公共类
    N_DECLARE_PUBLIC(NPushButton)
};

} // namespace QtNativeUI

#endif // QTNATIVEUI_NPUSHBUTTON_P_H 