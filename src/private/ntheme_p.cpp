#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>
#include <QVariant>
#include <QtNativeUI/NFluentColors.h>

// 初始化私有实现
NTheme::NThemePrivate::NThemePrivate(NTheme* q)
    : _themeMode(NTheme::ThemeMode::System), _isDark(false), _accentColor(NColors::blue), q_ptr(q) {
    // 初始化颜色和设计令牌
    initLightColors();
    initDarkColors();
    initDesignTokens();

    // 设置系统主题检测
    setupSystemThemeDetection();
    updateDarkModeState();
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
    // 使用自动生成的 Fluent 颜色
    for (auto it = LightThemeColors.begin(); it != LightThemeColors.end(); ++it) {
        _lightColors[it.key()] = it.value();
    }
    
    // 映射 Fluent 颜色到主题颜色
    mapFluentColorsToTheme(false);
}

// 初始化暗色主题颜色
void NTheme::NThemePrivate::initDarkColors() {
    // 使用自动生成的 Fluent 颜色
    for (auto it = DarkThemeColors.begin(); it != DarkThemeColors.end(); ++it) {
        _darkColors[it.key()] = it.value();
    }
    
    // 映射 Fluent 颜色到主题颜色
    mapFluentColorsToTheme(true);
}

void NTheme::NThemePrivate::mapFluentColorsToTheme(bool isDark) {
    QHash<QString, QColor>& colors = isDark ? _darkColors : _lightColors;
    
    // 文本颜色
    colors[NTheme::TextPrimary] = colors[NFluentColorConstants::TextFillColorPrimary];
    colors[NTheme::TextSecondary] = colors[NFluentColorConstants::TextFillColorSecondary];
    colors[NTheme::TextTertiary] = colors[NFluentColorConstants::TextFillColorTertiary];
    colors[NTheme::TextDisabled] = colors[NFluentColorConstants::TextFillColorDisabled];
    colors[NTheme::TextOnAccent] = colors[NFluentColorConstants::TextOnAccentFillColorPrimary];
    colors[NTheme::TextLink] = _accentColor.defaultBrushFor(isDark);
    
    // 背景颜色
    colors[NTheme::Background] = colors[NFluentColorConstants::SolidBackgroundFillColorBase];
    colors[NTheme::BackgroundSecondary] = colors[NFluentColorConstants::SolidBackgroundFillColorSecondary];
    colors[NTheme::BackgroundTertiary] = colors[NFluentColorConstants::SolidBackgroundFillColorTertiary];
    colors[NTheme::Surface] = colors[NFluentColorConstants::CardBackgroundFillColorDefault];
    colors[NTheme::SurfaceSecondary] = colors[NFluentColorConstants::CardBackgroundFillColorSecondary];
    
    // 控件填充颜色
    colors[NTheme::ControlFill] = colors[NFluentColorConstants::ControlFillColorDefault];
    colors[NTheme::ControlFillHover] = colors[NFluentColorConstants::ControlFillColorSecondary];
    colors[NTheme::ControlFillPressed] = colors[NFluentColorConstants::ControlFillColorTertiary];
    colors[NTheme::ControlFillSelected] = _accentColor.defaultBrushFor(isDark);
    colors[NTheme::ControlFillDisabled] = colors[NFluentColorConstants::ControlFillColorDisabled];
    
    // 控件描边颜色
    colors[NTheme::ControlStroke] = colors[NFluentColorConstants::ControlStrokeColorDefault];
    colors[NTheme::ControlStrokeHover] = colors[NFluentColorConstants::ControlStrokeColorSecondary];
    colors[NTheme::ControlStrokePressed] = colors[NFluentColorConstants::ControlStrokeColorDefault];
    colors[NTheme::ControlStrokeSelected] = _accentColor.defaultBrushFor(isDark);
    colors[NTheme::ControlStrokeDisabled] = colors[NFluentColorConstants::ControlStrongStrokeColorDisabled];
    
    // 卡片颜色
    colors[NTheme::CardBackground] = colors[NFluentColorConstants::CardBackgroundFillColorDefault];
    colors[NTheme::CardBackgroundHover] = colors[NFluentColorConstants::CardBackgroundFillColorSecondary];
    colors[NTheme::CardBackgroundPressed] = colors[NFluentColorConstants::LayerFillColorDefault];
    colors[NTheme::CardStroke] = colors[NFluentColorConstants::CardStrokeColorDefault];
    colors[NTheme::CardStrokeHover] = colors[NFluentColorConstants::ControlStrokeColorSecondary];
    
    // 状态颜色
    colors[NTheme::Success] = colors[NFluentColorConstants::SystemFillColorSuccess];
    colors[NTheme::Warning] = colors[NFluentColorConstants::SystemFillColorCaution];
    colors[NTheme::Error] = colors[NFluentColorConstants::SystemFillColorCritical];
    colors[NTheme::Info] = colors[NFluentColorConstants::SystemFillColorNeutral];
    
    // 其他颜色
    colors[NTheme::Divider] = colors[NFluentColorConstants::DividerStrokeColorDefault];
    colors[NTheme::DividerStrong] = colors[NFluentColorConstants::ControlStrongStrokeColorDefault];
    colors[NTheme::Shadow] = colors[NFluentColorConstants::SmokeFillColorDefault];
}

// 解析颜色 - 考虑当前主题模式和自定义颜色
QColor NTheme::NThemePrivate::resolveColor(const QString& key) const {
    // 首先检查自定义颜色
    if (_customColors.contains(key)) {
        return _customColors[key];
    }

    // 然后根据当前主题模式选择颜色
    if (_isDark) {
        if (_darkColors.contains(key)) {
            return _darkColors[key];
        }
    } else {
        if (_lightColors.contains(key)) {
            return _lightColors[key];
        }
    }

    // 如果找不到颜色，返回默认颜色
    return _isDark ? QColor("#FFFFFF") : QColor("#000000");
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

// 设置系统主题检测
void NTheme::NThemePrivate::setupSystemThemeDetection() {
    // 连接应用程序调色板变化信号
    QObject::connect(qApp, &QApplication::paletteChanged, [this]() {
        if (_themeMode == NTheme::ThemeMode::System) {
            updateThemeBasedOnSystem();
        }
    });

    // 初始检测
    updateThemeBasedOnSystem();
}

// 根据系统主题更新
void NTheme::NThemePrivate::updateThemeBasedOnSystem() {
    if (_themeMode == NTheme::ThemeMode::System) {
        // 检测系统主题
        QPalette palette    = qApp->palette();
        QColor   windowText = palette.color(QPalette::WindowText);
        QColor   window     = palette.color(QPalette::Window);

        // 如果窗口文本比窗口背景亮，则认为是暗色主题
        bool isDark = windowText.lightness() > window.lightness();

        if (_isDark != isDark) {
            _isDark = isDark;
            Q_Q(NTheme);
            emit q->darkModeChanged(_isDark);

            // 发出所有颜色变化的信号
            for (auto it = _lightColors.constBegin(); it != _lightColors.constEnd(); ++it) {
                emit q->colorChanged(it.key(), q->getColor(it.key()));
            }
        }
    }
}

// 更新暗色模式状态
void NTheme::NThemePrivate::updateDarkModeState() {
    bool newIsDark = (_themeMode == NTheme::ThemeMode::Dark);

    if (_isDark != newIsDark) {
        _isDark = newIsDark;
        Q_Q(NTheme);
        emit q->darkModeChanged(_isDark);

        // 发出所有颜色变化的信号
        for (auto it = _lightColors.constBegin(); it != _lightColors.constEnd(); ++it) {
            emit q->colorChanged(it.key(), q->getColor(it.key()));
        }
    }
}