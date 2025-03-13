#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>
#include <QStyleHints>
#include <QVariant>
#include <QtCore/qeasingcurve.h>
#include <QtNativeUI/NFluentColors.h>

// 初始化私有实现
NThemePrivate::NThemePrivate(NTheme* q)
    : q_ptr(q), _themeMode(NThemeType::ThemeMode::System), _isDark(false), _accentColor(NColors::blue) {
    // 初始化颜色和设计令牌
    initLightColors();
    initDarkColors();
    initDesignTokens();

    // 初始检测系统主题 - 由公共类负责
    _isDark = q->detectSystemTheme();
}

NThemePrivate::~NThemePrivate() {}

// 初始化设计令牌 - 基于 Windows 11 Fluent UI 规范
void NThemePrivate::initDesignTokens() {
    // 圆角大小 (CornerRadius)
    _designTokens[NDesignTokenKey::CornerRadiusNone]     = 0.0;
    _designTokens[NDesignTokenKey::CornerRadiusSmall]    = 2.0;
    _designTokens[NDesignTokenKey::CornerRadiusDefault]  = 4.0;
    _designTokens[NDesignTokenKey::CornerRadiusMedium]   = 8.0;
    _designTokens[NDesignTokenKey::CornerRadiusLarge]    = 12.0;
    _designTokens[NDesignTokenKey::CornerRadiusCircular] = 999.0; // 实际应用时需计算为宽度的50%

    // 间距 (Spacing)
    _designTokens[NDesignTokenKey::SpacingNone] = 0;
    _designTokens[NDesignTokenKey::SpacingXS]   = 2;
    _designTokens[NDesignTokenKey::SpacingS]    = 4;
    _designTokens[NDesignTokenKey::SpacingM]    = 8;
    _designTokens[NDesignTokenKey::SpacingL]    = 12;
    _designTokens[NDesignTokenKey::SpacingXL]   = 16;
    _designTokens[NDesignTokenKey::SpacingXXL]  = 20;
    _designTokens[NDesignTokenKey::SpacingXXXL] = 24;

    // 字体大小 (FontSize)
    _designTokens[NDesignTokenKey::FontSizeCaption]     = 12;
    _designTokens[NDesignTokenKey::FontSizeBody]        = 14;
    _designTokens[NDesignTokenKey::FontSizeBodyLarge]   = 16;
    _designTokens[NDesignTokenKey::FontSizeTitle]       = 18;
    _designTokens[NDesignTokenKey::FontSizeTitleLarge]  = 20;
    _designTokens[NDesignTokenKey::FontSizeHeader]      = 24;
    _designTokens[NDesignTokenKey::FontSizeHeaderLarge] = 28;

    // 字重 (FontWeight)
    _designTokens[NDesignTokenKey::FontWeightRegular]  = 400;
    _designTokens[NDesignTokenKey::FontWeightMedium]   = 500;
    _designTokens[NDesignTokenKey::FontWeightSemibold] = 600;
    _designTokens[NDesignTokenKey::FontWeightBold]     = 700;

    // 阴影层级 - 存储为 QGraphicsEffect 参数或自定义结构
    _designTokens[NDesignTokenKey::ElevationNone] =
        QVariantMap({{"blurRadius", 0}, {"xOffset", 0}, {"yOffset", 0}, {"color", QColor(0, 0, 0, 0)}});

    _designTokens[NDesignTokenKey::ElevationRest] =
        QVariantMap({{"blurRadius", 4}, {"xOffset", 0}, {"yOffset", 2}, {"color", QColor(0, 0, 0, 30)}});

    _designTokens[NDesignTokenKey::ElevationHover] =
        QVariantMap({{"blurRadius", 8}, {"xOffset", 0}, {"yOffset", 4}, {"color", QColor(0, 0, 0, 36)}});

    _designTokens[NDesignTokenKey::ElevationFlyout] =
        QVariantMap({{"blurRadius", 16}, {"xOffset", 0}, {"yOffset", 8}, {"color", QColor(0, 0, 0, 41)}});

    _designTokens[NDesignTokenKey::ElevationDialog] =
        QVariantMap({{"blurRadius", 32}, {"xOffset", 0}, {"yOffset", 16}, {"color", QColor(0, 0, 0, 51)}});

    // 动效时长 (AnimationDuration)
    _designTokens[NDesignTokenKey::AnimationFast]     = 100;
    _designTokens[NDesignTokenKey::AnimationNormal]   = 200;
    _designTokens[NDesignTokenKey::AnimationSlow]     = 400;
    _designTokens[NDesignTokenKey::AnimationVerySlow] = 600;

    // 缓动曲线 (EasingCurve)
    _designTokens[NDesignTokenKey::EasingStandard]   = QEasingCurve(QEasingCurve::OutCubic);
    _designTokens[NDesignTokenKey::EasingAccelerate] = QEasingCurve(QEasingCurve::InCubic);
    _designTokens[NDesignTokenKey::EasingDecelerate] = QEasingCurve(QEasingCurve::OutCubic);
    _designTokens[NDesignTokenKey::EasingLinear]     = QEasingCurve(QEasingCurve::Linear);
}

// 初始化亮色主题颜色
void NThemePrivate::initLightColors() {
    // 直接使用自动生成的 Fluent 颜色映射
    _lightColors = LightThemeColors;

    // 添加强调色相关颜色
    updateAccentDependentColors();
}

// 初始化暗色主题颜色
void NThemePrivate::initDarkColors() {
    // 直接使用自动生成的 Fluent 颜色映射
    _darkColors = DarkThemeColors;

    // 添加强调色相关颜色
    updateAccentDependentColors();
}

// 解析颜色 - 考虑当前主题模式和自定义颜色
QColor NThemePrivate::resolveColor(NFluentColorKey::Key key) const {
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
QVariant NThemePrivate::resolveToken(NDesignTokenKey::Key key) const {
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

void NThemePrivate::updateAccentDependentColors() {
    // 定义一些关键的强调色相关颜色
    // const QColor accentColor = _accentColor.defaultBrushFor(_isDark);
}