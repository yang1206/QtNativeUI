# NEnums 枚举定义

### 介绍

NEnums 定义了 QtNativeUI 中使用的各种枚举类型，包括主题模式、设计令牌、导航类型等。这些枚举提供了类型安全的方式来指定各种配置和状态。

### 主题模式

```cpp
#include <QtNativeUI/NEnums.h>

// 主题模式枚举
enum NThemeType::ThemeMode {
    Light,   // 明亮主题
    Dark,    // 暗黑主题
    System   // 跟随系统主题
};

// 使用示例
nTheme->setThemeMode(NThemeType::ThemeMode::Light);
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
nTheme->setThemeMode(NThemeType::ThemeMode::System);
```

### 设计令牌

设计令牌定义了 UI 设计中的各种参数，确保整个应用的视觉一致性。

#### 圆角

```cpp
enum NDesignTokenKey::Radius {
    CornerRadiusNone,      // 无圆角
    CornerRadiusSmall,     // 小圆角
    CornerRadiusDefault,   // 默认圆角
    CornerRadiusMedium,    // 中等圆角
    CornerRadiusLarge,     // 大圆角
    CornerRadiusCircular   // 圆形
};

// 使用示例
int radius = nTheme->getRadius(NDesignTokenKey::CornerRadiusDefault).toInt();
```

#### 间距

```cpp
enum NDesignTokenKey::Spacing {
    SpacingNone,   // 无间距
    SpacingXS,     // 超小间距
    SpacingS,      // 小间距
    SpacingM,      // 中等间距
    SpacingL,      // 大间距
    SpacingXL,     // 超大间距
    SpacingXXL,    // 特大间距
    SpacingXXXL    // 极大间距
};

// 使用示例
int spacing = nTheme->getSpacing(NDesignTokenKey::SpacingM).toInt();
```

#### 字体大小

```cpp
enum NDesignTokenKey::FontSize {
    FontSizeCaption,      // 说明文本 (12px)
    FontSizeBody,         // 正文文本 (14px)
    FontSizeBodyLarge,    // 大号正文 (16px)
    FontSizeSubTitle,     // 副标题 (20px)
    FontSizeTitle,        // 标题 (28px)
    FontSizeTitleLarge,   // 大号标题 (40px)
    FontSizeDisplay       // 展示型标题 (68px)
};

// 使用示例
int fontSize = nTheme->getFontSize(NDesignTokenKey::FontSizeBody).toInt();
```

#### 字重

```cpp
enum NDesignTokenKey::FontWeight {
    FontWeightRegular,    // 常规 (400)
    FontWeightMedium,     // 中等 (500)
    FontWeightSemibold,   // 半粗体 (600)
    FontWeightBold        // 粗体 (700)
};

// 使用示例
int fontWeight = nTheme->getFontWeight(NDesignTokenKey::FontWeightMedium).toInt();
```

#### 阴影层级

```cpp
enum NDesignTokenKey::Elevation {
    ElevationNone,     // 无阴影
    ElevationRest,     // 静止状态阴影
    ElevationHover,    // 悬停状态阴影
    ElevationFlyout,   // 浮出菜单阴影
    ElevationDialog    // 对话框阴影
};

// 使用示例
QVariantMap elevation = nTheme->getElevation(NDesignTokenKey::ElevationRest).toMap();
```

#### 动画时长

```cpp
enum NDesignTokenKey::AnimationDuration {
    AnimationFast,      // 快速 (150ms)
    AnimationNormal,    // 正常 (300ms)
    AnimationSlow,      // 缓慢 (500ms)
    AnimationVerySlow   // 非常缓慢 (1000ms)
};

// 使用示例
int duration = nTheme->getAnimationDuration(NDesignTokenKey::AnimationNormal).toInt();
```

#### 动画缓动

```cpp
enum NDesignTokenKey::AnimationEasing {
    EasingStandard,     // 标准缓动
    EasingAccelerate,   // 加速缓动
    EasingDecelerate,   // 减速缓动
    EasingLinear        // 线性缓动
};

// 使用示例
QString easing = nTheme->getAnimationEasing(NDesignTokenKey::EasingStandard).toString();
```

### 信息栏类型

```cpp
enum NInfoBarType::PositionPolicy {
    Top,         // 顶部居中
    TopLeft,     // 左上角
    TopRight,    // 右上角
    Bottom,      // 底部居中
    BottomLeft,  // 左下角
    BottomRight, // 右下角
    Left,        // 左侧居中
    Right        // 右侧居中
};

enum NInfoBarType::InfoMode {
    Information, // 信息
    Success,     // 成功
    Warning,     // 警告
    Error        // 错误
};

// 使用示例
NInfoBar* infoBar = new NInfoBar();
infoBar->setMode(NInfoBarType::InfoMode::Success);
infoBar->setPosition(NInfoBarType::PositionPolicy::TopRight);
```

### 标签类型

```cpp
enum NLabelType::Type {
    Caption,         // 说明文本 (12px 常规)
    Body,            // 正文文本 (14px 常规)
    BodyStrong,      // 强调正文 (14px 半粗体)
    BodyLarge,       // 大号正文 (16px 常规)
    BodyStrongLarge, // 大号正文 (16px 半粗体)
    Subtitle,        // 副标题 (20px 半粗体)
    Title,           // 标题 (28px 半粗体)
    TitleLarge,      // 大号标题 (40px 半粗体)
    Display,         // 展示型标题 (68px 半粗体)
    Default          // 默认样式
};

// 使用示例
NLabel* label = new NLabel("标题文本");
label->setType(NLabelType::Type::Title);
```

### 导航类型

#### 导航显示模式

```cpp
enum NNavigationType::NavigationDisplayMode {
    Auto,    // 自动模式，根据窗口大小自动调整
    Minimal, // 最小化模式，导航栏隐藏
    Compact, // 紧凑模式，只显示图标
    Maximal  // 最大化模式，显示完整导航栏
};

// 使用示例
navigationView->setDisplayMode(NNavigationType::NavigationDisplayMode::Auto);
```

#### 导航节点类型

```cpp
enum NNavigationType::NavigationNodeType {
    PageNode,  // 页面节点
    FooterNode // 页脚节点
};
```

#### 页面过渡类型

```cpp
enum NNavigationType::PageTransitionType {
    NoTransition,     // 无过渡效果
    PopupTransition,  // 弹出过渡
    ScaleTransition,  // 缩放过渡
    FlipTransition,   // 翻转过渡
    BlurTransition,   // 模糊过渡
    CubeTransition,   // 立方体翻转
    RippleTransition  // 波纹扩散
};

// 使用示例
stackedWidget->doPageSwitch(
    NNavigationType::PageTransitionType::SlideLeft,
    targetIndex,
    false,
    300
);
```

#### 导航操作返回类型

```cpp
enum NNavigationType::NodeOperateReturnType {
    Success,              // 操作成功
    PageInvalid,          // 页面无效
    TargetNodeInvalid,    // 目标节点无效
    TargetNodeTypeError,  // 目标节点类型错误
    TargetNodeDepthLimit, // 目标节点深度超限
    FooterUpperLimit      // 页脚上限
};
```

### 导航路由类型

```cpp
enum NNavigationRouterType::NavigationRouteType {
    Success,             // 操作成功
    ObjectInvalid,       // 对象无效
    FunctionNameInvalid, // 函数名无效
    PageNotFound,        // 页面未找到
    RouteRejected,       // 路由被拒绝
    NoHistory            // 无历史记录
};
```

## 最佳实践

### 1. 使用设计令牌保持一致性

```cpp
// 推荐：使用设计令牌
int radius = nTheme->getRadius(NDesignTokenKey::CornerRadiusDefault).toInt();
int spacing = nTheme->getSpacing(NDesignTokenKey::SpacingM).toInt();

// 避免：硬编码数值
// int radius = 8;
// int spacing = 16;
```

### 2. 使用枚举而不是魔法数字

```cpp
// 推荐：使用枚举
nTheme->setThemeMode(NThemeType::ThemeMode::System);

// 避免：使用数字
// nTheme->setThemeMode(2);
```

### 3. 根据设计令牌调整 UI

```cpp
// 推荐：根据令牌调整
int fontSize = nTheme->getFontSize(NDesignTokenKey::FontSizeBody).toInt();
label->setFont(QFont("Segoe UI", fontSize));

// 避免：硬编码字体大小
// label->setFont(QFont("Segoe UI", 14));
```

## 完整参考表

| 枚举类型 | 值 | 说明 |
|---------|-----|------|
| `NThemeType::ThemeMode` | Light, Dark, System | 主题模式 |
| `NDesignTokenKey::Radius` | 6 个值 | 圆角大小 |
| `NDesignTokenKey::Spacing` | 8 个值 | 间距大小 |
| `NDesignTokenKey::FontSize` | 7 个值 | 字体大小 |
| `NDesignTokenKey::FontWeight` | 4 个值 | 字重 |
| `NDesignTokenKey::Elevation` | 5 个值 | 阴影层级 |
| `NDesignTokenKey::AnimationDuration` | 4 个值 | 动画时长 |
| `NDesignTokenKey::AnimationEasing` | 4 个值 | 动画缓动 |
| `NInfoBarType::PositionPolicy` | 8 个值 | 信息栏位置 |
| `NInfoBarType::InfoMode` | 4 个值 | 信息栏模式 |
| `NLabelType::Type` | 10 个值 | 标签类型 |
| `NNavigationType::NavigationDisplayMode` | 4 个值 | 导航显示模式 |
| `NNavigationType::PageTransitionType` | 7 个值 | 页面过渡类型 |
