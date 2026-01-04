# NNavigationView 导航视图

### 介绍

导航视图是一个完整的导航容器，提供了侧边导航栏和内容区域。支持多级导航、页面路由、自适应显示模式和丰富的页面管理功能。

### 基本用法

```cpp
// 创建导航视图
NNavigationView* navView = new NNavigationView();

// 设置显示模式
navView->setDisplayMode(NNavigationType::Auto);

// 添加页面节点
navView->addPageNode("首页", new HomePage(), NRegularIconType::Home24Regular);
navView->addPageNode("设置", new SettingsPage(), NRegularIconType::Settings24Regular);
navView->addPageNode("关于", new AboutPage(), NRegularIconType::Info24Regular);

// 设置为主窗口的中央控件
setCentralWidget(navView);
```

### 多级导航结构

```cpp
NNavigationView* navView = new NNavigationView();

// 添加展开节点（分组）
QString documentsKey, mediaKey, toolsKey;
navView->addExpanderNode("文档", documentsKey, NRegularIconType::Document24Regular);
navView->addExpanderNode("媒体", mediaKey, NRegularIconType::Video24Regular);
navView->addExpanderNode("工具", toolsKey, NRegularIconType::Wrench24Regular);

// 在分组下添加页面
navView->addPageNode("文本文档", new TextDocPage(), documentsKey, NRegularIconType::DocumentText24Regular);
navView->addPageNode("表格文档", new SpreadsheetPage(), documentsKey, NRegularIconType::Table24Regular);
navView->addPageNode("演示文档", new PresentationPage(), documentsKey, NRegularIconType::Presentation24Regular);

navView->addPageNode("图片", new ImagePage(), mediaKey, NRegularIconType::Image24Regular);
navView->addPageNode("视频", new VideoPage(), mediaKey, NRegularIconType::Video24Regular);
navView->addPageNode("音频", new AudioPage(), mediaKey, NRegularIconType::MusicNote24Regular);

navView->addPageNode("计算器", new CalculatorPage(), toolsKey, NRegularIconType::Calculator24Regular);
navView->addPageNode("日历", new CalendarPage(), toolsKey, NRegularIconType::Calendar24Regular);
```

### 页面路由系统

```cpp
NNavigationView* navView = new NNavigationView();

// 添加页面
navView->addPageNode("用户列表", new UserListPage(), NRegularIconType::People24Regular);
navView->addPageNode("用户详情", new UserDetailPage(), NRegularIconType::Person24Regular);
navView->addPageNode("编辑用户", new EditUserPage(), NRegularIconType::Edit24Regular);

// 导航到指定页面并传递参数
QVariantMap params;
params["userId"] = 123;
params["mode"] = "edit";
navView->navigateTo("用户详情", params);

// 监听路由变化
connect(navView, &NNavigationView::routeChanged, 
        [](const QString& pageKey, const QVariantMap& params) {
    qDebug() << "导航到页面：" << pageKey;
    qDebug() << "参数：" << params;
});

// 返回上一页
navView->navigateBack();

// 监听返回事件
connect(navView, &NNavigationView::routeBack,
        [](const QString& fromKey, const QString& toKey, const QVariantMap& params) {
    qDebug() << "从" << fromKey << "返回到" << toKey;
});
```

### 徽章和通知

```cpp
NNavigationView* navView = new NNavigationView();

// 添加带徽章的页面
navView->addPageNode("消息", new MessagePage(), 5, NRegularIconType::Mail24Regular);
navView->addPageNode("通知", new NotificationPage(), 99, NRegularIconType::Alert24Regular);

// 动态更新徽章
navView->setNodeKeyPoints("消息", 3);  // 更新消息数量
navView->setNodeKeyPoints("通知", 0);  // 清除通知徽章

// 获取当前徽章数量
int messageCount = navView->getNodeKeyPoints("消息");
```

### 页脚节点

```cpp
NNavigationView* navView = new NNavigationView();

// 添加普通页面
navView->addPageNode("首页", new HomePage(), NRegularIconType::Home24Regular);
navView->addPageNode("文档", new DocsPage(), NRegularIconType::Document24Regular);

// 添加页脚节点（通常用于设置、帮助等）
QString settingsKey, helpKey;
navView->addFooterNode("设置", new SettingsPage(), settingsKey, 0, NRegularIconType::Settings24Regular);
navView->addFooterNode("帮助", new HelpPage(), helpKey, 0, NRegularIconType::QuestionCircle24Regular);
```

### 自定义头部和搜索

```cpp
NNavigationView* navView = new NNavigationView();

// 创建自定义头部控件
QWidget* headerWidget = new QWidget();
QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);

NLabel* titleLabel = new NLabel("我的应用", NLabelType::Title);
NPushButton* profileBtn = new NPushButton();
profileBtn->setFluentIcon(NRegularIconType::Person24Regular, 20);

headerLayout->addWidget(titleLabel);
headerLayout->addStretch();
headerLayout->addWidget(profileBtn);

navView->setHeaderWidget(headerWidget);

// 启用搜索功能
navView->setSearchVisible(true);
```

### 显示模式控制

```cpp
NNavigationView* navView = new NNavigationView();

// 自动模式（根据窗口大小自适应）
navView->setDisplayMode(NNavigationType::Auto);

// 手动控制显示模式
navView->setDisplayMode(NNavigationType::Compact);   // 紧凑模式
navView->setDisplayMode(NNavigationType::Minimal);   // 最小模式
navView->setDisplayMode(NNavigationType::Maximal);   // 完整模式

// 监听显示模式变化
connect(navView, &NNavigationView::displayModeChanged,
        [](NNavigationType::NavigationDisplayMode mode) {
    switch (mode) {
        case NNavigationType::Minimal:
            qDebug() << "切换到最小模式";
            break;
        case NNavigationType::Compact:
            qDebug() << "切换到紧凑模式";
            break;
        case NNavigationType::Maximal:
            qDebug() << "切换到完整模式";
            break;
    }
});
```

### 页面过渡动画

```cpp
NNavigationView* navView = new NNavigationView();

// 设置过渡动画类型
navView->setPageTransitionType(NNavigationType::ScaleTransition);

// 设置动画时长
navView->setPageTransitionDuration(300);

// 可用的过渡类型：
// - NoTransition: 无动画
// - PopupTransition: 弹出效果
// - ScaleTransition: 缩放效果
// - FlipTransition: 翻转效果
// - BlurTransition: 模糊效果
// - CubeTransition: 立方体翻转
// - RippleTransition: 波纹扩散
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NNavigationView(QWidget* parent = nullptr)` | 创建导航视图 |

### 显示模式

| 方法 | 说明 | 参数 |
|------|------|------|
| `setDisplayMode(NNavigationType::NavigationDisplayMode mode)` | 设置显示模式 | 显示模式枚举 |
| `displayMode()` | 获取显示模式 | - |
| `setNavigationBarVisible(bool visible)` | 设置导航栏可见性 | `true` / `false` |
| `isNavigationBarVisible()` | 获取导航栏可见性 | - |

### 头部控件

| 方法 | 说明 | 参数 |
|------|------|------|
| `setHeaderWidget(QWidget* widget)` | 设置头部控件 | 控件指针 |
| `headerWidget()` | 获取头部控件 | - |
| `setSearchVisible(bool visible)` | 设置搜索框可见性 | `true` / `false` |
| `isSearchVisible()` | 获取搜索框可见性 | - |

### 节点管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `addExpanderNode(QString title, QString& key, NRegularIconType::Icon icon)` | 添加展开节点 | 标题、键值、图标 |
| `addPageNode(QString title, QWidget* page, NRegularIconType::Icon icon)` | 添加页面节点 | 标题、页面、图标 |
| `addFooterNode(QString title, QWidget* page, QString& key, int keyPoints, NRegularIconType::Icon icon)` | 添加页脚节点 | 标题、页面、键值、徽章、图标 |
| `removeNavigationNode(QString nodeKey)` | 移除导航节点 | 节点键值 |
| `expandNavigationNode(QString expanderKey)` | 展开节点 | 展开器键值 |
| `collpaseNavigationNode(QString expanderKey)` | 折叠节点 | 展开器键值 |
| `getNavigationNodeIsExpanded(QString expanderKey)` | 获取节点展开状态 | 展开器键值 |

### 徽章管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `setNodeKeyPoints(QString nodeKey, int keyPoints)` | 设置节点徽章 | 节点键值、徽章数量 |
| `getNodeKeyPoints(QString nodeKey)` | 获取节点徽章 | 节点键值 |

### 路由系统

| 方法 | 说明 | 参数 |
|------|------|------|
| `navigateTo(const QString& pageKey, const QVariantMap& params)` | 导航到页面 | 页面键值、参数 |
| `navigateBack(const QVariantMap& params)` | 返回上一页 | 参数 |
| `navigation(QString pageKey, bool isLogClicked)` | 直接导航 | 页面键值、是否记录 |
| `currentRouteKey()` | 获取当前路由键值 | - |
| `currentRouteParams()` | 获取当前路由参数 | - |
| `historyCount()` | 获取历史记录数量 | - |
| `clearNavigationHistory()` | 清除导航历史 | - |
| `hasNavigationHistory()` | 是否有导航历史 | - |

### 页面管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `currentWidget()` | 获取当前页面控件 | - |
| `currentIndex()` | 获取当前页面索引 | - |
| `count()` | 获取页面总数 | - |

### 动画设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setPageTransitionType(NNavigationType::PageTransitionType type)` | 设置过渡动画类型 | 动画类型枚举 |
| `pageTransitionType()` | 获取过渡动画类型 | - |
| `setPageTransitionDuration(int duration)` | 设置动画时长 | 毫秒数 |
| `pageTransitionDuration()` | 获取动画时长 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `navigationNodeClicked(NNavigationType::NavigationNodeType, QString)` | 导航节点被点击 | 节点类型、键值 |
| `navigationNodeAdded(NNavigationType::NavigationNodeType, QString, QWidget*)` | 导航节点被添加 | 节点类型、键值、页面 |
| `navigationNodeRemoved(NNavigationType::NavigationNodeType, QString)` | 导航节点被移除 | 节点类型、键值 |
| `displayModeChanged(NNavigationType::NavigationDisplayMode)` | 显示模式改变 | 新的显示模式 |
| `currentChanged(int)` | 当前页面改变 | 新的页面索引 |
| `routeChanged(const QString&, const QVariantMap&)` | 路由改变 | 页面键值、参数 |
| `routeBack(const QString&, const QString&, const QVariantMap&)` | 路由返回 | 来源页面、目标页面、参数 |
| `navigationStateChanged(bool)` | 导航状态改变 | 是否有历史记录 |

### 枚举

#### NNavigationType::NavigationDisplayMode

| 值 | 说明 |
|----|------|
| `Auto` | 自动模式（根据窗口大小自适应） |
| `Minimal` | 最小模式（导航栏隐藏） |
| `Compact` | 紧凑模式（只显示图标） |
| `Maximal` | 完整模式（显示完整导航栏） |

#### NNavigationType::PageTransitionType

| 值 | 说明 |
|----|------|
| `NoTransition` | 无过渡效果 |
| `PopupTransition` | 弹出过渡 |
| `ScaleTransition` | 缩放过渡 |
| `FlipTransition` | 翻转过渡 |
| `BlurTransition` | 模糊过渡 |
| `CubeTransition` | 立方体翻转 |
| `RippleTransition` | 波纹扩散 |

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 导航栏宽度（完整） | `320px` | Maximal 模式下的导航栏宽度 |
| 导航栏宽度（紧凑） | `48px` | Compact 模式下的导航栏宽度 |
| 节点高度 | `40px` | 导航节点高度 |
| 动画时长 | `250ms` | 默认页面过渡动画时长 |

### 主题适配

```cpp
// 导航视图会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 导航栏和页面颜色会自动更新
});
```