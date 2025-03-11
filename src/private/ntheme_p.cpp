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
    // 确保颜色哈希表先初始化
    initLightColors();
    initDarkColors();
    initDesignTokens();
    
    // 然后再检测系统主题
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
    colors[NTheme::TextPrimary]   = colors[NFluentColorConstants::TextFillColorPrimary];
    colors[NTheme::TextSecondary] = colors[NFluentColorConstants::TextFillColorSecondary];
    colors[NTheme::TextTertiary]  = colors[NFluentColorConstants::TextFillColorTertiary];
    colors[NTheme::TextDisabled]  = colors[NFluentColorConstants::TextFillColorDisabled];
    colors[NTheme::TextOnAccent]  = colors[NFluentColorConstants::TextOnAccentFillColorPrimary];
    colors[NTheme::TextLink]      = _accentColor.defaultBrushFor(isDark);

    // 背景颜色
    colors[NTheme::Background]          = colors[NFluentColorConstants::SolidBackgroundFillColorBase];
    colors[NTheme::BackgroundSecondary] = colors[NFluentColorConstants::SolidBackgroundFillColorSecondary];
    colors[NTheme::BackgroundTertiary]  = colors[NFluentColorConstants::SolidBackgroundFillColorTertiary];
    colors[NTheme::Surface]             = colors[NFluentColorConstants::CardBackgroundFillColorDefault];
    colors[NTheme::SurfaceSecondary]    = colors[NFluentColorConstants::CardBackgroundFillColorSecondary];

    // 控件填充颜色
    colors[NTheme::ControlFill]         = colors[NFluentColorConstants::ControlFillColorDefault];
    colors[NTheme::ControlFillHover]    = colors[NFluentColorConstants::ControlFillColorSecondary];
    colors[NTheme::ControlFillPressed]  = colors[NFluentColorConstants::ControlFillColorTertiary];
    colors[NTheme::ControlFillSelected] = _accentColor.defaultBrushFor(isDark);
    colors[NTheme::ControlFillDisabled] = colors[NFluentColorConstants::ControlFillColorDisabled];

    // 控件描边颜色
    colors[NTheme::ControlStroke]         = colors[NFluentColorConstants::ControlStrokeColorDefault];
    colors[NTheme::ControlStrokeHover]    = colors[NFluentColorConstants::ControlStrokeColorSecondary];
    colors[NTheme::ControlStrokePressed]  = colors[NFluentColorConstants::ControlStrokeColorDefault];
    colors[NTheme::ControlStrokeSelected] = _accentColor.defaultBrushFor(isDark);
    colors[NTheme::ControlStrokeDisabled] = colors[NFluentColorConstants::ControlStrongStrokeColorDisabled];

    // 卡片颜色
    colors[NTheme::CardBackground]        = colors[NFluentColorConstants::CardBackgroundFillColorDefault];
    colors[NTheme::CardBackgroundHover]   = colors[NFluentColorConstants::CardBackgroundFillColorSecondary];
    colors[NTheme::CardBackgroundPressed] = colors[NFluentColorConstants::LayerFillColorDefault];
    colors[NTheme::CardStroke]            = colors[NFluentColorConstants::CardStrokeColorDefault];
    colors[NTheme::CardStrokeHover]       = colors[NFluentColorConstants::ControlStrokeColorSecondary];

    // 状态颜色
    colors[NTheme::Success] = colors[NFluentColorConstants::SystemFillColorSuccess];
    colors[NTheme::Warning] = colors[NFluentColorConstants::SystemFillColorCaution];
    colors[NTheme::Error]   = colors[NFluentColorConstants::SystemFillColorCritical];
    colors[NTheme::Info]    = colors[NFluentColorConstants::SystemFillColorNeutral];

    // 其他颜色
    colors[NTheme::Divider]       = colors[NFluentColorConstants::DividerStrokeColorDefault];
    colors[NTheme::DividerStrong] = colors[NFluentColorConstants::ControlStrongStrokeColorDefault];
    colors[NTheme::Shadow]        = colors[NFluentColorConstants::SmokeFillColorDefault];

    // 强调色相关颜色
    colors["accent"]          = _accentColor.normal();
    colors["accentDark"]      = _accentColor.dark();
    colors["accentLight"]     = _accentColor.light();
    colors["accentDefault"]   = _accentColor.defaultBrushFor(isDark);
    colors["accentSecondary"] = _accentColor.secondaryBrushFor(isDark);
    colors["accentTertiary"]  = _accentColor.tertiaryBrushFor(isDark);
}

// 解析颜色 - 考虑当前主题模式和自定义颜色
QColor NTheme::NThemePrivate::resolveColor(const QString& key) const {
    // 首先检查自定义颜色
    if (!key.isEmpty() && _customColors.contains(key)) {
        return _customColors[key];
    }

    // 然后根据当前主题模式选择颜色
    const QHash<QString, QColor>& themeColors = _isDark ? _darkColors : _lightColors;
    if (!key.isEmpty() && themeColors.contains(key)) {
        return themeColors[key];
    }

    // 处理强调色相关的特殊键
    if (!key.isEmpty() && key.startsWith("accent")) {
        if (key == "accent") {
            return _accentColor.normal();
        } else if (key == "accentDarkest") {
            return _accentColor.darkest();
        } else if (key == "accentDarker") {
            return _accentColor.darker();
        } else if (key == "accentDark") {
            return _accentColor.dark();
        } else if (key == "accentLight") {
            return _accentColor.light();
        } else if (key == "accentLighter") {
            return _accentColor.lighter();
        } else if (key == "accentLightest") {
            return _accentColor.lightest();
        } else if (key == "accentDefault") {
            return _accentColor.defaultBrushFor(_isDark);
        } else if (key == "accentSecondary") {
            return _accentColor.secondaryBrushFor(_isDark);
        } else if (key == "accentTertiary") {
            return _accentColor.tertiaryBrushFor(_isDark);
        }
    }

    // 返回安全的默认颜色而不是崩溃
    return QColor(128, 128, 128); // 返回中性灰色作为默认值
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
    // 确保颜色哈希表已初始化
    if (_lightColors.isEmpty()) initLightColors();
    if (_darkColors.isEmpty()) initDarkColors();
    
    // 连接应用程序调色板变化信号
    QObject::connect(qApp, &QApplication::paletteChanged, [this]() {
        if (_themeMode == NTheme::ThemeMode::System) {
            updateThemeBasedOnSystem();
        }
    });

    // 安全的初始检测
    try {
        updateThemeBasedOnSystem();
    } catch (...) {
        // 出现异常则默认为亮色模式
        _isDark = false;
        Q_Q(NTheme);
        emit q->darkModeChanged(_isDark);
    }
}

// 根据系统主题更新
void NTheme::NThemePrivate::updateThemeBasedOnSystem() {
    if (_themeMode == NTheme::ThemeMode::System) {
        // 获取系统调色板
        QPalette pal = QApplication::palette();
        
        // 通过调色板的基色来判断是否为深色主题
        QColor windowColor = pal.color(QPalette::Window);
        int brightness = (windowColor.red() + windowColor.green() + windowColor.blue()) / 3;
        
        bool newIsDark = brightness < 128; // 简单的亮度检测
        
        if (_isDark != newIsDark) {
            _isDark = newIsDark;
            Q_Q(NTheme);
            emit q->darkModeChanged(_isDark);
            
            // 重要：确保主题色表已经被初始化
            if (_lightColors.isEmpty()) initLightColors();
            if (_darkColors.isEmpty()) initDarkColors();
            
            // 发出所有颜色变化的信号 - 使用安全的方法
            QHash<QString, QColor> const & colors = _isDark ? _darkColors : _lightColors;
            for (auto it = colors.constBegin(); it != colors.constEnd(); ++it) {
                emit q->colorChanged(it.key(), it.value());
            }
        }
    }
}

// 更新暗色模式状态
void NTheme::NThemePrivate::updateDarkModeState() {
    bool newIsDark = false;
    
    switch (_themeMode) {
        case NTheme::ThemeMode::Light:
            newIsDark = false;
            break;
        case NTheme::ThemeMode::Dark:
            newIsDark = true;
            break;
        case NTheme::ThemeMode::System:
            // 获取系统主题
            {
                QPalette pal = QApplication::palette();
                QColor windowColor = pal.color(QPalette::Window);
                int brightness = (windowColor.red() + windowColor.green() + windowColor.blue()) / 3;
                newIsDark = brightness < 128;
            }
            break;
    }
    
    if (_isDark != newIsDark) {
        _isDark = newIsDark;
        Q_Q(NTheme);
        emit q->darkModeChanged(_isDark);
        
        // 确保颜色哈希表已初始化
        if (_lightColors.isEmpty()) initLightColors();
        if (_darkColors.isEmpty()) initDarkColors();
        
        // 安全地发送颜色变化信号
        const QHash<QString, QColor>& colors = _isDark ? _darkColors : _lightColors;
        for (auto it = colors.constBegin(); it != colors.constEnd(); ++it) {
            emit q->colorChanged(it.key(), it.value());
        }
    }
}

void NTheme::NThemePrivate::updateAccentDependentColors() {
    // 更新依赖于强调色的颜色
    QHash<QString, QColor>& colors = _isDark ? _darkColors : _lightColors;

    // 文本链接颜色
    colors[NTheme::TextLink] = _accentColor.defaultBrushFor(_isDark);

    // 控件选中状态颜色
    colors[NTheme::ControlFillSelected]   = _accentColor.defaultBrushFor(_isDark);
    colors[NTheme::ControlStrokeSelected] = _accentColor.defaultBrushFor(_isDark);

    // 发出颜色变化信号
    Q_Q(NTheme);
    emit q->colorChanged(NTheme::TextLink, colors[NTheme::TextLink]);
    emit q->colorChanged(NTheme::ControlFillSelected, colors[NTheme::ControlFillSelected]);
    emit q->colorChanged(NTheme::ControlStrokeSelected, colors[NTheme::ControlStrokeSelected]);
}