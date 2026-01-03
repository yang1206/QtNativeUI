# NRangeSlider 范围滑块

### 介绍

范围滑块用于选择一个数值范围。NRangeSlider 提供了两个可拖动的滑块，用户可以通过拖动来选择最小值和最大值，支持水平和竖直方向。

### 基本用法

```cpp
// 创建范围滑块
NRangeSlider* rangeSlider = new NRangeSlider();

// 设置范围
rangeSlider->setRange(0, 100);

// 设置初始值
rangeSlider->setValues(20, 80);

// 获取当前值
QPair<int, int> values = rangeSlider->values();
qDebug() << "下限：" << values.first << "上限：" << values.second;

// 监听值变化
connect(rangeSlider, QOverload<int>::of(&NRangeSlider::lowerValueChanged), 
        [](int value) {
    qDebug() << "下限已改变：" << value;
});

connect(rangeSlider, QOverload<int>::of(&NRangeSlider::upperValueChanged), 
        [](int value) {
    qDebug() << "上限已改变：" << value;
});

connect(rangeSlider, QOverload<int, int>::of(&NRangeSlider::rangeChanged), 
        [](int lower, int upper) {
    qDebug() << "范围已改变：" << lower << "-" << upper;
});
```

### 水平范围滑块

```cpp
// 创建水平范围滑块
NRangeSlider* horizontalSlider = new NRangeSlider(Qt::Horizontal);

// 设置范围
horizontalSlider->setRange(0, 100);
horizontalSlider->setValues(25, 75);

// 监听变化
connect(horizontalSlider, QOverload<int, int>::of(&NRangeSlider::rangeChanged),
        [](int lower, int upper) {
    qDebug() << "选择范围：" << lower << "-" << upper;
});
```

### 竖直范围滑块

```cpp
// 创建竖直范围滑块
NRangeSlider* verticalSlider = new NRangeSlider(Qt::Vertical);

// 设置范围
verticalSlider->setRange(0, 100);
verticalSlider->setValues(30, 70);

// 设置高度
verticalSlider->setMinimumHeight(200);

// 监听变化
connect(verticalSlider, QOverload<int, int>::of(&NRangeSlider::rangeChanged),
        [](int lower, int upper) {
    qDebug() << "选择范围：" << lower << "-" << upper;
});
```

### 价格范围选择

```cpp
// 创建价格范围选择器
QWidget* priceFilter = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(priceFilter);

// 标题
QLabel* titleLabel = new QLabel("价格范围");
titleLabel->setStyleSheet("font-weight: bold;");
layout->addWidget(titleLabel);

// 范围滑块
NRangeSlider* priceSlider = new NRangeSlider(Qt::Horizontal);
priceSlider->setRange(0, 10000);
priceSlider->setValues(1000, 5000);
layout->addWidget(priceSlider);

// 显示价格范围
QLabel* priceLabel = new QLabel();
auto updatePriceLabel = [=]() {
    QPair<int, int> values = priceSlider->values();
    priceLabel->setText(QString("¥%1 - ¥%2").arg(values.first).arg(values.second));
};

connect(priceSlider, QOverload<int, int>::of(&NRangeSlider::rangeChanged),
        [=](int lower, int upper) {
    updatePriceLabel();
});

layout->addWidget(priceLabel);
updatePriceLabel();
```

### 时间范围选择

```cpp
// 创建时间范围选择器
QWidget* timeFilter = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(timeFilter);

// 标题
QLabel* titleLabel = new QLabel("时间范围（小时）");
layout->addWidget(titleLabel);

// 范围滑块
NRangeSlider* timeSlider = new NRangeSlider(Qt::Horizontal);
timeSlider->setRange(0, 24);
timeSlider->setValues(8, 18);
layout->addWidget(timeSlider);

// 自定义格式化函数
timeSlider->setTooltipFormatter([](int value) {
    return QString("%1:00").arg(value, 2, 10, QChar('0'));
});

// 显示时间范围
QLabel* timeLabel = new QLabel();
auto updateTimeLabel = [=]() {
    QPair<int, int> values = timeSlider->values();
    timeLabel->setText(QString("%1:00 - %2:00")
        .arg(values.first, 2, 10, QChar('0'))
        .arg(values.second, 2, 10, QChar('0')));
};

connect(timeSlider, QOverload<int, int>::of(&NRangeSlider::rangeChanged),
        [=](int lower, int upper) {
    updateTimeLabel();
});

layout->addWidget(timeLabel);
updateTimeLabel();
```

### 自定义工具提示

```cpp
// 创建范围滑块
NRangeSlider* slider = new NRangeSlider(Qt::Horizontal);
slider->setRange(0, 100);
slider->setValues(30, 70);

// 启用工具提示
slider->setShowTooltip(true);

// 自定义工具提示格式
slider->setTooltipFormatter([](int value) {
    return QString::number(value) + "%";
});
```

### 自定义样式

```cpp
// 创建范围滑块
NRangeSlider* slider = new NRangeSlider(Qt::Horizontal);

// 设置轨道高度
slider->setTrackHeight(6);

// 设置滑块大小
slider->setThumbDiameter(20);
slider->setThumbInnerDiameter(12);

// 设置圆角
slider->setTrackCornerRadius(3);

// 自定义颜色
slider->setLightTrackColor(QColor(230, 230, 230));
slider->setLightProgressColor(QColor(0, 120, 215));
slider->setLightThumbOuterColor(QColor(255, 255, 255));

slider->setDarkTrackColor(QColor(60, 60, 60));
slider->setDarkProgressColor(QColor(0, 150, 255));
slider->setDarkThumbOuterColor(QColor(200, 200, 200));

// 设置范围
slider->setRange(0, 100);
slider->setValues(25, 75);
```

### 禁用状态

```cpp
// 创建范围滑块
NRangeSlider* slider = new NRangeSlider(Qt::Horizontal);
slider->setRange(0, 100);
slider->setValues(30, 70);

// 禁用滑块
slider->setEnabled(false);

// 自定义禁用状态颜色
slider->setLightDisabledTrackColor(QColor(240, 240, 240));
slider->setLightDisabledProgressColor(QColor(200, 200, 200));
slider->setDarkDisabledTrackColor(QColor(50, 50, 50));
slider->setDarkDisabledProgressColor(QColor(80, 80, 80));
```

### 键盘控制

```cpp
// 创建范围滑块
NRangeSlider* slider = new NRangeSlider(Qt::Horizontal);
slider->setRange(0, 100);
slider->setValues(30, 70);

// 设置焦点
slider->setFocus();

// 使用键盘控制：
// 左/上箭头：减小值
// 右/下箭头：增大值
// Tab：切换焦点到另一个滑块
```

### 范围限制

```cpp
// 创建范围滑块
NRangeSlider* slider = new NRangeSlider(Qt::Horizontal);

// 设置范围
slider->setRange(10, 90);

// 设置初始值（会被限制在范围内）
slider->setValues(5, 95);  // 实际会被设置为 10, 90

// 获取实际值
QPair<int, int> values = slider->values();
qDebug() << "实际值：" << values.first << "-" << values.second;
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NRangeSlider(QWidget* parent = nullptr)` | 创建水平范围滑块 |
| `NRangeSlider(Qt::Orientation orientation, QWidget* parent = nullptr)` | 创建指定方向的范围滑块 |

### 范围和值

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setRange(int min, int max)` | 设置范围 | 最小值、最大值 |
| `setValues(int lower, int upper)` | 设置下限和上限 | 下限值、上限值 |
| `values()` | 获取当前值 | 返回 `QPair<int, int>` |
| `setMinimum(int min)` | 设置最小值 | 最小值 |
| `minimum()` | 获取最小值 | 返回 `int` |
| `setMaximum(int max)` | 设置最大值 | 最大值 |
| `maximum()` | 获取最大值 | 返回 `int` |
| `setLowerValue(int value)` | 设置下限值 | 下限值 |
| `lowerValue()` | 获取下限值 | 返回 `int` |
| `setUpperValue(int value)` | 设置上限值 | 上限值 |
| `upperValue()` | 获取上限值 | 返回 `int` |

### 方向

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setOrientation(Qt::Orientation orientation)` | 设置方向 | `Qt::Horizontal` 或 `Qt::Vertical` |
| `orientation()` | 获取方向 | 返回 `Qt::Orientation` |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `trackHeight` | 轨道高度 | `int` |
| `thumbDiameter` | 滑块直径 | `int` |
| `thumbInnerDiameter` | 滑块内部直径 | `int` |
| `trackCornerRadius` | 轨道圆角半径 | `int` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightTrackColor` | 明亮主题轨道色 | `QColor` |
| `darkTrackColor` | 暗黑主题轨道色 | `QColor` |
| `lightProgressColor` | 明亮主题进度色 | `QColor` |
| `darkProgressColor` | 暗黑主题进度色 | `QColor` |
| `lightThumbOuterColor` | 明亮主题滑块外色 | `QColor` |
| `darkThumbOuterColor` | 暗黑主题滑块外色 | `QColor` |

### 工具提示

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setShowTooltip(bool show)` | 启用/禁用工具提示 | 布尔值 |
| `showTooltip()` | 获取是否显示工具提示 | 返回 `bool` |
| `setTooltipFormatter(std::function<QString(int)> formatter)` | 设置工具提示格式化函数 | 格式化函数 |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `lowerValueChanged(int value)` | 下限值已改变 | 新的下限值 |
| `upperValueChanged(int value)` | 上限值已改变 | 新的上限值 |
| `rangeChanged(int lower, int upper)` | 范围已改变 | 新的下限值、新的上限值 |

## 主题定制

范围滑块会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
