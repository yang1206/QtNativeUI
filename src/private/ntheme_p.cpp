#include "ntheme_p.h"
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QStyleHints>
#include <QVariant>
#include <QtCore/qeasingcurve.h>
#include <QtNativeUI/NFluentColors.h>

const QMap<NFluentColorKey::Key, QColor>* NThemePrivate::s_lightColors           = nullptr;
const QMap<NFluentColorKey::Key, QColor>* NThemePrivate::s_darkColors            = nullptr;
bool                                      NThemePrivate::s_staticDataInitialized = false;

QHash<NDesignTokenKey::Radius, QVariant>            NThemePrivate::s_defaultRadiusTokens;
QHash<NDesignTokenKey::Spacing, QVariant>           NThemePrivate::s_defaultSpacingTokens;
QHash<NDesignTokenKey::FontSize, QVariant>          NThemePrivate::s_defaultFontSizeTokens;
QHash<NDesignTokenKey::FontWeight, QVariant>        NThemePrivate::s_defaultFontWeightTokens;
QHash<NDesignTokenKey::Elevation, QVariant>         NThemePrivate::s_defaultElevationTokens;
QHash<NDesignTokenKey::AnimationDuration, QVariant> NThemePrivate::s_defaultAnimationDurationTokens;
QHash<NDesignTokenKey::AnimationEasing, QVariant>   NThemePrivate::s_defaultAnimationEasingTokens;

NThemePrivate::NThemePrivate(NTheme* q)
    : q_ptr(q),
      _themeMode(NThemeType::ThemeMode::System),
      _isDark(false),
      _accentColor(NColors::blue),
      _useSystemAccentColor(true),
      _systemAccentColor(Qt::transparent),
      _cacheInvalidationTimer(nullptr) {
    // 确保静态数据只初始化一次
    if (!s_staticDataInitialized) {
        initializeStaticData();
        s_staticDataInitialized = true;
    }

    // 设置缓存容量（限制内存使用）
    _colorCache.setMaxCost(100); // 最多缓存100个颜色

    // 设置缓存失效定时器
    setupCacheInvalidationTimer();

    _isDark            = detectSystemTheme();
    _systemAccentColor = detectSystemAccentColor();
}

NThemePrivate::~NThemePrivate() {
    if (_cacheInvalidationTimer) {
        _cacheInvalidationTimer->stop();
        delete _cacheInvalidationTimer;
    }
}

void NThemePrivate::initializeStaticData() {
    // 初始化静态颜色数据（指向全局常量，不复制）
    s_lightColors = &LightThemeColors;
    s_darkColors  = &DarkThemeColors;

    // 初始化设计令牌（移动原有的initDesignTokens内容到这里）
    // 圆角大小 (CornerRadius)
    s_defaultRadiusTokens[NDesignTokenKey::CornerRadiusNone]     = 0.0;
    s_defaultRadiusTokens[NDesignTokenKey::CornerRadiusSmall]    = 2.0;
    s_defaultRadiusTokens[NDesignTokenKey::CornerRadiusDefault]  = 4.0;
    s_defaultRadiusTokens[NDesignTokenKey::CornerRadiusMedium]   = 8.0;
    s_defaultRadiusTokens[NDesignTokenKey::CornerRadiusLarge]    = 12.0;
    s_defaultRadiusTokens[NDesignTokenKey::CornerRadiusCircular] = 999.0; // 实际应用时需计算为宽度的50%

    // 间距 (Spacing)
    s_defaultSpacingTokens[NDesignTokenKey::SpacingNone] = 0;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingXS]   = 2;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingS]    = 4;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingM]    = 8;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingL]    = 12;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingXL]   = 16;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingXXL]  = 20;
    s_defaultSpacingTokens[NDesignTokenKey::SpacingXXXL] = 24;

    // 字体大小 (FontSize)
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeCaption]    = 12;
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeBody]       = 14;
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeBodyLarge]  = 18;
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeSubTitle]   = 20;
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeTitle]      = 28;
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeTitleLarge] = 40;
    s_defaultFontSizeTokens[NDesignTokenKey::FontSizeDisplay]    = 68;

    // 字重 (FontWeight)
    s_defaultFontWeightTokens[NDesignTokenKey::FontWeightRegular]  = 400;
    s_defaultFontWeightTokens[NDesignTokenKey::FontWeightMedium]   = 500;
    s_defaultFontWeightTokens[NDesignTokenKey::FontWeightSemibold] = 600;
    s_defaultFontWeightTokens[NDesignTokenKey::FontWeightBold]     = 700;

    // 阴影层级 - 存储为 QGraphicsEffect 参数或自定义结构
    s_defaultElevationTokens[NDesignTokenKey::ElevationNone] =
        QVariantMap({{"blurRadius", 0}, {"xOffset", 0}, {"yOffset", 0}, {"color", QColor(0, 0, 0, 0)}});

    s_defaultElevationTokens[NDesignTokenKey::ElevationRest] =
        QVariantMap({{"blurRadius", 4}, {"xOffset", 0}, {"yOffset", 2}, {"color", QColor(0, 0, 0, 30)}});

    s_defaultElevationTokens[NDesignTokenKey::ElevationHover] =
        QVariantMap({{"blurRadius", 8}, {"xOffset", 0}, {"yOffset", 4}, {"color", QColor(0, 0, 0, 36)}});

    s_defaultElevationTokens[NDesignTokenKey::ElevationFlyout] =
        QVariantMap({{"blurRadius", 16}, {"xOffset", 0}, {"yOffset", 8}, {"color", QColor(0, 0, 0, 41)}});

    s_defaultElevationTokens[NDesignTokenKey::ElevationDialog] =
        QVariantMap({{"blurRadius", 32}, {"xOffset", 0}, {"yOffset", 16}, {"color", QColor(0, 0, 0, 51)}});

    // 动效时长 (AnimationDuration)
    s_defaultAnimationDurationTokens[NDesignTokenKey::AnimationFast]     = 100;
    s_defaultAnimationDurationTokens[NDesignTokenKey::AnimationNormal]   = 200;
    s_defaultAnimationDurationTokens[NDesignTokenKey::AnimationSlow]     = 400;
    s_defaultAnimationDurationTokens[NDesignTokenKey::AnimationVerySlow] = 600;

    // 缓动曲线 (EasingCurve)
    s_defaultAnimationEasingTokens[NDesignTokenKey::EasingStandard]   = QEasingCurve(QEasingCurve::OutCubic);
    s_defaultAnimationEasingTokens[NDesignTokenKey::EasingAccelerate] = QEasingCurve(QEasingCurve::InCubic);
    s_defaultAnimationEasingTokens[NDesignTokenKey::EasingDecelerate] = QEasingCurve(QEasingCurve::OutCubic);
    s_defaultAnimationEasingTokens[NDesignTokenKey::EasingLinear]     = QEasingCurve(QEasingCurve::Linear);
}

bool NThemePrivate::detectSystemTheme() const {
    if (_systemThemeCacheValid) {
        return _systemThemeCache;
    }

    bool isDark = false;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    isDark = qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#else
    QPalette pal         = QApplication::palette();
    QColor   windowColor = pal.color(QPalette::Window);
    QColor   textColor   = pal.color(QPalette::WindowText);

    isDark = textColor.lightness() > windowColor.lightness();

    if (!isDark) {
        int brightness = (windowColor.red() + windowColor.green() + windowColor.blue()) / 3;
        isDark         = brightness < 128;
    }
#endif

    _systemThemeCache      = isDark;
    _systemThemeCacheValid = true;

    if (_cacheInvalidationTimer && !_cacheInvalidationTimer->isActive()) {
        _cacheInvalidationTimer->start();
    }

    return isDark;
}

QColor NThemePrivate::detectSystemAccentColor() const {
    if (_systemAccentColorCacheValid) {
        return _systemAccentColorCache;
    }

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

    if (!accentColor.isValid() || accentColor == Qt::black || accentColor == Qt::white) {
        accentColor = QColor(0, 120, 215); // Windows 10/11 默认蓝色
    }
    _systemAccentColorCache      = accentColor;
    _systemAccentColorCacheValid = true;

    // 启动定时器，5秒后失效缓存
    if (_cacheInvalidationTimer && !_cacheInvalidationTimer->isActive()) {
        _cacheInvalidationTimer->start();
    }

    return accentColor;
}

QColor NThemePrivate::resolveColor(NFluentColorKey::Key key) const {
    if (QColor* cachedColor = _colorCache.object(key)) {
        return *cachedColor;
    }

    QColor resolvedColor;

    if (_customColors.contains(key)) {
        resolvedColor = _customColors[key];
    } else {
        const QMap<NFluentColorKey::Key, QColor>& themeColors = _isDark ? *s_darkColors : *s_lightColors;
        if (themeColors.contains(key)) {
            resolvedColor = themeColors[key];
        } else {
            resolvedColor = QColor(128, 128, 128);
        }
    }

    _colorCache.insert(key, new QColor(resolvedColor));

    return resolvedColor;
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Radius& key) const {
    if (_customRadiusTokens.contains(key)) {
        return _customRadiusTokens[key];
    }
    if (s_defaultRadiusTokens.contains(key)) {
        return s_defaultRadiusTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Spacing& key) const {
    if (_customSpacingTokens.contains(key)) {
        return _customSpacingTokens[key];
    }
    if (s_defaultSpacingTokens.contains(key)) {
        return s_defaultSpacingTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::FontSize& key) const {
    if (_customFontSizeTokens.contains(key)) {
        return _customFontSizeTokens[key];
    }
    if (s_defaultFontSizeTokens.contains(key)) {
        return s_defaultFontSizeTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::FontWeight& key) const {
    if (_customFontWeightTokens.contains(key)) {
        return _customFontWeightTokens[key];
    }
    if (s_defaultFontWeightTokens.contains(key)) {
        return s_defaultFontWeightTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::Elevation& key) const {
    if (_customElevationTokens.contains(key)) {
        return _customElevationTokens[key];
    }
    if (s_defaultElevationTokens.contains(key)) {
        return s_defaultElevationTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::AnimationDuration& key) const {
    if (_customAnimationDurationTokens.contains(key)) {
        return _customAnimationDurationTokens[key];
    }
    if (s_defaultAnimationDurationTokens.contains(key)) {
        return s_defaultAnimationDurationTokens[key];
    }
    return QVariant();
}

template <>
QVariant NThemePrivate::resolveToken(const NDesignTokenKey::AnimationEasing& key) const {
    if (_customAnimationEasingTokens.contains(key)) {
        return _customAnimationEasingTokens[key];
    }
    if (s_defaultAnimationEasingTokens.contains(key)) {
        return s_defaultAnimationEasingTokens[key];
    }
    return QVariant();
}

void NThemePrivate::setupCacheInvalidationTimer() {
    _cacheInvalidationTimer = new QTimer(q_ptr);
    _cacheInvalidationTimer->setSingleShot(true);
    _cacheInvalidationTimer->setInterval(5000); // 5秒后失效缓存

    QObject::connect(_cacheInvalidationTimer, &QTimer::timeout, [this]() { invalidateSystemCache(); });
}

void NThemePrivate::invalidateSystemCache() {
    _systemThemeCacheValid       = false;
    _systemAccentColorCacheValid = false;
}

void NThemePrivate::invalidateColorCache() { _colorCache.clear(); }
