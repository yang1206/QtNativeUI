// 自动生成的文件，请勿手动修改
// 从Microsoft Fluent Design颜色资源生成

#ifndef NFLUENTCOLORS_H
#define NFLUENTCOLORS_H

#include <QColor>
#include <QString>
#include <QMap>
#include <QMetaEnum>

// 辅助函数：创建带有alpha值的颜色
inline QColor colorWithAlpha(const QString& hex, int alpha) {
    QColor color(hex);
    color.setAlpha(alpha);
    return color;
}

// Fluent Design 颜色枚举 - 自动生成
namespace NFluentColorKey {
    enum Key {
        AccentFillColorDisabled,
        AccentTextFillColorDisabled,
        CardBackgroundFillColorDefault,
        CardBackgroundFillColorSecondary,
        CardStrokeColorDefault,
        CardStrokeColorDefaultSolid,
        ControlAltFillColorDisabled,
        ControlAltFillColorQuarternary,
        ControlAltFillColorSecondary,
        ControlAltFillColorTertiary,
        ControlAltFillColorTransparent,
        ControlFillColorDefault,
        ControlFillColorDisabled,
        ControlFillColorInputActive,
        ControlFillColorSecondary,
        ControlFillColorTertiary,
        ControlFillColorTransparent,
        ControlOnImageFillColorDefault,
        ControlOnImageFillColorDisabled,
        ControlOnImageFillColorSecondary,
        ControlOnImageFillColorTertiary,
        ControlSolidFillColorDefault,
        ControlStrokeColorDefault,
        ControlStrokeColorForStrongFillWhenOnImage,
        ControlStrokeColorOnAccentDefault,
        ControlStrokeColorOnAccentDisabled,
        ControlStrokeColorOnAccentSecondary,
        ControlStrokeColorOnAccentTertiary,
        ControlStrokeColorSecondary,
        ControlStrongFillColorDefault,
        ControlStrongFillColorDisabled,
        ControlStrongStrokeColorDefault,
        ControlStrongStrokeColorDisabled,
        DividerStrokeColorDefault,
        FocusStrokeColorInner,
        FocusStrokeColorOuter,
        LayerFillColorAlt,
        LayerFillColorDefault,
        LayerOnAccentAcrylicFillColorDefault,
        LayerOnAcrylicFillColorDefault,
        LayerOnMicaBaseAltFillColorDefault,
        LayerOnMicaBaseAltFillColorSecondary,
        LayerOnMicaBaseAltFillColorTertiary,
        LayerOnMicaBaseAltFillColorTransparent,
        SmokeFillColorDefault,
        SolidBackgroundFillColorBase,
        SolidBackgroundFillColorBaseAlt,
        SolidBackgroundFillColorQuarternary,
        SolidBackgroundFillColorSecondary,
        SolidBackgroundFillColorTertiary,
        SolidBackgroundFillColorTransparent,
        SubtleFillColorDisabled,
        SubtleFillColorSecondary,
        SubtleFillColorTertiary,
        SubtleFillColorTransparent,
        SurfaceStrokeColorDefault,
        SurfaceStrokeColorFlyout,
        SurfaceStrokeColorInverse,
        SystemFillColorAttentionBackground,
        SystemFillColorCaution,
        SystemFillColorCautionBackground,
        SystemFillColorCritical,
        SystemFillColorCriticalBackground,
        SystemFillColorNeutral,
        SystemFillColorNeutralBackground,
        SystemFillColorSolidAttentionBackground,
        SystemFillColorSolidNeutral,
        SystemFillColorSolidNeutralBackground,
        SystemFillColorSuccess,
        SystemFillColorSuccessBackground,
        TextFillColorDisabled,
        TextFillColorInverse,
        TextFillColorPrimary,
        TextFillColorSecondary,
        TextFillColorTertiary,
        TextOnAccentFillColorDisabled,
        TextOnAccentFillColorPrimary,
        TextOnAccentFillColorSecondary,
        TextOnAccentFillColorSelectedText,
        Count  // 用于计数和边界检查
    };
}

// 枚举到字符串的转换函数
inline QString fluentColorKeyToString(NFluentColorKey::Key key) {
    switch (key) {
        case NFluentColorKey::AccentFillColorDisabled:
            return "AccentFillColorDisabled";
        case NFluentColorKey::AccentTextFillColorDisabled:
            return "AccentTextFillColorDisabled";
        case NFluentColorKey::CardBackgroundFillColorDefault:
            return "CardBackgroundFillColorDefault";
        case NFluentColorKey::CardBackgroundFillColorSecondary:
            return "CardBackgroundFillColorSecondary";
        case NFluentColorKey::CardStrokeColorDefault:
            return "CardStrokeColorDefault";
        case NFluentColorKey::CardStrokeColorDefaultSolid:
            return "CardStrokeColorDefaultSolid";
        case NFluentColorKey::ControlAltFillColorDisabled:
            return "ControlAltFillColorDisabled";
        case NFluentColorKey::ControlAltFillColorQuarternary:
            return "ControlAltFillColorQuarternary";
        case NFluentColorKey::ControlAltFillColorSecondary:
            return "ControlAltFillColorSecondary";
        case NFluentColorKey::ControlAltFillColorTertiary:
            return "ControlAltFillColorTertiary";
        case NFluentColorKey::ControlAltFillColorTransparent:
            return "ControlAltFillColorTransparent";
        case NFluentColorKey::ControlFillColorDefault:
            return "ControlFillColorDefault";
        case NFluentColorKey::ControlFillColorDisabled:
            return "ControlFillColorDisabled";
        case NFluentColorKey::ControlFillColorInputActive:
            return "ControlFillColorInputActive";
        case NFluentColorKey::ControlFillColorSecondary:
            return "ControlFillColorSecondary";
        case NFluentColorKey::ControlFillColorTertiary:
            return "ControlFillColorTertiary";
        case NFluentColorKey::ControlFillColorTransparent:
            return "ControlFillColorTransparent";
        case NFluentColorKey::ControlOnImageFillColorDefault:
            return "ControlOnImageFillColorDefault";
        case NFluentColorKey::ControlOnImageFillColorDisabled:
            return "ControlOnImageFillColorDisabled";
        case NFluentColorKey::ControlOnImageFillColorSecondary:
            return "ControlOnImageFillColorSecondary";
        case NFluentColorKey::ControlOnImageFillColorTertiary:
            return "ControlOnImageFillColorTertiary";
        case NFluentColorKey::ControlSolidFillColorDefault:
            return "ControlSolidFillColorDefault";
        case NFluentColorKey::ControlStrokeColorDefault:
            return "ControlStrokeColorDefault";
        case NFluentColorKey::ControlStrokeColorForStrongFillWhenOnImage:
            return "ControlStrokeColorForStrongFillWhenOnImage";
        case NFluentColorKey::ControlStrokeColorOnAccentDefault:
            return "ControlStrokeColorOnAccentDefault";
        case NFluentColorKey::ControlStrokeColorOnAccentDisabled:
            return "ControlStrokeColorOnAccentDisabled";
        case NFluentColorKey::ControlStrokeColorOnAccentSecondary:
            return "ControlStrokeColorOnAccentSecondary";
        case NFluentColorKey::ControlStrokeColorOnAccentTertiary:
            return "ControlStrokeColorOnAccentTertiary";
        case NFluentColorKey::ControlStrokeColorSecondary:
            return "ControlStrokeColorSecondary";
        case NFluentColorKey::ControlStrongFillColorDefault:
            return "ControlStrongFillColorDefault";
        case NFluentColorKey::ControlStrongFillColorDisabled:
            return "ControlStrongFillColorDisabled";
        case NFluentColorKey::ControlStrongStrokeColorDefault:
            return "ControlStrongStrokeColorDefault";
        case NFluentColorKey::ControlStrongStrokeColorDisabled:
            return "ControlStrongStrokeColorDisabled";
        case NFluentColorKey::DividerStrokeColorDefault:
            return "DividerStrokeColorDefault";
        case NFluentColorKey::FocusStrokeColorInner:
            return "FocusStrokeColorInner";
        case NFluentColorKey::FocusStrokeColorOuter:
            return "FocusStrokeColorOuter";
        case NFluentColorKey::LayerFillColorAlt:
            return "LayerFillColorAlt";
        case NFluentColorKey::LayerFillColorDefault:
            return "LayerFillColorDefault";
        case NFluentColorKey::LayerOnAccentAcrylicFillColorDefault:
            return "LayerOnAccentAcrylicFillColorDefault";
        case NFluentColorKey::LayerOnAcrylicFillColorDefault:
            return "LayerOnAcrylicFillColorDefault";
        case NFluentColorKey::LayerOnMicaBaseAltFillColorDefault:
            return "LayerOnMicaBaseAltFillColorDefault";
        case NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary:
            return "LayerOnMicaBaseAltFillColorSecondary";
        case NFluentColorKey::LayerOnMicaBaseAltFillColorTertiary:
            return "LayerOnMicaBaseAltFillColorTertiary";
        case NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent:
            return "LayerOnMicaBaseAltFillColorTransparent";
        case NFluentColorKey::SmokeFillColorDefault:
            return "SmokeFillColorDefault";
        case NFluentColorKey::SolidBackgroundFillColorBase:
            return "SolidBackgroundFillColorBase";
        case NFluentColorKey::SolidBackgroundFillColorBaseAlt:
            return "SolidBackgroundFillColorBaseAlt";
        case NFluentColorKey::SolidBackgroundFillColorQuarternary:
            return "SolidBackgroundFillColorQuarternary";
        case NFluentColorKey::SolidBackgroundFillColorSecondary:
            return "SolidBackgroundFillColorSecondary";
        case NFluentColorKey::SolidBackgroundFillColorTertiary:
            return "SolidBackgroundFillColorTertiary";
        case NFluentColorKey::SolidBackgroundFillColorTransparent:
            return "SolidBackgroundFillColorTransparent";
        case NFluentColorKey::SubtleFillColorDisabled:
            return "SubtleFillColorDisabled";
        case NFluentColorKey::SubtleFillColorSecondary:
            return "SubtleFillColorSecondary";
        case NFluentColorKey::SubtleFillColorTertiary:
            return "SubtleFillColorTertiary";
        case NFluentColorKey::SubtleFillColorTransparent:
            return "SubtleFillColorTransparent";
        case NFluentColorKey::SurfaceStrokeColorDefault:
            return "SurfaceStrokeColorDefault";
        case NFluentColorKey::SurfaceStrokeColorFlyout:
            return "SurfaceStrokeColorFlyout";
        case NFluentColorKey::SurfaceStrokeColorInverse:
            return "SurfaceStrokeColorInverse";
        case NFluentColorKey::SystemFillColorAttentionBackground:
            return "SystemFillColorAttentionBackground";
        case NFluentColorKey::SystemFillColorCaution:
            return "SystemFillColorCaution";
        case NFluentColorKey::SystemFillColorCautionBackground:
            return "SystemFillColorCautionBackground";
        case NFluentColorKey::SystemFillColorCritical:
            return "SystemFillColorCritical";
        case NFluentColorKey::SystemFillColorCriticalBackground:
            return "SystemFillColorCriticalBackground";
        case NFluentColorKey::SystemFillColorNeutral:
            return "SystemFillColorNeutral";
        case NFluentColorKey::SystemFillColorNeutralBackground:
            return "SystemFillColorNeutralBackground";
        case NFluentColorKey::SystemFillColorSolidAttentionBackground:
            return "SystemFillColorSolidAttentionBackground";
        case NFluentColorKey::SystemFillColorSolidNeutral:
            return "SystemFillColorSolidNeutral";
        case NFluentColorKey::SystemFillColorSolidNeutralBackground:
            return "SystemFillColorSolidNeutralBackground";
        case NFluentColorKey::SystemFillColorSuccess:
            return "SystemFillColorSuccess";
        case NFluentColorKey::SystemFillColorSuccessBackground:
            return "SystemFillColorSuccessBackground";
        case NFluentColorKey::TextFillColorDisabled:
            return "TextFillColorDisabled";
        case NFluentColorKey::TextFillColorInverse:
            return "TextFillColorInverse";
        case NFluentColorKey::TextFillColorPrimary:
            return "TextFillColorPrimary";
        case NFluentColorKey::TextFillColorSecondary:
            return "TextFillColorSecondary";
        case NFluentColorKey::TextFillColorTertiary:
            return "TextFillColorTertiary";
        case NFluentColorKey::TextOnAccentFillColorDisabled:
            return "TextOnAccentFillColorDisabled";
        case NFluentColorKey::TextOnAccentFillColorPrimary:
            return "TextOnAccentFillColorPrimary";
        case NFluentColorKey::TextOnAccentFillColorSecondary:
            return "TextOnAccentFillColorSecondary";
        case NFluentColorKey::TextOnAccentFillColorSelectedText:
            return "TextOnAccentFillColorSelectedText";
        case NFluentColorKey::Count:
            return "";
    }
    return "";
}

// 字符串到枚举的转换函数
inline NFluentColorKey::Key stringToFluentColorKey(const QString& keyString) {
    if (keyString == "AccentFillColorDisabled") return NFluentColorKey::AccentFillColorDisabled;
    if (keyString == "AccentTextFillColorDisabled") return NFluentColorKey::AccentTextFillColorDisabled;
    if (keyString == "CardBackgroundFillColorDefault") return NFluentColorKey::CardBackgroundFillColorDefault;
    if (keyString == "CardBackgroundFillColorSecondary") return NFluentColorKey::CardBackgroundFillColorSecondary;
    if (keyString == "CardStrokeColorDefault") return NFluentColorKey::CardStrokeColorDefault;
    if (keyString == "CardStrokeColorDefaultSolid") return NFluentColorKey::CardStrokeColorDefaultSolid;
    if (keyString == "ControlAltFillColorDisabled") return NFluentColorKey::ControlAltFillColorDisabled;
    if (keyString == "ControlAltFillColorQuarternary") return NFluentColorKey::ControlAltFillColorQuarternary;
    if (keyString == "ControlAltFillColorSecondary") return NFluentColorKey::ControlAltFillColorSecondary;
    if (keyString == "ControlAltFillColorTertiary") return NFluentColorKey::ControlAltFillColorTertiary;
    if (keyString == "ControlAltFillColorTransparent") return NFluentColorKey::ControlAltFillColorTransparent;
    if (keyString == "ControlFillColorDefault") return NFluentColorKey::ControlFillColorDefault;
    if (keyString == "ControlFillColorDisabled") return NFluentColorKey::ControlFillColorDisabled;
    if (keyString == "ControlFillColorInputActive") return NFluentColorKey::ControlFillColorInputActive;
    if (keyString == "ControlFillColorSecondary") return NFluentColorKey::ControlFillColorSecondary;
    if (keyString == "ControlFillColorTertiary") return NFluentColorKey::ControlFillColorTertiary;
    if (keyString == "ControlFillColorTransparent") return NFluentColorKey::ControlFillColorTransparent;
    if (keyString == "ControlOnImageFillColorDefault") return NFluentColorKey::ControlOnImageFillColorDefault;
    if (keyString == "ControlOnImageFillColorDisabled") return NFluentColorKey::ControlOnImageFillColorDisabled;
    if (keyString == "ControlOnImageFillColorSecondary") return NFluentColorKey::ControlOnImageFillColorSecondary;
    if (keyString == "ControlOnImageFillColorTertiary") return NFluentColorKey::ControlOnImageFillColorTertiary;
    if (keyString == "ControlSolidFillColorDefault") return NFluentColorKey::ControlSolidFillColorDefault;
    if (keyString == "ControlStrokeColorDefault") return NFluentColorKey::ControlStrokeColorDefault;
    if (keyString == "ControlStrokeColorForStrongFillWhenOnImage") return NFluentColorKey::ControlStrokeColorForStrongFillWhenOnImage;
    if (keyString == "ControlStrokeColorOnAccentDefault") return NFluentColorKey::ControlStrokeColorOnAccentDefault;
    if (keyString == "ControlStrokeColorOnAccentDisabled") return NFluentColorKey::ControlStrokeColorOnAccentDisabled;
    if (keyString == "ControlStrokeColorOnAccentSecondary") return NFluentColorKey::ControlStrokeColorOnAccentSecondary;
    if (keyString == "ControlStrokeColorOnAccentTertiary") return NFluentColorKey::ControlStrokeColorOnAccentTertiary;
    if (keyString == "ControlStrokeColorSecondary") return NFluentColorKey::ControlStrokeColorSecondary;
    if (keyString == "ControlStrongFillColorDefault") return NFluentColorKey::ControlStrongFillColorDefault;
    if (keyString == "ControlStrongFillColorDisabled") return NFluentColorKey::ControlStrongFillColorDisabled;
    if (keyString == "ControlStrongStrokeColorDefault") return NFluentColorKey::ControlStrongStrokeColorDefault;
    if (keyString == "ControlStrongStrokeColorDisabled") return NFluentColorKey::ControlStrongStrokeColorDisabled;
    if (keyString == "DividerStrokeColorDefault") return NFluentColorKey::DividerStrokeColorDefault;
    if (keyString == "FocusStrokeColorInner") return NFluentColorKey::FocusStrokeColorInner;
    if (keyString == "FocusStrokeColorOuter") return NFluentColorKey::FocusStrokeColorOuter;
    if (keyString == "LayerFillColorAlt") return NFluentColorKey::LayerFillColorAlt;
    if (keyString == "LayerFillColorDefault") return NFluentColorKey::LayerFillColorDefault;
    if (keyString == "LayerOnAccentAcrylicFillColorDefault") return NFluentColorKey::LayerOnAccentAcrylicFillColorDefault;
    if (keyString == "LayerOnAcrylicFillColorDefault") return NFluentColorKey::LayerOnAcrylicFillColorDefault;
    if (keyString == "LayerOnMicaBaseAltFillColorDefault") return NFluentColorKey::LayerOnMicaBaseAltFillColorDefault;
    if (keyString == "LayerOnMicaBaseAltFillColorSecondary") return NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary;
    if (keyString == "LayerOnMicaBaseAltFillColorTertiary") return NFluentColorKey::LayerOnMicaBaseAltFillColorTertiary;
    if (keyString == "LayerOnMicaBaseAltFillColorTransparent") return NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent;
    if (keyString == "SmokeFillColorDefault") return NFluentColorKey::SmokeFillColorDefault;
    if (keyString == "SolidBackgroundFillColorBase") return NFluentColorKey::SolidBackgroundFillColorBase;
    if (keyString == "SolidBackgroundFillColorBaseAlt") return NFluentColorKey::SolidBackgroundFillColorBaseAlt;
    if (keyString == "SolidBackgroundFillColorQuarternary") return NFluentColorKey::SolidBackgroundFillColorQuarternary;
    if (keyString == "SolidBackgroundFillColorSecondary") return NFluentColorKey::SolidBackgroundFillColorSecondary;
    if (keyString == "SolidBackgroundFillColorTertiary") return NFluentColorKey::SolidBackgroundFillColorTertiary;
    if (keyString == "SolidBackgroundFillColorTransparent") return NFluentColorKey::SolidBackgroundFillColorTransparent;
    if (keyString == "SubtleFillColorDisabled") return NFluentColorKey::SubtleFillColorDisabled;
    if (keyString == "SubtleFillColorSecondary") return NFluentColorKey::SubtleFillColorSecondary;
    if (keyString == "SubtleFillColorTertiary") return NFluentColorKey::SubtleFillColorTertiary;
    if (keyString == "SubtleFillColorTransparent") return NFluentColorKey::SubtleFillColorTransparent;
    if (keyString == "SurfaceStrokeColorDefault") return NFluentColorKey::SurfaceStrokeColorDefault;
    if (keyString == "SurfaceStrokeColorFlyout") return NFluentColorKey::SurfaceStrokeColorFlyout;
    if (keyString == "SurfaceStrokeColorInverse") return NFluentColorKey::SurfaceStrokeColorInverse;
    if (keyString == "SystemFillColorAttentionBackground") return NFluentColorKey::SystemFillColorAttentionBackground;
    if (keyString == "SystemFillColorCaution") return NFluentColorKey::SystemFillColorCaution;
    if (keyString == "SystemFillColorCautionBackground") return NFluentColorKey::SystemFillColorCautionBackground;
    if (keyString == "SystemFillColorCritical") return NFluentColorKey::SystemFillColorCritical;
    if (keyString == "SystemFillColorCriticalBackground") return NFluentColorKey::SystemFillColorCriticalBackground;
    if (keyString == "SystemFillColorNeutral") return NFluentColorKey::SystemFillColorNeutral;
    if (keyString == "SystemFillColorNeutralBackground") return NFluentColorKey::SystemFillColorNeutralBackground;
    if (keyString == "SystemFillColorSolidAttentionBackground") return NFluentColorKey::SystemFillColorSolidAttentionBackground;
    if (keyString == "SystemFillColorSolidNeutral") return NFluentColorKey::SystemFillColorSolidNeutral;
    if (keyString == "SystemFillColorSolidNeutralBackground") return NFluentColorKey::SystemFillColorSolidNeutralBackground;
    if (keyString == "SystemFillColorSuccess") return NFluentColorKey::SystemFillColorSuccess;
    if (keyString == "SystemFillColorSuccessBackground") return NFluentColorKey::SystemFillColorSuccessBackground;
    if (keyString == "TextFillColorDisabled") return NFluentColorKey::TextFillColorDisabled;
    if (keyString == "TextFillColorInverse") return NFluentColorKey::TextFillColorInverse;
    if (keyString == "TextFillColorPrimary") return NFluentColorKey::TextFillColorPrimary;
    if (keyString == "TextFillColorSecondary") return NFluentColorKey::TextFillColorSecondary;
    if (keyString == "TextFillColorTertiary") return NFluentColorKey::TextFillColorTertiary;
    if (keyString == "TextOnAccentFillColorDisabled") return NFluentColorKey::TextOnAccentFillColorDisabled;
    if (keyString == "TextOnAccentFillColorPrimary") return NFluentColorKey::TextOnAccentFillColorPrimary;
    if (keyString == "TextOnAccentFillColorSecondary") return NFluentColorKey::TextOnAccentFillColorSecondary;
    if (keyString == "TextOnAccentFillColorSelectedText") return NFluentColorKey::TextOnAccentFillColorSelectedText;
    return NFluentColorKey::Count; // 无效的键
}

// 暗色主题颜色
static const QMap<NFluentColorKey::Key, QColor> DarkThemeColors = {
    {NFluentColorKey::TextFillColorPrimary, QColor("#FFFFFF")},
    {NFluentColorKey::TextFillColorSecondary, colorWithAlpha("#FFFFFF", 197)},
    {NFluentColorKey::TextFillColorTertiary, colorWithAlpha("#FFFFFF", 135)},
    {NFluentColorKey::TextFillColorDisabled, colorWithAlpha("#FFFFFF", 93)},
    {NFluentColorKey::TextFillColorInverse, colorWithAlpha("#000000", 228)},
    {NFluentColorKey::AccentTextFillColorDisabled, colorWithAlpha("#FFFFFF", 93)},
    {NFluentColorKey::TextOnAccentFillColorSelectedText, QColor("#FFFFFF")},
    {NFluentColorKey::TextOnAccentFillColorPrimary, QColor("#000000")},
    {NFluentColorKey::TextOnAccentFillColorSecondary, colorWithAlpha("#000000", 128)},
    {NFluentColorKey::TextOnAccentFillColorDisabled, colorWithAlpha("#FFFFFF", 135)},
    {NFluentColorKey::ControlFillColorDefault, colorWithAlpha("#FFFFFF", 15)},
    {NFluentColorKey::ControlFillColorSecondary, colorWithAlpha("#FFFFFF", 21)},
    {NFluentColorKey::ControlFillColorTertiary, colorWithAlpha("#FFFFFF", 8)},
    {NFluentColorKey::ControlFillColorDisabled, colorWithAlpha("#FFFFFF", 11)},
    {NFluentColorKey::ControlFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlFillColorInputActive, colorWithAlpha("#1E1E1E", 179)},
    {NFluentColorKey::ControlStrongFillColorDefault, colorWithAlpha("#FFFFFF", 139)},
    {NFluentColorKey::ControlStrongFillColorDisabled, colorWithAlpha("#FFFFFF", 63)},
    {NFluentColorKey::ControlSolidFillColorDefault, QColor("#454545")},
    {NFluentColorKey::SubtleFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::SubtleFillColorSecondary, colorWithAlpha("#FFFFFF", 15)},
    {NFluentColorKey::SubtleFillColorTertiary, colorWithAlpha("#FFFFFF", 10)},
    {NFluentColorKey::SubtleFillColorDisabled, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlAltFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlAltFillColorSecondary, colorWithAlpha("#000000", 25)},
    {NFluentColorKey::ControlAltFillColorTertiary, colorWithAlpha("#FFFFFF", 11)},
    {NFluentColorKey::ControlAltFillColorQuarternary, colorWithAlpha("#FFFFFF", 18)},
    {NFluentColorKey::ControlAltFillColorDisabled, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlOnImageFillColorDefault, colorWithAlpha("#1C1C1C", 179)},
    {NFluentColorKey::ControlOnImageFillColorSecondary, QColor("#1A1A1A")},
    {NFluentColorKey::ControlOnImageFillColorTertiary, QColor("#131313")},
    {NFluentColorKey::ControlOnImageFillColorDisabled, QColor("#1E1E1E")},
    {NFluentColorKey::AccentFillColorDisabled, colorWithAlpha("#FFFFFF", 40)},
    {NFluentColorKey::ControlStrokeColorDefault, colorWithAlpha("#FFFFFF", 18)},
    {NFluentColorKey::ControlStrokeColorSecondary, colorWithAlpha("#FFFFFF", 24)},
    {NFluentColorKey::ControlStrokeColorOnAccentDefault, colorWithAlpha("#FFFFFF", 20)},
    {NFluentColorKey::ControlStrokeColorOnAccentSecondary, colorWithAlpha("#000000", 35)},
    {NFluentColorKey::ControlStrokeColorOnAccentTertiary, colorWithAlpha("#000000", 55)},
    {NFluentColorKey::ControlStrokeColorOnAccentDisabled, colorWithAlpha("#000000", 51)},
    {NFluentColorKey::ControlStrokeColorForStrongFillWhenOnImage, colorWithAlpha("#000000", 107)},
    {NFluentColorKey::CardStrokeColorDefault, colorWithAlpha("#000000", 25)},
    {NFluentColorKey::CardStrokeColorDefaultSolid, QColor("#1C1C1C")},
    {NFluentColorKey::ControlStrongStrokeColorDefault, colorWithAlpha("#FFFFFF", 139)},
    {NFluentColorKey::ControlStrongStrokeColorDisabled, colorWithAlpha("#FFFFFF", 40)},
    {NFluentColorKey::SurfaceStrokeColorDefault, colorWithAlpha("#757575", 102)},
    {NFluentColorKey::SurfaceStrokeColorFlyout, colorWithAlpha("#000000", 51)},
    {NFluentColorKey::SurfaceStrokeColorInverse, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::DividerStrokeColorDefault, colorWithAlpha("#FFFFFF", 21)},
    {NFluentColorKey::FocusStrokeColorOuter, QColor("#FFFFFF")},
    {NFluentColorKey::FocusStrokeColorInner, colorWithAlpha("#000000", 179)},
    {NFluentColorKey::CardBackgroundFillColorDefault, colorWithAlpha("#FFFFFF", 13)},
    {NFluentColorKey::CardBackgroundFillColorSecondary, colorWithAlpha("#FFFFFF", 8)},
    {NFluentColorKey::SmokeFillColorDefault, colorWithAlpha("#000000", 77)},
    {NFluentColorKey::LayerFillColorDefault, colorWithAlpha("#3A3A3A", 76)},
    {NFluentColorKey::LayerFillColorAlt, colorWithAlpha("#FFFFFF", 13)},
    {NFluentColorKey::LayerOnAcrylicFillColorDefault, colorWithAlpha("#FFFFFF", 9)},
    {NFluentColorKey::LayerOnAccentAcrylicFillColorDefault, colorWithAlpha("#FFFFFF", 9)},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, colorWithAlpha("#3A3A3A", 115)},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, colorWithAlpha("#FFFFFF", 15)},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorTertiary, QColor("#2C2C2C")},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::SolidBackgroundFillColorBase, QColor("#202020")},
    {NFluentColorKey::SolidBackgroundFillColorSecondary, QColor("#1C1C1C")},
    {NFluentColorKey::SolidBackgroundFillColorTertiary, QColor("#282828")},
    {NFluentColorKey::SolidBackgroundFillColorQuarternary, QColor("#2C2C2C")},
    {NFluentColorKey::SolidBackgroundFillColorTransparent, colorWithAlpha("#202020", 0)},
    {NFluentColorKey::SolidBackgroundFillColorBaseAlt, QColor("#0A0A0A")},
    {NFluentColorKey::SystemFillColorSuccess, QColor("#6CCB5F")},
    {NFluentColorKey::SystemFillColorCaution, QColor("#FCE100")},
    {NFluentColorKey::SystemFillColorCritical, QColor("#FF99A4")},
    {NFluentColorKey::SystemFillColorNeutral, colorWithAlpha("#FFFFFF", 139)},
    {NFluentColorKey::SystemFillColorSolidNeutral, QColor("#9D9D9D")},
    {NFluentColorKey::SystemFillColorAttentionBackground, colorWithAlpha("#FFFFFF", 8)},
    {NFluentColorKey::SystemFillColorSuccessBackground, QColor("#393D1B")},
    {NFluentColorKey::SystemFillColorCautionBackground, QColor("#433519")},
    {NFluentColorKey::SystemFillColorCriticalBackground, QColor("#442726")},
    {NFluentColorKey::SystemFillColorNeutralBackground, colorWithAlpha("#FFFFFF", 8)},
    {NFluentColorKey::SystemFillColorSolidAttentionBackground, QColor("#2E2E2E")},
    {NFluentColorKey::SystemFillColorSolidNeutralBackground, QColor("#2E2E2E")},
};

// 亮色主题颜色
static const QMap<NFluentColorKey::Key, QColor> LightThemeColors = {
    {NFluentColorKey::TextFillColorPrimary, colorWithAlpha("#000000", 228)},
    {NFluentColorKey::TextFillColorSecondary, colorWithAlpha("#000000", 158)},
    {NFluentColorKey::TextFillColorTertiary, colorWithAlpha("#000000", 114)},
    {NFluentColorKey::TextFillColorDisabled, colorWithAlpha("#000000", 92)},
    {NFluentColorKey::TextFillColorInverse, QColor("#FFFFFF")},
    {NFluentColorKey::AccentTextFillColorDisabled, colorWithAlpha("#000000", 92)},
    {NFluentColorKey::TextOnAccentFillColorSelectedText, QColor("#FFFFFF")},
    {NFluentColorKey::TextOnAccentFillColorPrimary, QColor("#FFFFFF")},
    {NFluentColorKey::TextOnAccentFillColorSecondary, colorWithAlpha("#FFFFFF", 179)},
    {NFluentColorKey::TextOnAccentFillColorDisabled, QColor("#FFFFFF")},
    {NFluentColorKey::ControlFillColorDefault, colorWithAlpha("#FFFFFF", 179)},
    {NFluentColorKey::ControlFillColorSecondary, colorWithAlpha("#F9F9F9", 128)},
    {NFluentColorKey::ControlFillColorTertiary, colorWithAlpha("#F9F9F9", 77)},
    {NFluentColorKey::ControlFillColorDisabled, colorWithAlpha("#F9F9F9", 77)},
    {NFluentColorKey::ControlFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlFillColorInputActive, QColor("#FFFFFF")},
    {NFluentColorKey::ControlStrongFillColorDefault, colorWithAlpha("#000000", 114)},
    {NFluentColorKey::ControlStrongFillColorDisabled, colorWithAlpha("#000000", 81)},
    {NFluentColorKey::ControlSolidFillColorDefault, QColor("#FFFFFF")},
    {NFluentColorKey::SubtleFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::SubtleFillColorSecondary, colorWithAlpha("#000000", 9)},
    {NFluentColorKey::SubtleFillColorTertiary, colorWithAlpha("#000000", 6)},
    {NFluentColorKey::SubtleFillColorDisabled, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlAltFillColorTransparent, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlAltFillColorSecondary, colorWithAlpha("#000000", 6)},
    {NFluentColorKey::ControlAltFillColorTertiary, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::ControlAltFillColorQuarternary, colorWithAlpha("#000000", 24)},
    {NFluentColorKey::ControlAltFillColorDisabled, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::ControlOnImageFillColorDefault, colorWithAlpha("#FFFFFF", 201)},
    {NFluentColorKey::ControlOnImageFillColorSecondary, QColor("#F3F3F3")},
    {NFluentColorKey::ControlOnImageFillColorTertiary, QColor("#EBEBEB")},
    {NFluentColorKey::ControlOnImageFillColorDisabled, colorWithAlpha("#FFFFFF", 0)},
    {NFluentColorKey::AccentFillColorDisabled, colorWithAlpha("#000000", 55)},
    {NFluentColorKey::ControlStrokeColorDefault, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::ControlStrokeColorSecondary, colorWithAlpha("#000000", 41)},
    {NFluentColorKey::ControlStrokeColorOnAccentDefault, colorWithAlpha("#FFFFFF", 20)},
    {NFluentColorKey::ControlStrokeColorOnAccentSecondary, colorWithAlpha("#000000", 102)},
    {NFluentColorKey::ControlStrokeColorOnAccentTertiary, colorWithAlpha("#000000", 55)},
    {NFluentColorKey::ControlStrokeColorOnAccentDisabled, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::ControlStrokeColorForStrongFillWhenOnImage, colorWithAlpha("#FFFFFF", 89)},
    {NFluentColorKey::CardStrokeColorDefault, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::CardStrokeColorDefaultSolid, QColor("#EBEBEB")},
    {NFluentColorKey::ControlStrongStrokeColorDefault, colorWithAlpha("#000000", 114)},
    {NFluentColorKey::ControlStrongStrokeColorDisabled, colorWithAlpha("#000000", 55)},
    {NFluentColorKey::SurfaceStrokeColorDefault, colorWithAlpha("#757575", 102)},
    {NFluentColorKey::SurfaceStrokeColorFlyout, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::SurfaceStrokeColorInverse, colorWithAlpha("#FFFFFF", 21)},
    {NFluentColorKey::DividerStrokeColorDefault, colorWithAlpha("#000000", 15)},
    {NFluentColorKey::FocusStrokeColorOuter, colorWithAlpha("#000000", 228)},
    {NFluentColorKey::FocusStrokeColorInner, colorWithAlpha("#FFFFFF", 179)},
    {NFluentColorKey::CardBackgroundFillColorDefault, colorWithAlpha("#FFFFFF", 179)},
    {NFluentColorKey::CardBackgroundFillColorSecondary, colorWithAlpha("#F6F6F6", 128)},
    {NFluentColorKey::SmokeFillColorDefault, colorWithAlpha("#000000", 77)},
    {NFluentColorKey::LayerFillColorDefault, colorWithAlpha("#FFFFFF", 128)},
    {NFluentColorKey::LayerFillColorAlt, QColor("#FFFFFF")},
    {NFluentColorKey::LayerOnAcrylicFillColorDefault, colorWithAlpha("#FFFFFF", 64)},
    {NFluentColorKey::LayerOnAccentAcrylicFillColorDefault, colorWithAlpha("#FFFFFF", 64)},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, colorWithAlpha("#FFFFFF", 179)},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, colorWithAlpha("#000000", 10)},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorTertiary, QColor("#F9F9F9")},
    {NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, colorWithAlpha("#000000", 0)},
    {NFluentColorKey::SolidBackgroundFillColorBase, QColor("#F3F3F3")},
    {NFluentColorKey::SolidBackgroundFillColorSecondary, QColor("#EEEEEE")},
    {NFluentColorKey::SolidBackgroundFillColorTertiary, QColor("#F9F9F9")},
    {NFluentColorKey::SolidBackgroundFillColorQuarternary, QColor("#FFFFFF")},
    {NFluentColorKey::SolidBackgroundFillColorTransparent, colorWithAlpha("#F3F3F3", 0)},
    {NFluentColorKey::SolidBackgroundFillColorBaseAlt, QColor("#DADADA")},
    {NFluentColorKey::SystemFillColorSuccess, QColor("#0F7B0F")},
    {NFluentColorKey::SystemFillColorCaution, QColor("#9D5D00")},
    {NFluentColorKey::SystemFillColorCritical, QColor("#C42B1C")},
    {NFluentColorKey::SystemFillColorNeutral, colorWithAlpha("#000000", 114)},
    {NFluentColorKey::SystemFillColorSolidNeutral, QColor("#8A8A8A")},
    {NFluentColorKey::SystemFillColorAttentionBackground, colorWithAlpha("#F6F6F6", 128)},
    {NFluentColorKey::SystemFillColorSuccessBackground, QColor("#DFF6DD")},
    {NFluentColorKey::SystemFillColorCautionBackground, QColor("#FFF4CE")},
    {NFluentColorKey::SystemFillColorCriticalBackground, QColor("#FDE7E9")},
    {NFluentColorKey::SystemFillColorNeutralBackground, colorWithAlpha("#000000", 6)},
    {NFluentColorKey::SystemFillColorSolidAttentionBackground, QColor("#F7F7F7")},
    {NFluentColorKey::SystemFillColorSolidNeutralBackground, QColor("#F3F3F3")},
};

#endif // NFLUENTCOLORS_H
