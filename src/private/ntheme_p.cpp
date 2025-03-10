#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>

// 初始化私有实现
NTheme::NThemePrivate::NThemePrivate(NTheme* q) : _themeMode(NThemeType::ThemeMode::System), _isDark(false), q_ptr(q) {
    initLightColors();
    initDarkColors();
    setupSystemThemeDetection();
    updateDarkModeState();
}

NTheme::NThemePrivate::~NThemePrivate() {}

void NTheme::NThemePrivate::setupSystemThemeDetection() {
    // 在实际应用中，您需要根据不同平台实现系统主题检测
    // 这里仅提供一个简单的示例
    // 实际实现可能需要使用平台特定API

    // 例如，在Windows上可以监听注册表变化
    // 在macOS上可以使用NSAppearance API
    // 在Linux上可以使用GSetting或其他桌面环境提供的API

    // 这里简单地检测QPalette来判断系统主题
    // QObject::connect(qApp, &QApplication::paletteChanged, q_ptr, [this]() {
    //     if (_themeMode == NThemeType::ThemeMode::System) {
    //         updateThemeBasedOnSystem();
    //     }
    // });
}

void NTheme::NThemePrivate::updateThemeBasedOnSystem() {
    // 简单地通过QPalette颜色判断系统是否为暗色主题
    // 实际应用可能需要更复杂的逻辑或平台特定API
    QPalette palette     = QApplication::palette();
    QColor   windowColor = palette.color(QPalette::Window);
    QColor   textColor   = palette.color(QPalette::WindowText);

    // 如果窗口颜色的亮度小于文本颜色，则认为是暗色主题
    bool systemIsDark = windowColor.lightness() < textColor.lightness();

    if (_isDark != systemIsDark) {
        _isDark = systemIsDark;
        Q_EMIT q_ptr->darkModeChanged(_isDark);

        // 当模式改变时，发出所有颜色变化的信号
        for (int i = 0; i < static_cast<int>(NThemeType::ColorRole::ColorRoleCount); i++) {
            NThemeType::ColorRole role = static_cast<NThemeType::ColorRole>(i);
            Q_EMIT q_ptr->colorChanged(role, q_ptr->color(role));
        }
    }
}

void NTheme::NThemePrivate::updateDarkModeState() {
    bool shouldBeDark = false;

    switch (_themeMode) {
        case NThemeType::ThemeMode::Light:
            shouldBeDark = false;
            break;
        case NThemeType::ThemeMode::Dark:
            shouldBeDark = true;
            break;
        case NThemeType::ThemeMode::System:
            updateThemeBasedOnSystem();
            return; // 函数会发出必要的信号
    }

    if (_isDark != shouldBeDark) {
        _isDark = shouldBeDark;
        Q_EMIT q_ptr->darkModeChanged(_isDark);

        // 当模式改变时，发出所有颜色变化的信号
        for (int i = 0; i < static_cast<int>(NThemeType::ColorRole::ColorRoleCount); i++) {
            NThemeType::ColorRole role = static_cast<NThemeType::ColorRole>(i);
            Q_EMIT q_ptr->colorChanged(role, q_ptr->color(role));
        }
    }
}

// 获取当前模式下的颜色
QColor NTheme::NThemePrivate::currentColor(NThemeType::ColorRole role) const {
    // 优先使用自定义颜色
    if (customColors.contains(role)) {
        return customColors[role];
    }

    // 然后根据当前模式选择颜色
    if (_isDark) {
        return darkColors.value(role, Qt::white);
    } else {
        return lightColors.value(role, Qt::black);
    }
}

// 初始化亮色主题颜色
void NTheme::NThemePrivate::initLightColors() {
    // 文本颜色
    lightColors[NThemeType::ColorRole::TextPrimary]   = QColor("#242424"); // 主要文本
    lightColors[NThemeType::ColorRole::TextSecondary] = QColor("#616161"); // 次要文本
    lightColors[NThemeType::ColorRole::TextTertiary]  = QColor("#8A8A8A"); // 第三级文本
    lightColors[NThemeType::ColorRole::TextDisabled]  = QColor("#A6A6A6"); // 禁用文本
    lightColors[NThemeType::ColorRole::TextOnAccent]  = QColor("#FFFFFF"); // 强调色上的文本
    lightColors[NThemeType::ColorRole::TextLink]      = QColor("#0078D4"); // 链接文本

    // 背景颜色
    lightColors[NThemeType::ColorRole::Background]          = QColor("#FFFFFF"); // 主背景
    lightColors[NThemeType::ColorRole::BackgroundSecondary] = QColor("#F5F5F5"); // 次级背景
    lightColors[NThemeType::ColorRole::Surface]             = QColor("#FFFFFF"); // 表面
    lightColors[NThemeType::ColorRole::SurfaceSecondary]    = QColor("#F8F8F8"); // 次级表面

    // 强调色
    lightColors[NThemeType::ColorRole::AccentPrimary]   = QColor("#0078D4"); // 主要强调色
    lightColors[NThemeType::ColorRole::AccentSecondary] = QColor("#2B88D8"); // 次级强调色
    lightColors[NThemeType::ColorRole::AccentTertiary]  = QColor("#C7E0F4"); // 第三级强调色

    // 控件颜色
    lightColors[NThemeType::ColorRole::ControlBackground]         = QColor("#FFFFFF"); // 控件背景
    lightColors[NThemeType::ColorRole::ControlBackgroundHover]    = QColor("#F5F5F5"); // 悬停状态
    lightColors[NThemeType::ColorRole::ControlBackgroundPressed]  = QColor("#E0E0E0"); // 按下状态
    lightColors[NThemeType::ColorRole::ControlBackgroundSelected] = QColor("#E1EFFA"); // 选中状态
    lightColors[NThemeType::ColorRole::ControlBackgroundDisabled] = QColor("#F0F0F0"); // 禁用状态

    lightColors[NThemeType::ColorRole::ControlBorder]         = QColor("#8A8A8A"); // 控件边框
    lightColors[NThemeType::ColorRole::ControlBorderHover]    = QColor("#616161"); // 悬停状态
    lightColors[NThemeType::ColorRole::ControlBorderPressed]  = QColor("#424242"); // 按下状态
    lightColors[NThemeType::ColorRole::ControlBorderSelected] = QColor("#0078D4"); // 选中状态
    lightColors[NThemeType::ColorRole::ControlBorderDisabled] = QColor("#C8C8C8"); // 禁用状态

    // 状态颜色
    lightColors[NThemeType::ColorRole::Success] = QColor("#107C10"); // 成功
    lightColors[NThemeType::ColorRole::Warning] = QColor("#FFB900"); // 警告
    lightColors[NThemeType::ColorRole::Error]   = QColor("#D83B01"); // 错误
    lightColors[NThemeType::ColorRole::Info]    = QColor("#0078D4"); // 信息

    // 分隔线颜色
    lightColors[NThemeType::ColorRole::Divider] = QColor("#E1E1E1"); // 分隔线

    // 阴影颜色
    lightColors[NThemeType::ColorRole::Shadow] = QColor(0, 0, 0, 40); // 阴影 (16% 透明度)
}

// 初始化暗色主题颜色
void NTheme::NThemePrivate::initDarkColors() {
    // 文本颜色
    darkColors[NThemeType::ColorRole::TextPrimary]   = QColor("#FFFFFF"); // 主要文本
    darkColors[NThemeType::ColorRole::TextSecondary] = QColor("#CCCCCC"); // 次要文本
    darkColors[NThemeType::ColorRole::TextTertiary]  = QColor("#9E9E9E"); // 第三级文本
    darkColors[NThemeType::ColorRole::TextDisabled]  = QColor("#666666"); // 禁用文本
    darkColors[NThemeType::ColorRole::TextOnAccent]  = QColor("#000000"); // 强调色上的文本
    darkColors[NThemeType::ColorRole::TextLink]      = QColor("#4CC2FF"); // 链接文本

    // 背景颜色
    darkColors[NThemeType::ColorRole::Background]          = QColor("#202020"); // 主背景
    darkColors[NThemeType::ColorRole::BackgroundSecondary] = QColor("#292929"); // 次级背景
    darkColors[NThemeType::ColorRole::Surface]             = QColor("#2C2C2C"); // 表面
    darkColors[NThemeType::ColorRole::SurfaceSecondary]    = QColor("#252525"); // 次级表面

    // 强调色
    darkColors[NThemeType::ColorRole::AccentPrimary]   = QColor("#4CC2FF"); // 主要强调色
    darkColors[NThemeType::ColorRole::AccentSecondary] = QColor("#82CAFF"); // 次级强调色
    darkColors[NThemeType::ColorRole::AccentTertiary]  = QColor("#0F3B5C"); // 第三级强调色

    // 控件颜色
    darkColors[NThemeType::ColorRole::ControlBackground]         = QColor("#3B3B3B"); // 控件背景
    darkColors[NThemeType::ColorRole::ControlBackgroundHover]    = QColor("#4A4A4A"); // 悬停状态
    darkColors[NThemeType::ColorRole::ControlBackgroundPressed]  = QColor("#5D5D5D"); // 按下状态
    darkColors[NThemeType::ColorRole::ControlBackgroundSelected] = QColor("#0A3055"); // 选中状态
    darkColors[NThemeType::ColorRole::ControlBackgroundDisabled] = QColor("#333333"); // 禁用状态

    darkColors[NThemeType::ColorRole::ControlBorder]         = QColor("#707070"); // 控件边框
    darkColors[NThemeType::ColorRole::ControlBorderHover]    = QColor("#8A8A8A"); // 悬停状态
    darkColors[NThemeType::ColorRole::ControlBorderPressed]  = QColor("#9E9E9E"); // 按下状态
    darkColors[NThemeType::ColorRole::ControlBorderSelected] = QColor("#4CC2FF"); // 选中状态
    darkColors[NThemeType::ColorRole::ControlBorderDisabled] = QColor("#4A4A4A"); // 禁用状态

    // 状态颜色
    darkColors[NThemeType::ColorRole::Success] = QColor("#0AD001"); // 成功
    darkColors[NThemeType::ColorRole::Warning] = QColor("#FFCB66"); // 警告
    darkColors[NThemeType::ColorRole::Error]   = QColor("#F05E41"); // 错误
    darkColors[NThemeType::ColorRole::Info]    = QColor("#4CC2FF"); // 信息

    // 分隔线颜色
    darkColors[NThemeType::ColorRole::Divider] = QColor("#4A4A4A"); // 分隔线

    // 阴影颜色
    darkColors[NThemeType::ColorRole::Shadow] = QColor(0, 0, 0, 90); // 阴影 (35% 透明度)
}