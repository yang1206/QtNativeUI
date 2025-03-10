#ifndef QTNATIVEUI_NTHEME_P_H
#define QTNATIVEUI_NTHEME_P_H

#include <QColor>
#include <QHash>
#include <QTimer>
#include <QtNativeUI/NTheme.h>

namespace QtNativeUI {

class NTheme::NThemePrivate
{
public:
    NThemePrivate(NTheme* q);
    ~NThemePrivate();
    
    // 颜色存储
    QHash<NColorRole, QColor> lightColors;
    QHash<NColorRole, QColor> darkColors;
    QHash<NColorRole, QColor> customColors; // 用户自定义颜色
    
    // 当前主题模式
    NThemeMode _themeMode;
    bool _isDark; // 缓存当前是否处于暗色模式
    
    // 初始化颜色
    void initLightColors();
    void initDarkColors();
    
    // 自动模式的定时器
    QTimer autoModeTimer;
    void setupAutoModeTimer();
    void updateThemeBasedOnTime();
    
    // 检测系统主题变化
    void setupSystemThemeDetection();
    void updateThemeBasedOnSystem();
    
    // 获取当前模式下的颜色
    QColor currentColor(NColorRole role) const;
    
    // 更新当前暗色模式状态
    void updateDarkModeState();
    
    // 使用宏声明公共类
    N_DECLARE_PUBLIC(NTheme)
};

} // namespace QtNativeUI

#endif // QTNATIVEUI_NTHEME_P_H 