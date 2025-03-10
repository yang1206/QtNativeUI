#ifndef QTNATIVEUI_NTHEME_P_H
#define QTNATIVEUI_NTHEME_P_H

#include <QColor>
#include <QHash>
#include <QtNativeUI/NTheme.h>

class NTheme::NThemePrivate {
  public:
    NThemePrivate(NTheme* q);
    ~NThemePrivate();

    // 颜色存储
    QHash<NThemeType::ColorRole, QColor> lightColors;
    QHash<NThemeType::ColorRole, QColor> darkColors;
    QHash<NThemeType::ColorRole, QColor> customColors; // 用户自定义颜色

    // 当前主题模式
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark; // 缓存当前是否处于暗色模式

    // 初始化颜色
    void initLightColors();
    void initDarkColors();

    // 检测系统主题变化
    void setupSystemThemeDetection();
    void updateThemeBasedOnSystem();

    // 获取当前模式下的颜色
    QColor currentColor(NThemeType::ColorRole role) const;

    // 更新当前暗色模式状态
    void updateDarkModeState();

    // 使用宏声明公共类
    N_DECLARE_PUBLIC(NTheme)
};

#endif // QTNATIVEUI_NTHEME_P_H