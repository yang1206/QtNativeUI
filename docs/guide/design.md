# 设计理念

QtNativeUI 旨在将现代化的设计语言带入 Qt 桌面应用开发中。我们的设计核心遵循 **Fluent Design** (流畅设计体系)，强调光感、深度、动效、材质和缩放。

## 核心原则

### 1. 原生质感 (Native Feel)

组件不仅要"看起来"像现代操作系统的一部分，还要"感觉"像。我们花费大量精力调整交互细节，例如：

-   **微交互**: 按钮按下的缩放动画、复选框的平滑过渡。
-   **原生渲染**: 在 Windows 上尽可能贴合 WinUI 3 的视觉标准。
-   **字体渲染**: 优化字体抗锯齿和字重显示。

### 2. 流畅设计 (Fluent Design)

我们实现了 Fluent Design 的关键特征：

-   **Acrylic (亚克力)**: 半透明磨砂材质，让背景内容隐约可见，建立视觉层次。
-   **Mica (云母)**: 一种不透明的动态材质，取色于用户桌面壁纸，让应用窗口与环境融为一体。
-   **Reveal Focus (光照焦点)**: 列表项和按钮边缘会跟随鼠标位置产生光照效果（计划中）。

## 设计系统

QtNativeUI 提供了一套完整的设计令牌（Design Tokens），让开发者无需硬编码样式。

### 颜色 (Colors)

我们要避免使用 `#0078D7` 这样的硬编码颜色，而是使用语义化的颜色变量：

-   `AccentFillColorDefault`: 强调色（默认状态）。
-   `TextFillColorPrimary`: 主要文本色。
-   `ControlFillColorDefault`: 控件背景色。

详见 [颜色系统](/tools/NFluentColors)。

### 排版 (Typography)

我们预定义了标准的字体阶梯，确保信息层级清晰：

| 样式名称 | 大小 (px) | 字重 | 用途 |
| :--- | :--- | :--- | :--- |
| `Display` | 68 | SemiBold | 巨型展示标题 |
| `TitleLarge` | 40 | SemiBold | 页面主标题 |
| `Title` | 28 | SemiBold | 模块标题 |
| `Subtitle` | 20 | SemiBold | 子标题 |
| `BodyLarge` | 16 | Regular | 重要正文 |
| `Body` | 14 | Regular | 标准正文 |
| `Caption` | 12 | Regular | 辅助性说明 |

详见 [NLabel 组件](/components/NLabel)。

### 圆角与间距 (Geometry)

-   **圆角**: 默认控件圆角为 `4px` 或 `8px`，窗口圆角根据系统自适应。
-   **间距**: 使用 `4px` 的倍数作为间距单位（4, 8, 12, 16, 24...）。

## 黑暗模式 (Dark Mode)

QtNativeUI 原生支持黑暗模式。所有组件都内置了两套配色方案。开发者只需调用 `NTheme` 即可一键切换，无需手动编写复杂的样式表。

```cpp
// 自动适配系统明暗模式
nTheme->setThemeMode(NThemeType::ThemeMode::System);
```
