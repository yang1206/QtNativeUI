#include <QApplication>
#include <QPainterPath>
#include <QPalette>
#include <QStyleHints>
#include <QtNativeUI/NTheme.h>
#include "../private/ntheme_p.h"

Q_SINGLETON_CREATE_CPP(NTheme)
NTheme::NTheme(QObject* parent) : QObject(parent), d_ptr(new NThemePrivate(this)) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    connect(qApp->styleHints(), &QStyleHints::colorSchemeChanged, this, [this](Qt::ColorScheme) {
        updateThemeState();
        if (d_ptr->_themeMode == NThemeType::ThemeMode::System) {
            emit themeModeChanged(d_ptr->_themeMode);
        }
    });
    connect(qApp, &QApplication::paletteChanged, this, [this]() {
        if (d_ptr->_useSystemAccentColor) {
            QColor newAccentColor = d_ptr->detectSystemAccentColor();
            if (newAccentColor.isValid() && newAccentColor != d_ptr->_systemAccentColor) {
                d_ptr->_systemAccentColor = newAccentColor;
                setAccentColor(newAccentColor);
                emit systemAccentColorChanged(newAccentColor);
            }
        }
    });
#else
    connect(qApp, &QApplication::paletteChanged, this, [this]() {
        updateThemeState();
        if (d_ptr->_themeMode == NThemeType::ThemeMode::System) {
            emit themeModeChanged(d_ptr->_themeMode);
        }
        if (d_ptr->_useSystemAccentColor) {
            QColor newAccentColor = d_ptr->detectSystemAccentColor();
            if (newAccentColor.isValid() && newAccentColor != d_ptr->_systemAccentColor) {
                d_ptr->_systemAccentColor = newAccentColor;
                setAccentColor(newAccentColor);
                emit systemAccentColorChanged(newAccentColor);
            }
        }
    });
#endif

    useSystemAccentColor();
}

NTheme::~NTheme() {}

bool NTheme::isDarkMode() const {
    Q_D(const NTheme);
    return d->_isDark;
}

void NTheme::setThemeMode(NThemeType::ThemeMode mode) {
    Q_D(NTheme);
    if (d->_themeMode != mode) {
        d->_themeMode = mode;
        updateThemeState();
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
        if (mode == NThemeType::ThemeMode::Light) {
            qApp->styleHints()->setColorScheme(Qt::ColorScheme::Light);
        } else if (mode == NThemeType::ThemeMode::Dark) {
            qApp->styleHints()->setColorScheme(Qt::ColorScheme::Dark);
        } else if (mode == NThemeType::ThemeMode::System) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
            qApp->styleHints()->unsetColorScheme();
#else
            Qt::ColorScheme systemScheme = d->detectSystemTheme() ? Qt::ColorScheme::Dark : Qt::ColorScheme::Light;
            qApp->styleHints()->setColorScheme(systemScheme);
#endif
        }
#endif
        emit themeModeChanged(mode);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
void NTheme::setQtColorScheme(Qt::ColorScheme scheme) {
    qApp->styleHints()->setColorScheme(scheme);
    NThemeType::ThemeMode newMode;
    switch (scheme) {
        case Qt::ColorScheme::Light:
            newMode = NThemeType::ThemeMode::Light;
            break;
        case Qt::ColorScheme::Dark:
            newMode = NThemeType::ThemeMode::Dark;
            break;
        default:
            newMode = NThemeType::ThemeMode::System;
            break;
    }

    if (themeMode() != newMode) {
        setThemeMode(newMode);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
void NTheme::unsetQtColorScheme() {
    qApp->styleHints()->unsetColorScheme();
    if (themeMode() != NThemeType::ThemeMode::System) {
        setThemeMode(NThemeType::ThemeMode::System);
    }
}
#endif
#endif

void NTheme::updateThemeState() {
    Q_D(NTheme);

    bool newIsDark = false;
    switch (d->_themeMode) {
        case NThemeType::ThemeMode::Light:
            newIsDark = false;
            break;
        case NThemeType::ThemeMode::Dark:
            newIsDark = true;
            break;
        case NThemeType::ThemeMode::System:
            newIsDark = d_ptr->detectSystemTheme();
            break;
    }

    if (d->_isDark != newIsDark) {
        d->_isDark = newIsDark;
        emit darkModeChanged(newIsDark);
    }
}

QColor NTheme::getSystemAccentColor() const {
    Q_D(const NTheme);
    return d->detectSystemAccentColor();
}

void NTheme::useSystemAccentColor() {
    Q_D(NTheme);
    d->_useSystemAccentColor = true;

    // 获取当前系统强调色
    QColor systemColor = d->detectSystemAccentColor();

    if (systemColor.isValid() && d->_accentColor.normal() != systemColor) {
        setAccentColor(systemColor);
        emit systemAccentColorChanged(systemColor);
    }
}

NThemeType::ThemeMode NTheme::themeMode() const {
    Q_D(const NTheme);
    return d->_themeMode;
}

NAccentColor NTheme::accentColor() const {
    Q_D(const NTheme);
    return d->_accentColor;
}

void NTheme::setAccentColor(const NAccentColor& color) {
    Q_D(NTheme);
    if (d->_accentColor != color) {
        d->_accentColor = color;
        emit accentColorChanged(color);
    }
}

void NTheme::setAccentColor(const QColor& color) { setAccentColor(NColorUtils::toAccentColor(color)); }

QColor NTheme::getColor(NFluentColorKey::Key key) const {
    Q_D(const NTheme);
    return d->resolveColor(key);
}

void NTheme::setColor(NFluentColorKey::Key key, const QColor& color) {
    Q_D(NTheme);
    d->_customColors[key] = color;
}

QColor NTheme::getColorForTheme(NFluentColorKey::Key key, NThemeType::ThemeMode mode) const {
    Q_D(const NTheme);
    bool isDarkForMode = false;
    switch (mode) {
        case NThemeType::ThemeMode::Light:
            isDarkForMode = false;
            break;
        case NThemeType::ThemeMode::Dark:
            isDarkForMode = true;
            break;
        case NThemeType::ThemeMode::System:
            isDarkForMode = d->detectSystemTheme();
            break;
    }
    if (d->_customColors.contains(key)) {
        return d->_customColors[key];
    }
    const QMap<NFluentColorKey::Key, QColor>& themeColors = isDarkForMode ? d->_darkColors : d->_lightColors;
    if (themeColors.contains(key)) {
        return themeColors[key];
    }
    return QColor(128, 128, 128);
}

NAccentColor NTheme::getAccentColor(NAccentColorType::Type type) const {
    if (type == NAccentColorType::Custom) {
        return accentColor();
    }
    return NColors::getAccentColor(type);
}

QColor NTheme::getThemedAccentColor(NAccentColorType::Type type) const {
    Q_D(const NTheme);
    NAccentColor color = getAccentColor(type);
    return color.defaultBrushFor(d->_isDark);
}

QColor NTheme::getAccentColorVariant(NAccentColorType::Type type, const QString& variant) const {
    NAccentColor color = getAccentColor(type);
    return color[variant];
}

QList<NFluentColorKey::Key> NTheme::getAllColorKeys() const {
    QList<NFluentColorKey::Key> keys;
    for (int i = 0; i < NFluentColorKey::Count; i++) {
        keys.append(static_cast<NFluentColorKey::Key>(i));
    }
    return keys;
}

QMap<NFluentColorKey::Key, QColor> NTheme::getAllColors() const {
    Q_D(const NTheme);
    QMap<NFluentColorKey::Key, QColor> result;

    for (int i = 0; i < NFluentColorKey::Count; i++) {
        NFluentColorKey::Key key = static_cast<NFluentColorKey::Key>(i);
        result[key]              = d->resolveColor(key);
    }

    return result;
}

// 泛型设计令牌方法实现
template <typename T>
QVariant NTheme::getToken(const T& key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

template <typename T>
void NTheme::setToken(const T& key, const QVariant& value) {
    // 这里不实现，让特化实现具体功能
}

// 特定类型的设计令牌获取方法
QVariant NTheme::getRadius(NDesignTokenKey::Radius key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

QVariant NTheme::getSpacing(NDesignTokenKey::Spacing key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

QVariant NTheme::getFontSize(NDesignTokenKey::FontSize key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

QVariant NTheme::getFontWeight(NDesignTokenKey::FontWeight key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

QVariant NTheme::getElevation(NDesignTokenKey::Elevation key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

QVariant NTheme::getAnimation(NDesignTokenKey::Animation key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

QVariant NTheme::getEasing(NDesignTokenKey::Easing key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

// 特定类型的设计令牌设置方法 - 需要特化
template <>
void NTheme::setToken(const NDesignTokenKey::Radius& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customRadiusTokens[key] = value;
}

template <>
void NTheme::setToken(const NDesignTokenKey::Spacing& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customSpacingTokens[key] = value;
}

template <>
void NTheme::setToken(const NDesignTokenKey::FontSize& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customFontSizeTokens[key] = value;
}

template <>
void NTheme::setToken(const NDesignTokenKey::FontWeight& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customFontWeightTokens[key] = value;
}

template <>
void NTheme::setToken(const NDesignTokenKey::Elevation& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customElevationTokens[key] = value;
}

template <>
void NTheme::setToken(const NDesignTokenKey::Animation& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customAnimationTokens[key] = value;
}

template <>
void NTheme::setToken(const NDesignTokenKey::Easing& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customEasingTokens[key] = value;
}

void NTheme::drawEffectShadow(QPainter*                  painter,
                              QRect                      widgetRect,
                              int                        shadowBorderWidth,
                              int                        borderRadius,
                              NDesignTokenKey::Elevation elevationKey) {
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    // 获取阴影层级设置
    QVariantMap elevation   = getElevation(elevationKey).toMap();
    int         yOffset     = elevation["yOffset"].toInt();
    QColor      shadowColor = elevation["color"].value<QColor>();

    // 根据 yOffset 调整阴影范围
    int effectiveShadowWidth = qMax(shadowBorderWidth, yOffset);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    // 调整阴影绘制范围以适应 yOffset
    for (int i = 0; i < effectiveShadowWidth; i++) {
        path.addRoundedRect(shadowBorderWidth - i,
                            shadowBorderWidth - i + (yOffset / 4),
                            widgetRect.width() - (shadowBorderWidth - i) * 2,
                            widgetRect.height() - (shadowBorderWidth - i) * 2,
                            borderRadius + i,
                            borderRadius + i);

        // 使用阴影颜色的 alpha 值作为基准，随距离渐变
        int alpha = 1 * (shadowBorderWidth - i + 1);
        shadowColor.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(shadowColor);
        painter->drawPath(path);
    }

    painter->restore();
}

void NTheme::resetToDefaults() {
    Q_D(NTheme);
    d->_customColors.clear();

    // 清理所有自定义令牌
    d->_customRadiusTokens.clear();
    d->_customSpacingTokens.clear();
    d->_customFontSizeTokens.clear();
    d->_customFontWeightTokens.clear();
    d->_customElevationTokens.clear();
    d->_customAnimationTokens.clear();
    d->_customEasingTokens.clear();
}

template QVariant NTheme::getToken<NDesignTokenKey::Radius>(const NDesignTokenKey::Radius& key) const;
template QVariant NTheme::getToken<NDesignTokenKey::Spacing>(const NDesignTokenKey::Spacing& key) const;
template QVariant NTheme::getToken<NDesignTokenKey::FontSize>(const NDesignTokenKey::FontSize& key) const;
template QVariant NTheme::getToken<NDesignTokenKey::FontWeight>(const NDesignTokenKey::FontWeight& key) const;
template QVariant NTheme::getToken<NDesignTokenKey::Elevation>(const NDesignTokenKey::Elevation& key) const;
template QVariant NTheme::getToken<NDesignTokenKey::Animation>(const NDesignTokenKey::Animation& key) const;
template QVariant NTheme::getToken<NDesignTokenKey::Easing>(const NDesignTokenKey::Easing& key) const;
