#ifndef QTNATIVEUI_NTHEME_P_H
#define QTNATIVEUI_NTHEME_P_H

#include <QColor>
#include <QVariant>
#include <QtNativeUI/NColor.h>
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NTheme.h>

class NTheme;
class NThemePrivate {
    Q_D_CREATE(NTheme)
  public:
    NThemePrivate(NTheme* q);
    ~NThemePrivate();

    // 主题状态
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    // 强调色
    NAccentColor _accentColor;
    bool         _useSystemAccentColor;
    QColor       _systemAccentColor;

    // 颜色存储 - 使用枚举键
    QMap<NFluentColorKey::Key, QColor> _lightColors;
    QMap<NFluentColorKey::Key, QColor> _darkColors;
    QMap<NFluentColorKey::Key, QColor> _customColors;

    // 设计令牌 - 按类别存储
    QHash<NDesignTokenKey::Radius, QVariant>            _radiusTokens;
    QHash<NDesignTokenKey::Spacing, QVariant>           _spacingTokens;
    QHash<NDesignTokenKey::FontSize, QVariant>          _fontSizeTokens;
    QHash<NDesignTokenKey::FontWeight, QVariant>        _fontWeightTokens;
    QHash<NDesignTokenKey::Elevation, QVariant>         _elevationTokens;
    QHash<NDesignTokenKey::AnimationDuration, QVariant> _animationDurationTokens;
    QHash<NDesignTokenKey::AnimationEasing, QVariant>   _animationEasingTokens;

    // 自定义令牌 - 按类别存储
    QHash<NDesignTokenKey::Radius, QVariant>            _customRadiusTokens;
    QHash<NDesignTokenKey::Spacing, QVariant>           _customSpacingTokens;
    QHash<NDesignTokenKey::FontSize, QVariant>          _customFontSizeTokens;
    QHash<NDesignTokenKey::FontWeight, QVariant>        _customFontWeightTokens;
    QHash<NDesignTokenKey::Elevation, QVariant>         _customElevationTokens;
    QHash<NDesignTokenKey::AnimationDuration, QVariant> _customAnimationDurationTokens;
    QHash<NDesignTokenKey::AnimationEasing, QVariant>   _customAnimationEasingTokens;

    // 初始化方法
    void initLightColors();
    void initDarkColors();
    void initDesignTokens();

    // 系统主题检测
    bool detectSystemTheme() const;

    QColor detectSystemAccentColor() const;

    // 颜色解析
    QColor resolveColor(NFluentColorKey::Key key) const;

    // 令牌解析 - 泛型方法
    template <typename T>
    QVariant resolveToken(const T& key) const;
};

template <typename T>
QVariant NThemePrivate::resolveToken(const T& key) const {
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Radius& key) const;

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Spacing& key) const;

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::FontSize& key) const;

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::FontWeight& key) const;

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Elevation& key) const;

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::AnimationDuration& key) const;

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::AnimationEasing& key) const;

#endif // QTNATIVEUI_NTHEME_P_H