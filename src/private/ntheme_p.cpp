#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>
#include <QStyleHints>
#include <QVariant>
#include <QtCore/qeasingcurve.h>
#include <QtNativeUI/NFluentColors.h>

NThemePrivate::NThemePrivate(NTheme* q)
    : q_ptr(q),
      _themeMode(NThemeType::ThemeMode::System),
      _isDark(false),
      _accentColor(NColors::blue),
      _useSystemAccentColor(true),
      _systemAccentColor(Qt::transparent) {
    initLightColors();
    initDarkColors();
    initDesignTokens();

    _isDark = detectSystemTheme();

    _systemAccentColor = detectSystemAccentColor();
}

NThemePrivate::~NThemePrivate() {}

void NThemePrivate::initDesignTokens() {
    // 圆角大小 (CornerRadius)
    _radiusTokens[NDesignTokenKey::CornerRadiusNone]     = 0.0;
    _radiusTokens[NDesignTokenKey::CornerRadiusSmall]    = 2.0;
    _radiusTokens[NDesignTokenKey::CornerRadiusDefault]  = 4.0;
    _radiusTokens[NDesignTokenKey::CornerRadiusMedium]   = 8.0;
    _radiusTokens[NDesignTokenKey::CornerRadiusLarge]    = 12.0;
    _radiusTokens[NDesignTokenKey::CornerRadiusCircular] = 999.0; // 实际应用时需计算为宽度的50%

    // 间距 (Spacing)
    _spacingTokens[NDesignTokenKey::SpacingNone] = 0;
    _spacingTokens[NDesignTokenKey::SpacingXS]   = 2;
    _spacingTokens[NDesignTokenKey::SpacingS]    = 4;
    _spacingTokens[NDesignTokenKey::SpacingM]    = 8;
    _spacingTokens[NDesignTokenKey::SpacingL]    = 12;
    _spacingTokens[NDesignTokenKey::SpacingXL]   = 16;
    _spacingTokens[NDesignTokenKey::SpacingXXL]  = 20;
    _spacingTokens[NDesignTokenKey::SpacingXXXL] = 24;

    // 字体大小 (FontSize)
    _fontSizeTokens[NDesignTokenKey::FontSizeCaption]    = 12;
    _fontSizeTokens[NDesignTokenKey::FontSizeBody]       = 14;
    _fontSizeTokens[NDesignTokenKey::FontSizeSubTitle]   = 20;
    _fontSizeTokens[NDesignTokenKey::FontSizeTitle]      = 28;
    _fontSizeTokens[NDesignTokenKey::FontSizeTitleLarge] = 40;
    _fontSizeTokens[NDesignTokenKey::FontSizeDisplay]    = 68;

    // 字重 (FontWeight)
    _fontWeightTokens[NDesignTokenKey::FontWeightRegular]  = 400;
    _fontWeightTokens[NDesignTokenKey::FontWeightMedium]   = 500;
    _fontWeightTokens[NDesignTokenKey::FontWeightSemibold] = 600;
    _fontWeightTokens[NDesignTokenKey::FontWeightBold]     = 700;

    // 阴影层级 - 存储为 QGraphicsEffect 参数或自定义结构
    _elevationTokens[NDesignTokenKey::ElevationNone] =
        QVariantMap({{"blurRadius", 0}, {"xOffset", 0}, {"yOffset", 0}, {"color", QColor(0, 0, 0, 0)}});

    _elevationTokens[NDesignTokenKey::ElevationRest] =
        QVariantMap({{"blurRadius", 4}, {"xOffset", 0}, {"yOffset", 2}, {"color", QColor(0, 0, 0, 30)}});

    _elevationTokens[NDesignTokenKey::ElevationHover] =
        QVariantMap({{"blurRadius", 8}, {"xOffset", 0}, {"yOffset", 4}, {"color", QColor(0, 0, 0, 36)}});

    _elevationTokens[NDesignTokenKey::ElevationFlyout] =
        QVariantMap({{"blurRadius", 16}, {"xOffset", 0}, {"yOffset", 8}, {"color", QColor(0, 0, 0, 41)}});

    _elevationTokens[NDesignTokenKey::ElevationDialog] =
        QVariantMap({{"blurRadius", 32}, {"xOffset", 0}, {"yOffset", 16}, {"color", QColor(0, 0, 0, 51)}});

    // 动效时长 (AnimationDuration)
    _animationDurationTokens[NDesignTokenKey::AnimationFast]     = 100;
    _animationDurationTokens[NDesignTokenKey::AnimationNormal]   = 200;
    _animationDurationTokens[NDesignTokenKey::AnimationSlow]     = 400;
    _animationDurationTokens[NDesignTokenKey::AnimationVerySlow] = 600;

    // 缓动曲线 (EasingCurve)
    _animationEasingTokens[NDesignTokenKey::EasingStandard]   = QEasingCurve(QEasingCurve::OutCubic);
    _animationEasingTokens[NDesignTokenKey::EasingAccelerate] = QEasingCurve(QEasingCurve::InCubic);
    _animationEasingTokens[NDesignTokenKey::EasingDecelerate] = QEasingCurve(QEasingCurve::OutCubic);
    _animationEasingTokens[NDesignTokenKey::EasingLinear]     = QEasingCurve(QEasingCurve::Linear);
}

// 初始化亮色主题颜色
void NThemePrivate::initLightColors() {
    // 直接使用自动生成的 Fluent 颜色映射
    _lightColors = LightThemeColors;
}

// 初始化暗色主题颜色
void NThemePrivate::initDarkColors() {
    // 直接使用自动生成的 Fluent 颜色映射
    _darkColors = DarkThemeColors;
}

bool NThemePrivate::detectSystemTheme() const {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    return qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#else
    QPalette pal         = QApplication::palette();
    QColor   windowColor = pal.color(QPalette::Window);
    QColor   textColor   = pal.color(QPalette::WindowText);

    bool isDark = textColor.lightness() > windowColor.lightness();

    if (!isDark) {
        int brightness = (windowColor.red() + windowColor.green() + windowColor.blue()) / 3;
        isDark         = brightness < 128;
    }

    return isDark;
#endif
}

QColor NThemePrivate::detectSystemAccentColor() const {
    QColor accentColor;
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    accentColor = QApplication::palette().color(QPalette::Active, QPalette::Accent);
#else
    // 在 Qt 6.6 之前，使用 QPalette::Highlight 作为替代
    accentColor = QApplication::palette().color(QPalette::Active, QPalette::Highlight);

    // 在某些平台上，Highlight 可能是一个不适合作为强调色的颜色
    // 例如，在某些主题中它可能是纯黑色或纯白色
    if (accentColor == Qt::black || accentColor == Qt::white) {
        // 尝试使用链接颜色作为备选
        accentColor = QApplication::palette().color(QPalette::Active, QPalette::Link);
    }
#endif

    // 如果获取的颜色无效或是黑白色，使用默认的蓝色
    if (!accentColor.isValid() || accentColor == Qt::black || accentColor == Qt::white) {
        accentColor = QColor(0, 120, 215); // Windows 10/11 默认蓝色
    }

    return accentColor;
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
    return QColor(128, 128, 128);
}

// 令牌解析模板特化
template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Radius& key) const {
    if (_customRadiusTokens.contains(key)) {
        return _customRadiusTokens[key];
    }
    if (_radiusTokens.contains(key)) {
        return _radiusTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Spacing& key) const {
    if (_customSpacingTokens.contains(key)) {
        return _customSpacingTokens[key];
    }
    if (_spacingTokens.contains(key)) {
        return _spacingTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::FontSize& key) const {
    if (_customFontSizeTokens.contains(key)) {
        return _customFontSizeTokens[key];
    }
    if (_fontSizeTokens.contains(key)) {
        return _fontSizeTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::FontWeight& key) const {
    if (_customFontWeightTokens.contains(key)) {
        return _customFontWeightTokens[key];
    }
    if (_fontWeightTokens.contains(key)) {
        return _fontWeightTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Elevation& key) const {
    if (_customElevationTokens.contains(key)) {
        return _customElevationTokens[key];
    }
    if (_elevationTokens.contains(key)) {
        return _elevationTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::AnimationDuration& key) const {
    if (_customAnimationDurationTokens.contains(key)) {
        return _customAnimationDurationTokens[key];
    }
    if (_animationDurationTokens.contains(key)) {
        return _animationDurationTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::AnimationEasing& key) const {
    if (_customAnimationEasingTokens.contains(key)) {
        return _customAnimationEasingTokens[key];
    }
    if (_animationEasingTokens.contains(key)) {
        return _animationEasingTokens[key];
    }
    return QVariant();
}
