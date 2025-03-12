#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>
#include <QStyleHints>
#include <QVariant>
#include <QtNativeUI/NFluentColors.h>

// 初始化私有实现
NTheme::NThemePrivate::NThemePrivate(NTheme* q)
    : _themeMode(NThemeType::ThemeMode::System), _isDark(false), _accentColor(NColors::blue), q_ptr(q) {
    // 初始化颜色和设计令牌
    initLightColors();
    initDarkColors();
    initDesignTokens();

    // 初始检测系统主题 - 由公共类负责
    _isDark = q->detectSystemTheme();
}

NTheme::NThemePrivate::~NThemePrivate() {}

// 初始化设计令牌 - 基于 Windows 11 Fluent UI 规范
void NTheme::NThemePrivate::initDesignTokens() {
    // 圆角大小 (CornerRadius)
    _designTokens["cornerRadiusNone"]     = 0.0;
    _designTokens["cornerRadiusSmall"]    = 2.0;
    _designTokens["cornerRadiusDefault"]  = 4.0;
    _designTokens["cornerRadiusMedium"]   = 8.0;
    _designTokens["cornerRadiusLarge"]    = 12.0;
    _designTokens["cornerRadiusCircular"] = 999.0; // 实际应用时需计算为宽度的50%

    // 间距 (Spacing)
    _designTokens["spacingNone"] = 0;
    _designTokens["spacingXS"]   = 2;
    _designTokens["spacingS"]    = 4;
    _designTokens["spacingM"]    = 8;
    _designTokens["spacingL"]    = 12;
    _designTokens["spacingXL"]   = 16;
    _designTokens["spacingXXL"]  = 20;
    _designTokens["spacingXXXL"] = 24;

    // 字体大小 (FontSize)
    _designTokens["fontSizeCaption"]     = 12;
    _designTokens["fontSizeBody"]        = 14;
    _designTokens["fontSizeBodyLarge"]   = 16;
    _designTokens["fontSizeTitle"]       = 18;
    _designTokens["fontSizeTitleLarge"]  = 20;
    _designTokens["fontSizeHeader"]      = 24;
    _designTokens["fontSizeHeaderLarge"] = 28;

    // 字重 (FontWeight)
    _designTokens["fontWeightRegular"]  = 400;
    _designTokens["fontWeightMedium"]   = 500;
    _designTokens["fontWeightSemibold"] = 600;
    _designTokens["fontWeightBold"]     = 700;

    // 阴影层级 (Elevation) - 存储为CSS样式字符串，便于应用
    _designTokens["elevationNone"]   = QString();
    _designTokens["elevationRest"]   = QString("0 2px 4px rgba(0,0,0,0.12)");
    _designTokens["elevationHover"]  = QString("0 4px 8px rgba(0,0,0,0.14)");
    _designTokens["elevationFlyout"] = QString("0 8px 16px rgba(0,0,0,0.16)");
    _designTokens["elevationDialog"] = QString("0 16px 32px rgba(0,0,0,0.20)");

    // 动效时长 (AnimationDuration)
    _designTokens["animationFast"]     = 100;
    _designTokens["animationNormal"]   = 200;
    _designTokens["animationSlow"]     = 400;
    _designTokens["animationVerySlow"] = 600;

    // 缓动曲线 (EasingCurve)
    _designTokens["easingStandard"]   = QString("cubic-bezier(0.8, 0, 0.2, 1)"); // 标准
    _designTokens["easingAccelerate"] = QString("cubic-bezier(0.9, 0, 0.1, 1)"); // 加速
    _designTokens["easingDecelerate"] = QString("cubic-bezier(0.1, 0, 0.9, 1)"); // 减速
    _designTokens["easingLinear"]     = QString("linear");                       // 线性
}

// 初始化亮色主题颜色
void NTheme::NThemePrivate::initLightColors() {
    // 直接使用自动生成的 Fluent 颜色映射
    _lightColors = LightThemeColors;

    // 添加强调色相关颜色
    updateAccentDependentColors();
}

// 初始化暗色主题颜色
void NTheme::NThemePrivate::initDarkColors() {
    // 直接使用自动生成的 Fluent 颜色映射
    _darkColors = DarkThemeColors;

    // 添加强调色相关颜色
    updateAccentDependentColors();
}

// 解析颜色 - 考虑当前主题模式和自定义颜色
QColor NTheme::NThemePrivate::resolveColor(NFluentColorKey::Key key) const {
    // 首先检查自定义颜色
    if (_customColors.contains(key)) {
        return _customColors[key];
    }

    // 然后根据当前主题模式选择颜色
    const QMap<NFluentColorKey::Key, QColor>& themeColors = _isDark ? _darkColors : _lightColors;
    if (themeColors.contains(key)) {
        return themeColors[key];
    }

    // 返回安全的默认颜色
    return QColor(128, 128, 128); // 返回中性灰色作为默认值
}

// 解析设计令牌
QVariant NTheme::NThemePrivate::resolveToken(const QString& key) const {
    // 首先检查自定义令牌
    if (_customTokens.contains(key)) {
        return _customTokens[key];
    }

    // 然后检查默认令牌
    if (_designTokens.contains(key)) {
        return _designTokens[key];
    }

    // 如果找不到令牌，返回空值
    return QVariant();
}

void NTheme::NThemePrivate::updateAccentDependentColors() {
    // 定义一些关键的强调色相关颜色
    // const QColor accentColor = _accentColor.defaultBrushFor(_isDark);
}