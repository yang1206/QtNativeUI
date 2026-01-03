# NColors 预定义颜色集合

### 介绍

NColors 提供了一套完整的预定义颜色集合，遵循 Fluent Design 设计系统。包括基础颜色、强调色和状态颜色，每种颜色都包含多个色调变体，用于创建一致的、现代化的 UI。

### 基础颜色

```cpp
#include <QtNativeUI/NColor.h>

// 基础颜色
QColor transparent = NColors::transparent;  // 透明色
QColor black = NColors::black;              // 黑色
QColor white = NColors::white;              // 白色
```

### 灰色色调

灰色提供了 11 个色调，从最深（220）到最浅（10）：

```cpp
// 获取灰色的特定色调
QColor grey160 = NColors::grey[160];  // 主色调
QColor grey220 = NColors::grey[220];  // 最深
QColor grey10 = NColors::grey[10];    // 最浅

// 灰色主色调
QColor greyPrimary = NColors::grey.primary();
```

**灰色色调索引：**
- 220, 210, 200, 190, 180, 170, 160（主色调）, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10

### 强调色

提供 8 种预定义的强调色，每种都包含 7 个色调变体：

```cpp
// 访问强调色
NAccentColor yellow = NColors::yellow;
NAccentColor orange = NColors::orange;
NAccentColor red = NColors::red;
NAccentColor magenta = NColors::magenta;
NAccentColor purple = NColors::purple;
NAccentColor blue = NColors::blue;
NAccentColor teal = NColors::teal;
NAccentColor green = NColors::green;
```

### 强调色的色调变体

每个强调色都包含 7 个预定义的色调：

```cpp
// 获取蓝色的不同色调
QColor blueDarkest = NColors::blue.darkest();   // 最深
QColor blueDarker = NColors::blue.darker();     // 较深
QColor blueDark = NColors::blue.dark();         // 深
QColor blueNormal = NColors::blue.normal();     // 标准
QColor blueLight = NColors::blue.light();       // 浅
QColor blueLighter = NColors::blue.lighter();   // 较浅
QColor blueLightest = NColors::blue.lightest(); // 最浅
```

### 根据主题获取强调色

强调色可以根据亮度模式自动返回合适的变体：

```cpp
// 根据主题模式获取强调色
bool isDarkMode = true;

// 获取默认笔刷颜色（根据主题自动选择）
QColor defaultColor = NColors::blue.defaultBrushFor(isDarkMode);

// 获取次要笔刷颜色（带透明度）
QColor secondaryColor = NColors::blue.secondaryBrushFor(isDarkMode);

// 获取三级笔刷颜色（更高透明度）
QColor tertiaryColor = NColors::blue.tertiaryBrushFor(isDarkMode);
```

### 按名称访问色调

```cpp
// 通过名称访问特定色调
QColor darkShade = NColors::red["dark"];
QColor normalShade = NColors::red["normal"];
QColor lightShade = NColors::red["light"];
```

### 状态颜色

用于表示不同的操作状态：

```cpp
// 警告状态
QColor warningPrimary = NColors::warningPrimaryColor;
NAccentColor warningSecondary = NColors::warningSecondaryColor;

// 错误状态
QColor errorPrimary = NColors::errorPrimaryColor;
NAccentColor errorSecondary = NColors::errorSecondaryColor;

// 成功状态
QColor successPrimary = NColors::successPrimaryColor;
NAccentColor successSecondary = NColors::successSecondaryColor;
```

### 按枚举获取强调色

```cpp
// 使用枚举获取强调色
NAccentColor yellowColor = NColors::getAccentColor(NAccentColorType::Yellow);
NAccentColor orangeColor = NColors::getAccentColor(NAccentColorType::Orange);
NAccentColor redColor = NColors::getAccentColor(NAccentColorType::Red);
NAccentColor magentaColor = NColors::getAccentColor(NAccentColorType::Magenta);
NAccentColor purpleColor = NColors::getAccentColor(NAccentColorType::Purple);
NAccentColor blueColor = NColors::getAccentColor(NAccentColorType::Blue);
NAccentColor tealColor = NColors::getAccentColor(NAccentColorType::Teal);
NAccentColor greenColor = NColors::getAccentColor(NAccentColorType::Green);
```

### 所有强调色列表

```cpp
// 获取所有强调色
QList<NAccentColor> allAccentColors = NColors::accentColors;

// 遍历所有强调色
for (const NAccentColor& color : allAccentColors) {
    QColor normal = color.normal();
    // 使用颜色...
}
```

### 按钮样式示例

```cpp
// 创建按钮并应用强调色
NPushButton* button = new NPushButton("确定");

// 使用蓝色强调色
NAccentColor accentColor = NColors::blue;
button->setAccentColor(accentColor);

// 或使用特定的色调
button->setAccentColor(accentColor.normal());
```

### 状态指示器示例

```cpp
// 创建状态指示器
QLabel* statusLabel = new QLabel("处理中");

// 根据状态选择颜色
QColor statusColor;
if (isSuccess) {
    statusColor = NColors::successPrimaryColor;
} else if (isError) {
    statusColor = NColors::errorPrimaryColor;
} else if (isWarning) {
    statusColor = NColors::warningPrimaryColor;
}

statusLabel->setStyleSheet(QString("color: %1;").arg(statusColor.name()));
```

### 主题适配示例

```cpp
// 创建控件并根据主题选择颜色
QWidget* card = new QWidget();

// 获取当前主题
bool isDarkMode = nTheme->isDarkMode();

// 使用强调色的主题感知变体
QColor cardAccent = NColors::blue.defaultBrushFor(isDarkMode);

// 设置样式
card->setStyleSheet(QString("background-color: %1;").arg(cardAccent.name()));
```

### 颜色对比示例

```cpp
// 为不同的背景选择合适的强调色
QColor backgroundColor = NColors::grey[220];

// 在浅色背景上使用深色强调色
QColor accentForLight = NColors::blue.dark();

// 在深色背景上使用浅色强调色
QColor accentForDark = NColors::blue.light();
```

### 渐变色示例

```cpp
// 创建使用强调色的渐变
QLinearGradient gradient(0, 0, 100, 100);
gradient.setColorAt(0, NColors::blue.darkest());
gradient.setColorAt(0.5, NColors::blue.normal());
gradient.setColorAt(1, NColors::blue.lightest());

QPainter painter;
painter.fillRect(rect, gradient);
```

## API 参考

### NColors 静态成员

#### 基础颜色

| 成员 | 类型 | 说明 |
|------|------|------|
| `transparent` | `QColor` | 透明色 (0, 0, 0, 0) |
| `black` | `QColor` | 黑色 (0, 0, 0) |
| `white` | `QColor` | 白色 (255, 255, 255) |

#### 灰色

| 成员 | 类型 | 说明 |
|------|------|------|
| `grey` | `NShadedColor` | 灰色色调集合，包含 11 个色调 |

#### 强调色

| 成员 | 类型 | 说明 |
|------|------|------|
| `yellow` | `NAccentColor` | 黄色强调色 |
| `orange` | `NAccentColor` | 橙色强调色 |
| `red` | `NAccentColor` | 红色强调色 |
| `magenta` | `NAccentColor` | 品红色强调色 |
| `purple` | `NAccentColor` | 紫色强调色 |
| `blue` | `NAccentColor` | 蓝色强调色 |
| `teal` | `NAccentColor` | 青色强调色 |
| `green` | `NAccentColor` | 绿色强调色 |

#### 状态颜色

| 成员 | 类型 | 说明 |
|------|------|------|
| `warningPrimaryColor` | `QColor` | 警告主色 |
| `warningSecondaryColor` | `NAccentColor` | 警告辅助色 |
| `errorPrimaryColor` | `QColor` | 错误主色 |
| `errorSecondaryColor` | `NAccentColor` | 错误辅助色 |
| `successPrimaryColor` | `QColor` | 成功主色 |
| `successSecondaryColor` | `NAccentColor` | 成功辅助色 |

#### 列表

| 成员 | 类型 | 说明 |
|------|------|------|
| `accentColors` | `QList<NAccentColor>` | 所有强调色的列表 |

### NColors 静态方法

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getAccentColor(NAccentColorType::Type type)` | 根据枚举获取强调色 | 强调色类型 | `NAccentColor` |

### NAccentColor 方法

#### 色调访问

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `darkest()` | 获取最深色调 | `QColor` |
| `darker()` | 获取较深色调 | `QColor` |
| `dark()` | 获取深色调 | `QColor` |
| `normal()` | 获取标准色调 | `QColor` |
| `light()` | 获取浅色调 | `QColor` |
| `lighter()` | 获取较浅色调 | `QColor` |
| `lightest()` | 获取最浅色调 | `QColor` |

#### 主题感知方法

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `defaultBrushFor(bool isDark)` | 获取默认笔刷颜色 | 是否暗黑主题 | `QColor` |
| `secondaryBrushFor(bool isDark)` | 获取次要笔刷颜色 | 是否暗黑主题 | `QColor` |
| `tertiaryBrushFor(bool isDark)` | 获取三级笔刷颜色 | 是否暗黑主题 | `QColor` |

#### 访问操作符

| 操作符 | 说明 | 参数 | 返回值 |
|--------|------|------|--------|
| `operator[](const QString& name)` | 按名称访问色调 | 色调名称 | `QColor` |

#### 比较操作符

| 操作符 | 说明 | 参数 | 返回值 |
|--------|------|------|--------|
| `operator==(const NAccentColor& other)` | 相等比较 | 另一个强调色 | `bool` |
| `operator!=(const NAccentColor& other)` | 不相等比较 | 另一个强调色 | `bool` |

### NShadedColor 方法

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `operator[](int shade)` | 按索引访问色调 | 色调索引 | `QColor` |
| `primary()` | 获取主色调 | - | `QColor` |

### NAccentColorType 枚举

| 值 | 说明 |
|----|------|
| `Yellow` | 黄色 |
| `Orange` | 橙色 |
| `Red` | 红色 |
| `Magenta` | 品红色 |
| `Purple` | 紫色 |
| `Blue` | 蓝色 |
| `Teal` | 青色 |
| `Green` | 绿色 |
| `Custom` | 自定义 |

## 色调名称参考

使用 `operator[]` 访问色调时，支持以下名称：

| 名称 | 说明 |
|------|------|
| `"darkest"` | 最深色调 |
| `"darker"` | 较深色调 |
| `"dark"` | 深色调 |
| `"normal"` | 标准色调 |
| `"light"` | 浅色调 |
| `"lighter"` | 较浅色调 |
| `"lightest"` | 最浅色调 |

## 灰色色调索引参考

灰色支持以下色调索引（从深到浅）：

220, 210, 200, 190, 180, 170, 160（主色调）, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10

## 最佳实践

### 1. 使用强调色的色调变体

```cpp
// 好的做法：使用预定义的色调
QColor buttonColor = NColors::blue.normal();
QColor hoverColor = NColors::blue.light();
QColor pressColor = NColors::blue.dark();

// 避免：手动调整颜色
// QColor buttonColor = NColors::blue.normal().lighter(120);
```

### 2. 根据主题选择颜色

```cpp
// 好的做法：使用主题感知方法
QColor accentColor = NColors::blue.defaultBrushFor(isDarkMode);

// 避免：硬编码颜色
// QColor accentColor = isDarkMode ? NColors::blue.lighter() : NColors::blue.darker();
```

### 3. 使用状态颜色表示状态

```cpp
// 好的做法：使用预定义的状态颜色
if (isError) {
    label->setStyleSheet(QString("color: %1;").arg(NColors::errorPrimaryColor.name()));
} else if (isSuccess) {
    label->setStyleSheet(QString("color: %1;").arg(NColors::successPrimaryColor.name()));
}
```

### 4. 灰色用于背景和边框

```cpp
// 好的做法：使用灰色色调
QColor backgroundColor = NColors::grey[50];
QColor borderColor = NColors::grey[150];

// 避免：使用强调色作为背景
// QColor backgroundColor = NColors::blue[50];
```
