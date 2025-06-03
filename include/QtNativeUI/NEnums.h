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

Q_BEGIN_ENUM_CREATE(NInfoBarType)
enum PositionPolicy {
    Top,         // 顶部居中
    TopLeft,     // 左上角
    TopRight,    // 右上角
    Bottom,      // 底部居中
    BottomLeft,  // 左下角
    BottomRight, // 右下角
    Left,        // 左侧居中
    Right        // 右侧居中
};

Q_ENUM_CREATE(PositionPolicy)
enum InfoMode {
    Information, // 信息
    Success,     // 成功
    Warning,     // 警告
    Error        // 错误
};
Q_ENUM_CREATE(InfoMode)

Q_END_ENUM_CREATE(NInfoBarType)

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

    // 阴影层级
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

    // 缓动曲线
    EasingStandard,
    EasingAccelerate,
    EasingDecelerate,
    EasingLinear,

    Count // 用于计数和边界检查
};
Q_ENUM_CREATE(Key)
Q_END_ENUM_CREATE(NDesignTokenKey)

#endif // QTNATIVEUI_NENUMS_H