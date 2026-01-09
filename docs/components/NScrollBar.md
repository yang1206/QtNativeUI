# NScrollBar 滚动条

### 介绍

滚动条用于在有限的显示区域内浏览大量内容。NScrollBar 基于 Qt 的 QScrollBar，提供了现代化的外观、平滑的动画和丰富的自定义选项。

### 基本用法

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Horizontal);

// 设置范围
scrollBar->setRange(0, 100);

// 设置当前值
scrollBar->setValue(50);

// 监听值变化
connect(scrollBar, &QScrollBar::valueChanged, [](int value) {
    qDebug() << "滚动条值：" << value;
});
```

### 水平滚动条

```cpp
// 创建水平滚动条
NScrollBar* hScrollBar = new NScrollBar(Qt::Horizontal);
hScrollBar->setRange(0, 1000);
hScrollBar->setPageStep(100);

// 监听值变化
connect(hScrollBar, &QScrollBar::valueChanged, [](int value) {
    qDebug() << "水平滚动：" << value;
});
```

### 竖直滚动条

```cpp
// 创建竖直滚动条
NScrollBar* vScrollBar = new NScrollBar(Qt::Vertical);
vScrollBar->setRange(0, 1000);
vScrollBar->setPageStep(100);

// 监听值变化
connect(vScrollBar, &QScrollBar::valueChanged, [](int value) {
    qDebug() << "竖直滚动：" << value;
});
```

### 替换现有滚动条

```cpp
// 创建文本编辑器
NTextEdit* textEdit = new NTextEdit();

// 获取原始滚动条
QScrollBar* originalScrollBar = textEdit->verticalScrollBar();

// 创建新的 NScrollBar 替换
NScrollBar* nativeScrollBar = new NScrollBar(originalScrollBar, textEdit);

// 新的滚动条会自动替换原始滚动条
```

### 自定义样式

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Vertical);

// 设置滑块厚度
scrollBar->setThumbThickness(12);
scrollBar->setThumbNormalThickness(8);

// 设置轨道厚度
scrollBar->setTrackThickness(12);

// 设置圆角
scrollBar->setThumbCornerRadius(6);
scrollBar->setTrackCornerRadius(6);

// 自定义颜色
scrollBar->setLightTrackColor(QColor(240, 240, 240));
scrollBar->setLightThumbColor(QColor(200, 200, 200));
scrollBar->setLightThumbHoverColor(QColor(150, 150, 150));
scrollBar->setLightThumbPressColor(QColor(100, 100, 100));

scrollBar->setDarkTrackColor(QColor(50, 50, 50));
scrollBar->setDarkThumbColor(QColor(100, 100, 100));
scrollBar->setDarkThumbHoverColor(QColor(150, 150, 150));
scrollBar->setDarkThumbPressColor(QColor(200, 200, 200));

// 设置范围
scrollBar->setRange(0, 1000);
```

### 在滚动区域中使用

```cpp
// 创建滚动区域
NScrollArea* scrollArea = new NScrollArea();

// 创建内容
QWidget* content = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(content);

// 添加大量内容
for (int i = 0; i < 100; ++i) {
    layout->addWidget(new QLabel(QString("项目 %1").arg(i + 1)));
}

layout->addStretch();
scrollArea->setWidget(content);

// 获取滚动条
NScrollBar* vScrollBar = qobject_cast<NScrollBar*>(scrollArea->verticalScrollBar());
if (vScrollBar) {
    // 自定义滚动条样式
    vScrollBar->setThumbThickness(12);
}
```

### 滚动条事件

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Vertical);
scrollBar->setRange(0, 1000);

// 监听值变化
connect(scrollBar, &QScrollBar::valueChanged, [](int value) {
    qDebug() << "值已改变：" << value;
});

// 监听滑块按下
connect(scrollBar, &QScrollBar::sliderPressed, [](int value) {
    qDebug() << "滑块已按下";
});

// 监听滑块释放
connect(scrollBar, &QScrollBar::sliderReleased, [](int value) {
    qDebug() << "滑块已释放";
});

// 监听范围变化
connect(scrollBar, &QScrollBar::rangeChanged, [](int min, int max) {
    qDebug() << "范围已改变：" << min << "-" << max;
});
```

### 平滑滚动

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Vertical);
scrollBar->setRange(0, 1000);

// 平滑滚动到指定位置
void smoothScroll(NScrollBar* scrollBar, int targetValue) {
    QPropertyAnimation* animation = new QPropertyAnimation(scrollBar, "value");
    animation->setDuration(300);
    animation->setEndValue(targetValue);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenDone);
}

// 使用平滑滚动
smoothScroll(scrollBar, 500);
```

### 禁用状态

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Vertical);
scrollBar->setRange(0, 1000);

// 禁用滚动条
scrollBar->setEnabled(false);

// 自定义禁用状态颜色
scrollBar->setLightDisabledTrackColor(QColor(245, 245, 245));
scrollBar->setLightDisabledThumbColor(QColor(220, 220, 220));
scrollBar->setDarkDisabledTrackColor(QColor(40, 40, 40));
scrollBar->setDarkDisabledThumbColor(QColor(60, 60, 60));
```

### 箭头按钮颜色

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Vertical);

// 自定义箭头颜色
scrollBar->setLightArrowColor(QColor(100, 100, 100));
scrollBar->setLightArrowHoverColor(QColor(50, 50, 50));
scrollBar->setLightArrowPressColor(QColor(0, 0, 0));

scrollBar->setDarkArrowColor(QColor(150, 150, 150));
scrollBar->setDarkArrowHoverColor(QColor(200, 200, 200));
scrollBar->setDarkArrowPressColor(QColor(255, 255, 255));

scrollBar->setRange(0, 1000);
```

### 范围动画完成

```cpp
// 创建滚动条
NScrollBar* scrollBar = new NScrollBar(Qt::Vertical);
scrollBar->setRange(0, 1000);

// 监听范围动画完成
connect(scrollBar, &NScrollBar::rangeAnimationFinished, [](int value) {
    qDebug() << "范围动画已完成";
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NScrollBar(QWidget* parent = nullptr)` | 创建水平滚动条 |
| `NScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr)` | 创建指定方向的滚动条 |
| `NScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr)` | 替换现有滚动条 |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `thumbThickness` | 滑块厚度（悬停时） | `int` |
| `thumbNormalThickness` | 滑块正常厚度 | `int` |
| `trackThickness` | 轨道厚度 | `int` |
| `thumbCornerRadius` | 滑块圆角半径 | `int` |
| `trackCornerRadius` | 轨道圆角半径 | `int` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightTrackColor` | 明亮主题轨道色 | `QColor` |
| `darkTrackColor` | 暗黑主题轨道色 | `QColor` |
| `lightThumbColor` | 明亮主题滑块色 | `QColor` |
| `darkThumbColor` | 暗黑主题滑块色 | `QColor` |
| `lightThumbHoverColor` | 明亮主题滑块悬停色 | `QColor` |
| `darkThumbHoverColor` | 暗黑主题滑块悬停色 | `QColor` |
| `lightThumbPressColor` | 明亮主题滑块按下色 | `QColor` |
| `darkThumbPressColor` | 暗黑主题滑块按下色 | `QColor` |
| `lightArrowColor` | 明亮主题箭头色 | `QColor` |
| `darkArrowColor` | 暗黑主题箭头色 | `QColor` |
| `lightArrowHoverColor` | 明亮主题箭头悬停色 | `QColor` |
| `darkArrowHoverColor` | 暗黑主题箭头悬停色 | `QColor` |
| `lightArrowPressColor` | 明亮主题箭头按下色 | `QColor` |
| `darkArrowPressColor` | 暗黑主题箭头按下色 | `QColor` |

### 继承的 API

NScrollBar 继承自 `QScrollBar`，支持所有标准 QScrollBar 的方法：

- `setRange()` / `range()` - 设置/获取范围
- `setValue()` / `value()` - 设置/获取当前值
- `setPageStep()` / `pageStep()` - 设置/获取页面步长
- `setSingleStep()` / `singleStep()` - 设置/获取单步长
- `valueChanged()` - 值改变信号
- `sliderPressed()` / `sliderReleased()` - 滑块按下/释放信号

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `rangeAnimationFinished()` | 范围动画已完成 | - |

## 主题定制

滚动条会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
