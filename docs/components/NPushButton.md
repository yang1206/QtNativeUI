# NPushButton 按钮

### 介绍

按钮用于触发一个操作，如提交表单。NPushButton 基于 Qt 的 QPushButton，提供了 Fluent Design 风格的现代化外观。

### 按钮类型

按钮支持 `Standard`、`Accent` 两种类型，默认为 `Standard`。

```cpp
// 标准按钮
NPushButton* standardBtn = new NPushButton("标准按钮");

// 强调按钮
NPushButton* accentBtn = new NPushButton("强调按钮");
accentBtn->setButtonType(NPushButton::Accent);
```

### 禁用状态

通过 `setEnabled()` 来禁用按钮，禁用状态下按钮不可点击。

```cpp
NPushButton* disabledBtn = new NPushButton("禁用按钮");
disabledBtn->setEnabled(false);
```

### 透明背景

通过 `setTransparentBackground()` 设置透明背景，适用于工具栏按钮。

```cpp
NPushButton* transparentBtn = new NPushButton("透明按钮");
transparentBtn->setTransparentBackground(true);
```

### 图标按钮

支持 Fluent 图标，可设置图标大小和颜色。

```cpp
// 带图标的按钮
NPushButton* iconBtn = new NPushButton("设置");
iconBtn->setFluentIcon(NRegularIconType::Settings24Regular, 16);

// 仅图标按钮
NPushButton* iconOnlyBtn = new NPushButton();
iconOnlyBtn->setFluentIcon(NRegularIconType::Add24Regular, 20);
iconOnlyBtn->setFixedSize(32, 32);

// 自定义图标颜色
NPushButton* colorIconBtn = new NPushButton("搜索");
colorIconBtn->setFluentIcon(NRegularIconType::Search24Regular, 16, Qt::blue);
```

### 圆角设置

通过 `setBorderRadius()` 设置按钮圆角。

```cpp
NPushButton* roundBtn = new NPushButton("圆角按钮");
roundBtn->setBorderRadius(12);
```

### 自定义颜色

支持自定义按钮颜色，包括明暗主题。

```cpp
NPushButton* customBtn = new NPushButton("自定义颜色");
// 明亮主题颜色
customBtn->setLightDefaultColor(QColor(100, 150, 255));
customBtn->setLightHoverColor(QColor(80, 130, 235));
customBtn->setLightTextDefaultColor(Qt::white);

// 或使用强调色
customBtn->setAccentColor(QColor(255, 0, 0));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NPushButton(QWidget* parent = nullptr)` | 创建空按钮 |
| `NPushButton(const QString& text, QWidget* parent = nullptr)` | 创建带文本的按钮 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `setButtonType(ButtonType type)` | 设置按钮类型 | `Standard` / `Accent` |
| `buttonType()` | 获取按钮类型 | - |
| `setFluentIcon(NRegularIconType::Icon icon, int size, const QColor& color)` | 设置 Regular 图标 | 图标类型、大小、颜色 |
| `setFluentIcon(NFilledIconType::Icon icon, int size, const QColor& color)` | 设置 Filled 图标 | 图标类型、大小、颜色 |
| `setAccentColor(const QColor& color)` | 设置强调色 | 颜色值 |
| `setBorderRadius(int radius)` | 设置圆角半径 | 半径像素值 |
| `borderRadius()` | 获取圆角半径 | - |
| `setTransparentBackground(bool transparent)` | 设置透明背景 | `true` / `false` |
| `transparentBackground()` | 获取是否透明背景 | - |
| `setAlignment(Qt::Alignment align)` | 设置文本对齐 | 对齐方式 |
| `alignment()` | 获取文本对齐 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightDefaultColor` | 明亮主题默认背景色 | `QColor` |
| `darkDefaultColor` | 暗黑主题默认背景色 | `QColor` |
| `lightHoverColor` | 明亮主题悬停背景色 | `QColor` |
| `darkHoverColor` | 暗黑主题悬停背景色 | `QColor` |
| `lightPressColor` | 明亮主题按下背景色 | `QColor` |
| `darkPressColor` | 暗黑主题按下背景色 | `QColor` |
| `lightTextDefaultColor` | 明亮主题默认文本色 | `QColor` |
| `darkTextDefaultColor` | 暗黑主题默认文本色 | `QColor` |
| `lightTextPressColor` | 明亮主题按下文本色 | `QColor` |
| `darkTextPressColor` | 暗黑主题按下文本色 | `QColor` |

### 枚举

#### ButtonType

| 值 | 说明 |
|----|------|
| `Standard` | 标准按钮（默认） |
| `Accent` | 强调按钮 |

### 继承的 API

NPushButton 继承自 `QPushButton`，支持所有标准 QPushButton 的方法和信号：

- `setText()` / `text()` - 设置/获取按钮文本
- `setIcon()` / `icon()` - 设置/获取按钮图标（建议使用 `setFluentIcon()`）
- `clicked()` - 点击信号
- `pressed()` / `released()` - 按下/释放信号
- `setEnabled()` / `isEnabled()` - 启用/禁用状态
- `setVisible()` / `isVisible()` - 显示/隐藏状态

## 主题定制

### 样式变量

组件会自动适配系统主题，也可以通过颜色属性进行自定义：

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 圆角半径 | `4px` | 按钮圆角 |
| 默认高度 | `32px` | 标准按钮高度 |
| 内边距 | `8px 16px` | 按钮内边距 |
| 字体大小 | 继承父级 | 按钮文字大小 |

### 主题适配

```cpp
// 按钮会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 按钮颜色会自动更新
});
```