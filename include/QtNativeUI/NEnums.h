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
enum Radius {
    // 圆角大小
    CornerRadiusNone,
    CornerRadiusSmall,
    CornerRadiusDefault,
    CornerRadiusMedium,
    CornerRadiusLarge,
    CornerRadiusCircular,
};
Q_ENUM_CREATE(Radius)

enum Spacing {
    // 间距
    SpacingNone,
    SpacingXS,
    SpacingS,
    SpacingM,
    SpacingL,
    SpacingXL,
    SpacingXXL,
    SpacingXXXL,
};
Q_ENUM_CREATE(Spacing)

enum FontSize {
    // 字体大小
    FontSizeCaption,
    FontSizeBody,
    FontSizeBodyLarge,
    FontSizeSubTitle,
    FontSizeTitle,
    FontSizeTitleLarge,
    FontSizeDisplay,
};
Q_ENUM_CREATE(FontSize)

enum FontWeight {
    // 字重
    FontWeightRegular,
    FontWeightMedium,
    FontWeightSemibold,
    FontWeightBold,
};

Q_ENUM_CREATE(FontWeight)

enum Elevation {
    // 阴影层级
    ElevationNone,
    ElevationRest,
    ElevationHover,
    ElevationFlyout,
    ElevationDialog,
};

Q_ENUM_CREATE(Elevation)

enum AnimationDuration {
    // 动效时长
    AnimationFast,
    AnimationNormal,
    AnimationSlow,
    AnimationVerySlow,
};
Q_ENUM_CREATE(AnimationDuration)

enum AnimationEasing {
    EasingStandard,
    EasingAccelerate,
    EasingDecelerate,
    EasingLinear,
};
Q_ENUM_CREATE(AnimationEasing)

Q_END_ENUM_CREATE(NDesignTokenKey)

Q_BEGIN_ENUM_CREATE(NLabelType)
enum Type {
    Caption,         // 说明文本 (12px 常规)
    Body,            // 正文文本 (14px 常规)
    BodyStrong,      // 强调正文 (14px 半粗体)
    BodyLarge,       // 大号正文 (16px 常规)
    BodyStrongLarge, // 大号正文 (16px 半粗体)
    Subtitle,        // 副标题 (20px 半粗体)
    Title,           // 标题 (28px 半粗体)
    TitleLarge,      // 大号标题 (40px 半粗体)
    Display,         // 展示型标题 (68px 半粗体)
    Default          // 默认样式 (使用系统默认)
};
Q_ENUM_CREATE(Type)
Q_END_ENUM_CREATE(NLabelType)

// 导航相关枚举
Q_BEGIN_ENUM_CREATE(NNavigationType)

enum NavigationDisplayMode {
    Auto,    // 自动模式，根据窗口大小自动调整显示模式
    Minimal, // 最小化模式，导航栏隐藏
    Compact, // 紧凑模式，只显示图标
    Maximal  // 最大化模式，显示完整导航栏
};
Q_ENUM_CREATE(NavigationDisplayMode)

enum NavigationNodeType {
    PageNode,  // 页面节点
    FooterNode // 页脚节点
};
Q_ENUM_CREATE(NavigationNodeType)

enum NodeOperateReturnType {
    Success,              // 操作成功
    PageInvalid,          // 页面无效
    TargetNodeInvalid,    // 目标节点无效
    TargetNodeTypeError,  // 目标节点类型错误
    TargetNodeDepthLimit, // 目标节点深度超限
    FooterUpperLimit,
};
Q_ENUM_CREATE(NodeOperateReturnType)

enum PageTransitionType {
    NoTransition,    // 无过渡效果
    FadeTransition,  // 淡入淡出
    SlideHorizontal, // 水平滑动
    SlideVertical,   // 垂直滑动
    ZoomTransition   // 缩放过渡
};
Q_ENUM_CREATE(PageTransitionType)

Q_END_ENUM_CREATE(NNavigationType)

// 导航路由相关枚举
Q_BEGIN_ENUM_CREATE(NNavigationRouterType)

enum NavigationRouteType {
    Success,             // 操作成功
    ObjectInvalid,       // 对象无效
    FunctionNameInvalid, // 函数名无效
    PageNotFound,
    RouteRejected,
    NoHistory

};
Q_ENUM_CREATE(NavigationRouteType)

Q_END_ENUM_CREATE(NNavigationRouterType)

#endif // QTNATIVEUI_NENUMS_H