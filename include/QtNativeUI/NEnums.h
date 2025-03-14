#ifndef QTNATIVEUI_NENUMS_H
#define QTNATIVEUI_NENUMS_H
#include <QObject>
#include "stdafx.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define Q_BEGIN_ENUM_CREATE(CLASS)                                                                                     \
    namespace CLASS {                                                                                                  \
    Q_NAMESPACE_EXPORT(QTNATIVEUI_EXPORT)

#define Q_END_ENUM_CREATE(CLASS) }

#define Q_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)
#else
#define Q_BEGIN_ENUM_CREATE(CLASS)                                                                                     \
    class QTNATIVEUI_EXPORT CLASS : public QObject {                                                                   \
        Q_OBJECT                                                                                                       \
      public:

#define Q_END_ENUM_CREATE(CLASS)                                                                                       \
  private:                                                                                                             \
    Q_DISABLE_COPY(CLASS)                                                                                              \
    }                                                                                                                  \
    ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif

// 主题模式枚举
Q_BEGIN_ENUM_CREATE(NThemeType)

enum ThemeMode {
    Light, // 亮色主题
    Dark,  // 暗色主题
    System // 跟随系统主题
};
Q_ENUM_CREATE(ThemeMode)

Q_END_ENUM_CREATE(NThemeType)

// 设计令牌枚举
Q_BEGIN_ENUM_CREATE(NDesignTokenKey)
enum Key {
    // 圆角大小
    CornerRadiusNone,
    CornerRadiusSmall,
    CornerRadiusDefault,
    CornerRadiusMedium,
    CornerRadiusLarge,
    CornerRadiusCircular,

    // 间距
    SpacingNone,
    SpacingXS,
    SpacingS,
    SpacingM,
    SpacingL,
    SpacingXL,
    SpacingXXL,
    SpacingXXXL,

    // 字体大小
    FontSizeCaption,
    FontSizeBody,
    FontSizeBodyLarge,
    FontSizeTitle,
    FontSizeTitleLarge,
    FontSizeHeader,
    FontSizeHeaderLarge,

    // 字重
    FontWeightRegular,
    FontWeightMedium,
    FontWeightSemibold,
    FontWeightBold,

    // 阴影层级 - 转换为 Qt 友好的格式
    ElevationNone,
    ElevationRest,
    ElevationHover,
    ElevationFlyout,
    ElevationDialog,

    // 动效时长
    AnimationFast,
    AnimationNormal,
    AnimationSlow,
    AnimationVerySlow,

    // 缓动曲线 - 转换为 Qt 友好的格式
    EasingStandard,
    EasingAccelerate,
    EasingDecelerate,
    EasingLinear,

    Count // 用于计数和边界检查
};
Q_ENUM_CREATE(Key)
Q_END_ENUM_CREATE(NDesignTokenKey)

// 枚举到字符串的转换函数
inline QString designTokenKeyToString(NDesignTokenKey::Key key) {
    switch (key) {
        // 圆角大小
        case NDesignTokenKey::CornerRadiusNone:
            return "cornerRadiusNone";
        case NDesignTokenKey::CornerRadiusSmall:
            return "cornerRadiusSmall";
        case NDesignTokenKey::CornerRadiusDefault:
            return "cornerRadiusDefault";
        case NDesignTokenKey::CornerRadiusMedium:
            return "cornerRadiusMedium";
        case NDesignTokenKey::CornerRadiusLarge:
            return "cornerRadiusLarge";
        case NDesignTokenKey::CornerRadiusCircular:
            return "cornerRadiusCircular";

        // 间距
        case NDesignTokenKey::SpacingNone:
            return "spacingNone";
        case NDesignTokenKey::SpacingXS:
            return "spacingXS";
        case NDesignTokenKey::SpacingS:
            return "spacingS";
        case NDesignTokenKey::SpacingM:
            return "spacingM";
        case NDesignTokenKey::SpacingL:
            return "spacingL";
        case NDesignTokenKey::SpacingXL:
            return "spacingXL";
        case NDesignTokenKey::SpacingXXL:
            return "spacingXXL";
        case NDesignTokenKey::SpacingXXXL:
            return "spacingXXXL";

        // 字体大小
        case NDesignTokenKey::FontSizeCaption:
            return "fontSizeCaption";
        case NDesignTokenKey::FontSizeBody:
            return "fontSizeBody";
        case NDesignTokenKey::FontSizeBodyLarge:
            return "fontSizeBodyLarge";
        case NDesignTokenKey::FontSizeTitle:
            return "fontSizeTitle";
        case NDesignTokenKey::FontSizeTitleLarge:
            return "fontSizeTitleLarge";
        case NDesignTokenKey::FontSizeHeader:
            return "fontSizeHeader";
        case NDesignTokenKey::FontSizeHeaderLarge:
            return "fontSizeHeaderLarge";

        // 字重
        case NDesignTokenKey::FontWeightRegular:
            return "fontWeightRegular";
        case NDesignTokenKey::FontWeightMedium:
            return "fontWeightMedium";
        case NDesignTokenKey::FontWeightSemibold:
            return "fontWeightSemibold";
        case NDesignTokenKey::FontWeightBold:
            return "fontWeightBold";

        // 阴影层级
        case NDesignTokenKey::ElevationNone:
            return "elevationNone";
        case NDesignTokenKey::ElevationRest:
            return "elevationRest";
        case NDesignTokenKey::ElevationHover:
            return "elevationHover";
        case NDesignTokenKey::ElevationFlyout:
            return "elevationFlyout";
        case NDesignTokenKey::ElevationDialog:
            return "elevationDialog";

        // 动效时长
        case NDesignTokenKey::AnimationFast:
            return "animationFast";
        case NDesignTokenKey::AnimationNormal:
            return "animationNormal";
        case NDesignTokenKey::AnimationSlow:
            return "animationSlow";
        case NDesignTokenKey::AnimationVerySlow:
            return "animationVerySlow";

        // 缓动曲线
        case NDesignTokenKey::EasingStandard:
            return "easingStandard";
        case NDesignTokenKey::EasingAccelerate:
            return "easingAccelerate";
        case NDesignTokenKey::EasingDecelerate:
            return "easingDecelerate";
        case NDesignTokenKey::EasingLinear:
            return "easingLinear";

        case NDesignTokenKey::Count:
            return "";
    }
    return "";
}

// 字符串到枚举的转换函数
inline NDesignTokenKey::Key stringToDesignTokenKey(const QString& keyString) {
    // 圆角大小
    if (keyString == "cornerRadiusNone")
        return NDesignTokenKey::CornerRadiusNone;
    if (keyString == "cornerRadiusSmall")
        return NDesignTokenKey::CornerRadiusSmall;
    if (keyString == "cornerRadiusDefault")
        return NDesignTokenKey::CornerRadiusDefault;
    if (keyString == "cornerRadiusMedium")
        return NDesignTokenKey::CornerRadiusMedium;
    if (keyString == "cornerRadiusLarge")
        return NDesignTokenKey::CornerRadiusLarge;
    if (keyString == "cornerRadiusCircular")
        return NDesignTokenKey::CornerRadiusCircular;

    // 间距
    if (keyString == "spacingNone")
        return NDesignTokenKey::SpacingNone;
    if (keyString == "spacingXS")
        return NDesignTokenKey::SpacingXS;
    if (keyString == "spacingS")
        return NDesignTokenKey::SpacingS;
    if (keyString == "spacingM")
        return NDesignTokenKey::SpacingM;
    if (keyString == "spacingL")
        return NDesignTokenKey::SpacingL;
    if (keyString == "spacingXL")
        return NDesignTokenKey::SpacingXL;
    if (keyString == "spacingXXL")
        return NDesignTokenKey::SpacingXXL;
    if (keyString == "spacingXXXL")
        return NDesignTokenKey::SpacingXXXL;

    // 字体大小
    if (keyString == "fontSizeCaption")
        return NDesignTokenKey::FontSizeCaption;
    if (keyString == "fontSizeBody")
        return NDesignTokenKey::FontSizeBody;
    if (keyString == "fontSizeBodyLarge")
        return NDesignTokenKey::FontSizeBodyLarge;
    if (keyString == "fontSizeTitle")
        return NDesignTokenKey::FontSizeTitle;
    if (keyString == "fontSizeTitleLarge")
        return NDesignTokenKey::FontSizeTitleLarge;
    if (keyString == "fontSizeHeader")
        return NDesignTokenKey::FontSizeHeader;
    if (keyString == "fontSizeHeaderLarge")
        return NDesignTokenKey::FontSizeHeaderLarge;

    // 字重
    if (keyString == "fontWeightRegular")
        return NDesignTokenKey::FontWeightRegular;
    if (keyString == "fontWeightMedium")
        return NDesignTokenKey::FontWeightMedium;
    if (keyString == "fontWeightSemibold")
        return NDesignTokenKey::FontWeightSemibold;
    if (keyString == "fontWeightBold")
        return NDesignTokenKey::FontWeightBold;

    // 阴影层级
    if (keyString == "elevationNone")
        return NDesignTokenKey::ElevationNone;
    if (keyString == "elevationRest")
        return NDesignTokenKey::ElevationRest;
    if (keyString == "elevationHover")
        return NDesignTokenKey::ElevationHover;
    if (keyString == "elevationFlyout")
        return NDesignTokenKey::ElevationFlyout;
    if (keyString == "elevationDialog")
        return NDesignTokenKey::ElevationDialog;

    // 动效时长
    if (keyString == "animationFast")
        return NDesignTokenKey::AnimationFast;
    if (keyString == "animationNormal")
        return NDesignTokenKey::AnimationNormal;
    if (keyString == "animationSlow")
        return NDesignTokenKey::AnimationSlow;
    if (keyString == "animationVerySlow")
        return NDesignTokenKey::AnimationVerySlow;

    // 缓动曲线
    if (keyString == "easingStandard")
        return NDesignTokenKey::EasingStandard;
    if (keyString == "easingAccelerate")
        return NDesignTokenKey::EasingAccelerate;
    if (keyString == "easingDecelerate")
        return NDesignTokenKey::EasingDecelerate;
    if (keyString == "easingLinear")
        return NDesignTokenKey::EasingLinear;

    return NDesignTokenKey::Count; // 无效的键
}

#endif // QTNATIVEUI_NENUMS_H