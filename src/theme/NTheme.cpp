#include <QApplication>
#include <QPainterPath>
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
#else
    connect(qApp, &QApplication::paletteChanged, this, [this]() {
        updateThemeState();
        if (d_ptr->_themeMode == ThemeMode::System) {
            emit themeModeChanged(d_ptr->_themeMode);
        }
    });
#endif
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
        emit themeModeChanged(mode);
    }
}

void NTheme::updateThemeState() {
    Q_D(NTheme);

    // 根据当前主题模式更新暗色模式状态
    bool newIsDark = false;
    switch (d->_themeMode) {
        case NThemeType::ThemeMode::Light:
            newIsDark = false;
            break;
        case NThemeType::ThemeMode::Dark:
            newIsDark = true;
            break;
        case NThemeType::ThemeMode::System:
            newIsDark = detectSystemTheme();
            break;
    }

    // 仅在状态变化时发出信号
    if (d->_isDark != newIsDark) {
        d->_isDark = newIsDark;
        emit darkModeChanged(newIsDark);
    }
}

bool NTheme::detectSystemTheme() const {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    return qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#else
    QPalette pal         = QApplication::palette();
    QColor   windowColor = pal.color(QPalette::Window);
    QColor   textColor   = pal.color(QPalette::WindowText);

    // 比较文本与背景的相对亮度
    bool isDark = textColor.lightness() > windowColor.lightness();

    // 备选方法
    if (!isDark) {
        int brightness = (windowColor.red() + windowColor.green() + windowColor.blue()) / 3;
        isDark         = brightness < 128;
    }

    return isDark;
#endif
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
        // 我们不再调用updateAccentDependentColors，因为它现在是空的
        // 或者可以保留调用，以备将来扩展
        d->updateAccentDependentColors();
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

    // 保存当前主题模式
    NThemeType::ThemeMode originalMode   = d->_themeMode;
    bool                  originalIsDark = d->_isDark;

    // 临时修改内部状态以获取指定主题模式的颜色
    const_cast<NThemePrivate*>(d)->_themeMode = mode;

    // 根据指定的主题模式设置暗色状态
    bool isDarkForMode = false;
    switch (mode) {
        case NThemeType::ThemeMode::Light:
            isDarkForMode = false;
            break;
        case NThemeType::ThemeMode::Dark:
            isDarkForMode = true;
            break;
        case NThemeType::ThemeMode::System:
            isDarkForMode = detectSystemTheme();
            break;
    }
    const_cast<NThemePrivate*>(d)->_isDark = isDarkForMode;

    // 获取指定主题模式下的颜色
    QColor result = d->resolveColor(key);

    // 恢复原始主题模式
    const_cast<NThemePrivate*>(d)->_themeMode = originalMode;
    const_cast<NThemePrivate*>(d)->_isDark    = originalIsDark;

    return result;
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
    // 返回所有有效的颜色键（排除Count）
    QList<NFluentColorKey::Key> keys;
    for (int i = 0; i < NFluentColorKey::Count; i++) {
        keys.append(static_cast<NFluentColorKey::Key>(i));
    }
    return keys;
}

QMap<NFluentColorKey::Key, QColor> NTheme::getAllColors() const {
    Q_D(const NTheme);
    QMap<NFluentColorKey::Key, QColor> result;

    // 遍历所有颜色键
    for (int i = 0; i < NFluentColorKey::Count; i++) {
        NFluentColorKey::Key key = static_cast<NFluentColorKey::Key>(i);
        result[key]              = d->resolveColor(key);
    }

    return result;
}

QVariant NTheme::getToken(NDesignTokenKey::Key key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

void NTheme::setToken(NDesignTokenKey::Key key, const QVariant& value) {
    Q_D(NTheme);
    d->_customTokens[key] = value;
}

void NTheme::drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius) {
    Q_D(NTheme);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = d->_themeMode == NThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
    for (int i = 0; i < shadowBorderWidth; i++) {
        path.addRoundedRect(shadowBorderWidth - i,
                            shadowBorderWidth - i,
                            widgetRect.width() - (shadowBorderWidth - i) * 2,
                            widgetRect.height() - (shadowBorderWidth - i) * 2,
                            borderRadius + i,
                            borderRadius + i);
        int alpha = 1 * (shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(color);
        painter->drawPath(path);
    }
    painter->restore();
}

void NTheme::resetToDefaults() {
    Q_D(NTheme);
    d->_customColors.clear();
    d->_customTokens.clear();
}
