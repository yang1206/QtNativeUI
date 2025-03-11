#ifndef QTNATIVEUI_NCOLOR_H
#define QTNATIVEUI_NCOLOR_H

#include <QColor>
#include <QMap>
#include <QString>
#include "stdafx.h"

/**
 * @brief 带有多个色调的颜色
 * 类似于 Flutter 的 ShadedColor
 */
class QTNATIVEUI_EXPORT NShadedColor {
public:
    NShadedColor(const QColor& primary, const QMap<int, QColor>& swatch);
    
    // 通过色调索引访问颜色
    QColor operator[](int shade) const;
    
    // 获取主色调
    QColor primary() const { return _primary; }
    
private:
    QColor _primary;
    QMap<int, QColor> _swatch;
};

/**
 * @brief 强调色，具有预定义的色调名称
 * 类似于 Flutter 的 AccentColor
 */
class QTNATIVEUI_EXPORT NAccentColor {
public:
    // 从色调映射创建强调色
    NAccentColor(const QMap<QString, QColor>& swatch);
    
    // 从单一颜色创建强调色，自动生成色调
    static NAccentColor fromColor(
        const QColor& color,
        double darkestFactor = 0.38,
        double darkerFactor = 0.30,
        double darkFactor = 0.15,
        double lightFactor = 0.15,
        double lighterFactor = 0.30,
        double lightestFactor = 0.38
    );
    
    // 色调访问方法
    QColor darkest() const;
    QColor darker() const;
    QColor dark() const;
    QColor normal() const;
    QColor light() const;
    QColor lighter() const;
    QColor lightest() const;
    
    // 根据亮度模式获取适当的颜色
    QColor defaultBrushFor(bool isDark) const;
    QColor secondaryBrushFor(bool isDark) const;
    QColor tertiaryBrushFor(bool isDark) const;
    
    // 通过名称访问色调
    QColor operator[](const QString& name) const;
    
    // 插值两个强调色
    static NAccentColor lerp(const NAccentColor& a, const NAccentColor& b, double t);
    
    // 添加比较运算符
    bool operator==(const NAccentColor& other) const;
    bool operator!=(const NAccentColor& other) const;
    
private:
    QMap<QString, QColor> _swatch;
};

/**
 * @brief 预定义的颜色集合
 * 类似于 Flutter 的 Colors 类
 */
class QTNATIVEUI_EXPORT NColors {
public:
    // 基础颜色
    static const QColor transparent;
    static const QColor black;
    static const QColor white;
    
    // 灰色色调
    static const NShadedColor grey;
    
    // 强调色
    static const NAccentColor yellow;
    static const NAccentColor orange;
    static const NAccentColor red;
    static const NAccentColor magenta;
    static const NAccentColor purple;
    static const NAccentColor blue;
    static const NAccentColor teal;
    static const NAccentColor green;
    
    // 状态颜色
    static const QColor warningPrimaryColor;
    static const NAccentColor warningSecondaryColor;
    static const QColor errorPrimaryColor;
    static const NAccentColor errorSecondaryColor;
    static const QColor successPrimaryColor;
    static const NAccentColor successSecondaryColor;
    
    // 所有强调色列表
    static const QList<NAccentColor> accentColors;
};

/**
 * @brief 颜色工具类
 */
class QTNATIVEUI_EXPORT NColorUtils {
public:
    // 基于亮度选择对比色
    static QColor contrastColor(const QColor& color, 
                                const QColor& darkColor = NColors::black,
                                const QColor& lightColor = NColors::white);
    
    // 将颜色与另一个颜色混合
    static QColor lerpColor(const QColor& a, const QColor& b, double t);
    
    // 将颜色转换为 AccentColor
    static NAccentColor toAccentColor(const QColor& color);
    
    // 调整颜色的亮度
    static QColor adjustBrightness(const QColor& color, double factor);
    
    // 创建颜色的变体
    static QColor withAlpha(const QColor& color, int alpha);
    static QColor withOpacity(const QColor& color, double opacity);
};

#endif // QTNATIVEUI_NCOLOR_H 