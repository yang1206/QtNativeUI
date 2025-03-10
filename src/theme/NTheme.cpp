#include <QApplication>
#include <QtNativeUI/NTheme.h>
#include "../private/ntheme_p.h"

// 定义静态颜色键常量
const QString NTheme::TextPrimary   = "textPrimary";
const QString NTheme::TextSecondary = "textSecondary";
const QString NTheme::TextTertiary  = "textTertiary";
const QString NTheme::TextDisabled  = "textDisabled";
const QString NTheme::TextOnAccent  = "textOnAccent";
const QString NTheme::TextLink      = "textLink";

const QString NTheme::Background          = "background";
const QString NTheme::BackgroundSecondary = "backgroundSecondary";
const QString NTheme::BackgroundTertiary  = "backgroundTertiary";
const QString NTheme::Surface             = "surface";
const QString NTheme::SurfaceSecondary    = "surfaceSecondary";

const QString NTheme::ControlFill         = "controlFill";
const QString NTheme::ControlFillHover    = "controlFillHover";
const QString NTheme::ControlFillPressed  = "controlFillPressed";
const QString NTheme::ControlFillSelected = "controlFillSelected";
const QString NTheme::ControlFillDisabled = "controlFillDisabled";

const QString NTheme::ControlStroke         = "controlStroke";
const QString NTheme::ControlStrokeHover    = "controlStrokeHover";
const QString NTheme::ControlStrokePressed  = "controlStrokePressed";
const QString NTheme::ControlStrokeSelected = "controlStrokeSelected";
const QString NTheme::ControlStrokeDisabled = "controlStrokeDisabled";

const QString NTheme::CardBackground        = "cardBackground";
const QString NTheme::CardBackgroundHover   = "cardBackgroundHover";
const QString NTheme::CardBackgroundPressed = "cardBackgroundPressed";
const QString NTheme::CardStroke            = "cardStroke";
const QString NTheme::CardStrokeHover       = "cardStrokeHover";

const QString NTheme::Success = "success";
const QString NTheme::Warning = "warning";
const QString NTheme::Error   = "error";
const QString NTheme::Info    = "info";

const QString NTheme::Divider       = "divider";
const QString NTheme::DividerStrong = "dividerStrong";
const QString NTheme::Shadow        = "shadow";

// 静态实例
NTheme* NTheme::s_instance = nullptr;

// 构造函数
NTheme::NTheme(QObject* parent) : QObject(parent), d_ptr(new NThemePrivate(this)) {}

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

void NTheme::setDarkMode(bool dark) {
    Q_D(NTheme);
    if (d->_isDark != dark) {
        // 设置为固定的明亮/暗黑模式
        d->_themeMode = dark ? ThemeMode::Dark : ThemeMode::Light;
        d->_isDark    = dark;

        emit darkModeChanged(dark);
        emit themeModeChanged(d->_themeMode);

        // 当模式改变时，发出所有颜色变化的信号
        for (auto it = d->_lightColors.constBegin(); it != d->_lightColors.constEnd(); ++it) {
            emit colorChanged(it.key(), getColor(it.key()));
        }
    }
}

void NTheme::setThemeMode(ThemeMode mode) {
    Q_D(NTheme);
    if (d->_themeMode != mode) {
        d->_themeMode = mode;

        if (mode == ThemeMode::System) {
            d->updateThemeBasedOnSystem();
        } else {
            d->updateDarkModeState();
        }

        emit themeModeChanged(mode);
    }
}

NTheme::ThemeMode NTheme::themeMode() const {
    Q_D(const NTheme);
    return d->_themeMode;
}

NAccentColor NTheme::accentColor() const {
    Q_D(const NTheme);
    return d->_accentColor;
}

void NTheme::setAccentColor(const NAccentColor& color) {
    Q_D(NTheme);
    d->_accentColor = color;
    emit accentColorChanged(color);
}

void NTheme::setAccentColor(const QColor& color) { setAccentColor(NAccentColor::fromColor(color)); }

QColor NTheme::getColor(const QString& key) const {
    Q_D(const NTheme);
    return d->resolveColor(key);
}

void NTheme::setColor(const QString& key, const QColor& color) {
    Q_D(NTheme);
    d->_customColors[key] = color;
    emit colorChanged(key, color);
}

QVariant NTheme::getToken(const QString& key) const {
    Q_D(const NTheme);
    return d->resolveToken(key);
}

void NTheme::setToken(const QString& key, const QVariant& value) {
    Q_D(NTheme);
    d->_customTokens[key] = value;
    emit tokenChanged(key, value);
}

qreal NTheme::cornerRadius(const QString& key) const { return getToken(key).toReal(); }

int NTheme::spacing(const QString& key) const { return getToken(key).toInt(); }

int NTheme::fontSize(const QString& key) const { return getToken(key).toInt(); }

int NTheme::fontWeight(const QString& key) const { return getToken(key).toInt(); }

QString NTheme::elevation(const QString& key) const { return getToken(key).toString(); }

int NTheme::animationDuration(const QString& key) const { return getToken(key).toInt(); }

QString NTheme::easingCurve(const QString& key) const { return getToken(key).toString(); }

void NTheme::resetToDefaults() {
    resetColorsToDefaults();
    resetTokensToDefaults();
}

void NTheme::resetTokensToDefaults() {
    Q_D(NTheme);
    QStringList keys = d->_customTokens.keys();
    d->_customTokens.clear();

    // 发出所有已更改令牌的信号
    for (const QString& key : keys) {
        emit tokenChanged(key, getToken(key));
    }
}

void NTheme::resetColorsToDefaults() {
    Q_D(NTheme);
    QStringList keys = d->_customColors.keys();
    d->_customColors.clear();

    // 发出所有已更改颜色的信号
    for (const QString& key : keys) {
        emit colorChanged(key, getColor(key));
    }
}