#ifndef QTNATIVEUI_NTHEME_H
#define QTNATIVEUI_NTHEME_H

#include <QObject>
#include <QColor>
#include <QVariant>
#include <QHash>
#include <QList>
#include "NColor.h"
#include "NEnums.h"
#include "stdafx.h"

/**
 * @brief Fluent Design主题管理类
 * 提供对Windows 11 Fluent UI设计系统的访问
 */
class QTNATIVEUI_EXPORT NTheme : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isDarkMode READ isDarkMode NOTIFY darkModeChanged)
    Q_PROPERTY(ThemeMode themeMode READ themeMode WRITE setThemeMode NOTIFY themeModeChanged)
    Q_PROPERTY(NAccentColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

  public:
    static NTheme* instance();

    // 主题模式
    enum class ThemeMode {
        Light,
        Dark,
        System
    };
    Q_ENUM(ThemeMode)

    // 获取/设置主题模式
    bool isDarkMode() const;
    void setDarkMode(bool dark);
    void setThemeMode(ThemeMode mode);
    ThemeMode themeMode() const;

    // 颜色系统
    NAccentColor accentColor() const;
    void setAccentColor(const NAccentColor& color);
    void setAccentColor(const QColor& color);

    // 获取强调色的特定色调
    QColor accentDarkest() const;
    QColor accentDarker() const;
    QColor accentDark() const;
    QColor accentNormal() const;
    QColor accentLight() const;
    QColor accentLighter() const;
    QColor accentLightest() const;
    
    // 获取当前主题下推荐的强调色变体
    QColor accentDefault() const;
    QColor accentSecondary() const;
    QColor accentTertiary() const;
    
    // 获取所有可用的强调色
    QList<NAccentColor> availableAccentColors() const;

    // 颜色获取与设置
    QColor getColor(const QString& key) const;
    void setColor(const QString& key, const QColor& color);

    // 设计令牌
    QVariant getToken(const QString& key) const;
    void setToken(const QString& key, const QVariant& value);

    // 常用令牌快捷访问
    qreal cornerRadius(const QString& key = "cornerRadiusDefault") const;
    int spacing(const QString& key = "spacingM") const;
    int fontSize(const QString& key = "fontSizeBody") const;
    int fontWeight(const QString& key = "fontWeightRegular") const;
    QString elevation(const QString& key = "elevationRest") const;
    int animationDuration(const QString& key = "animationNormal") const;
    QString easingCurve(const QString& key = "easingStandard") const;

    // 重置为默认值
    void resetToDefaults();
    void resetTokensToDefaults();
    void resetColorsToDefaults();

    // 预定义的颜色键
    static const QString TextPrimary;
    static const QString TextSecondary;
    static const QString TextTertiary;
    static const QString TextDisabled;
    static const QString TextOnAccent;
    static const QString TextLink;
    
    static const QString Background;
    static const QString BackgroundSecondary;
    static const QString BackgroundTertiary;
    static const QString Surface;
    static const QString SurfaceSecondary;
    
    static const QString ControlFill;
    static const QString ControlFillHover;
    static const QString ControlFillPressed;
    static const QString ControlFillSelected;
    static const QString ControlFillDisabled;
    
    static const QString ControlStroke;
    static const QString ControlStrokeHover;
    static const QString ControlStrokePressed;
    static const QString ControlStrokeSelected;
    static const QString ControlStrokeDisabled;
    
    static const QString CardBackground;
    static const QString CardBackgroundHover;
    static const QString CardBackgroundPressed;
    static const QString CardStroke;
    static const QString CardStrokeHover;
    
    static const QString Success;
    static const QString Warning;
    static const QString Error;
    static const QString Info;
    
    static const QString Divider;
    static const QString DividerStrong;
    static const QString Shadow;

  signals:
    void colorChanged(const QString& key, const QColor& color);
    void tokenChanged(const QString& key, const QVariant& value);
    void accentColorChanged(const NAccentColor& color);
    void darkModeChanged(bool isDark);
    void themeModeChanged(ThemeMode mode);

  private:
    NTheme(QObject* parent = nullptr);
    ~NTheme();

    // 使用宏创建d指针
    class NThemePrivate;
    N_DECLARE_PRIVATE(NTheme)

    static NTheme* s_instance;
};

// 便捷访问宏
#define N_COLOR(KEY) NTheme::instance()->getColor(KEY)
#define N_TOKEN(KEY) NTheme::instance()->getToken(KEY)
#define N_ACCENT NTheme::instance()->accentColor()
#define N_ACCENT_COLOR(SHADE) NTheme::instance()->accentColor().shade(SHADE)

#endif // QTNATIVEUI_NTHEME_H