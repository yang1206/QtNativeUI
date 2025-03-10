#include <QApplication>
#include <QtNativeUI/NTheme.h>
#include "../private/ntheme_p.h"

namespace QtNativeUI {

// 静态实例
NTheme* NTheme::s_instance = nullptr;

// 构造函数，使用新的宏模式
NTheme::NTheme(NThemePrivate& dd, QObject* parent)
    : QObject(parent), d_ptr(&dd)
{
}

// 标准构造函数
NTheme::NTheme(QObject* parent)
    : QObject(parent), d_ptr(new NThemePrivate(this))
{
}

NTheme::~NTheme()
{
    // QScopedPointer会自动删除d_ptr
}

NTheme* NTheme::instance()
{
    if (!s_instance) {
        s_instance = new NTheme(qApp);
    }
    return s_instance;
}

QColor NTheme::color(NColorRole role) const
{
    Q_D(const NTheme);
    return d->currentColor(role);
}

void NTheme::setColor(NColorRole role, const QColor& color)
{
    Q_D(NTheme);
    d->customColors[role] = color;
    emit colorChanged(role, color);
}

bool NTheme::isDarkMode() const
{
    Q_D(const NTheme);
    return d->_isDark;
}

void NTheme::setDarkMode(bool dark)
{
    Q_D(NTheme);
    if (d->_isDark != dark) {
        // 设置为固定的明亮/暗黑模式
        d->_themeMode = dark ? NThemeMode::Dark : NThemeMode::Light;
        d->_isDark = dark;
        
        // 如果是自动模式，停止定时器
        if (d->autoModeTimer.isActive()) {
            d->autoModeTimer.stop();
        }
        
        emit darkModeChanged(dark);
        emit themeModeChanged(d->_themeMode);
        
        // 当模式改变时，发出所有颜色变化的信号
        for (int i = 0; i < static_cast<int>(NColorRole::ColorRoleCount); i++) {
            NColorRole role = static_cast<NColorRole>(i);
            emit colorChanged(role, color(role));
        }
    }
}

void NTheme::setThemeMode(NThemeMode mode)
{
    Q_D(NTheme);
    if (d->_themeMode != mode) {
        d->_themeMode = mode;
        
        // 根据模式更新定时器
        if (mode == NThemeMode::Auto) {
            d->updateThemeBasedOnTime();
            if (!d->autoModeTimer.isActive()) {
                d->autoModeTimer.start();
            }
        } else {
            if (d->autoModeTimer.isActive()) {
                d->autoModeTimer.stop();
            }
            
            if (mode == NThemeMode::System) {
                d->updateThemeBasedOnSystem();
            } else {
                d->updateDarkModeState();
            }
        }
        
        emit themeModeChanged(mode);
    }
}

NThemeMode NTheme::themeMode() const
{
    Q_D(const NTheme);
    return d->_themeMode;
}

void NTheme::resetToDefaults()
{
    Q_D(NTheme);
    d->customColors.clear();
    
    // 发出所有颜色变化的信号
    for (int i = 0; i < static_cast<int>(NColorRole::ColorRoleCount); i++) {
        NColorRole role = static_cast<NColorRole>(i);
        emit colorChanged(role, color(role));
    }
}

} // namespace QtNativeUI 