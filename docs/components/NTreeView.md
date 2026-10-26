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
| `setRevealAnimationEnabled(bool)` / `isRevealAnimationEnabled()` | 开关自定义展开 reveal 与箭头旋转，默认开启 |
| `setAnimated(bool)` / `isAnimated()` | 自定义动效开关的兼容入口；Qt 内置展开动画始终关闭 |

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

### 多选与勾选

- 普通单选和不带复选框的多选，继续使用 `QItemSelectionModel` 表达选择。
- 多选模式下，首列可勾选节点的选中背景、蓝色标记以 `Qt::CheckStateRole == Qt::Checked` 为准；未勾选不高亮，半选只显示半选复选框。
- 点击可勾选行的正文或复选框、按空格键，使用同一套父子勾选联动；点击箭头只展开或折叠。禁用节点保留原有勾选值。
- `currentIndex()` 表示键盘焦点所在项；获取业务勾选结果应读取模型的 `Qt::CheckStateRole`，不能用 `selectedIndexes()` 代替。
- 大子树的勾选联动分批返回事件循环执行，处理中父节点显示半选，完成后汇总祖先状态。界面仍可滚动和响应输入；模型清空、销毁或相关结构改变会取消旧任务并校正仍然有效的父节点。
- 右键保留原选择。菜单目标用 `indexAt(viewportPosition)` 获取；动画尚未揭露的区域返回无效索引。

### 动效与多列

展开 reveal 只裁剪子树，不平移 Qt 布局中的后续行，绘制与鼠标命中保持相同位置。动画范围和高度查询限制在视口内；无关分支的行插入不会结束当前动画。`setRevealAnimationEnabled(false)` 会立即收尾当前动画；Qt 属性 `animated` 也映射到该开关。

整行圆角背景只绘制一次，复选框和箭头只出现在首列；其他列的正文与编辑器按各自单元格边界布局。默认代理使用统一行高并开启 `uniformRowHeights`，替换为不等高的自定义代理时请调用 `setUniformRowHeights(false)`。

### 无障碍

使用 Qt 内置 `QAccessibleTree`，无需自定义工厂。给控件设 `accessibleName`；空数据且有 `placeholderText` 时会写入 `accessibleDescription`。模型先于视图析构时不会再访问已释放的 `model()`。项级补充说明用 `Qt::AccessibleDescriptionRole`。

## 主题定制

树形视图会自动响应应用主题与 accent 变化。
