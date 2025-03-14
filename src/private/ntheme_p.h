#ifndef QTNATIVEUI_NTHEME_P_H
#define QTNATIVEUI_NTHEME_P_H

#include <QColor>
#include <QVariant>
#include <QtNativeUI/NColor.h>
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NTheme.h>

class NTheme;
class NThemePrivate {
    Q_D_CREATE(NTheme)
  public:
    NThemePrivate(NTheme* q);
    ~NThemePrivate();

    // 主题状态
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    // 强调色
    NAccentColor _accentColor;

    // 颜色存储 - 使用枚举键
    QMap<NFluentColorKey::Key, QColor> _lightColors;
    QMap<NFluentColorKey::Key, QColor> _darkColors;
    QMap<NFluentColorKey::Key, QColor> _customColors;

    // 设计令牌
    QHash<NDesignTokenKey::Key, QVariant> _designTokens;
    QHash<NDesignTokenKey::Key, QVariant> _customTokens;

    // 初始化方法
    void initLightColors();
    void initDarkColors();
    void initDesignTokens();

    // 颜色解析
    QColor resolveColor(NFluentColorKey::Key key) const;

    // 令牌解析
    QVariant resolveToken(NDesignTokenKey::Key key) const;
};

#endif // QTNATIVEUI_NTHEME_P_H