#include "npushbutton_p.h"
#include <QtNativeUI/NTheme.h>
#include <QApplication>
#include <QPainter>
#include <QPainterPath>

namespace QtNativeUI {

NPushButton::NPushButtonPrivate::NPushButtonPrivate(NPushButton* q)
    : _buttonState(ButtonState::Normal)
    , _buttonStyle(NPushButton::NPushButtonStyle::Primary)
    , _cornerRadius(4.0)
    , _elevationEnabled(true)
    , q_ptr(q)
{
    initStyleColors();
    connectThemeSignals();
}

NPushButton::NPushButtonPrivate::~NPushButtonPrivate()
{
}

QColor NPushButton::NPushButtonPrivate::currentBackgroundColor() const
{
    if (!q_ptr->isEnabled()) {
        return disabledBackgroundColor;
    }
    
    // 如果设置了自定义背景色，则使用自定义色
    if (_backgroundColor.isValid()) {
        return _backgroundColor;
    }
    
    switch (_buttonState) {
        case ButtonState::Normal:
            return normalBackgroundColor;
        case ButtonState::Hover:
            return hoverBackgroundColor;
        case ButtonState::Pressed:
            return pressedBackgroundColor;
        default:
            return normalBackgroundColor;
    }
}

QColor NPushButton::NPushButtonPrivate::currentTextColor() const
{
    if (!q_ptr->isEnabled()) {
        return disabledTextColor;
    }
    
    // 如果设置了自定义文本色，则使用自定义色
    if (_textColor.isValid()) {
        return _textColor;
    }
    
    switch (_buttonState) {
        case ButtonState::Normal:
            return normalTextColor;
        case ButtonState::Hover:
            return hoverTextColor;
        case ButtonState::Pressed:
            return pressedTextColor;
        default:
            return normalTextColor;
    }
}

QColor NPushButton::NPushButtonPrivate::currentBorderColor() const
{
    if (!q_ptr->isEnabled()) {
        return disabledBorderColor;
    }
    
    // 如果设置了自定义边框色，则使用自定义色
    if (_borderColor.isValid()) {
        return _borderColor;
    }
    
    switch (_buttonState) {
        case ButtonState::Normal:
            return normalBorderColor;
        case ButtonState::Hover:
            return hoverBorderColor;
        case ButtonState::Pressed:
            return pressedBorderColor;
        default:
            return normalBorderColor;
    }
}

void NPushButton::NPushButtonPrivate::initStyleColors()
{
    auto theme = NTheme::instance();
    
    switch (_buttonStyle) {
    case NPushButton::NPushButtonStyle::Primary:
        // 主要按钮 - 使用强调色
        normalBackgroundColor = theme->color(NColorRole::AccentPrimary);
        hoverBackgroundColor = theme->color(NColorRole::AccentSecondary);
        pressedBackgroundColor = theme->color(NColorRole::AccentPrimary).darker(110);
        disabledBackgroundColor = normalBackgroundColor;
        disabledBackgroundColor.setAlpha(128);
        
        normalTextColor = Qt::white;
        hoverTextColor = Qt::white;
        pressedTextColor = Qt::white;
        disabledTextColor = Qt::white;
        disabledTextColor.setAlpha(128);
        
        normalBorderColor = Qt::transparent;
        hoverBorderColor = Qt::transparent;
        pressedBorderColor = Qt::transparent;
        disabledBorderColor = Qt::transparent;
        break;
        
    case NPushButton::NPushButtonStyle::Secondary:
        // 次要按钮 - 使用控件背景色
        normalBackgroundColor = theme->color(NColorRole::ControlBackground);
        hoverBackgroundColor = theme->color(NColorRole::ControlBackgroundHover);
        pressedBackgroundColor = theme->color(NColorRole::ControlBackgroundPressed);
        disabledBackgroundColor = normalBackgroundColor;
        disabledBackgroundColor.setAlpha(128);
        
        normalTextColor = theme->color(NColorRole::TextPrimary);
        hoverTextColor = theme->color(NColorRole::TextPrimary);
        pressedTextColor = theme->color(NColorRole::TextPrimary);
        disabledTextColor = theme->color(NColorRole::TextDisabled);
        
        normalBorderColor = theme->color(NColorRole::ControlBorder);
        hoverBorderColor = theme->color(NColorRole::ControlBorderHover);
        pressedBorderColor = theme->color(NColorRole::ControlBorderPressed);
        disabledBorderColor = theme->color(NColorRole::ControlBorderDisabled);
        break;
        
    case NPushButton::NPushButtonStyle::Text:
        // 文本按钮 - 无背景
        normalBackgroundColor = Qt::transparent;
        hoverBackgroundColor = theme->color(NColorRole::ControlBackgroundHover);
        pressedBackgroundColor = theme->color(NColorRole::ControlBackgroundPressed);
        disabledBackgroundColor = Qt::transparent;
        
        normalTextColor = theme->color(NColorRole::AccentPrimary);
        hoverTextColor = theme->color(NColorRole::AccentSecondary);
        pressedTextColor = theme->color(NColorRole::AccentPrimary).darker(120);
        disabledTextColor = theme->color(NColorRole::TextDisabled);
        
        normalBorderColor = Qt::transparent;
        hoverBorderColor = Qt::transparent;
        pressedBorderColor = Qt::transparent;
        disabledBorderColor = Qt::transparent;
        break;
        
    case NPushButton::NPushButtonStyle::Outline:
        // 轮廓按钮 - 透明背景带边框
        normalBackgroundColor = Qt::transparent;
        hoverBackgroundColor = theme->color(NColorRole::ControlBackgroundHover).darker(110);
        pressedBackgroundColor = theme->color(NColorRole::ControlBackgroundPressed).darker(110);
        disabledBackgroundColor = Qt::transparent;
        
        normalTextColor = theme->color(NColorRole::AccentPrimary);
        hoverTextColor = theme->color(NColorRole::AccentSecondary);
        pressedTextColor = theme->color(NColorRole::AccentPrimary).darker(120);
        disabledTextColor = theme->color(NColorRole::TextDisabled);
        
        normalBorderColor = theme->color(NColorRole::AccentPrimary);
        hoverBorderColor = theme->color(NColorRole::AccentSecondary);
        pressedBorderColor = theme->color(NColorRole::AccentPrimary).darker(120);
        disabledBorderColor = theme->color(NColorRole::ControlBorderDisabled);
        break;
        
    case NPushButton::NPushButtonStyle::Danger:
        // 危险按钮 - 使用错误色
        normalBackgroundColor = theme->color(NColorRole::Error);
        hoverBackgroundColor = theme->color(NColorRole::Error).lighter(110);
        pressedBackgroundColor = theme->color(NColorRole::Error).darker(110);
        disabledBackgroundColor = normalBackgroundColor;
        disabledBackgroundColor.setAlpha(128);
        
        normalTextColor = Qt::white;
        hoverTextColor = Qt::white;
        pressedTextColor = Qt::white;
        disabledTextColor = QColor(Qt::white);
        disabledTextColor.setAlpha(128);
        
        normalBorderColor = Qt::transparent;
        hoverBorderColor = Qt::transparent;
        pressedBorderColor = Qt::transparent;
        disabledBorderColor = Qt::transparent;
        break;
    }
}

void NPushButton::NPushButtonPrivate::handleThemeChanged()
{
    initStyleColors();
    q_ptr->update();
}

void NPushButton::NPushButtonPrivate::connectThemeSignals()
{
    // 连接主题变化信号
    QObject::connect(NTheme::instance(), &NTheme::darkModeChanged,
                     q_ptr, [this](bool) { handleThemeChanged(); });
                    
    // 连接颜色变化信号
    QObject::connect(NTheme::instance(), &NTheme::colorChanged,
                     q_ptr, [this](NColorRole, const QColor&) { handleThemeChanged(); });
}

} // namespace QtNativeUI 