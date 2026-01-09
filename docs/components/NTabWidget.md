# NTabWidget 选项卡控件

### 介绍

选项卡控件用于在多个页面之间切换，每个选项卡对应一个页面内容。NTabWidget 基于 Qt 的 QTabWidget，提供了现代化的外观和增强的功能。

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

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 选项卡高度 | `32px` | 选项卡标签高度 |
| 边框圆角 | `4px` | 选项卡圆角半径 |
| 边框宽度 | `1px` | 选项卡边框宽度 |
| 内容边距 | `8px` | 选项卡内容边距 |

### 主题适配

```cpp
// 选项卡控件会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 选项卡颜色会自动更新
});
```