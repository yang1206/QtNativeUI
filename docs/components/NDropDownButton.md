# NDropDownButton 下拉按钮

### 介绍

下拉按钮是一个带有下拉菜单的按钮控件。点击按钮时会显示一个菜单，用户可以从菜单中选择一个选项。NDropDownButton 基于 NPushButton，提供了便捷的菜单集成。

### 基本用法

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("选项");

// 创建菜单
NMenu* menu = new NMenu();
menu->addItem("选项1");
menu->addItem("选项2");
menu->addItem("选项3");

// 设置菜单
dropdownBtn->setMenu(menu);

// 监听菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    qDebug() << "选择了：" << action->text();
});
```

### 带图标的菜单

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("文件");

// 创建菜单
NMenu* menu = new NMenu();
menu->addItem("新建", NRegularIconType::DocumentAdd24Regular);
menu->addItem("打开", NRegularIconType::FolderOpen24Regular);
menu->addItem("保存", NRegularIconType::Save24Regular);
menu->addSeparator();
menu->addItem("退出", NRegularIconType::SignOut24Regular);

dropdownBtn->setMenu(menu);

// 处理菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    QString text = action->text();
    if (text == "新建") {
        qDebug() << "创建新文件";
    } else if (text == "打开") {
        qDebug() << "打开文件";
    } else if (text == "保存") {
        qDebug() << "保存文件";
    }
});
```

### 子菜单

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("编辑");

// 创建主菜单
NMenu* menu = new NMenu();
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

dropdownBtn->setMenu(menu);

// 处理选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    qDebug() << "执行：" << action->text();
});
```

### 可检查的菜单项

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("视图");

// 创建菜单
NMenu* menu = new NMenu();
menu->addCheckableItem("显示工具栏", true);
menu->addCheckableItem("显示状态栏", true);
menu->addCheckableItem("显示侧边栏", false);
menu->addSeparator();
menu->addItem("全屏");

dropdownBtn->setMenu(menu);

// 处理菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    if (action->isCheckable()) {
        qDebug() << action->text() << "：" << (action->isChecked() ? "启用" : "禁用");
    } else {
        qDebug() << "执行：" << action->text();
    }
});
```

### 快捷键支持

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("文件");

// 创建菜单
NMenu* menu = new NMenu();
menu->addItem("新建", NRegularIconType::DocumentAdd24Regular, QKeySequence::New);
menu->addItem("打开", NRegularIconType::FolderOpen24Regular, QKeySequence::Open);
menu->addItem("保存", NRegularIconType::Save24Regular, QKeySequence::Save);
menu->addItem("另存为", NRegularIconType::Save24Regular, QKeySequence::SaveAs);
menu->addSeparator();
menu->addItem("退出", NRegularIconType::SignOut24Regular, QKeySequence::Quit);

dropdownBtn->setMenu(menu);

// 处理菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    qDebug() << "执行：" << action->text();
});
```

### 动态菜单

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("最近文件");

// 创建菜单
NMenu* menu = new NMenu();

// 动态添加最近打开的文件
QStringList recentFiles = {
    "document1.txt",
    "document2.txt",
    "document3.txt"
};

for (const QString& file : recentFiles) {
    menu->addItem(file);
}

menu->addSeparator();
menu->addItem("清除历史");

dropdownBtn->setMenu(menu);

// 处理菜单项选择
connect(menu, &QMenu::triggered, [](QAction* action) {
    if (action->text() == "清除历史") {
        qDebug() << "清除最近文件历史";
    } else {
        qDebug() << "打开：" << action->text();
    }
});
```

### 禁用菜单项

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("编辑");

// 创建菜单
NMenu* menu = new NMenu();
QAction* undoAction = menu->addItem("撤销");
QAction* redoAction = menu->addItem("重做");
menu->addSeparator();
menu->addItem("剪切");
menu->addItem("复制");
menu->addItem("粘贴");

dropdownBtn->setMenu(menu);

// 初始状态下禁用撤销和重做
menu->setItemEnabled(undoAction, false);
menu->setItemEnabled(redoAction, false);

// 根据条件启用/禁用菜单项
bool canUndo = false;
bool canRedo = false;

if (canUndo) {
    menu->setItemEnabled(undoAction, true);
}
if (canRedo) {
    menu->setItemEnabled(redoAction, true);
}
```

### 菜单项工具提示

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("工具");

// 创建菜单
NMenu* menu = new NMenu();
QAction* findAction = menu->addItem("查找", NRegularIconType::Search24Regular);
QAction* replaceAction = menu->addItem("替换", NRegularIconType::Replace24Regular);
QAction* settingsAction = menu->addItem("设置", NRegularIconType::Settings24Regular);

// 添加工具提示
menu->setItemToolTip(findAction, "在文档中查找文本 (Ctrl+F)");
menu->setItemToolTip(replaceAction, "查找并替换文本 (Ctrl+H)");
menu->setItemToolTip(settingsAction, "打开应用设置");

dropdownBtn->setMenu(menu);
```

### 隐藏下拉箭头

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("菜单");

// 隐藏下拉箭头
dropdownBtn->setShowArrow(false);

// 创建菜单
NMenu* menu = new NMenu();
menu->addItem("选项1");
menu->addItem("选项2");

dropdownBtn->setMenu(menu);
```

### 自定义样式

```cpp
// 创建下拉按钮
NDropDownButton* dropdownBtn = new NDropDownButton("选项");

// 设置按钮样式（继承自 NPushButton）
dropdownBtn->setMinimumWidth(120);
dropdownBtn->setMinimumHeight(36);

// 创建菜单
NMenu* menu = new NMenu();
menu->addItem("选项1");
menu->addItem("选项2");
menu->addItem("选项3");

dropdownBtn->setMenu(menu);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NDropDownButton(QWidget* parent = nullptr)` | 创建下拉按钮 |
| `NDropDownButton(const QString& text, QWidget* parent = nullptr)` | 创建带文本的下拉按钮 |

### 菜单管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setMenu(NMenu* menu)` | 设置下拉菜单 | 菜单对象 |
| `menu()` | 获取下拉菜单 | 返回 `QMenu*` |

### 外观控制

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setShowArrow(bool show)` | 设置是否显示下拉箭头 | 布尔值 |
| `showArrow()` | 获取是否显示下拉箭头 | 返回 `bool` |

### 菜单操作

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `showMenu()` | 显示下拉菜单 | - |

### 继承的 API

NDropDownButton 继承自 `NPushButton`，支持所有 NPushButton 的方法和属性：

- `setText()` / `text()` - 设置/获取按钮文本
- `setIcon()` / `icon()` - 设置/获取按钮图标
- `setEnabled()` / `isEnabled()` - 启用/禁用按钮
- `clicked()` - 点击信号

## 主题定制

下拉按钮会自动响应应用主题变化，菜单也会自动适配当前主题。
