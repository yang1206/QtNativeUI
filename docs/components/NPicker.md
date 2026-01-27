# NPicker 滚轮选择器

### 介绍

滚轮选择器用于从预定义的选项列表中通过滚动选择一个值。NPicker 提供了现代化的外观和流畅的滚动体验，支持循环滚动和自定义样式。

### 基本用法

```cpp
// 基本用法
NPicker* picker = new NPicker();
picker->setItems({"选项1", "选项2", "选项3"});

// 设置当前选中项
picker->setCurrentIndex(1);

// 监听选择改变
connect(picker, &NPicker::currentDataChanged, [](const QString& data) {
    qDebug() << "当前选择：" << data;
});
```

### 循环滚动

默认情况下，NPicker 开启了循环滚动功能，用户可以无限滚动选项列表。

```cpp
NPicker* picker = new NPicker();
picker->setLoopEnabled(true); // 默认即为 true
picker->setItems({"上午", "下午"});
```

### 容器模式

当设置 `Container` 属性为 `true` 且可见项数量大于等于 5 时，组件会在顶部和底部显示滚动箭头，支持点击箭头进行滚动。

```cpp
NPicker* picker = new NPicker();
picker->setContainer(true);
picker->setVisibleItemCount(5);
picker->setItems({"1", "2", "3", "4", "5", "6", "7"});
```

### 自定义外观

可以调整选项高度、可见数量和边框圆角等属性。

```cpp
NPicker* picker = new NPicker();
picker->setItemHeight(40); // 设置选项高度，默认为 30
picker->setVisibleItemCount(7); // 设置可见选项数量，默认为 5
picker->setBorderRadius(8); // 设置圆角
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NPicker(QWidget* parent = nullptr)` | 创建滚轮选择器 |

### 属性

| 属性 | 说明 | 类型 | 默认值 |
|------|------|------|--------|
| `BorderRadius` | 边框圆角 | `int` | `DefaultCornerRadius` |
| `Items` | 选项列表 | `QStringList` | - |
| `ItemHeight` | 选项高度 | `int` | `30` |
| `VisibleItemCount` | 可见选项数量 | `int` | `5` |
| `CurrentIndex` | 当前索引 | `int` | `0` |
| `Container` | 是否作为容器（显示滚动箭头） | `bool` | `false` |
| `LoopEnabled` | 是否循环滚动 | `bool` | `true` |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `setCurrentData(const QString& data)` | 设置当前数据 | `data`: 数据字符串 |
| `getCurrentData()` | 获取当前数据 | - |
| `setItems(QStringList Items)` | 设置选项列表 | `Items`: 字符串列表 |
| `getItems()` | 获取选项列表 | - |
| `setItemHeight(int height)` | 设置选项高度 | `height`: 高度值 |
| `getItemHeight()` | 获取选项高度 | - |
| `setVisibleItemCount(int count)` | 设置可见选项数量 | `count`: 数量 |
| `getVisibleItemCount()` | 获取可见选项数量 | - |
| `setCurrentIndex(int index)` | 设置当前索引 | `index`: 索引值 |
| `getCurrentIndex()` | 获取当前索引 | - |
| `setLoopEnabled(bool loop)` | 设置是否循环滚动 | `loop`: 是否循环 |
| `setContainer(bool container)` | 设置是否作为容器 | `container`: 是否容器 |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `currentDataChanged(const QString& data)` | 当前数据改变时触发 | `data`: 新的数据 |
