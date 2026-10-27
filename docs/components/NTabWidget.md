# NTabWidget 选项卡控件

### 介绍

选项卡控件用于在多个页面之间切换，每个选项卡对应一个页面内容。NTabWidget 基于 Qt 的 QTabWidget，提供了现代化的外观和增强的功能。

标签栏与页面共用一个完整外框，标签栏两端和空白区使用 `SolidBackgroundFillColorBase`，
选中页签与页面沿用 `LightSelectedBackground` / `DarkSelectedBackground`。
`setBorderRadius()` 控制整体外框圆角，默认取 `CornerRadiusMedium`；`setBorderWidth()` 控制外框线宽。
内部只绘制标签栏与页面的分隔线，并在当前页签处断开。

### 基本用法

```cpp
// 创建选项卡控件
NTabWidget* tabWidget = new NTabWidget();

// 添加选项卡页面
QWidget* page1 = new QWidget();
QWidget* page2 = new QWidget();
QWidget* page3 = new QWidget();

tabWidget->addTab(page1, "首页");
tabWidget->addTab(page2, "设置");
tabWidget->addTab(page3, "关于");

// 设置当前选中的选项卡
tabWidget->setCurrentIndex(0);

// 监听选项卡切换
connect(tabWidget, &QTabWidget::currentChanged, [](int index) {
    qDebug() << "切换到选项卡：" << index;
});
```

### 可关闭选项卡

支持在选项卡上显示关闭按钮，用户可以关闭不需要的选项卡。

```cpp
NTabWidget* closableTabWidget = new NTabWidget();

// 启用选项卡关闭功能
closableTabWidget->setTabsClosable(true);

// 添加一些选项卡
closableTabWidget->addTab(new QTextEdit(), "文档1");
closableTabWidget->addTab(new QTextEdit(), "文档2");
closableTabWidget->addTab(new QTextEdit(), "文档3");

// 监听选项卡关闭请求
connect(closableTabWidget, &QTabWidget::tabCloseRequested, [=](int index) {
    // 确认关闭
    QWidget* widget = closableTabWidget->widget(index);
    closableTabWidget->removeTab(index);
    widget->deleteLater();
});
```

### 标签布局与新增按钮

```cpp
tabWidget->setTabWidthMode(NTabBar::TabWidthMode::SizeToContent);
tabWidget->setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::OnHover);
tabWidget->setTabClosable(0, false);
tabWidget->setMovable(true);
tabWidget->setTabDragEnabled(true);
tabWidget->setAddButtonVisible(true); // 默认隐藏
connect(tabWidget, &NTabWidget::tabAddRequested, tabWidget, [tabWidget]() {
    const int index = tabWidget->addTab(new QWidget(), "新标签");
    tabWidget->setCurrentIndex(index);
});
```

新增按钮及 Ctrl+T 发出 `tabAddRequested`，不会自动创建业务页面。
中键、Ctrl+W 和 Ctrl+F4 发出关闭请求，遵守全局及逐页关闭权限。
这些快捷键使用 `WidgetWithChildrenShortcut`，即使焦点位于当前页的文本编辑器等子控件中也有效。
重排时页面由 Qt 同步移动。组件保留页面显式设置的调色板和背景填充策略。
`Compact` 模式只折叠带图标的非当前标签；无图标标签会按内容显示，不会全部退化成省略号。

### QTabWidget 兼容性

默认 `documentMode` 与 `QTabWidget` 一致为 `false`，显示完整 Fluent 外框。调用
`setDocumentMode(true)` 后去除外层圆角和外框，保留页签与内容区分隔线，适合嵌入主窗口文档区。
`heightForWidth()`、角落控件、自定义页签按钮、逐页文字颜色、自动隐藏、溢出滚动、选择移除策略
和页签重排继续遵循 Qt 语义。

不要在应用级样式表中设置全局 `QTabWidget::pane` 规则。Qt 的 `QStyleSheetStyle` 会在命中该规则时
绕过 `NTabWidget` 的自定义 pane/tab-bar 几何。需要透出父容器背景时，使用
`setTransparentBackground(true)`；它只关闭组件表面和内容区填充，选中标签、边框与分隔线仍会保留。

`removeTab()` / `clear()` 与 Qt 一样只从容器移除页面，不负责销毁页面。`tabCloseRequested()`
也只表示用户请求关闭，业务应保存数据、判断能否关闭，然后调用 `removeTab()` 和 `deleteLater()`。

`setMovable(true)` 提供同一标签栏内的原生重排；`setTabDragEnabled(true)` 在指针越过标签栏边界后
自动启动栏外拖拽。页签拖出窗口后的页面所有权迁移属于文档管理器能力，不是 `QTabWidget`
自带能力，跨窗口接收和转移仍由业务实现。

### 带图标的选项卡

```cpp
NTabWidget* iconTabWidget = new NTabWidget();

// 添加带图标的选项卡
iconTabWidget->addTab(new QWidget(), QIcon(":/icons/home.png"), "首页");
iconTabWidget->addTab(new QWidget(), QIcon(":/icons/settings.png"), "设置");
iconTabWidget->addTab(new QWidget(), QIcon(":/icons/help.png"), "帮助");

// 也可以后续设置图标
iconTabWidget->setTabIcon(0, QIcon(":/icons/new_home.png"));
```

### 文档编辑器示例

```cpp
// 创建多文档编辑器
NTabWidget* documentTabs = new NTabWidget();
documentTabs->setTabsClosable(true);

// 新建文档按钮
QPushButton* newDocBtn = new QPushButton("新建文档");
connect(newDocBtn, &QPushButton::clicked, [=]() {
    static int docCount = 1;
    
    QTextEdit* editor = new QTextEdit();
    editor->setPlaceholderText("开始输入内容...");
    
    QString title = QString("未命名文档%1").arg(docCount++);
    int index = documentTabs->addTab(editor, title);
    documentTabs->setCurrentIndex(index);
    
    // 监听文档修改
    connect(editor, &QTextEdit::textChanged, [=]() {
        QString currentTitle = documentTabs->tabText(documentTabs->currentIndex());
        if (!currentTitle.endsWith("*")) {
            documentTabs->setTabText(documentTabs->currentIndex(), currentTitle + "*");
        }
    });
});

// 保存文档功能
QPushButton* saveBtn = new QPushButton("保存");
connect(saveBtn, &QPushButton::clicked, [=]() {
    int currentIndex = documentTabs->currentIndex();
    if (currentIndex >= 0) {
        QString title = documentTabs->tabText(currentIndex);
        if (title.endsWith("*")) {
            title.chop(1); // 移除 * 标记
            documentTabs->setTabText(currentIndex, title);
        }
        // 这里添加实际的保存逻辑
    }
});
```

### 选项卡位置设置

```cpp
NTabWidget* positionTabWidget = new NTabWidget();

// 设置选项卡位置
positionTabWidget->setTabPosition(QTabWidget::North);  // 顶部（默认）
// positionTabWidget->setTabPosition(QTabWidget::South);  // 底部
// positionTabWidget->setTabPosition(QTabWidget::West);   // 左侧
// positionTabWidget->setTabPosition(QTabWidget::East);   // 右侧

// 设置选项卡形状
positionTabWidget->setTabShape(QTabWidget::Rounded);   // 圆角（默认）
// positionTabWidget->setTabShape(QTabWidget::Triangular); // 三角形
```

### 动态选项卡管理

```cpp
NTabWidget* dynamicTabs = new NTabWidget();
dynamicTabs->setTabsClosable(true);

// 添加右键菜单
dynamicTabs->setContextMenuPolicy(Qt::CustomContextMenu);
connect(dynamicTabs, &QWidget::customContextMenuRequested, [=](const QPoint& pos) {
    int tabIndex = -1;
    
    // 查找点击的选项卡
    for (int i = 0; i < dynamicTabs->count(); ++i) {
        QRect tabRect = dynamicTabs->tabBar()->tabRect(i);
        if (tabRect.contains(pos)) {
            tabIndex = i;
            break;
        }
    }
    
    if (tabIndex >= 0) {
        QMenu menu;
        
        QAction* closeAction = menu.addAction("关闭");
        QAction* closeOthersAction = menu.addAction("关闭其他");
        QAction* closeAllAction = menu.addAction("关闭全部");
        
        QAction* selected = menu.exec(dynamicTabs->mapToGlobal(pos));
        
        if (selected == closeAction) {
            QWidget* widget = dynamicTabs->widget(tabIndex);
            dynamicTabs->removeTab(tabIndex);
            widget->deleteLater();
        } else if (selected == closeOthersAction) {
            // 关闭其他选项卡
            for (int i = dynamicTabs->count() - 1; i >= 0; --i) {
                if (i != tabIndex) {
                    QWidget* widget = dynamicTabs->widget(i);
                    dynamicTabs->removeTab(i);
                    widget->deleteLater();
                }
            }
        } else if (selected == closeAllAction) {
            // 关闭所有选项卡
            dynamicTabs->clear();
        }
    }
});
```

### 自定义样式

```cpp
NTabWidget* styledTabWidget = new NTabWidget();

// 设置边框样式
styledTabWidget->setBorderRadius(8);
styledTabWidget->setBorderWidth(1);

// 自定义颜色
styledTabWidget->setLightSelectedBackground(QColor(240, 248, 255));
styledTabWidget->setDarkSelectedBackground(QColor(45, 45, 45));
styledTabWidget->setLightBorderColor(QColor(200, 200, 200));
styledTabWidget->setDarkBorderColor(QColor(100, 100, 100));

// 透出父容器背景，不要使用全局 QTabWidget::pane 样式表
styledTabWidget->setTransparentBackground(true);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NTabWidget(QWidget* parent = nullptr)` | 创建选项卡控件 |

### 选项卡管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `tabBar()` | 获取选项卡栏 | - |
| `setTabsClosable(bool closable)` | 设置选项卡可关闭 | `true` / `false` |
| `tabsClosable()` | 获取选项卡是否可关闭 | - |

### 样式属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |
| `setBorderWidth(int width)` | 设置边框宽度 | 边框宽度 |
| `borderWidth()` | 获取边框宽度 | - |
| `setTransparentBackground(bool transparent)` | 设置组件表面和内容区是否透明 | `true` / `false` |
| `getTransparentBackground()` | 获取透明背景状态 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightSelectedBackground` | 明亮主题选中背景色 | `QColor` |
| `darkSelectedBackground` | 暗黑主题选中背景色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |

### 继承的 API

NTabWidget 继承自 `QTabWidget`，支持所有标准 QTabWidget 的方法和信号：

- `addTab()` / `insertTab()` - 添加/插入选项卡
- `removeTab()` / `clear()` - 移除选项卡/清空所有
- `setCurrentIndex()` / `currentIndex()` - 设置/获取当前索引
- `setCurrentWidget()` / `currentWidget()` - 设置/获取当前控件
- `count()` / `widget()` - 获取选项卡数量/指定控件
- `setTabText()` / `tabText()` - 设置/获取选项卡文本
- `setTabIcon()` / `tabIcon()` - 设置/获取选项卡图标
- `setTabEnabled()` / `isTabEnabled()` - 设置/获取选项卡启用状态
- `setTabVisible()` / `isTabVisible()` - 设置/获取选项卡可见性
- `setTabPosition()` / `tabPosition()` - 设置/获取选项卡位置
- `setTabShape()` / `tabShape()` - 设置/获取选项卡形状
- `currentChanged(int)` - 当前选项卡改变信号
- `tabCloseRequested(int)` - 选项卡关闭请求信号
- `tabBarClicked(int)` - 选项卡栏点击信号

### 增强 API

- `setTabClosable(int, bool)` / `getTabClosable(int)` - 逐页控制关闭权限
- `setTabWidthMode()` / `getTabWidthMode()` - 等宽、适应内容或紧凑模式
- `setCloseButtonDisplayMode()` / `getCloseButtonDisplayMode()` - 始终、悬停或从不显示关闭按钮
- `setTabDragEnabled(bool)` / `getTabDragEnabled()` - 启用越过标签栏边界后的栏外拖拽
- `setAddButtonVisible(bool)` / `getAddButtonVisible()` - 显示内建新增按钮
- `tabAddRequested()` - 新增按钮或 Ctrl+T 的请求信号

## 无障碍

`NTabWidget` 和内建标签栏、添加按钮、关闭按钮均提供默认可访问名称；标签项继续使用 Qt 原生
`PageTabList` / `PageTab` 语义、选择接口和按压动作。键盘焦点使用设计 token 绘制双层焦点框。

应用仍应提供上下文名称，例如：

```cpp
tabs->setAccessibleName(QStringLiteral("设备配置页面"));
tabs->tabBar()->setAccessibleTabName(0, QStringLiteral("网络配置标签"));
pageEditor->setAccessibleName(QStringLiteral("网络配置编辑器"));
```

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 选项卡高度 | `32px` | 选项卡标签高度 |
| 边框圆角 | `CornerRadiusMedium`（默认 `8px`） | 整体外框和选中页签顶部圆角 |
| 边框宽度 | `1px` | 选项卡边框宽度 |
| 内容边距 | `8px` | 选项卡内容边距 |

### 主题适配

```cpp
// 选项卡控件会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 选项卡颜色会自动更新
});
```
