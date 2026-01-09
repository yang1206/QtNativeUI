# NMenu 菜单

### 介绍

菜单是一个弹出式的选项列表，用于显示一组相关的操作。NMenu 基于 Qt 的 QMenu，提供了现代化的外观、动画效果和丰富的菜单项类型支持。

### 基本用法

```cpp
// 创建菜单
NMenu* menu = new NMenu();

// 添加菜单项
menu->addItem("新建");
menu->addItem("打开");
menu->addItem("保存");

// 显示菜单
menu->exec(QCursor::pos());

// 监听菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    qDebug() << "选择了：" << action->text();
});
```

### 带图标的菜单项

```cpp
// 创建菜单
NMenu* menu = new NMenu("文件");

// 添加带图标的菜单项
menu->addItem("新建", NRegularIconType::DocumentAdd24Regular);
menu->addItem("打开", NRegularIconType::FolderOpen24Regular);
menu->addItem("保存", NRegularIconType::Save24Regular);
menu->addSeparator();
menu->addItem("退出", NRegularIconType::SignOut24Regular);

// 显示菜单
menu->exec(QCursor::pos());
```

### 快捷键

```cpp
// 创建菜单
NMenu* menu = new NMenu("编辑");

// 添加带快捷键的菜单项
menu->addItem("撤销", QKeySequence::Undo);
menu->addItem("重做", QKeySequence::Redo);
menu->addSeparator();
menu->addItem("剪切", QKeySequence::Cut);
menu->addItem("复制", QKeySequence::Copy);
menu->addItem("粘贴", QKeySequence::Paste);

// 显示菜单
menu->exec(QCursor::pos());
```

### 可检查的菜单项

```cpp
// 创建菜单
NMenu* menu = new NMenu("视图");

// 添加可检查的菜单项
QAction* toolbarAction = menu->addCheckableItem("显示工具栏", true);
QAction* statusbarAction = menu->addCheckableItem("显示状态栏", true);
QAction* sidebarAction = menu->addCheckableItem("显示侧边栏", false);

// 监听菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    if (action->isCheckable()) {
        qDebug() << action->text() << "：" << (action->isChecked() ? "启用" : "禁用");
    }
});

// 显示菜单
menu->exec(QCursor::pos());
```

### 子菜单

```cpp
// 创建主菜单
NMenu* menu = new NMenu("编辑");

menu->addItem("撤销");
menu->addItem("重做");
menu->addSeparator();

// 创建子菜单
NMenu* selectMenu = menu->addSubMenu("选择");
selectMenu->addItem("全选");
selectMenu->addItem("反选");
selectMenu->addItem("清除选择");

// 创建另一个子菜单
NMenu* formatMenu = menu->addSubMenu("格式");
formatMenu->addItem("加粗");
formatMenu->addItem("斜体");
formatMenu->addItem("下划线");

// 显示菜单
menu->exec(QCursor::pos());
```

### 分隔线

```cpp
// 创建菜单
NMenu* menu = new NMenu("文件");

menu->addItem("新建");
menu->addItem("打开");
menu->addItem("保存");
menu->addSeparator();  // 添加分隔线
menu->addItem("打印");
menu->addItem("导出");
menu->addSeparator();  // 添加分隔线
menu->addItem("退出");

// 显示菜单
menu->exec(QCursor::pos());
```

### 禁用菜单项

```cpp
// 创建菜单
NMenu* menu = new NMenu("编辑");

QAction* undoAction = menu->addItem("撤销");
QAction* redoAction = menu->addItem("重做");
menu->addSeparator();
menu->addItem("剪切");
menu->addItem("复制");
menu->addItem("粘贴");

// 初始状态下禁用撤销和重做
menu->setItemEnabled(undoAction, false);
menu->setItemEnabled(redoAction, false);

// 根据条件启用/禁用
bool canUndo = false;
if (canUndo) {
    menu->setItemEnabled(undoAction, true);
}

// 显示菜单
menu->exec(QCursor::pos());
```

### 隐藏菜单项

```cpp
// 创建菜单
NMenu* menu = new NMenu("工具");

QAction* debugAction = menu->addItem("调试");
QAction* profileAction = menu->addItem("性能分析");
menu->addSeparator();
menu->addItem("设置");

// 隐藏某些菜单项
bool isDebugMode = false;
menu->setItemVisible(debugAction, isDebugMode);
menu->setItemVisible(profileAction, isDebugMode);

// 显示菜单
menu->exec(QCursor::pos());
```

### 菜单项工具提示

```cpp
// 创建菜单
NMenu* menu = new NMenu("工具");

QAction* findAction = menu->addItem("查找", NRegularIconType::Search24Regular);
QAction* replaceAction = menu->addItem("替换", NRegularIconType::Replace24Regular);
QAction* settingsAction = menu->addItem("设置", NRegularIconType::Settings24Regular);

// 添加工具提示
menu->setItemToolTip(findAction, "在文档中查找文本 (Ctrl+F)");
menu->setItemToolTip(replaceAction, "查找并替换文本 (Ctrl+H)");
menu->setItemToolTip(settingsAction, "打开应用设置");

// 显示菜单
menu->exec(QCursor::pos());
```

### 动画效果

```cpp
// 创建菜单
NMenu* menu = new NMenu("文件");

// 启用下拉动画
menu->setDropDownAnimation(true);

menu->addItem("新建");
menu->addItem("打开");
menu->addItem("保存");
menu->addSeparator();
menu->addItem("退出");

// 显示菜单
menu->exec(QCursor::pos());
```

### 上下文菜单

```cpp
// 创建列表视图
NListView* listView = new NListView();

// 创建上下文菜单
NMenu* contextMenu = new NMenu();
contextMenu->addItem("编辑", NRegularIconType::Edit24Regular);
contextMenu->addItem("删除", NRegularIconType::Delete24Regular);
contextMenu->addItem("复制", NRegularIconType::Copy24Regular);
contextMenu->addSeparator();
contextMenu->addItem("属性", NRegularIconType::Settings24Regular);

// 在列表视图上显示上下文菜单
connect(listView, &QWidget::customContextMenuRequested, [=](const QPoint& pos) {
    contextMenu->exec(listView->mapToGlobal(pos));
});

listView->setContextMenuPolicy(Qt::CustomContextMenu);
```

### 自定义样式

```cpp
// 创建菜单
NMenu* menu = new NMenu("文件");

// 设置圆角
menu->setBorderRadius(8);

// 自定义颜色
menu->setLightBackgroundColor(QColor(255, 255, 255));
menu->setLightBorderColor(QColor(200, 200, 200));
menu->setLightSeparatorColor(QColor(230, 230, 230));

menu->setDarkBackgroundColor(QColor(45, 45, 45));
menu->setDarkBorderColor(QColor(80, 80, 80));
menu->setDarkSeparatorColor(QColor(70, 70, 70));

menu->addItem("新建");
menu->addItem("打开");
menu->addItem("保存");

// 显示菜单
menu->exec(QCursor::pos());
```

### 复杂菜单结构

```cpp
// 创建主菜单
NMenu* menu = new NMenu("应用");

// 文件菜单
NMenu* fileMenu = menu->addSubMenu("文件", NRegularIconType::Document24Regular);
fileMenu->addItem("新建", NRegularIconType::DocumentAdd24Regular, QKeySequence::New);
fileMenu->addItem("打开", NRegularIconType::FolderOpen24Regular, QKeySequence::Open);
fileMenu->addItem("保存", NRegularIconType::Save24Regular, QKeySequence::Save);
fileMenu->addSeparator();
fileMenu->addItem("退出", NRegularIconType::SignOut24Regular, QKeySequence::Quit);

// 编辑菜单
NMenu* editMenu = menu->addSubMenu("编辑", NRegularIconType::Edit24Regular);
editMenu->addItem("撤销", QKeySequence::Undo);
editMenu->addItem("重做", QKeySequence::Redo);
editMenu->addSeparator();
editMenu->addItem("剪切", QKeySequence::Cut);
editMenu->addItem("复制", QKeySequence::Copy);
editMenu->addItem("粘贴", QKeySequence::Paste);

// 视图菜单
NMenu* viewMenu = menu->addSubMenu("视图", NRegularIconType::Eye24Regular);
viewMenu->addCheckableItem("显示工具栏", true);
viewMenu->addCheckableItem("显示状态栏", true);
viewMenu->addCheckableItem("全屏", false);

// 帮助菜单
NMenu* helpMenu = menu->addSubMenu("帮助", NRegularIconType::QuestionCircle24Regular);
helpMenu->addItem("关于");
helpMenu->addItem("关于 Qt");

// 显示菜单
menu->exec(QCursor::pos());
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NMenu(QWidget* parent = nullptr)` | 创建菜单 |
| `NMenu(const QString& title, QWidget* parent = nullptr)` | 创建带标题的菜单 |

### 添加菜单项

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `addItem(const QString& text)` | 添加菜单项 | 返回 `QAction*` |
| `addItem(const QString& text, NRegularIconType::Icon icon)` | 添加带图标的菜单项 | 返回 `QAction*` |
| `addItem(const QString& text, const QKeySequence& shortcut)` | 添加带快捷键的菜单项 | 返回 `QAction*` |
| `addCheckableItem(const QString& text, bool checked)` | 添加可检查的菜单项 | 返回 `QAction*` |
| `addSubMenu(const QString& text)` | 添加子菜单 | 返回 `NMenu*` |
| `addSubMenu(const QString& text, NRegularIconType::Icon icon)` | 添加带图标的子菜单 | 返回 `NMenu*` |
| `addSeparator()` | 添加分隔线 | - |

### 菜单项管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setItemEnabled(QAction* action, bool enabled)` | 启用/禁用菜单项 | - |
| `setItemVisible(QAction* action, bool visible)` | 显示/隐藏菜单项 | - |
| `setItemToolTip(QAction* action, const QString& toolTip)` | 设置菜单项工具提示 | - |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `borderRadius` | 边框圆角半径 | `int` |
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |
| `lightSeparatorColor` | 明亮主题分隔线色 | `QColor` |
| `darkSeparatorColor` | 暗黑主题分隔线色 | `QColor` |

### 动画

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setDropDownAnimation(bool enabled)` | 启用/禁用下拉动画 | 布尔值 |
| `dropDownAnimation()` | 获取是否启用下拉动画 | 返回 `bool` |

### 继承的 API

NMenu 继承自 `QMenu`，支持所有标准 QMenu 的方法：

- `exec()` - 显示菜单
- `triggered()` - 菜单项被选择信号
- `aboutToShow()` - 菜单即将显示信号
- `aboutToHide()` - 菜单即将隐藏信号

## 主题定制

菜单会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
