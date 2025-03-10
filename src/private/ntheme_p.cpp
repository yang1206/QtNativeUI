#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QDateTime>
#include <QScreen>
#include <QStyle>

namespace QtNativeUI {

// 初始化私有实现
NTheme::NThemePrivate::NThemePrivate(NTheme* q)
    : _themeMode(NThemeMode::System)
    , _isDark(false)
    , q_ptr(q)
{
    initLightColors();
    initDarkColors();
    setupAutoModeTimer();
    setupSystemThemeDetection();
    updateDarkModeState();
}

NTheme::NThemePrivate::~NThemePrivate()
{
    autoModeTimer.stop();
}

void NTheme::NThemePrivate::setupAutoModeTimer()
{
    // 设置每小时检查一次
    autoModeTimer.setInterval(60 * 60 * 1000); // 每小时
    QObject::connect(&autoModeTimer, &QTimer::timeout, q_ptr, [this]() {
        if (_themeMode == NThemeMode::Auto) {
            updateThemeBasedOnTime();
        }
    });
    
    // 如果是自动模式，立即启动
    if (_themeMode == NThemeMode::Auto) {
        updateThemeBasedOnTime();
        autoModeTimer.start();
    }
}

void NTheme::NThemePrivate::updateThemeBasedOnTime()
{
    // 根据时间自动切换暗色/亮色模式
    // 默认: 7:00-19:00 为亮色模式, 19:00-7:00 为暗色模式
    QTime currentTime = QTime::currentTime();
    bool shouldBeDark = currentTime < QTime(7, 0) || currentTime >= QTime(19, 0);
    
    if (_isDark != shouldBeDark) {
        _isDark = shouldBeDark;
        Q_EMIT q_ptr->darkModeChanged(_isDark);
        
        // 当模式改变时，发出所有颜色变化的信号
        for (int i = 0; i < static_cast<int>(NColorRole::ColorRoleCount); i++) {
            NColorRole role = static_cast<NColorRole>(i);
            Q_EMIT q_ptr->colorChanged(role, q_ptr->color(role));
        }
    }
}

void NTheme::NThemePrivate::setupSystemThemeDetection()
{
    // 在实际应用中，您需要根据不同平台实现系统主题检测
    // 这里仅提供一个简单的示例
    // 实际实现可能需要使用平台特定API
    
    // 例如，在Windows上可以监听注册表变化
    // 在macOS上可以使用NSAppearance API
    // 在Linux上可以使用GSetting或其他桌面环境提供的API
    
    // 这里简单地检测QPalette来判断系统主题
    QObject::connect(qApp, &QApplication::paletteChanged, q_ptr, [this]() {
        if (_themeMode == NThemeMode::System) {
            updateThemeBasedOnSystem();
        }
    });
}

void NTheme::NThemePrivate::updateThemeBasedOnSystem()
{
    // 简单地通过QPalette颜色判断系统是否为暗色主题
    // 实际应用可能需要更复杂的逻辑或平台特定API
    QPalette palette = QApplication::palette();
    QColor windowColor = palette.color(QPalette::Window);
    QColor textColor = palette.color(QPalette::WindowText);
    
    // 如果窗口颜色的亮度小于文本颜色，则认为是暗色主题
    bool systemIsDark = windowColor.lightness() < textColor.lightness();
    
    if (_isDark != systemIsDark) {
        _isDark = systemIsDark;
        Q_EMIT q_ptr->darkModeChanged(_isDark);
        
        // 当模式改变时，发出所有颜色变化的信号
        for (int i = 0; i < static_cast<int>(NColorRole::ColorRoleCount); i++) {
            NColorRole role = static_cast<NColorRole>(i);
            Q_EMIT q_ptr->colorChanged(role, q_ptr->color(role));
        }
    }
}

void NTheme::NThemePrivate::updateDarkModeState()
{
    bool shouldBeDark = false;
    
    switch (_themeMode) {
    case NThemeMode::Light:
        shouldBeDark = false;
        break;
    case NThemeMode::Dark:
        shouldBeDark = true;
        break;
    case NThemeMode::System:
        updateThemeBasedOnSystem();
        return; // 函数会发出必要的信号
    case NThemeMode::Auto:
        updateThemeBasedOnTime();
        return; // 函数会发出必要的信号
    }
    
    if (_isDark != shouldBeDark) {
        _isDark = shouldBeDark;
        Q_EMIT q_ptr->darkModeChanged(_isDark);
        
        // 当模式改变时，发出所有颜色变化的信号
        for (int i = 0; i < static_cast<int>(NColorRole::ColorRoleCount); i++) {
            NColorRole role = static_cast<NColorRole>(i);
            Q_EMIT q_ptr->colorChanged(role, q_ptr->color(role));
        }
    }
}

// 获取当前模式下的颜色
QColor NTheme::NThemePrivate::currentColor(NColorRole role) const
{
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
void NTheme::NThemePrivate::initLightColors()
{
    // 文本颜色
    lightColors[NColorRole::TextPrimary] = QColor("#242424");     // 主要文本
    lightColors[NColorRole::TextSecondary] = QColor("#616161");   // 次要文本
    lightColors[NColorRole::TextTertiary] = QColor("#8A8A8A");    // 第三级文本
    lightColors[NColorRole::TextDisabled] = QColor("#A6A6A6");    // 禁用文本
    lightColors[NColorRole::TextOnAccent] = QColor("#FFFFFF");    // 强调色上的文本
    lightColors[NColorRole::TextLink] = QColor("#0078D4");        // 链接文本
    
    // 背景颜色
    lightColors[NColorRole::Background] = QColor("#FFFFFF");      // 主背景
    lightColors[NColorRole::BackgroundSecondary] = QColor("#F5F5F5"); // 次级背景
    lightColors[NColorRole::Surface] = QColor("#FFFFFF");         // 表面
    lightColors[NColorRole::SurfaceSecondary] = QColor("#F8F8F8"); // 次级表面
    
    // 强调色
    lightColors[NColorRole::AccentPrimary] = QColor("#0078D4");   // 主要强调色
    lightColors[NColorRole::AccentSecondary] = QColor("#2B88D8"); // 次级强调色
    lightColors[NColorRole::AccentTertiary] = QColor("#C7E0F4");  // 第三级强调色
    
    // 控件颜色
    lightColors[NColorRole::ControlBackground] = QColor("#FFFFFF");              // 控件背景
    lightColors[NColorRole::ControlBackgroundHover] = QColor("#F5F5F5");         // 悬停状态
    lightColors[NColorRole::ControlBackgroundPressed] = QColor("#E0E0E0");       // 按下状态
    lightColors[NColorRole::ControlBackgroundSelected] = QColor("#E1EFFA");      // 选中状态
    lightColors[NColorRole::ControlBackgroundDisabled] = QColor("#F0F0F0");      // 禁用状态
    
    lightColors[NColorRole::ControlBorder] = QColor("#8A8A8A");                  // 控件边框
    lightColors[NColorRole::ControlBorderHover] = QColor("#616161");             // 悬停状态
    lightColors[NColorRole::ControlBorderPressed] = QColor("#424242");           // 按下状态
    lightColors[NColorRole::ControlBorderSelected] = QColor("#0078D4");          // 选中状态
    lightColors[NColorRole::ControlBorderDisabled] = QColor("#C8C8C8");          // 禁用状态
    
    // 状态颜色
    lightColors[NColorRole::Success] = QColor("#107C10");         // 成功
    lightColors[NColorRole::Warning] = QColor("#FFB900");         // 警告
    lightColors[NColorRole::Error] = QColor("#D83B01");           // 错误
    lightColors[NColorRole::Info] = QColor("#0078D4");            // 信息
    
    // 分隔线颜色
    lightColors[NColorRole::Divider] = QColor("#E1E1E1");         // 分隔线
    
    // 阴影颜色
    lightColors[NColorRole::Shadow] = QColor(0, 0, 0, 40);        // 阴影 (16% 透明度)
}

// 初始化暗色主题颜色
void NTheme::NThemePrivate::initDarkColors()
{
    // 文本颜色
    darkColors[NColorRole::TextPrimary] = QColor("#FFFFFF");      // 主要文本
    darkColors[NColorRole::TextSecondary] = QColor("#CCCCCC");    // 次要文本
    darkColors[NColorRole::TextTertiary] = QColor("#9E9E9E");     // 第三级文本
    darkColors[NColorRole::TextDisabled] = QColor("#666666");     // 禁用文本
    darkColors[NColorRole::TextOnAccent] = QColor("#000000");     // 强调色上的文本
    darkColors[NColorRole::TextLink] = QColor("#4CC2FF");         // 链接文本
    
    // 背景颜色
    darkColors[NColorRole::Background] = QColor("#202020");       // 主背景
    darkColors[NColorRole::BackgroundSecondary] = QColor("#292929"); // 次级背景
    darkColors[NColorRole::Surface] = QColor("#2C2C2C");          // 表面
    darkColors[NColorRole::SurfaceSecondary] = QColor("#252525"); // 次级表面
    
    // 强调色
    darkColors[NColorRole::AccentPrimary] = QColor("#4CC2FF");    // 主要强调色
    darkColors[NColorRole::AccentSecondary] = QColor("#82CAFF");  // 次级强调色
    darkColors[NColorRole::AccentTertiary] = QColor("#0F3B5C");   // 第三级强调色
    
    // 控件颜色
    darkColors[NColorRole::ControlBackground] = QColor("#3B3B3B");                // 控件背景
    darkColors[NColorRole::ControlBackgroundHover] = QColor("#4A4A4A");           // 悬停状态
    darkColors[NColorRole::ControlBackgroundPressed] = QColor("#5D5D5D");         // 按下状态
    darkColors[NColorRole::ControlBackgroundSelected] = QColor("#0A3055");        // 选中状态
    darkColors[NColorRole::ControlBackgroundDisabled] = QColor("#333333");        // 禁用状态
    
    darkColors[NColorRole::ControlBorder] = QColor("#707070");                    // 控件边框
    darkColors[NColorRole::ControlBorderHover] = QColor("#8A8A8A");               // 悬停状态
    darkColors[NColorRole::ControlBorderPressed] = QColor("#9E9E9E");             // 按下状态
    darkColors[NColorRole::ControlBorderSelected] = QColor("#4CC2FF");            // 选中状态
    darkColors[NColorRole::ControlBorderDisabled] = QColor("#4A4A4A");            // 禁用状态
    
    // 状态颜色
    darkColors[NColorRole::Success] = QColor("#0AD001");          // 成功
    darkColors[NColorRole::Warning] = QColor("#FFCB66");          // 警告
    darkColors[NColorRole::Error] = QColor("#F05E41");            // 错误
    darkColors[NColorRole::Info] = QColor("#4CC2FF");             // 信息
    
    // 分隔线颜色
    darkColors[NColorRole::Divider] = QColor("#4A4A4A");          // 分隔线
    
    // 阴影颜色
    darkColors[NColorRole::Shadow] = QColor(0, 0, 0, 90);         // 阴影 (35% 透明度)
}

} // namespace QtNativeUI 