//
// Created by Yang1206 on 2025/4/6.
//

#include "nscrollbar_p.h"
#include "QtNativeUI/NTheme.h"

NScrollBarPrivate::NScrollBarPrivate(QObject* parent) : QObject(parent) {
    themeMode         = nTheme->themeMode();
    isDark            = nTheme->isDarkMode();
    hoveredSubControl = QStyle::SC_None;
    pressedSubControl = QStyle::SC_None;

    // 默认轨道颜色使用透明度低的颜色
    _pLightTrackColor = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    _pDarkTrackColor  = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);

    // 半透明的滑块颜色
    _pLightThumbColor = QColor(NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light));
    _pLightThumbColor.setAlphaF(0.5);
    _pDarkThumbColor = QColor(NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark));
    _pDarkThumbColor.setAlphaF(0.5);

    // 悬浮和按下状态的不透明滑块颜色
    _pLightThumbHoverColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    _pDarkThumbHoverColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);

    _pLightThumbPressColor = NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, NThemeType::Light);
    _pDarkThumbPressColor  = NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, NThemeType::Dark);

    _pTrackThickness       = 8; // 轨道厚度，现在明确大于常规滑块厚度
    _pThumbThickness       = 6; // 悬浮/按下时的厚度，比轨道小一些
    _pThumbNormalThickness = 3; // 正常状态下的厚度，更细
    _pThumbCornerRadius    = 3;
    _pTrackCornerRadius    = 4;
}

NScrollBarPrivate::~NScrollBarPrivate() {}

void NScrollBarPrivate::updateStyle() {
    if (!style)
        return;

    QColor thumbColor      = isDark ? _pDarkThumbColor : _pLightThumbColor;
    QColor thumbHoverColor = isDark ? _pDarkThumbHoverColor : _pLightThumbHoverColor;
    QColor thumbPressColor = isDark ? _pDarkThumbPressColor : _pLightThumbPressColor;
    QColor trackColor      = isDark ? _pDarkTrackColor : _pLightTrackColor;
    QColor arrowColor      = isDark ? _pDarkArrowColor : _pLightArrowColor;
    QColor arrowHoverColor = isDark ? _pDarkArrowHoverColor : _pLightArrowHoverColor;
    QColor arrowPressColor = isDark ? _pDarkArrowPressColor : _pLightArrowPressColor;

    style->setThumbColor(thumbColor);
    style->setThumbHoverColor(thumbHoverColor);
    style->setThumbPressColor(thumbPressColor);
    style->setTrackColor(trackColor);
    style->setArrowColor(arrowColor);
    style->setArrowHoverColor(arrowHoverColor);
    style->setArrowPressColor(arrowPressColor);
    style->setThumbThickness(_pThumbThickness);
    style->setThumbNormalThickness(_pThumbNormalThickness);
    style->setTrackThickness(_pTrackThickness);
    style->setThumbCornerRadius(_pThumbCornerRadius);
    style->setTrackCornerRadius(_pTrackCornerRadius);
    style->setHovered(isHovered);
    style->setPressed(isPressed);

    // 更新箭头状态
    style->setLineHovered(QStyle::SC_ScrollBarAddLine, hoveredSubControl == QStyle::SC_ScrollBarAddLine);
    style->setLineHovered(QStyle::SC_ScrollBarSubLine, hoveredSubControl == QStyle::SC_ScrollBarSubLine);
    style->setLinePressed(QStyle::SC_ScrollBarAddLine, pressedSubControl == QStyle::SC_ScrollBarAddLine);
    style->setLinePressed(QStyle::SC_ScrollBarSubLine, pressedSubControl == QStyle::SC_ScrollBarSubLine);

    q_ptr->update();
}

void NScrollBarPrivate::initScrollBarStyle() {
    if (!style) {
        style = new NScrollBarStyle(q_ptr->style());
        q_ptr->setStyle(style);
    }
    updateStyle();
}