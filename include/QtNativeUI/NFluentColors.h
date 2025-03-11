// 自动生成的文件，请勿手动修改
// 从Microsoft Fluent Design颜色资源生成

#ifndef NFLUENTCOLORS_H
#define NFLUENTCOLORS_H

#include <QColor>
#include <QString>
#include <QMap>

// 辅助函数：创建带有alpha值的颜色
inline QColor colorWithAlpha(const QString& hex, int alpha) {
    QColor color(hex);
    color.setAlpha(alpha);
    return color;
}

// 暗色主题颜色
static const QMap<QString, QColor> DarkThemeColors = {
    {"TextFillColorPrimary", QColor("#FFFFFF")},
    {"TextFillColorSecondary", colorWithAlpha("#FFFFFF", 197)},
    {"TextFillColorTertiary", colorWithAlpha("#FFFFFF", 135)},
    {"TextFillColorDisabled", colorWithAlpha("#FFFFFF", 93)},
    {"TextFillColorInverse", colorWithAlpha("#000000", 228)},
    {"AccentTextFillColorDisabled", colorWithAlpha("#FFFFFF", 93)},
    {"TextOnAccentFillColorSelectedText", QColor("#FFFFFF")},
    {"TextOnAccentFillColorPrimary", QColor("#000000")},
    {"TextOnAccentFillColorSecondary", colorWithAlpha("#000000", 128)},
    {"TextOnAccentFillColorDisabled", colorWithAlpha("#FFFFFF", 135)},
    {"ControlFillColorDefault", colorWithAlpha("#FFFFFF", 15)},
    {"ControlFillColorSecondary", colorWithAlpha("#FFFFFF", 21)},
    {"ControlFillColorTertiary", colorWithAlpha("#FFFFFF", 8)},
    {"ControlFillColorDisabled", colorWithAlpha("#FFFFFF", 11)},
    {"ControlFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"ControlFillColorInputActive", colorWithAlpha("#1E1E1E", 179)},
    {"ControlStrongFillColorDefault", colorWithAlpha("#FFFFFF", 139)},
    {"ControlStrongFillColorDisabled", colorWithAlpha("#FFFFFF", 63)},
    {"ControlSolidFillColorDefault", QColor("#454545")},
    {"SubtleFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"SubtleFillColorSecondary", colorWithAlpha("#FFFFFF", 15)},
    {"SubtleFillColorTertiary", colorWithAlpha("#FFFFFF", 10)},
    {"SubtleFillColorDisabled", colorWithAlpha("#FFFFFF", 0)},
    {"ControlAltFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"ControlAltFillColorSecondary", colorWithAlpha("#000000", 25)},
    {"ControlAltFillColorTertiary", colorWithAlpha("#FFFFFF", 11)},
    {"ControlAltFillColorQuarternary", colorWithAlpha("#FFFFFF", 18)},
    {"ControlAltFillColorDisabled", colorWithAlpha("#FFFFFF", 0)},
    {"ControlOnImageFillColorDefault", colorWithAlpha("#1C1C1C", 179)},
    {"ControlOnImageFillColorSecondary", QColor("#1A1A1A")},
    {"ControlOnImageFillColorTertiary", QColor("#131313")},
    {"ControlOnImageFillColorDisabled", QColor("#1E1E1E")},
    {"AccentFillColorDisabled", colorWithAlpha("#FFFFFF", 40)},
    {"ControlStrokeColorDefault", colorWithAlpha("#FFFFFF", 18)},
    {"ControlStrokeColorSecondary", colorWithAlpha("#FFFFFF", 24)},
    {"ControlStrokeColorOnAccentDefault", colorWithAlpha("#FFFFFF", 20)},
    {"ControlStrokeColorOnAccentSecondary", colorWithAlpha("#000000", 35)},
    {"ControlStrokeColorOnAccentTertiary", colorWithAlpha("#000000", 55)},
    {"ControlStrokeColorOnAccentDisabled", colorWithAlpha("#000000", 51)},
    {"ControlStrokeColorForStrongFillWhenOnImage", colorWithAlpha("#000000", 107)},
    {"CardStrokeColorDefault", colorWithAlpha("#000000", 25)},
    {"CardStrokeColorDefaultSolid", QColor("#1C1C1C")},
    {"ControlStrongStrokeColorDefault", colorWithAlpha("#FFFFFF", 139)},
    {"ControlStrongStrokeColorDisabled", colorWithAlpha("#FFFFFF", 40)},
    {"SurfaceStrokeColorDefault", colorWithAlpha("#757575", 102)},
    {"SurfaceStrokeColorFlyout", colorWithAlpha("#000000", 51)},
    {"SurfaceStrokeColorInverse", colorWithAlpha("#000000", 15)},
    {"DividerStrokeColorDefault", colorWithAlpha("#FFFFFF", 21)},
    {"FocusStrokeColorOuter", QColor("#FFFFFF")},
    {"FocusStrokeColorInner", colorWithAlpha("#000000", 179)},
    {"CardBackgroundFillColorDefault", colorWithAlpha("#FFFFFF", 13)},
    {"CardBackgroundFillColorSecondary", colorWithAlpha("#FFFFFF", 8)},
    {"SmokeFillColorDefault", colorWithAlpha("#000000", 77)},
    {"LayerFillColorDefault", colorWithAlpha("#3A3A3A", 76)},
    {"LayerFillColorAlt", colorWithAlpha("#FFFFFF", 13)},
    {"LayerOnAcrylicFillColorDefault", colorWithAlpha("#FFFFFF", 9)},
    {"LayerOnAccentAcrylicFillColorDefault", colorWithAlpha("#FFFFFF", 9)},
    {"LayerOnMicaBaseAltFillColorDefault", colorWithAlpha("#3A3A3A", 115)},
    {"LayerOnMicaBaseAltFillColorSecondary", colorWithAlpha("#FFFFFF", 15)},
    {"LayerOnMicaBaseAltFillColorTertiary", QColor("#2C2C2C")},
    {"LayerOnMicaBaseAltFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"SolidBackgroundFillColorBase", QColor("#202020")},
    {"SolidBackgroundFillColorSecondary", QColor("#1C1C1C")},
    {"SolidBackgroundFillColorTertiary", QColor("#282828")},
    {"SolidBackgroundFillColorQuarternary", QColor("#2C2C2C")},
    {"SolidBackgroundFillColorTransparent", colorWithAlpha("#202020", 0)},
    {"SolidBackgroundFillColorBaseAlt", QColor("#0A0A0A")},
    {"SystemFillColorSuccess", QColor("#6CCB5F")},
    {"SystemFillColorCaution", QColor("#FCE100")},
    {"SystemFillColorCritical", QColor("#FF99A4")},
    {"SystemFillColorNeutral", colorWithAlpha("#FFFFFF", 139)},
    {"SystemFillColorSolidNeutral", QColor("#9D9D9D")},
    {"SystemFillColorAttentionBackground", colorWithAlpha("#FFFFFF", 8)},
    {"SystemFillColorSuccessBackground", QColor("#393D1B")},
    {"SystemFillColorCautionBackground", QColor("#433519")},
    {"SystemFillColorCriticalBackground", QColor("#442726")},
    {"SystemFillColorNeutralBackground", colorWithAlpha("#FFFFFF", 8)},
    {"SystemFillColorSolidAttentionBackground", QColor("#2E2E2E")},
    {"SystemFillColorSolidNeutralBackground", QColor("#2E2E2E")},
};

// 亮色主题颜色
static const QMap<QString, QColor> LightThemeColors = {
    {"TextFillColorPrimary", colorWithAlpha("#000000", 228)},
    {"TextFillColorSecondary", colorWithAlpha("#000000", 158)},
    {"TextFillColorTertiary", colorWithAlpha("#000000", 114)},
    {"TextFillColorDisabled", colorWithAlpha("#000000", 92)},
    {"TextFillColorInverse", QColor("#FFFFFF")},
    {"AccentTextFillColorDisabled", colorWithAlpha("#000000", 92)},
    {"TextOnAccentFillColorSelectedText", QColor("#FFFFFF")},
    {"TextOnAccentFillColorPrimary", QColor("#FFFFFF")},
    {"TextOnAccentFillColorSecondary", colorWithAlpha("#FFFFFF", 179)},
    {"TextOnAccentFillColorDisabled", QColor("#FFFFFF")},
    {"ControlFillColorDefault", colorWithAlpha("#FFFFFF", 179)},
    {"ControlFillColorSecondary", colorWithAlpha("#F9F9F9", 128)},
    {"ControlFillColorTertiary", colorWithAlpha("#F9F9F9", 77)},
    {"ControlFillColorDisabled", colorWithAlpha("#F9F9F9", 77)},
    {"ControlFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"ControlFillColorInputActive", QColor("#FFFFFF")},
    {"ControlStrongFillColorDefault", colorWithAlpha("#000000", 114)},
    {"ControlStrongFillColorDisabled", colorWithAlpha("#000000", 81)},
    {"ControlSolidFillColorDefault", QColor("#FFFFFF")},
    {"SubtleFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"SubtleFillColorSecondary", colorWithAlpha("#000000", 9)},
    {"SubtleFillColorTertiary", colorWithAlpha("#000000", 6)},
    {"SubtleFillColorDisabled", colorWithAlpha("#FFFFFF", 0)},
    {"ControlAltFillColorTransparent", colorWithAlpha("#FFFFFF", 0)},
    {"ControlAltFillColorSecondary", colorWithAlpha("#000000", 6)},
    {"ControlAltFillColorTertiary", colorWithAlpha("#000000", 15)},
    {"ControlAltFillColorQuarternary", colorWithAlpha("#000000", 24)},
    {"ControlAltFillColorDisabled", colorWithAlpha("#FFFFFF", 0)},
    {"ControlOnImageFillColorDefault", colorWithAlpha("#FFFFFF", 201)},
    {"ControlOnImageFillColorSecondary", QColor("#F3F3F3")},
    {"ControlOnImageFillColorTertiary", QColor("#EBEBEB")},
    {"ControlOnImageFillColorDisabled", colorWithAlpha("#FFFFFF", 0)},
    {"AccentFillColorDisabled", colorWithAlpha("#000000", 55)},
    {"ControlStrokeColorDefault", colorWithAlpha("#000000", 15)},
    {"ControlStrokeColorSecondary", colorWithAlpha("#000000", 41)},
    {"ControlStrokeColorOnAccentDefault", colorWithAlpha("#FFFFFF", 20)},
    {"ControlStrokeColorOnAccentSecondary", colorWithAlpha("#000000", 102)},
    {"ControlStrokeColorOnAccentTertiary", colorWithAlpha("#000000", 55)},
    {"ControlStrokeColorOnAccentDisabled", colorWithAlpha("#000000", 15)},
    {"ControlStrokeColorForStrongFillWhenOnImage", colorWithAlpha("#FFFFFF", 89)},
    {"CardStrokeColorDefault", colorWithAlpha("#000000", 15)},
    {"CardStrokeColorDefaultSolid", QColor("#EBEBEB")},
    {"ControlStrongStrokeColorDefault", colorWithAlpha("#000000", 114)},
    {"ControlStrongStrokeColorDisabled", colorWithAlpha("#000000", 55)},
    {"SurfaceStrokeColorDefault", colorWithAlpha("#757575", 102)},
    {"SurfaceStrokeColorFlyout", colorWithAlpha("#000000", 15)},
    {"SurfaceStrokeColorInverse", colorWithAlpha("#FFFFFF", 21)},
    {"DividerStrokeColorDefault", colorWithAlpha("#000000", 15)},
    {"FocusStrokeColorOuter", colorWithAlpha("#000000", 228)},
    {"FocusStrokeColorInner", colorWithAlpha("#FFFFFF", 179)},
    {"CardBackgroundFillColorDefault", colorWithAlpha("#FFFFFF", 179)},
    {"CardBackgroundFillColorSecondary", colorWithAlpha("#F6F6F6", 128)},
    {"SmokeFillColorDefault", colorWithAlpha("#000000", 77)},
    {"LayerFillColorDefault", colorWithAlpha("#FFFFFF", 128)},
    {"LayerFillColorAlt", QColor("#FFFFFF")},
    {"LayerOnAcrylicFillColorDefault", colorWithAlpha("#FFFFFF", 64)},
    {"LayerOnAccentAcrylicFillColorDefault", colorWithAlpha("#FFFFFF", 64)},
    {"LayerOnMicaBaseAltFillColorDefault", colorWithAlpha("#FFFFFF", 179)},
    {"LayerOnMicaBaseAltFillColorSecondary", colorWithAlpha("#000000", 10)},
    {"LayerOnMicaBaseAltFillColorTertiary", QColor("#F9F9F9")},
    {"LayerOnMicaBaseAltFillColorTransparent", colorWithAlpha("#000000", 0)},
    {"SolidBackgroundFillColorBase", QColor("#F3F3F3")},
    {"SolidBackgroundFillColorSecondary", QColor("#EEEEEE")},
    {"SolidBackgroundFillColorTertiary", QColor("#F9F9F9")},
    {"SolidBackgroundFillColorQuarternary", QColor("#FFFFFF")},
    {"SolidBackgroundFillColorTransparent", colorWithAlpha("#F3F3F3", 0)},
    {"SolidBackgroundFillColorBaseAlt", QColor("#DADADA")},
    {"SystemFillColorSuccess", QColor("#0F7B0F")},
    {"SystemFillColorCaution", QColor("#9D5D00")},
    {"SystemFillColorCritical", QColor("#C42B1C")},
    {"SystemFillColorNeutral", colorWithAlpha("#000000", 114)},
    {"SystemFillColorSolidNeutral", QColor("#8A8A8A")},
    {"SystemFillColorAttentionBackground", colorWithAlpha("#F6F6F6", 128)},
    {"SystemFillColorSuccessBackground", QColor("#DFF6DD")},
    {"SystemFillColorCautionBackground", QColor("#FFF4CE")},
    {"SystemFillColorCriticalBackground", QColor("#FDE7E9")},
    {"SystemFillColorNeutralBackground", colorWithAlpha("#000000", 6)},
    {"SystemFillColorSolidAttentionBackground", QColor("#F7F7F7")},
    {"SystemFillColorSolidNeutralBackground", QColor("#F3F3F3")},
};

// 颜色常量
class NFluentColorConstants {
public:
    static const QString AccentFillColorDisabled;
    static const QString AccentTextFillColorDisabled;
    static const QString CardBackgroundFillColorDefault;
    static const QString CardBackgroundFillColorSecondary;
    static const QString CardStrokeColorDefault;
    static const QString CardStrokeColorDefaultSolid;
    static const QString ControlAltFillColorDisabled;
    static const QString ControlAltFillColorQuarternary;
    static const QString ControlAltFillColorSecondary;
    static const QString ControlAltFillColorTertiary;
    static const QString ControlAltFillColorTransparent;
    static const QString ControlFillColorDefault;
    static const QString ControlFillColorDisabled;
    static const QString ControlFillColorInputActive;
    static const QString ControlFillColorSecondary;
    static const QString ControlFillColorTertiary;
    static const QString ControlFillColorTransparent;
    static const QString ControlOnImageFillColorDefault;
    static const QString ControlOnImageFillColorDisabled;
    static const QString ControlOnImageFillColorSecondary;
    static const QString ControlOnImageFillColorTertiary;
    static const QString ControlSolidFillColorDefault;
    static const QString ControlStrokeColorDefault;
    static const QString ControlStrokeColorForStrongFillWhenOnImage;
    static const QString ControlStrokeColorOnAccentDefault;
    static const QString ControlStrokeColorOnAccentDisabled;
    static const QString ControlStrokeColorOnAccentSecondary;
    static const QString ControlStrokeColorOnAccentTertiary;
    static const QString ControlStrokeColorSecondary;
    static const QString ControlStrongFillColorDefault;
    static const QString ControlStrongFillColorDisabled;
    static const QString ControlStrongStrokeColorDefault;
    static const QString ControlStrongStrokeColorDisabled;
    static const QString DividerStrokeColorDefault;
    static const QString FocusStrokeColorInner;
    static const QString FocusStrokeColorOuter;
    static const QString LayerFillColorAlt;
    static const QString LayerFillColorDefault;
    static const QString LayerOnAccentAcrylicFillColorDefault;
    static const QString LayerOnAcrylicFillColorDefault;
    static const QString LayerOnMicaBaseAltFillColorDefault;
    static const QString LayerOnMicaBaseAltFillColorSecondary;
    static const QString LayerOnMicaBaseAltFillColorTertiary;
    static const QString LayerOnMicaBaseAltFillColorTransparent;
    static const QString SmokeFillColorDefault;
    static const QString SolidBackgroundFillColorBase;
    static const QString SolidBackgroundFillColorBaseAlt;
    static const QString SolidBackgroundFillColorQuarternary;
    static const QString SolidBackgroundFillColorSecondary;
    static const QString SolidBackgroundFillColorTertiary;
    static const QString SolidBackgroundFillColorTransparent;
    static const QString SubtleFillColorDisabled;
    static const QString SubtleFillColorSecondary;
    static const QString SubtleFillColorTertiary;
    static const QString SubtleFillColorTransparent;
    static const QString SurfaceStrokeColorDefault;
    static const QString SurfaceStrokeColorFlyout;
    static const QString SurfaceStrokeColorInverse;
    static const QString SystemFillColorAttentionBackground;
    static const QString SystemFillColorCaution;
    static const QString SystemFillColorCautionBackground;
    static const QString SystemFillColorCritical;
    static const QString SystemFillColorCriticalBackground;
    static const QString SystemFillColorNeutral;
    static const QString SystemFillColorNeutralBackground;
    static const QString SystemFillColorSolidAttentionBackground;
    static const QString SystemFillColorSolidNeutral;
    static const QString SystemFillColorSolidNeutralBackground;
    static const QString SystemFillColorSuccess;
    static const QString SystemFillColorSuccessBackground;
    static const QString TextFillColorDisabled;
    static const QString TextFillColorInverse;
    static const QString TextFillColorPrimary;
    static const QString TextFillColorSecondary;
    static const QString TextFillColorTertiary;
    static const QString TextOnAccentFillColorDisabled;
    static const QString TextOnAccentFillColorPrimary;
    static const QString TextOnAccentFillColorSecondary;
    static const QString TextOnAccentFillColorSelectedText;
};

inline const QString NFluentColorConstants::AccentFillColorDisabled = "AccentFillColorDisabled";
inline const QString NFluentColorConstants::AccentTextFillColorDisabled = "AccentTextFillColorDisabled";
inline const QString NFluentColorConstants::CardBackgroundFillColorDefault = "CardBackgroundFillColorDefault";
inline const QString NFluentColorConstants::CardBackgroundFillColorSecondary = "CardBackgroundFillColorSecondary";
inline const QString NFluentColorConstants::CardStrokeColorDefault = "CardStrokeColorDefault";
inline const QString NFluentColorConstants::CardStrokeColorDefaultSolid = "CardStrokeColorDefaultSolid";
inline const QString NFluentColorConstants::ControlAltFillColorDisabled = "ControlAltFillColorDisabled";
inline const QString NFluentColorConstants::ControlAltFillColorQuarternary = "ControlAltFillColorQuarternary";
inline const QString NFluentColorConstants::ControlAltFillColorSecondary = "ControlAltFillColorSecondary";
inline const QString NFluentColorConstants::ControlAltFillColorTertiary = "ControlAltFillColorTertiary";
inline const QString NFluentColorConstants::ControlAltFillColorTransparent = "ControlAltFillColorTransparent";
inline const QString NFluentColorConstants::ControlFillColorDefault = "ControlFillColorDefault";
inline const QString NFluentColorConstants::ControlFillColorDisabled = "ControlFillColorDisabled";
inline const QString NFluentColorConstants::ControlFillColorInputActive = "ControlFillColorInputActive";
inline const QString NFluentColorConstants::ControlFillColorSecondary = "ControlFillColorSecondary";
inline const QString NFluentColorConstants::ControlFillColorTertiary = "ControlFillColorTertiary";
inline const QString NFluentColorConstants::ControlFillColorTransparent = "ControlFillColorTransparent";
inline const QString NFluentColorConstants::ControlOnImageFillColorDefault = "ControlOnImageFillColorDefault";
inline const QString NFluentColorConstants::ControlOnImageFillColorDisabled = "ControlOnImageFillColorDisabled";
inline const QString NFluentColorConstants::ControlOnImageFillColorSecondary = "ControlOnImageFillColorSecondary";
inline const QString NFluentColorConstants::ControlOnImageFillColorTertiary = "ControlOnImageFillColorTertiary";
inline const QString NFluentColorConstants::ControlSolidFillColorDefault = "ControlSolidFillColorDefault";
inline const QString NFluentColorConstants::ControlStrokeColorDefault = "ControlStrokeColorDefault";
inline const QString NFluentColorConstants::ControlStrokeColorForStrongFillWhenOnImage = "ControlStrokeColorForStrongFillWhenOnImage";
inline const QString NFluentColorConstants::ControlStrokeColorOnAccentDefault = "ControlStrokeColorOnAccentDefault";
inline const QString NFluentColorConstants::ControlStrokeColorOnAccentDisabled = "ControlStrokeColorOnAccentDisabled";
inline const QString NFluentColorConstants::ControlStrokeColorOnAccentSecondary = "ControlStrokeColorOnAccentSecondary";
inline const QString NFluentColorConstants::ControlStrokeColorOnAccentTertiary = "ControlStrokeColorOnAccentTertiary";
inline const QString NFluentColorConstants::ControlStrokeColorSecondary = "ControlStrokeColorSecondary";
inline const QString NFluentColorConstants::ControlStrongFillColorDefault = "ControlStrongFillColorDefault";
inline const QString NFluentColorConstants::ControlStrongFillColorDisabled = "ControlStrongFillColorDisabled";
inline const QString NFluentColorConstants::ControlStrongStrokeColorDefault = "ControlStrongStrokeColorDefault";
inline const QString NFluentColorConstants::ControlStrongStrokeColorDisabled = "ControlStrongStrokeColorDisabled";
inline const QString NFluentColorConstants::DividerStrokeColorDefault = "DividerStrokeColorDefault";
inline const QString NFluentColorConstants::FocusStrokeColorInner = "FocusStrokeColorInner";
inline const QString NFluentColorConstants::FocusStrokeColorOuter = "FocusStrokeColorOuter";
inline const QString NFluentColorConstants::LayerFillColorAlt = "LayerFillColorAlt";
inline const QString NFluentColorConstants::LayerFillColorDefault = "LayerFillColorDefault";
inline const QString NFluentColorConstants::LayerOnAccentAcrylicFillColorDefault = "LayerOnAccentAcrylicFillColorDefault";
inline const QString NFluentColorConstants::LayerOnAcrylicFillColorDefault = "LayerOnAcrylicFillColorDefault";
inline const QString NFluentColorConstants::LayerOnMicaBaseAltFillColorDefault = "LayerOnMicaBaseAltFillColorDefault";
inline const QString NFluentColorConstants::LayerOnMicaBaseAltFillColorSecondary = "LayerOnMicaBaseAltFillColorSecondary";
inline const QString NFluentColorConstants::LayerOnMicaBaseAltFillColorTertiary = "LayerOnMicaBaseAltFillColorTertiary";
inline const QString NFluentColorConstants::LayerOnMicaBaseAltFillColorTransparent = "LayerOnMicaBaseAltFillColorTransparent";
inline const QString NFluentColorConstants::SmokeFillColorDefault = "SmokeFillColorDefault";
inline const QString NFluentColorConstants::SolidBackgroundFillColorBase = "SolidBackgroundFillColorBase";
inline const QString NFluentColorConstants::SolidBackgroundFillColorBaseAlt = "SolidBackgroundFillColorBaseAlt";
inline const QString NFluentColorConstants::SolidBackgroundFillColorQuarternary = "SolidBackgroundFillColorQuarternary";
inline const QString NFluentColorConstants::SolidBackgroundFillColorSecondary = "SolidBackgroundFillColorSecondary";
inline const QString NFluentColorConstants::SolidBackgroundFillColorTertiary = "SolidBackgroundFillColorTertiary";
inline const QString NFluentColorConstants::SolidBackgroundFillColorTransparent = "SolidBackgroundFillColorTransparent";
inline const QString NFluentColorConstants::SubtleFillColorDisabled = "SubtleFillColorDisabled";
inline const QString NFluentColorConstants::SubtleFillColorSecondary = "SubtleFillColorSecondary";
inline const QString NFluentColorConstants::SubtleFillColorTertiary = "SubtleFillColorTertiary";
inline const QString NFluentColorConstants::SubtleFillColorTransparent = "SubtleFillColorTransparent";
inline const QString NFluentColorConstants::SurfaceStrokeColorDefault = "SurfaceStrokeColorDefault";
inline const QString NFluentColorConstants::SurfaceStrokeColorFlyout = "SurfaceStrokeColorFlyout";
inline const QString NFluentColorConstants::SurfaceStrokeColorInverse = "SurfaceStrokeColorInverse";
inline const QString NFluentColorConstants::SystemFillColorAttentionBackground = "SystemFillColorAttentionBackground";
inline const QString NFluentColorConstants::SystemFillColorCaution = "SystemFillColorCaution";
inline const QString NFluentColorConstants::SystemFillColorCautionBackground = "SystemFillColorCautionBackground";
inline const QString NFluentColorConstants::SystemFillColorCritical = "SystemFillColorCritical";
inline const QString NFluentColorConstants::SystemFillColorCriticalBackground = "SystemFillColorCriticalBackground";
inline const QString NFluentColorConstants::SystemFillColorNeutral = "SystemFillColorNeutral";
inline const QString NFluentColorConstants::SystemFillColorNeutralBackground = "SystemFillColorNeutralBackground";
inline const QString NFluentColorConstants::SystemFillColorSolidAttentionBackground = "SystemFillColorSolidAttentionBackground";
inline const QString NFluentColorConstants::SystemFillColorSolidNeutral = "SystemFillColorSolidNeutral";
inline const QString NFluentColorConstants::SystemFillColorSolidNeutralBackground = "SystemFillColorSolidNeutralBackground";
inline const QString NFluentColorConstants::SystemFillColorSuccess = "SystemFillColorSuccess";
inline const QString NFluentColorConstants::SystemFillColorSuccessBackground = "SystemFillColorSuccessBackground";
inline const QString NFluentColorConstants::TextFillColorDisabled = "TextFillColorDisabled";
inline const QString NFluentColorConstants::TextFillColorInverse = "TextFillColorInverse";
inline const QString NFluentColorConstants::TextFillColorPrimary = "TextFillColorPrimary";
inline const QString NFluentColorConstants::TextFillColorSecondary = "TextFillColorSecondary";
inline const QString NFluentColorConstants::TextFillColorTertiary = "TextFillColorTertiary";
inline const QString NFluentColorConstants::TextOnAccentFillColorDisabled = "TextOnAccentFillColorDisabled";
inline const QString NFluentColorConstants::TextOnAccentFillColorPrimary = "TextOnAccentFillColorPrimary";
inline const QString NFluentColorConstants::TextOnAccentFillColorSecondary = "TextOnAccentFillColorSecondary";
inline const QString NFluentColorConstants::TextOnAccentFillColorSelectedText = "TextOnAccentFillColorSelectedText";

#endif // NFLUENTCOLORS_H
