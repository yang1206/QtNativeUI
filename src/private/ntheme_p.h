#ifndef QTNATIVEUI_NTHEME_P_H
#define QTNATIVEUI_NTHEME_P_H

#include <QColor>
#include <QHash>
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NColor.h>
#include <QtNativeUI/NFluentColors.h>

class NTheme::NThemePrivate {
  public:
    NThemePrivate(NTheme* q);
    ~NThemePrivate();

    // 主题状态
    NTheme::ThemeMode _themeMode;
    bool _isDark;

    // 强调色
    NAccentColor _accentColor;

    // 颜色存储
    QHash<QString, QColor> _lightColors;
    QHash<QString, QColor> _darkColors;
    QHash<QString, QColor> _customColors;

    // 设计令牌
    QHash<QString, QVariant> _designTokens;
    QHash<QString, QVariant> _customTokens;

    // 初始化方法
    void initLightColors();
    void initDarkColors();
    void initDesignTokens();
    
    // 映射 Fluent 颜色到主题颜色
    void mapFluentColorsToTheme(bool isDark);

    // 颜色解析
    QColor resolveColor(const QString& key) const;

    // 令牌解析
    QVariant resolveToken(const QString& key) const;

    // 系统主题检测
    void setupSystemThemeDetection();
    void updateThemeBasedOnSystem();
    void updateDarkModeState();

    // 使用宏声明公共类
    N_DECLARE_PUBLIC(NTheme)
};

#endif // QTNATIVEUI_NTHEME_P_H