# NTabBar 标签栏

### 介绍

标签栏用于在多个标签之间切换。NTabBar 基于 Qt 的 QTabBar，提供了现代化的外观、可关闭标签、拖拽支持和丰富的自定义选项。

### 基本用法

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");

// 设置当前标签
tabBar->setCurrentIndex(0);

// 监听标签切换
connect(tabBar, QOverload<int>::of(&QTabBar::currentChanged), [](int index) {
    qDebug() << "切换到标签：" << index;
});
```

### 带图标的标签

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加带图标的标签
tabBar->addTab(QIcon(":/icons/home.png"), "首页");
tabBar->addTab(QIcon(":/icons/settings.png"), "设置");
tabBar->addTab(QIcon(":/icons/about.png"), "关于");

// 设置当前标签
tabBar->setCurrentIndex(0);
```

### 可关闭的标签

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 启用标签关闭按钮
tabBar->setTabsClosable(true);

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");

// 监听标签关闭
connect(tabBar, &QTabBar::tabCloseRequested, [=](int index) {
    qDebug() << "关闭标签：" << index;
    tabBar->removeTab(index);
});
```

### 标签工具提示

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("文件");
tabBar->addTab("编辑");
tabBar->addTab("视图");

// 设置工具提示
tabBar->setTabToolTip(0, "文件操作：新建、打开、保存等");
tabBar->setTabToolTip(1, "编辑操作：撤销、重做、剪切等");
tabBar->setTabToolTip(2, "视图选项：显示/隐藏工具栏等");
```

### 标签宽度与关闭策略

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");

tabBar->setTabWidthMode(NTabBar::TabWidthMode::Equal); // 默认，等宽
// SizeToContent 按内容定宽；Compact 仅将带图标的非当前标签缩为图标
tabBar->setTabMinimumWidth(100);
tabBar->setTabMaximumWidth(240);
tabBar->setTabsClosable(true);
tabBar->setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::OnHover);
// Always 始终显示；OnHover 在悬停或选中时显示；Never 隐藏按钮
tabBar->setTabClosable(0, false); // 也禁止中键和 Ctrl+W / Ctrl+F4 关闭
```

标签文字由 `setTabText()` 更新；`setTabData()` 只保存业务数据，不启用文字编辑。
颜色、字体、间距和圆角继续使用本项目 token，原有颜色属性仍可覆盖。
`Compact` 模式下，没有图标的标签会自动按 `SizeToContent` 显示，避免只剩无意义的省略号；
带图标的非当前标签才折叠为图标，当前标签始终显示完整内容。
页签顶部默认取 `CornerRadiusMedium`（当前 8px），底部外扩弧独立取 `CornerRadiusDefault`（当前 4px）。
`setBorderRadius()` 只调整页签顶部圆角；`NTabWidget::setBorderRadius()` 调整整体外框圆角，两者互不覆盖。

### 标签拖拽

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");

// 栏内重排使用 Qt 原生行为
tabBar->setMovable(true);

// 拖出通知默认关闭；越过栏边界并达到拖拽距离后才会创建截图
tabBar->setTabDragEnabled(true);
connect(tabBar, &NTabBar::tabDragStarted, [](QDrag* drag) {
    // 可在真正开始前调整热点、拖拽图或光标；不要在这里再次调用 exec()
    drag->setDragCursor(QPixmap(":/cursors/move.png"), Qt::MoveAction);
});

// 监听拖拽完成
connect(tabBar, &NTabBar::tabDropped, [](QMimeData* data) {
    qDebug() << "标签拖拽已完成";
});
```

组件会在 `tabDragStarted` 同步回调返回后自动执行 `QDrag::exec(Qt::MoveAction)`。
该信号只用于开始前配置拖拽对象，不应再次调用 `exec()`。
`QDrag*` 和 `tabDropped` 的 `QMimeData*` 都是临时借用对象，不应跨回调保存。
内建放下处理只接受同一标签栏的稳定 ID；跨窗口转移页面由业务实现。

### 与 QTabBar 兼容

`NTabBar` 保留 `QTabBar` 的标签数据、助记键、逐项文字颜色、自定义左右按钮、滚动按钮、
自动隐藏、移动和移除选择策略。`setTabButton()` 放入的业务按钮不会被当成关闭按钮，尺寸、
可见性、焦点策略和绘制都仍由业务控件自己管理。

```cpp
tabBar->setTabTextColor(0, QColor(196, 43, 28));
tabBar->setAccessibleTabName(0, QStringLiteral("告警文档标签"));

auto* statusButton = new QToolButton(tabBar);
statusButton->setAccessibleName(QStringLiteral("查看同步状态"));
tabBar->setTabButton(0, QTabBar::RightSide, statusButton);
```

未知 MIME 的拖放事件会继续交给 `QTabBar`，因此 `changeCurrentOnDrag` 等 Qt 原生行为不会被
组件自己的页签拖出协议截断。

### 标签按下事件

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");

// 监听标签按下
connect(tabBar, &NTabBar::tabBarPressed, [](int index) {
    qDebug() << "标签已按下：" << index;
});
```

### 自定义样式

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 设置圆角
tabBar->setBorderRadius(6);

// 自定义颜色
tabBar->setLightItemHeaderBackground(QColor(240, 240, 240));
tabBar->setLightItemHeaderBackgroundSelected(QColor(0, 120, 215));
tabBar->setLightItemHeaderBackgroundPointerOver(QColor(230, 230, 230));
tabBar->setLightItemHeaderBackgroundPressed(QColor(200, 200, 200));

tabBar->setLightItemHeaderForeground(QColor(100, 100, 100));
tabBar->setLightItemHeaderForegroundSelected(QColor(255, 255, 255));
tabBar->setLightItemHeaderForegroundPointerOver(QColor(50, 50, 50));
tabBar->setLightItemHeaderForegroundPressed(QColor(0, 0, 0));

// 暗黑主题颜色
tabBar->setDarkItemHeaderBackground(QColor(50, 50, 50));
tabBar->setDarkItemHeaderBackgroundSelected(QColor(0, 150, 255));
tabBar->setDarkItemHeaderBackgroundPointerOver(QColor(70, 70, 70));
tabBar->setDarkItemHeaderBackgroundPressed(QColor(100, 100, 100));

tabBar->setDarkItemHeaderForeground(QColor(200, 200, 200));
tabBar->setDarkItemHeaderForegroundSelected(QColor(255, 255, 255));
tabBar->setDarkItemHeaderForegroundPointerOver(QColor(220, 220, 220));
tabBar->setDarkItemHeaderForegroundPressed(QColor(255, 255, 255));

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");
```

### 图标颜色自定义

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 自定义图标颜色
tabBar->setLightItemIconForeground(QColor(100, 100, 100));
tabBar->setLightItemIconForegroundSelected(QColor(255, 255, 255));
tabBar->setLightItemIconForegroundPointerOver(QColor(50, 50, 50));
tabBar->setLightItemIconForegroundPressed(QColor(0, 0, 0));

tabBar->setDarkItemIconForeground(QColor(200, 200, 200));
tabBar->setDarkItemIconForegroundSelected(QColor(255, 255, 255));
tabBar->setDarkItemIconForegroundPointerOver(QColor(220, 220, 220));
tabBar->setDarkItemIconForegroundPressed(QColor(255, 255, 255));

// 添加带图标的标签
tabBar->addTab(QIcon(":/icons/home.png"), "首页");
tabBar->addTab(QIcon(":/icons/settings.png"), "设置");
```

### 分隔线颜色

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 自定义分隔线颜色
tabBar->setLightItemSeparator(QColor(200, 200, 200));
tabBar->setDarkItemSeparator(QColor(80, 80, 80));

// 自定义选中状态下的边框颜色
tabBar->setLightItemHeaderBorderColorSelected(QColor(0, 120, 215));
tabBar->setDarkItemHeaderBorderColorSelected(QColor(0, 150, 255));

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");
```

### 悬停状态

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");

// 获取悬停的标签索引
int hoverIndex = tabBar->hoverIndex();
if (hoverIndex >= 0) {
    qDebug() << "悬停在标签：" << hoverIndex;
}
```

### 禁用状态

```cpp
// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab("标签2");
tabBar->addTab("标签3");

// 禁用某个标签
tabBar->setTabEnabled(1, false);

// 自定义禁用状态颜色
tabBar->setLightItemHeaderBackgroundDisabled(QColor(245, 245, 245));
tabBar->setLightItemHeaderForegroundDisabled(QColor(180, 180, 180));
tabBar->setDarkItemHeaderBackgroundDisabled(QColor(40, 40, 40));
tabBar->setDarkItemHeaderForegroundDisabled(QColor(100, 100, 100));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NTabBar(QWidget* parent = nullptr)` | 创建标签栏 |

### 标签管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `addTab(const QString& text)` | 添加标签 | 返回标签索引 |
| `addTab(const QIcon& icon, const QString& text)` | 添加带图标的标签 | 返回标签索引 |
| `insertTab(int index, const QString& text)` | 在指定位置插入标签 | 返回标签索引 |
| `removeTab(int index)` | 移除标签 | - |
| `setTabText(int index, const QString& text)` | 设置标签文本 | - |
| `tabText(int index)` | 获取标签文本 | 返回 `QString` |
| `setTabIcon(int index, const QIcon& icon)` | 设置标签图标 | - |
| `tabIcon(int index)` | 获取标签图标 | 返回 `QIcon` |
| `count()` | 获取标签总数 | 返回 `int` |

### 标签状态

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setCurrentIndex(int index)` | 设置当前标签 | 标签索引 |
| `currentIndex()` | 获取当前标签索引 | 返回 `int` |
| `setTabEnabled(int index, bool enabled)` | 启用/禁用标签 | - |
| `isTabEnabled(int index)` | 判断标签是否启用 | 返回 `bool` |
| `hoverIndex()` | 获取悬停的标签索引 | 返回 `int` |
| `setAccessibleTabName(int index, const QString& name)` | 设置读屏使用的标签名称 | - |

### 标签关闭

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setTabsClosable(bool closable)` | 启用/禁用标签关闭按钮 | 布尔值 |
| `tabsClosable()` | 获取是否启用标签关闭按钮 | 返回 `bool` |
| `setTabClosable(int index, bool closable)` | 设置单个标签能否关闭 | - |
| `getTabClosable(int index)` | 获取单个标签关闭权限 | 返回 `bool` |
| `requestTabClose(int index)` | 按权限发出关闭请求 | - |

### 布局与拖拽

| 方法 | 说明 |
|------|------|
| `setTabWidthMode(TabWidthMode)` | 设置 `Equal`、`SizeToContent` 或 `Compact` 宽度模式 |
| `setCloseButtonDisplayMode(CloseButtonDisplayMode)` | 设置 `Always`、`OnHover` 或 `Never` |
| `setTabMinimumWidth(int)` / `setTabMaximumWidth(int)` | 设置标签宽度范围 |
| `setTabHeight(int)` | 设置标签高度 |
| `setTabDragEnabled(bool)` | 启用稳定 ID 的栏外拖出协议 |

### 工具提示

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setTabToolTip(int index, const QString& tip)` | 设置标签工具提示 | - |
| `tabToolTip(int index)` | 获取标签工具提示 | 返回 `QString` |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `borderRadius` | 边框圆角半径 | `int` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightItemHeaderBackground` | 明亮主题标签背景色 | `QColor` |
| `darkItemHeaderBackground` | 暗黑主题标签背景色 | `QColor` |
| `lightItemHeaderBackgroundSelected` | 明亮主题选中背景色 | `QColor` |
| `darkItemHeaderBackgroundSelected` | 暗黑主题选中背景色 | `QColor` |
| `lightItemHeaderForeground` | 明亮主题标签文本色 | `QColor` |
| `darkItemHeaderForeground` | 暗黑主题标签文本色 | `QColor` |
| `lightItemIconForeground` | 明亮主题图标色 | `QColor` |
| `darkItemIconForeground` | 暗黑主题图标色 | `QColor` |
| `lightItemSeparator` | 明亮主题分隔线色 | `QColor` |
| `darkItemSeparator` | 暗黑主题分隔线色 | `QColor` |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `currentChanged(int index)` | 当前标签已改变 | 新的标签索引 |
| `tabCloseRequested(int index)` | 标签关闭被请求 | 标签索引 |
| `tabTextChanged(int index)` | 标签文本已改变 | 标签索引 |
| `tabBarPressed(int index)` | 标签已按下 | 标签索引 |
| `tabDragStarted(QDrag* drag)` | 栏外拖拽即将开始，可同步配置拖拽对象 | 临时借用的拖拽对象 |
| `tabDropped(QMimeData* data)` | 标签拖拽已完成 | MIME 数据 |

## 主题定制

标签栏会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。

## 无障碍与键盘

控件沿用 Qt 原生 `QAccessibleTabBar`，向 UI Automation/辅助技术暴露 `PageTabList`、
`PageTab`、当前选择和按压动作。方向键、助记键以及 Qt 原生的 Ctrl+Tab 行为保持不变；
当前标签的关闭按钮可通过 Tab 键获得焦点，并提供标签名称、说明和焦点视觉。

业务应为含义不够清楚的标签调用 `setAccessibleTabName()`，为自定义 `tabButton()` 设置
`accessibleName`。关闭信号仍只是请求，页面是否允许关闭及销毁方式由业务处理。
