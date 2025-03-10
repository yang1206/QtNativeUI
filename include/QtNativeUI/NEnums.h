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

enum ColorRole {
    // 文本颜色
    TextPrimary,   // 主要文本
    TextSecondary, // 次要文本
    TextTertiary,  // 第三级文本
    TextDisabled,  // 禁用文本
    TextOnAccent,  // 强调色上的文本
    TextLink,      // 链接文本

    // 背景颜色
    Background,          // 主背景
    BackgroundSecondary, // 次级背景
    Surface,             // 表面
    SurfaceSecondary,    // 次级表面

    // 强调色
    AccentPrimary,   // 主要强调色
    AccentSecondary, // 次级强调色
    AccentTertiary,  // 第三级强调色

    // 控件颜色
    ControlBackground,         // 控件背景
    ControlBackgroundHover,    // 悬停状态控件背景
    ControlBackgroundPressed,  // 按下状态控件背景
    ControlBackgroundSelected, // 选中状态控件背景
    ControlBackgroundDisabled, // 禁用状态控件背景

    ControlBorder,         // 控件边框
    ControlBorderHover,    // 悬停状态控件边框
    ControlBorderPressed,  // 按下状态控件边框
    ControlBorderSelected, // 选中状态控件边框
    ControlBorderDisabled, // 禁用状态控件边框

    // 状态颜色
    Success, // 成功
    Warning, // 警告
    Error,   // 错误
    Info,    // 信息

    // 分隔线颜色
    Divider, // 分隔线

    // 阴影颜色
    Shadow, // 阴影

    // 计数
    ColorRoleCount // 颜色角色总数
};
Q_ENUM_CREATE(ColorRole)

Q_END_ENUM_CREATE(NThemeType)

#endif // QTNATIVEUI_NENUMS_H