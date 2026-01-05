# NSlider 滑块

### 介绍

滑块用于在指定范围内选择数值。NSlider 基于 Qt 的 QSlider，提供了现代化的外观、工具提示显示和自定义格式化功能。

### 基本用法

```cpp
// 水平滑块
NSlider* horizontalSlider = new NSlider(Qt::Horizontal);
horizontalSlider->setRange(0, 100);
horizontalSlider->setValue(50);

// 垂直滑块
NSlider* verticalSlider = new NSlider(Qt::Vertical);
verticalSlider->setRange(0, 255);
verticalSlider->setValue(128);

// 监听值变化
connect(horizontalSlider, &QSlider::valueChanged, [](int value) {
    qDebug() << "滑块值：" << value;
});
```

### 工具提示

支持显示当前值的工具提示，可以自定义格式化函数。

```cpp
NSlider* tooltipSlider = new NSlider(Qt::Horizontal);
tooltipSlider->setRange(0, 100);
tooltipSlider->setShowTooltip(true);

// 自定义工具提示格式
tooltipSlider->setTooltipFormatter([](int value) {
    return QString("%1%").arg(value);
});

// 音量滑块示例
NSlider* volumeSlider = new NSlider(Qt::Horizontal);
volumeSlider->setRange(0, 100);
volumeSlider->setShowTooltip(true);
volumeSlider->setTooltipFormatter([](int value) {
    if (value == 0) return "静音";
    if (value < 30) return QString("低音量 %1%").arg(value);
    if (value < 70) return QString("中音量 %1%").arg(value);
    return QString("高音量 %1%").arg(value);
});
```

### 刻度标记

支持显示刻度标记，可以自定义刻度的外观。

```cpp
NSlider* tickSlider = new NSlider(Qt::Horizontal);
tickSlider->setRange(0, 10);
tickSlider->setTickPosition(QSlider::TicksBelow);
tickSlider->setTickInterval(1);

// 自定义刻度样式
tickSlider->setTickLength(8);
tickSlider->setTickThickness(2);
tickSlider->setLightTickColor(QColor(150, 150, 150));
```

### 颜色和亮度调节

```cpp
// RGB 颜色调节器
QWidget* colorWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(colorWidget);

// 红色分量
QHBoxLayout* redLayout = new QHBoxLayout();
redLayout->addWidget(new NLabel("R:", NLabelType::Body));
NSlider* redSlider = new NSlider(Qt::Horizontal);
redSlider->setRange(0, 255);
redSlider->setShowTooltip(true);
redSlider->setAccentColor(QColor(255, 0, 0));
redLayout->addWidget(redSlider);
layout->addLayout(redLayout);

// 绿色分量
QHBoxLayout* greenLayout = new QHBoxLayout();
greenLayout->addWidget(new NLabel("G:", NLabelType::Body));
NSlider* greenSlider = new NSlider(Qt::Horizontal);
greenSlider->setRange(0, 255);
greenSlider->setShowTooltip(true);
greenSlider->setAccentColor(QColor(0, 255, 0));
greenLayout->addWidget(greenSlider);
layout->addLayout(greenLayout);

// 蓝色分量
QHBoxLayout* blueLayout = new QHBoxLayout();
blueLayout->addWidget(new NLabel("B:", NLabelType::Body));
NSlider* blueSlider = new NSlider(Qt::Horizontal);
blueSlider->setRange(0, 255);
blueSlider->setShowTooltip(true);
blueSlider->setAccentColor(QColor(0, 0, 255));
blueLayout->addWidget(blueSlider);
layout->addLayout(blueLayout);

// 连接颜色变化
auto updateColor = [=]() {
    QColor color(redSlider->value(), greenSlider->value(), blueSlider->value());
    // 更新预览颜色
};

connect(redSlider, &QSlider::valueChanged, updateColor);
connect(greenSlider, &QSlider::valueChanged, updateColor);
connect(blueSlider, &QSlider::valueChanged, updateColor);
```

### 自定义样式

```cpp
NSlider* styledSlider = new NSlider(Qt::Horizontal);

// 设置轨道样式
styledSlider->setTrackHeight(6);
styledSlider->setTrackCornerRadius(3);

// 设置滑块样式
styledSlider->setThumbDiameter(20);
styledSlider->setThumbInnerDiameter(16);

// 自定义颜色
styledSlider->setLightTrackColor(QColor(230, 230, 230));
styledSlider->setLightProgressColor(QColor(0, 120, 215));
styledSlider->setLightThumbOuterColor(QColor(255, 255, 255));

// 禁用状态颜色
styledSlider->setLightDisabledTrackColor(QColor(200, 200, 200));
styledSlider->setLightDisabledProgressColor(QColor(150, 150, 150));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NSlider(QWidget* parent = nullptr)` | 创建水平滑块 |
| `NSlider(Qt::Orientation orientation, QWidget* parent = nullptr)` | 创建指定方向的滑块 |

### 颜色设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setAccentColor(const QColor& color)` | 设置强调色 | 颜色值 |
| `setAccentColor(const NAccentColor& color)` | 设置主题强调色 | 主题颜色对象 |
| `resetAccentColor()` | 重置为默认强调色 | - |

### 工具提示

| 方法 | 说明 | 参数 |
|------|------|------|
| `setShowTooltip(bool show)` | 设置是否显示工具提示 | `true` / `false` |
| `showTooltip()` | 获取是否显示工具提示 | - |
| `setTooltipFormatter(std::function<QString(int)> formatter)` | 设置工具提示格式化函数 | 格式化函数 |

### 样式属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setTrackHeight(int height)` | 设置轨道高度 | 高度像素值 |
| `trackHeight()` | 获取轨道高度 | - |
| `setThumbDiameter(int diameter)` | 设置滑块直径 | 直径像素值 |
| `thumbDiameter()` | 获取滑块直径 | - |
| `setThumbInnerDiameter(int diameter)` | 设置滑块内径 | 内径像素值 |
| `thumbInnerDiameter()` | 获取滑块内径 | - |
| `setTrackCornerRadius(int radius)` | 设置轨道圆角 | 圆角半径 |
| `trackCornerRadius()` | 获取轨道圆角 | - |
| `setTickLength(int length)` | 设置刻度长度 | 长度像素值 |
| `tickLength()` | 获取刻度长度 | - |
| `setTickThickness(int thickness)` | 设置刻度粗细 | 粗细像素值 |
| `tickThickness()` | 获取刻度粗细 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightTrackColor` | 明亮主题轨道色 | `QColor` |
| `darkTrackColor` | 暗黑主题轨道色 | `QColor` |
| `lightProgressColor` | 明亮主题进度色 | `QColor` |
| `darkProgressColor` | 暗黑主题进度色 | `QColor` |
| `lightDisabledTrackColor` | 明亮主题禁用轨道色 | `QColor` |
| `darkDisabledTrackColor` | 暗黑主题禁用轨道色 | `QColor` |
| `lightDisabledProgressColor` | 明亮主题禁用进度色 | `QColor` |
| `darkDisabledProgressColor` | 暗黑主题禁用进度色 | `QColor` |
| `lightThumbOuterColor` | 明亮主题滑块外圈色 | `QColor` |
| `darkThumbOuterColor` | 暗黑主题滑块外圈色 | `QColor` |
| `lightTickColor` | 明亮主题刻度色 | `QColor` |
| `darkTickColor` | 暗黑主题刻度色 | `QColor` |
| `lightDisabledTickColor` | 明亮主题禁用刻度色 | `QColor` |
| `darkDisabledTickColor` | 暗黑主题禁用刻度色 | `QColor` |

### 继承的 API

NSlider 继承自 `QSlider`，支持所有标准 QSlider 的方法和信号：

- `setRange()` / `minimum()` / `maximum()` - 设置/获取范围
- `setValue()` / `value()` - 设置/获取当前值
- `setOrientation()` / `orientation()` - 设置/获取方向
- `setTickPosition()` / `tickPosition()` - 设置/获取刻度位置
- `setTickInterval()` / `tickInterval()` - 设置/获取刻度间隔
- `valueChanged(int)` - 值改变信号
- `sliderPressed()` / `sliderReleased()` - 滑块按下/释放信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 轨道高度 | `4px` | 滑块轨道高度 |
| 滑块直径 | `20px` | 滑块圆形直径 |
| 滑块内径 | `16px` | 滑块内圈直径 |
| 轨道圆角 | `2px` | 轨道圆角半径 |
| 刻度长度 | `6px` | 刻度标记长度 |
| 刻度粗细 | `1px` | 刻度标记粗细 |

### 主题适配

```cpp
// 滑块会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 滑块颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 进度色会自动更新为新的强调色
});
```