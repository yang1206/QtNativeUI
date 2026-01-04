# NFluentColors

NFluentColors 是一个自动生成的颜色系统头文件，包含 Microsoft Fluent Design 的完整颜色定义。该文件从 Microsoft 官方的 Fluent UI XAML 主题资源自动生成，确保与 Fluent Design 系统的一致性。

## 概述

NFluentColors 提供了 80+ 个预定义的颜色键，涵盖了 Fluent Design 系统中的所有颜色类别。这些颜色自动适配亮色和暗色主题，为应用程序提供一致的视觉体验。

该文件从 Microsoft 官方 Fluent UI XAML 主题资源自动生成。

## 颜色分类

NFluentColors 包含以下颜色类别：

### 文本颜色（Text Fill Colors）

用于文本内容的颜色，支持不同的层级和状态：

| 颜色键 | 用途 | 亮色主题 | 暗色主题 |
|--------|------|---------|---------|
| `TextFillColorPrimary` | 主要文本 | 黑色 (228 alpha) | 白色 |
| `TextFillColorSecondary` | 次要文本 | 黑色 (158 alpha) | 白色 (197 alpha) |
| `TextFillColorTertiary` | 三级文本 | 黑色 (114 alpha) | 白色 (135 alpha) |
| `TextFillColorDisabled` | 禁用文本 | 黑色 (92 alpha) | 白色 (93 alpha) |
| `TextFillColorInverse` | 反色文本 | 白色 | 黑色 (228 alpha) |

### 强调色文本（Text On Accent Colors）

用于强调色背景上的文本：

| 颜色键 | 用途 |
|--------|------|
| `TextOnAccentFillColorPrimary` | 强调色背景上的主要文本 |
| `TextOnAccentFillColorSecondary` | 强调色背景上的次要文本 |
| `TextOnAccentFillColorSelectedText` | 强调色背景上的选中文本 |
| `TextOnAccentFillColorDisabled` | 强调色背景上的禁用文本 |

### 控件填充颜色（Control Fill Colors）

用于控件背景的颜色：

| 颜色键 | 用途 |
|--------|------|
| `ControlFillColorDefault` | 默认控件背景 |
| `ControlFillColorSecondary` | 次要控件背景 |
| `ControlFillColorTertiary` | 三级控件背景 |
| `ControlFillColorDisabled` | 禁用控件背景 |
| `ControlFillColorTransparent` | 透明控件背景 |
| `ControlFillColorInputActive` | 输入框激活状态背景 |

### 控件强调填充（Control Strong Fill Colors）

用于强调的控件背景：

| 颜色键 | 用途 |
|--------|------|
| `ControlStrongFillColorDefault` | 默认强调控件背景 |
| `ControlStrongFillColorDisabled` | 禁用强调控件背景 |
| `ControlSolidFillColorDefault` | 实心控件背景 |

### 控件替代填充（Control Alt Fill Colors）

用于替代控件背景的颜色：

| 颜色键 | 用途 |
|--------|------|
| `ControlAltFillColorTransparent` | 透明替代背景 |
| `ControlAltFillColorSecondary` | 次要替代背景 |
| `ControlAltFillColorTertiary` | 三级替代背景 |
| `ControlAltFillColorQuarternary` | 四级替代背景 |
| `ControlAltFillColorDisabled` | 禁用替代背景 |

### 微妙填充颜色（Subtle Fill Colors）

用于微妙背景效果的颜色：

| 颜色键 | 用途 |
|--------|------|
| `SubtleFillColorTransparent` | 透明微妙背景 |
| `SubtleFillColorSecondary` | 次要微妙背景 |
| `SubtleFillColorTertiary` | 三级微妙背景 |
| `SubtleFillColorDisabled` | 禁用微妙背景 |

### 控件边框颜色（Control Stroke Colors）

用于控件边框的颜色：

| 颜色键 | 用途 |
|--------|------|
| `ControlStrokeColorDefault` | 默认控件边框 |
| `ControlStrokeColorSecondary` | 次要控件边框 |
| `ControlStrokeColorOnAccentDefault` | 强调色上的默认边框 |
| `ControlStrokeColorOnAccentSecondary` | 强调色上的次要边框 |
| `ControlStrokeColorOnAccentTertiary` | 强调色上的三级边框 |
| `ControlStrokeColorOnAccentDisabled` | 强调色上的禁用边框 |

### 卡片颜色（Card Colors）

用于卡片组件的颜色：

| 颜色键 | 用途 |
|--------|------|
| `CardBackgroundFillColorDefault` | 默认卡片背景 |
| `CardBackgroundFillColorSecondary` | 次要卡片背景 |
| `CardStrokeColorDefault` | 默认卡片边框 |
| `CardStrokeColorDefaultSolid` | 实心卡片边框 |

### 层级颜色（Layer Colors）

用于分层效果的颜色：

| 颜色键 | 用途 |
|--------|------|
| `LayerFillColorDefault` | 默认层级背景 |
| `LayerFillColorAlt` | 替代层级背景 |
| `LayerOnAcrylicFillColorDefault` | 亚克力效果上的默认层级 |
| `LayerOnAccentAcrylicFillColorDefault` | 强调色亚克力效果上的层级 |
| `LayerOnMicaBaseAltFillColorDefault` | Mica 效果上的默认层级 |
| `LayerOnMicaBaseAltFillColorSecondary` | Mica 效果上的次要层级 |
| `LayerOnMicaBaseAltFillColorTertiary` | Mica 效果上的三级层级 |

### 背景颜色（Background Colors）

用于应用程序背景的颜色：

| 颜色键 | 用途 |
|--------|------|
| `SolidBackgroundFillColorBase` | 基础背景 |
| `SolidBackgroundFillColorSecondary` | 次要背景 |
| `SolidBackgroundFillColorTertiary` | 三级背景 |
| `SolidBackgroundFillColorQuarternary` | 四级背景 |
| `SolidBackgroundFillColorBaseAlt` | 替代基础背景 |

### 系统状态颜色（System Fill Colors）

用于表示系统状态的颜色：

| 颜色键 | 用途 |
|--------|------|
| `SystemFillColorSuccess` | 成功状态 |
| `SystemFillColorCaution` | 警告状态 |
| `SystemFillColorCritical` | 错误状态 |
| `SystemFillColorNeutral` | 中性状态 |
| `SystemFillColorSolidNeutral` | 实心中性状态 |
| `SystemFillColorAttentionBackground` | 注意背景 |
| `SystemFillColorSuccessBackground` | 成功背景 |
| `SystemFillColorCautionBackground` | 警告背景 |
| `SystemFillColorCriticalBackground` | 错误背景 |
| `SystemFillColorNeutralBackground` | 中性背景 |

### 其他颜色

| 颜色键 | 用途 |
|--------|------|
| `AccentFillColorDisabled` | 禁用强调色填充 |
| `AccentTextFillColorDisabled` | 禁用强调色文本 |
| `DividerStrokeColorDefault` | 分割线颜色 |
| `FocusStrokeColorOuter` | 焦点外边框 |
| `FocusStrokeColorInner` | 焦点内边框 |
| `SmokeFillColorDefault` | 烟雾效果颜色 |
| `SurfaceStrokeColorDefault` | 表面边框 |
| `SurfaceStrokeColorFlyout` | 浮出菜单边框 |
| `SurfaceStrokeColorInverse` | 反色表面边框 |
| `ControlBaseLine` | 控件基线 |

## 公共 API

### 枚举

```cpp
namespace NFluentColorKey {
    enum Key {
        // 80+ 颜色键...
        Count  // 用于计数和边界检查
    };
}
```

### 颜色映射表

```cpp
// 暗色主题颜色映射
static const QMap<NFluentColorKey::Key, QColor> DarkThemeColors;

// 亮色主题颜色映射
static const QMap<NFluentColorKey::Key, QColor> LightThemeColors;
```

### 转换函数

#### 枚举到字符串

```cpp
QString fluentColorKeyToString(NFluentColorKey::Key key);
```

将颜色枚举转换为对应的字符串名称。

**参数**：
- `key`：颜色枚举值

**返回值**：颜色键的字符串表示，如 "TextFillColorPrimary"

**示例**：
```cpp
QString colorName = fluentColorKeyToString(NFluentColorKey::TextFillColorPrimary);
// colorName = "TextFillColorPrimary"
```

#### 字符串到枚举

```cpp
NFluentColorKey::Key stringToFluentColorKey(const QString& keyString);
```

将字符串转换为对应的颜色枚举。

**参数**：
- `keyString`：颜色键的字符串表示

**返回值**：对应的颜色枚举，如果无效则返回 `NFluentColorKey::Count`

**示例**：
```cpp
NFluentColorKey::Key key = stringToFluentColorKey("TextFillColorPrimary");
// key = NFluentColorKey::TextFillColorPrimary
```

### Alpha 值辅助函数

#### 创建带 Alpha 值的颜色

```cpp
inline QColor colorWithAlpha(int r, int g, int b, int alpha);
inline QColor colorWithAlpha(const QColor& baseColor, int alpha);
```

创建具有指定 Alpha 值（0-255）的颜色。

**参数**：
- `r, g, b`：RGB 分量（0-255）
- `baseColor`：基础颜色
- `alpha`：Alpha 值（0-255，0 为完全透明，255 为完全不透明）

**示例**：
```cpp
// 创建半透明白色
QColor semiTransparentWhite = colorWithAlpha(255, 255, 255, 128);

// 从现有颜色创建半透明版本
QColor baseColor = QColor(255, 0, 0);
QColor semiTransparent = colorWithAlpha(baseColor, 128);
```

#### 创建带百分比 Alpha 值的颜色

```cpp
inline QColor colorWithAlphaPercent(int r, int g, int b, int alphaPercent);
inline QColor colorWithAlphaPercent(const QColor& baseColor, int alphaPercent);
```

创建具有指定 Alpha 百分比（0-100）的颜色。

**参数**：
- `r, g, b`：RGB 分量（0-255）
- `baseColor`：基础颜色
- `alphaPercent`：Alpha 百分比（0-100）

**示例**：
```cpp
// 创建 50% 透明的白色
QColor semiTransparentWhite = colorWithAlphaPercent(255, 255, 255, 50);

// 从现有颜色创建 75% 透明版本
QColor baseColor = QColor(0, 0, 255);
QColor transparent = colorWithAlphaPercent(baseColor, 75);
```

## 使用示例

### 基础用法

```cpp
#include <QtNativeUI/NFluentColors.h>
#include <QPainter>

void drawButton(QPainter& painter) {
    // 获取当前主题的按钮背景颜色
    QColor backgroundColor = DarkThemeColors[NFluentColorKey::ControlFillColorDefault];
    
    // 获取文本颜色
    QColor textColor = DarkThemeColors[NFluentColorKey::TextFillColorPrimary];
    
    // 绘制按钮
    painter.fillRect(0, 0, 100, 40, backgroundColor);
    painter.setPen(textColor);
    painter.drawText(0, 0, 100, 40, Qt::AlignCenter, "Button");
}
```

### 主题切换

```cpp
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NTheme.h>

void updateUIColors(bool isDarkMode) {
    const QMap<NFluentColorKey::Key, QColor>& colors = 
        isDarkMode ? DarkThemeColors : LightThemeColors;
    
    // 使用颜色更新 UI
    QColor backgroundColor = colors[NFluentColorKey::SolidBackgroundFillColorBase];
    QColor textColor = colors[NFluentColorKey::TextFillColorPrimary];
    
    // 应用到窗口
    QPalette palette;
    palette.setColor(QPalette::Window, backgroundColor);
    palette.setColor(QPalette::WindowText, textColor);
    window->setPalette(palette);
}
```

### 动态颜色获取

```cpp
#include <QtNativeUI/NFluentColors.h>

QColor getControlColor(NFluentColorKey::Key colorKey, bool isDarkMode) {
    const QMap<NFluentColorKey::Key, QColor>& colors = 
        isDarkMode ? DarkThemeColors : LightThemeColors;
    
    if (colors.contains(colorKey)) {
        return colors[colorKey];
    }
    
    // 返回默认颜色
    return QColor(128, 128, 128);
}

// 使用
QColor buttonBg = getControlColor(NFluentColorKey::ControlFillColorDefault, true);
```

### 字符串到颜色的转换

```cpp
#include <QtNativeUI/NFluentColors.h>

QColor getColorByName(const QString& colorName, bool isDarkMode) {
    NFluentColorKey::Key key = stringToFluentColorKey(colorName);
    
    if (key == NFluentColorKey::Count) {
        // 无效的颜色名称
        return QColor();
    }
    
    const QMap<NFluentColorKey::Key, QColor>& colors = 
        isDarkMode ? DarkThemeColors : LightThemeColors;
    
    return colors[key];
}

// 使用
QColor color = getColorByName("TextFillColorPrimary", true);
```

### 与 NTheme 集成

```cpp
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NTheme.h>

void setupButtonStyle(QPushButton* button) {
    bool isDarkMode = NTheme::instance()->themeMode() == NThemeType::Dark;
    
    const QMap<NFluentColorKey::Key, QColor>& colors = 
        isDarkMode ? DarkThemeColors : LightThemeColors;
    
    QColor bgColor = colors[NFluentColorKey::ControlFillColorDefault];
    QColor textColor = colors[NFluentColorKey::TextFillColorPrimary];
    QColor borderColor = colors[NFluentColorKey::ControlStrokeColorDefault];
    
    // 应用样式
    QString styleSheet = QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "}"
    ).arg(bgColor.name(), textColor.name(), borderColor.name());
    
    button->setStyleSheet(styleSheet);
}
```

## 颜色生成脚本

该文件由 `tools/color_generator.py` 从 Microsoft 官方 Fluent UI XAML 主题资源自动生成。

### 运行脚本

```bash
python3 tools/color_generator.py <output_file> [xaml_file] [custom_colors_file]
```

**参数**：
- `output_file`：输出的 C++ 头文件路径
- `xaml_file`（可选）：XAML 源文件路径。如果不提供，将从 Microsoft GitHub 下载
- `custom_colors_file`（可选）：自定义颜色配置文件路径，默认为 `tools/custom_colors.json`

### 自定义颜色

编辑 `tools/custom_colors.json` 添加自定义颜色：

```json
{
  "custom_colors": {
    "MyCustomColor": {
      "dark": "QColor(255, 100, 100)",
      "light": "QColor(200, 50, 50)"
    }
  }
}
```

### 强制重新下载

```bash
python3 tools/color_generator.py output.h --force-download
```

## 最佳实践

1. **使用枚举而非硬编码颜色**：始终使用 `NFluentColorKey` 枚举而非硬编码 RGB 值，以便在主题更新时自动适配。

2. **主题感知**：根据当前主题选择合适的颜色映射表：
   ```cpp
   bool isDarkMode = NTheme::instance()->themeMode() == NThemeType::Dark;
   const auto& colors = isDarkMode ? DarkThemeColors : LightThemeColors;
   ```

3. **Alpha 值管理**：使用辅助函数创建透明颜色，而非手动计算 Alpha 值。

4. **颜色一致性**：对于相同用途的 UI 元素，使用相同的颜色键，确保视觉一致性。

5. **性能考虑**：颜色映射表是静态的，查询性能很高，可以频繁使用。

6. **与 NTheme 配合**：NFluentColors 提供基础颜色定义，NTheme 提供主题管理和强调色支持。两者配合使用可实现完整的主题系统。

## 常见问题

**Q: 如何在运行时切换主题颜色？**

A: 使用 NTheme 的主题切换功能。当主题改变时，重新查询 DarkThemeColors 或 LightThemeColors 映射表即可获得新的颜色。

**Q: 能否添加自定义颜色？**

A: 可以。编辑 `tools/custom_colors.json` 文件添加自定义颜色定义，然后重新运行生成脚本。

**Q: 为什么某些系统颜色显示为 (255, 0, 255)？**

A: 这些是占位符颜色，表示需要从系统获取的颜色。在实际应用中，应该通过 NTheme 或其他方式获取真实的系统颜色。

**Q: NFluentColors 和 NColors 有什么区别？**

A: NColors 提供基础颜色系统（灰度、强调色等），NFluentColors 提供 Fluent Design 的完整颜色定义。NFluentColors 是更高级的、与 Microsoft 官方设计系统对齐的颜色系统。

**Q: 如何获取特定状态下的颜色？**

A: 使用对应的颜色键。例如，禁用状态使用 `ControlFillColorDisabled`，悬停状态可以使用 `ControlFillColorSecondary` 等。
