# NTreeView 树形视图

### 介绍

NTreeView 基于 `QTreeView`，提供 Fluent 行绘制（全宽圆角背景、主题色选中条、chevron 旋转、展开 reveal）、空状态占位与头文字。C++ 属性访问为 `setXxx` / `getXxx`。模型、选择、编辑等仍走 `QTreeView`。

勾选联动：项带 `ItemIsUserCheckable` 时，点击复选框会向下级联；父节点根据子项自动变为选中 / 未选 / 半选（`PartiallyChecked`）。父节点建议加 `ItemIsAutoTristate`。

### 基本用法

```cpp
NTreeView* treeView = new NTreeView();
treeView->setHeaderText(QStringLiteral("资源管理器"));
treeView->setPlaceholderText(QStringLiteral("暂无数据"));

auto* model = new QStandardItemModel(treeView);
QStandardItem* root = new QStandardItem(QStringLiteral("文档"));
root->appendRow(new QStandardItem(QStringLiteral("快速开始")));
model->appendRow(root);
treeView->setModel(model);
treeView->expandAll();
```

### 勾选联动

```cpp
auto* group = new QStandardItem(QStringLiteral("分类"));
group->setFlags(group->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
group->setCheckState(Qt::Unchecked);

auto* child = new QStandardItem(QStringLiteral("子项"));
child->setFlags(child->flags() | Qt::ItemIsUserCheckable);
child->setCheckState(Qt::Unchecked);
group->appendRow(child);
```

### 展开 / 折叠

```cpp
treeView->expandAll();
treeView->collapseAll();
treeView->toggleExpanded(model->index(0, 0));
```

### 项目选择

```cpp
treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
treeView->setSelectionIndicatorVisible(true);

connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
        [](const QItemSelection& selected, const QItemSelection&) {
            qDebug() << selected.indexes().size();
        });
```

### 项目编辑

```cpp
treeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
item->setFlags(item->flags() | Qt::ItemIsEditable);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NTreeView(QWidget* parent = nullptr)` | 创建树形视图 |

### 行为属性 / 方法

| 名称 | 说明 |
|------|------|
| `placeholderText` | 空数据占位 |
| `headerText` | 视口上方标题 |
| `selectionIndicatorVisible` | 左侧选中条 |
| `isShowingPlaceholder()` | 是否在画占位 |
| `expandAll()` / `collapseAll()` | 展开 / 折叠全部 |
| `toggleExpanded(index)` | 切换展开并播动画 |
| `chevronRotation(index)` | chevron 旋转插值 `[0,1]` |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `itemHeight` | 项目高度 | `int` |
| `itemBorderRadius` | 项目圆角半径 | `int` |
| `borderRadius` | 边框圆角半径 | `int` |
| `indentation` | 缩进宽度 | `int` |
| `borderVisible` | 是否显示边框 | `bool` |
| `backgroundVisible` | 是否显示背景 | `bool` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` / `darkBackgroundColor` | 背景色 | `QColor` |
| `lightItemHoverColor` / `darkItemHoverColor` | 悬停色 | `QColor` |
| `lightItemSelectedColor` / `darkItemSelectedColor` | 选中色 | `QColor` |
| `lightItemPressedColor` / `darkItemPressedColor` | 按下色 | `QColor` |
| `lightTextColor` / `darkTextColor` | 文本色 | `QColor` |
| `lightPlaceholderTextColor` / `darkPlaceholderTextColor` | 占位文本色 | `QColor` |
| `lightBorderColor` / `darkBorderColor` | 边框色 | `QColor` |

### 继承的 API

NTreeView 继承自 `QTreeView`，模型、选择、编辑、滚动等仍用标准接口。`setModel()` 会接上模型信号以刷新空状态占位和无障碍描述。

### 无障碍

使用 Qt 内置 `QAccessibleTree`，无需自定义工厂。给控件设 `accessibleName`；空数据且有 `placeholderText` 时会写入 `accessibleDescription`。模型先于视图析构时不会再访问已释放的 `model()`。项级补充说明用 `Qt::AccessibleDescriptionRole`。

## 主题定制

树形视图会自动响应应用主题与 accent 变化。
