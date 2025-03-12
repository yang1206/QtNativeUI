#include <QApplication>
#include <QStyleHints>
#include <QtNativeUI/NTheme.h>
#include "../private/ntheme_p.h"

// 静态实例
NTheme* NTheme::s_instance = nullptr;

// 构造函数
NTheme::NTheme(QObject* parent) : QObject(parent), d_ptr(new NThemePrivate(this)) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    connect(qApp->styleHints(), &QStyleHints::colorSchemeChanged, this, [this](Qt::ColorScheme) {
        if (d_ptr->_themeMode == NThemeType::ThemeMode::System) {
            updateThemeState();
        }
    });
#else
    connect(qApp, &QApplication::paletteChanged, this, [this]() {
        if (d_ptr->_themeMode == ThemeMode::System) {
            updateThemeState();
        }
    });
#endif
}

NTheme::~NTheme() {}

NTheme* NTheme::instance() {
    if (!s_instance) {
        s_instance = new NTheme(qApp);
    }
    return s_instance;
}

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

void NTheme::setAccentColor(const QColor& color) { 
    setAccentColor(NColorUtils::toAccentColor(color)); 
}

QColor NTheme::getColor(NFluentColorKey::Key key) const {
    Q_D(const NTheme);
    return d->resolveColor(key);
}

void NTheme::setColor(NFluentColorKey::Key key, const QColor& color) {
    Q_D(NTheme);
    d->_customColors[key] = color;
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
        result[key] = d->resolveColor(key);
    }
    
    return result;
}

QVariant NTheme::getToken(const QString& key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

void NTheme::setToken(const QString& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customTokens[key] = value;
}

void NTheme::resetToDefaults() {
    Q_D(NTheme);
    d->_customColors.clear();
    d->_customTokens.clear();
}
