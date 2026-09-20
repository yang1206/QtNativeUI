# NTableView 表格视图

### 介绍

NTableView 基于 `QTableView`，API 与用法与 Qt 表格视图兼容，并增加 Fluent 风格外观：圆角卡片边框、主题色、自绘表头与单元格、覆盖式 `NScrollBar`、空状态占位与行级交互反馈。

### 基本用法

```cpp
NTableView* table = new NTableView();
QStandardItemModel* model = new QStandardItemModel();
model->setHorizontalHeaderLabels({QStringLiteral("名称"), QStringLiteral("值")});
model->appendRow({new QStandardItem(QStringLiteral("条目 A")), new QStandardItem(QStringLiteral("1"))});
table->setModel(model);
```

默认启用表头排序（`setSortingEnabled(true)`），默认按**整行**选择（`SelectRows`）。选择模式、编辑触发器等与 `QTableView` 相同，可按需覆盖。

### 表头排序

点击列标题在升序 / 降序之间切换，当前排序列显示指示箭头。

- `QStandardItemModel` 等自带 `sort()` 的模型可直接使用。
- 自定义 `QAbstractTableModel` 必须重写 `sort(int column, Qt::SortOrder order)`，否则点击表头无数据变化。
- 也可将 `QSortFilterProxyModel` 设为视图模型，由代理负责排序。

```cpp
void MyTableModel::sort(int column, Qt::SortOrder order) override {
    beginResetModel();
    // 重排内部容器
    endResetModel();
}
```

### 单元格编辑

默认 `EditTriggers` 为 `NoEditTriggers`（与常见只读列表一致）。需要编辑时自行开启：

```cpp
table->setSelectionBehavior(QAbstractItemView::SelectItems);
table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked
                       | QAbstractItemView::EditKeyPressed);
```

编辑器使用 `NLineEdit`，样式与主题一致。模型项需具备 `Qt::ItemIsEditable`（`QStandardItem` 默认可编辑）。

### 空状态

```cpp
table->setPlaceholderText(QStringLiteral("暂无数据"));
// 模型无行/无列时，在视口中央显示占位文案，并写入 accessibleDescription
```

### 行号列

```cpp
table->setVerticalHeaderVisible(true);
```

显示左侧行号表头（默认隐藏）。圆角裁剪会随表头显示一并更新。

### 与 QTableView 的差异（增强项）

| 能力 | 说明 |
|------|------|
| 主题色属性 | Light/Dark 成对颜色，随 `NTheme` 切换 |
| `borderRadius` / `borderVisible` / `backgroundVisible` | 圆角卡片与边框 |
| `rowHeight` / `headerHeight` / `cellHorizontalPadding` | 行高、表头高、单元格内边距 |
| `placeholderText` | 空表占位 |
| 覆盖滚动条 | 原生滚动条隐藏，使用 `NScrollBar` 浮层 |
| 行悬停 / 行选强调条 | `SelectRows` 时首列显示强调色条 |

其余 `setModel`、`setSelectionModel`、`horizontalHeader()`、`verticalHeader()`、`setColumnWidth`、`setSpan` 等均与 `QTableView` 一致。

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NTableView(QWidget* parent = nullptr)` | 创建表格视图 |

### 布局与外观

| 属性 / 方法 | 说明 | 类型 |
|------------|------|------|
| `rowHeight` | 默认行高 | `int` |
| `headerHeight` | 水平表头高度 | `int` |
| `borderRadius` | 外框圆角 | `int` |
| `cellHorizontalPadding` | 单元格左右内边距 | `int` |
| `borderVisible` | 是否绘制圆角边框 | `bool` |
| `backgroundVisible` | 是否绘制不透明底色 | `bool` |
| `placeholderText` | 空状态文案 | `QString` |
| `setVerticalHeaderVisible` / `isVerticalHeaderVisible` | 行号列表头 | `bool` |
| `isShowingPlaceholder` | 当前是否显示占位 | `bool` |

### 颜色属性

与 `NListView` / `NTreeView` 相同模式：`light*` / `dark*` 成对属性，涵盖背景、表头、悬停、选中、文本、分隔线、边框、占位符等。

### 继承的 API

NTableView 继承 `QTableView`，支持标准表格 API，例如：

- `setModel()` / `model()`、`setSelectionModel()`
- `setSelectionMode()` / `setSelectionBehavior()`
- `setSortingEnabled()` / `sortByColumn()`
- `setEditTriggers()`、`currentIndex()`、`indexAt()`
- `horizontalHeader()` / `verticalHeader()`
- 信号：`clicked`、`doubleClicked`、`activated`

无障碍使用 Qt 内置 `QAccessibleTable`，无需额外工厂。

## 主题定制

表格会监听 `NTheme` 的 `themeModeChanged` / `accentColorChanged`，自动刷新调色板与绘制。
