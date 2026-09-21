# NListView 列表视图

### 介绍

NListView 基于 `QListView`，API 与 Qt 列表视图兼容，并增加 Fluent 行样式：圆角卡片、主题色、覆盖式 `NScrollBar`、空状态占位、头尾文字、分组吸顶、拖拽重排与左侧选中指示条。

C++ 属性访问为 `setXxx` / `getXxx`（例如 `setItemHeight` / `getItemHeight`）。模型、选择、滚动、`viewMode` 等仍走 `QListView`。

### 基本用法

```cpp
NListView* list = new NListView();
auto* model = new QStandardItemModel(list);
model->appendRow(new QStandardItem(QStringLiteral("项目 1")));
model->appendRow(new QStandardItem(QStringLiteral("项目 2")));
list->setModel(model);

connect(list->selectionModel(), &QItemSelectionModel::currentChanged,
        [](const QModelIndex& current, const QModelIndex&) {
            if (current.isValid())
                qDebug() << current.data().toString();
        });
```

### Fluent 行样式

内置 `NListItemDelegate` 在 **ListMode** 读取下列角色（`NListViewType::ItemDataRole`）：

| 角色 | 用途 |
|------|------|
| `Qt::DisplayRole` | 主标题 |
| `Qt::DecorationRole` | 行图标 |
| `NListViewType::SubtitleRole` | 副标题（双行，行高自动加约 18px） |
| `NListViewType::ShowChevronRole` | 右侧 Chevron（`bool`） |
| `Qt::CheckStateRole` + `ItemIsUserCheckable` | 行首复选指示器 |
| `NListViewType::ComboChoicesRole` | 内联下拉选项（`QStringList`） |
| `NListViewType::SectionRole` | 分组标题（相邻相同字符串为一组） |
| `NListViewType::SectionForegroundRole` | 该组标题颜色（`QColor`） |

```cpp
QStandardItem* item = new QStandardItem(QStringLiteral("账户"));
item->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
item->setData(QStringLiteral("管理登录与安全"), NListViewType::SubtitleRole);
item->setData(QStringLiteral("管理登录与安全"), Qt::AccessibleDescriptionRole);
item->setData(true, NListViewType::ShowChevronRole);
```

`SubtitleRole` 只用于绘制，不会自动进入读屏。需要无障碍时再写 `Qt::AccessibleDescriptionRole`。

### 空状态与头尾文字

```cpp
list->setPlaceholderText(QStringLiteral("暂无数据"));
list->setHeaderText(QStringLiteral("任务"));
list->setFooterText(QStringLiteral("共 3 项"));
```

`isShowingPlaceholder()` 在设置了 `placeholderText` 且 `rowCount()==0` 时为 `true`。头尾文字画在视口外边距里，不占用 item 布局。

### 分组与吸顶

默认关闭。打开后，相邻 `SectionRole` 相同的行合成一组；组头高度由 `sectionHeaderHeight` 控制（默认 28），字体用 `QFont`，不要再拆字号/字重 API。

```cpp
list->setSectionsEnabled(true);
list->setSectionHeaderHeight(36);
QFont sectionFont = list->font();
sectionFont.setPixelSize(16);
sectionFont.setWeight(QFont::DemiBold);
list->setSectionHeaderFont(sectionFont);

auto* work = new QStandardItem(QStringLiteral("写周报"));
work->setData(QStringLiteral("工作"), NListViewType::SectionRole);
work->setData(nTheme->accentColor().normal(), NListViewType::SectionForegroundRole);
```

ListMode 下分组标题会吸顶：当前组头滚出视口后钉在视口顶，下一组上来时把它顶走。IconMode、拖拽重排过程中不画吸顶。没有单独的「吸顶开关」。

### 拖拽重排

```cpp
list->setReorderEnabled(true);
list->setSelectionMode(QAbstractItemView::SingleSelection);
item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled
               | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

connect(list, &NListView::rowsReordered, [](int from, int to) {
    qDebug() << from << "->" << to;
});
```

这是自定义鼠标拖放（幽灵行 + 落点线 + 行位移），**不是** `QDrag` / `InternalMove`。约束：

- 仅 ListMode。
- `QStandardItemModel` 走 `takeRow` / `insertRow`；其它模型需要 `moveRow` 真能改数据。
- 视图会关掉 Qt 拖放（`NoDragDrop`），外部文件拖入不做。
- 成功后发 `rowsReordered(sourceRow, destinationRow)`。

### 图标网格（IconMode）

网格用 Qt 自己的 API，不要再加属性：

```cpp
list->setViewMode(QListView::IconMode);
list->setWrapping(true);
list->setFlow(QListView::LeftToRight);
list->setResizeMode(QListView::Adjust);
list->setMovement(QListView::Static);
list->setIconSize(QSize(48, 48));
list->setGridSize(QSize(104, 112));
list->setEditTriggers(QAbstractItemView::NoEditTriggers);

auto* tile = new QStandardItem(QStringLiteral("文档"));
tile->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
tile->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
```

`QStandardItem` 默认带 `ItemIsEditable`。IconMode 的内置 delegate **不创建编辑器**；演示里仍建议 `NoEditTriggers` 并去掉可编辑标志。IconMode 不做分组吸顶、拖拽重排和左侧选中指示条。只设 `IconMode` 不设 `wrapping` / `gridSize` 不会变成网格。

### 内联编辑（ListMode）

默认触发器与 `QListView` 相同。需要编辑时自行打开，项上要有 `ItemIsEditable`：

```cpp
list->setEditTriggers(QAbstractItemView::DoubleClicked
                      | QAbstractItemView::SelectedClicked
                      | QAbstractItemView::EditKeyPressed);

QStandardItem* text = new QStandardItem(QStringLiteral("可编辑"));
text->setFlags(text->flags() | Qt::ItemIsEditable);

QStandardItem* combo = new QStandardItem(QStringLiteral("A"));
combo->setFlags(combo->flags() | Qt::ItemIsEditable);
combo->setData(QStringList{QStringLiteral("A"), QStringLiteral("B")},
               NListViewType::ComboChoicesRole);
```

编辑器与 `NTableView` 文本单元格一致：`NLineEdit` / `NComboBox`，数值按 `Qt::EditRole` 元类型用 `NSpinBox` / `NDoubleSpinBox`。`ComboChoicesRole` 未设时，可回退 `Qt::UserRole` 里的 `QStringList`。调用 `setItemDelegate()` 后需自己处理 `createEditor`。

### 选中指示条

`selectionIndicatorVisible` 默认 `true`。`SingleSelection` 下由 `NSelectionIndicatorMotion` 驱动（约 270ms）；`MultiSelection` / `ExtendedSelection` 为每行静态条。`selectionIndicatorAnimated` 只影响单选动画。

### 过滤

过滤用 Qt 的 `QSortFilterProxyModel`，视图不包一层：

```cpp
auto* proxy = new QSortFilterProxyModel(list);
proxy->setSourceModel(model);
proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
list->setModel(proxy);
connect(searchEdit, &QLineEdit::textChanged, proxy, &QSortFilterProxyModel::setFilterFixedString);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NListView(QWidget* parent = nullptr)` | 创建列表视图 |

### 布局与外观

| 属性 | 方法 | 说明 | 默认 |
|------|------|------|------|
| `itemHeight` | `setItemHeight` / `getItemHeight` | ListMode 行高；有副标题时会加高 | `36` |
| `itemBorderRadius` | `setItemBorderRadius` / `getItemBorderRadius` | 行圆角 | `4` |
| `borderRadius` | `setBorderRadius` / `getBorderRadius` | 外框圆角 | `8` |
| `borderVisible` | `setBorderVisible` / `getBorderVisible` | 外框描边 | `true` |
| `backgroundVisible` | `setBackgroundVisible` / `getBackgroundVisible` | 控件背景 | `true` |
| `placeholderText` | `setPlaceholderText` / `getPlaceholderText` | 空数据占位 | 空 |
| `headerText` | `setHeaderText` / `getHeaderText` | 视口上方标题 | 空 |
| `footerText` | `setFooterText` / `getFooterText` | 视口下方页脚 | 空 |
| `isShowingPlaceholder()` | — | 当前是否在画占位 | — |

### 分组

| 属性 | 方法 | 说明 | 默认 |
|------|------|------|------|
| `sectionsEnabled` | `setSectionsEnabled` / `getSectionsEnabled` | 按 `SectionRole` 分组，并启用吸顶 | `false` |
| `sectionHeaderHeight` | `setSectionHeaderHeight` / `getSectionHeaderHeight` | 组头高度 | `28` |
| `sectionHeaderFont` | `setSectionHeaderFont` / `getSectionHeaderFont` | 组头字体 | Caption 字号 |
| `lightSectionTextColor` / `darkSectionTextColor` | `set*` / `get*` | 组头默认颜色 | Secondary 文本色 |

### 交互

| 属性 / 信号 | 方法 | 说明 | 默认 |
|-------------|------|------|------|
| `selectionIndicatorVisible` | `setSelectionIndicatorVisible` / `getSelectionIndicatorVisible` | 左侧选中条 | `true` |
| `selectionIndicatorAnimated` | `setSelectionIndicatorAnimated` / `getSelectionIndicatorAnimated` | 单选指示条动画 | `true` |
| `reorderEnabled` | `setReorderEnabled` / `getReorderEnabled` | ListMode 鼠标重排 | `false` |
| `rowsReordered(int, int)` | — | 重排完成 | — |

### 颜色属性

Light / Dark 成对：`background`、`itemHover`、`itemSelected`、`itemPressed`、`text`、`placeholderText`、`border`、`sectionText`。随 `NTheme` 切换，无需手写 `themeModeChanged`。

### 继承的 API

- `setModel()` / `model()`：切换模型时会断开旧模型信号
- `setSelectionMode()` / `setSelectionModel()`
- `setViewMode()` / `setWrapping()` / `setGridSize()` / `setIconSize()` / `setFlow()` / `setResizeMode()`
- `setEditTriggers()` / `editTriggers()`
- `visualRect()`：拖拽位移绘制期间会偏移，命中测试仍用布局矩形
- 信号：`clicked`、`doubleClicked`、`activated`

有垂直或水平滚动范围时，滚轮事件会被 `accept()`，避免嵌套在页面里时滚出到父级。

### 无障碍

使用 Qt 内置 `QAccessibleList`，无需自定义工厂。给控件设 `accessibleName`；空数据且有 `placeholderText` 时会写入 `accessibleDescription`。模型先于视图析构时不会再访问已释放的 `model()`。
