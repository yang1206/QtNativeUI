#ifndef QTNATIVEUI_NTHEME_P_H
#define QTNATIVEUI_NTHEME_P_H

#include <QCache>
#include <QColor>
#include <QPixmap>
#include <QTimer>
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

    static const QMap<NFluentColorKey::Key, QColor>* s_lightColors;
    static const QMap<NFluentColorKey::Key, QColor>* s_darkColors;
    static bool                                      s_staticDataInitialized;

    QMap<NFluentColorKey::Key, QColor> _customColors;

    // 设计令牌
    static QHash<NDesignTokenKey::Radius, QVariant>            s_defaultRadiusTokens;
    static QHash<NDesignTokenKey::Spacing, QVariant>           s_defaultSpacingTokens;
    static QHash<NDesignTokenKey::FontSize, QVariant>          s_defaultFontSizeTokens;
    static QHash<NDesignTokenKey::FontWeight, QVariant>        s_defaultFontWeightTokens;
    static QHash<NDesignTokenKey::Elevation, QVariant>         s_defaultElevationTokens;
    static QHash<NDesignTokenKey::AnimationDuration, QVariant> s_defaultAnimationDurationTokens;
    static QHash<NDesignTokenKey::AnimationEasing, QVariant>   s_defaultAnimationEasingTokens;

    // 自定义令牌 - 按类别存储
    QHash<NDesignTokenKey::Radius, QVariant>            _customRadiusTokens;
    QHash<NDesignTokenKey::Spacing, QVariant>           _customSpacingTokens;
    QHash<NDesignTokenKey::FontSize, QVariant>          _customFontSizeTokens;
    QHash<NDesignTokenKey::FontWeight, QVariant>        _customFontWeightTokens;
    QHash<NDesignTokenKey::Elevation, QVariant>         _customElevationTokens;
    QHash<NDesignTokenKey::AnimationDuration, QVariant> _customAnimationDurationTokens;
    QHash<NDesignTokenKey::AnimationEasing, QVariant>   _customAnimationEasingTokens;

    mutable QCache<NFluentColorKey::Key, QColor> _colorCache;

    // 移除阴影缓存，新算法本身就很高效

    mutable bool   _systemThemeCacheValid       = false;
    mutable bool   _systemThemeCache            = false;
    mutable bool   _systemAccentColorCacheValid = false;
    mutable QColor _systemAccentColorCache;

    QTimer* _cacheInvalidationTimer;

    static void initializeStaticData();

    bool   detectSystemTheme() const;
    QColor detectSystemAccentColor() const;

    QColor resolveColor(NFluentColorKey::Key key) const;
    template <typename T>
    QVariant resolveToken(const T& key) const;

    void setupCacheInvalidationTimer();
    void invalidateSystemCache();
    void invalidateColorCache();
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