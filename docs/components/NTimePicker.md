# NTimePicker 时间选择器

### 介绍

时间选择器用于选择时间，提供滚轮式的交互体验。NTimePicker 基于 Qt 的 QPushButton，点击后弹出滚轮选择器，支持 24 小时制和 12 小时制，可选择是否显示秒数。

### 基本用法

创建一个基本的时间选择器，默认为 24 小时制。

```cpp
NTimePicker* timePicker = new NTimePicker();
timePicker->setTime(QTime::currentTime());

connect(timePicker, &NTimePicker::timeChanged, [](const QTime& time) {
    qDebug() << "选择的时间:" << time.toString("HH:mm:ss");
});
```

### 24 小时制 vs 12 小时制

通过 `setUse24HourFormat()` 切换时间格式。

```cpp
// 24 小时制（默认）
NTimePicker* picker24 = new NTimePicker();
picker24->setUse24HourFormat(true);
picker24->setTime(QTime(14, 30));

// 12 小时制
NTimePicker* picker12 = new NTimePicker();
picker12->setUse24HourFormat(false);
picker12->setTime(QTime(14, 30)); // 显示为 02:30
```

### 显示秒数

通过 `setShowSeconds()` 控制是否显示秒数选择。

```cpp
NTimePicker* timePicker = new NTimePicker();
timePicker->setShowSeconds(true);
timePicker->setTime(QTime(9, 30, 45));
```

### 设置和获取时间

使用 `setTime()` 和 `getTime()` 方法操作时间。

```cpp
NTimePicker* timePicker = new NTimePicker();

// 设置时间
timePicker->setTime(QTime(15, 30, 0));

// 获取时间
QTime currentTime = timePicker->getTime();
qDebug() << "当前时间:" << currentTime.toString("HH:mm:ss");
```

### 自定义宽度

根据是否显示秒数，可以调整选择器宽度。

```cpp
NTimePicker* timePicker = new NTimePicker();
timePicker->setShowSeconds(true);
timePicker->setFixedWidth(180); // 显示秒数时需要更宽
```

### 圆角设置

通过 `setBorderRadius()` 设置按钮圆角。

```cpp
NTimePicker* timePicker = new NTimePicker();
timePicker->setBorderRadius(8);
```

## API

### 构造函数

| 构造函数                                 | 说明           |
| ---------------------------------------- | -------------- |
| `NTimePicker(QWidget* parent = nullptr)` | 创建时间选择器 |

### 属性

| 属性              | 类型    | 说明               | 默认值       |
| ----------------- | ------- | ------------------ | ------------ |
| `time`            | `QTime` | 当前选中的时间     | 当前系统时间 |
| `use24HourFormat` | `bool`  | 是否使用 24 小时制 | `true`       |
| `showSeconds`     | `bool`  | 是否显示秒数       | `false`      |
| `borderRadius`    | `int`   | 按钮圆角半径       | `4`          |

### 方法

| 方法                                 | 说明             | 参数                                      |
| ------------------------------------ | ---------------- | ----------------------------------------- |
| `setTime(const QTime& time)`         | 设置时间         | 时间对象                                  |
| `getTime()`                          | 获取当前时间     | -                                         |
| `setUse24HourFormat(bool use24Hour)` | 设置时间格式     | `true` 为 24 小时制，`false` 为 12 小时制 |
| `getUse24HourFormat()`               | 获取时间格式     | -                                         |
| `setShowSeconds(bool showSeconds)`   | 设置是否显示秒数 | `true` 显示，`false` 不显示               |
| `getShowSeconds()`                   | 获取是否显示秒数 | -                                         |
| `setBorderRadius(int radius)`        | 设置圆角半径     | 半径像素值                                |
| `getBorderRadius()`                  | 获取圆角半径     | -                                         |

### 信号

| 信号                             | 说明                           | 参数       |
| -------------------------------- | ------------------------------ | ---------- |
| `timeChanged(const QTime& time)` | 时间改变时触发（点击确认按钮） | 新的时间值 |

### 继承的 API

NTimePicker 继承自 `QPushButton`，支持所有标准 QPushButton 的方法和信号：

- `setEnabled()` / `isEnabled()` - 启用/禁用状态
- `setVisible()` / `isVisible()` - 显示/隐藏状态
- `setToolTip()` - 设置工具提示

## 交互说明

### 弹出选择器

点击按钮后，会弹出一个包含滚轮选择器的浮动窗口：

- **小时滚轮**：选择小时（24 小时制：0-23，12 小时制：1-12）
- **分钟滚轮**：选择分钟（0-59）
- **秒数滚轮**：选择秒数（0-59，仅在 `showSeconds` 为 `true` 时显示）

### 操作方式

- **滚轮滚动**：使用鼠标滚轮滚动选择
- **点击跳转**：点击非中心项快速跳转
- **上下箭头**：点击上下箭头按钮滚动（Container 模式）
- **确认/取消**：点击底部的确认或取消按钮

### 确认和取消

- **确认按钮**（✓）：应用选择的时间并触发 `timeChanged` 信号
- **取消按钮**（✕）：放弃修改，恢复到打开前的时间
- **点击外部**：等同于取消操作

## 使用示例

### 完整示例

```cpp
#include <QtNativeUI/NTimePicker.h>
#include <QVBoxLayout>
#include <QLabel>

// 创建时间选择器
NTimePicker* timePicker = new NTimePicker();
timePicker->setShowSeconds(true);
timePicker->setTime(QTime(9, 0, 0));

// 显示结果的标签
QLabel* resultLabel = new QLabel("选择的时间: 09:00:00");

// 连接信号
connect(timePicker, &NTimePicker::timeChanged, [resultLabel](const QTime& time) {
    resultLabel->setText("选择的时间: " + time.toString("HH:mm:ss"));
});

// 添加到布局
QVBoxLayout* layout = new QVBoxLayout();
layout->addWidget(timePicker);
layout->addWidget(resultLabel);
```

### 表单中使用

```cpp
// 创建表单
QFormLayout* formLayout = new QFormLayout();

// 开始时间
NTimePicker* startTimePicker = new NTimePicker();
startTimePicker->setTime(QTime(9, 0));
formLayout->addRow("开始时间:", startTimePicker);

// 结束时间
NTimePicker* endTimePicker = new NTimePicker();
endTimePicker->setTime(QTime(18, 0));
formLayout->addRow("结束时间:", endTimePicker);

// 验证时间范围
connect(endTimePicker, &NTimePicker::timeChanged, [=](const QTime& endTime) {
    QTime startTime = startTimePicker->getTime();
    if (endTime <= startTime) {
        qWarning() << "结束时间必须晚于开始时间";
    }
});
```

### 12 小时制示例

```cpp
NTimePicker* timePicker = new NTimePicker();
timePicker->setUse24HourFormat(false);
timePicker->setTime(QTime(14, 30)); // 显示为 02:30

connect(timePicker, &NTimePicker::timeChanged, [](const QTime& time) {
    // 格式化为 12 小时制显示
    QString timeStr = time.toString("hh:mm AP");
    qDebug() << "选择的时间:" << timeStr; // 例如: "02:30 PM"
});
```

## 主题定制

### 样式变量

组件会自动适配系统主题：

| 变量     | 默认值                          | 说明                 |
| -------- | ------------------------------- | -------------------- |
| 按钮高度 | `32px`                          | 时间选择器按钮高度   |
| 按钮宽度 | `120px` (无秒) / `180px` (有秒) | 时间选择器按钮宽度   |
| 圆角半径 | `4px`                           | 按钮圆角             |
| 滚轮高度 | `245px`                         | 弹出选择器的滚轮高度 |
| 可见项数 | `7`                             | 滚轮可见项数量       |
| 项高度   | `35px`                          | 滚轮每项的高度       |

### 主题适配

```cpp
// 时间选择器会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 组件颜色会自动更新
});
```

## 注意事项

1. **时间格式**：使用 `QTime` 对象，确保时间有效性
2. **12 小时制**：内部仍使用 24 小时制存储，仅显示时转换
3. **宽度调整**：显示秒数时建议设置更宽的宽度（180px）
4. **信号触发**：只有点击确认按钮才会触发 `timeChanged` 信号
5. **取消操作**：点击取消或外部区域会恢复到打开前的时间
