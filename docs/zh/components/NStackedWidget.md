# NStackedWidget 堆栈控件

### 介绍

堆栈控件用于在多个页面之间切换，一次只显示一个页面。NStackedWidget 基于 Qt 的 QStackedWidget，提供了页面切换动画和现代化的外观。

### 基本用法

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 创建页面1
QWidget* page1 = new QWidget();
QVBoxLayout* layout1 = new QVBoxLayout(page1);
layout1->addWidget(new QLabel("页面 1"));

// 创建页面2
QWidget* page2 = new QWidget();
QVBoxLayout* layout2 = new QVBoxLayout(page2);
layout2->addWidget(new QLabel("页面 2"));

// 创建页面3
QWidget* page3 = new QWidget();
QVBoxLayout* layout3 = new QVBoxLayout(page3);
layout3->addWidget(new QLabel("页面 3"));

// 添加页面
stackedWidget->addWidget(page1);
stackedWidget->addWidget(page2);
stackedWidget->addWidget(page3);

// 设置当前页面
stackedWidget->setCurrentIndex(0);
```

### 页面切换

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 添加页面
for (int i = 0; i < 5; ++i) {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel(QString("页面 %1").arg(i + 1)));
    stackedWidget->addWidget(page);
}

// 创建导航按钮
NPushButton* prevBtn = new NPushButton("上一页");
NPushButton* nextBtn = new NPushButton("下一页");

// 处理按钮点击
connect(prevBtn, &NPushButton::clicked, [=]() {
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex > 0) {
        stackedWidget->setCurrentIndex(currentIndex - 1);
    }
});

connect(nextBtn, &NPushButton::clicked, [=]() {
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex < stackedWidget->count() - 1) {
        stackedWidget->setCurrentIndex(currentIndex + 1);
    }
});
```

### 带动画的页面切换

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 添加页面
QWidget* page1 = new QWidget();
QVBoxLayout* layout1 = new QVBoxLayout(page1);
layout1->addWidget(new QLabel("页面 1"));

QWidget* page2 = new QWidget();
QVBoxLayout* layout2 = new QVBoxLayout(page2);
layout2->addWidget(new QLabel("页面 2"));

stackedWidget->addWidget(page1);
stackedWidget->addWidget(page2);

// 使用动画切换页面
NPushButton* switchBtn = new NPushButton("切换页面");
connect(switchBtn, &NPushButton::clicked, [=]() {
    int currentIndex = stackedWidget->currentIndex();
    int nextIndex = (currentIndex + 1) % stackedWidget->count();
    
    // 使用页面切换动画
    stackedWidget->doPageSwitch(
        NNavigationType::PageTransitionType::SlideLeft,
        nextIndex,
        false,
        300  // 动画时长（毫秒）
    );
});
```

### 标签页导航

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 创建标签栏
NTabBar* tabBar = new NTabBar();

// 添加页面和标签
QStringList pageNames = {"首页", "设置", "关于"};
for (int i = 0; i < pageNames.size(); ++i) {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel(pageNames[i]));
    
    stackedWidget->addWidget(page);
    tabBar->addTab(pageNames[i]);
}

// 同步标签栏和堆栈控件
connect(tabBar, &QTabBar::currentChanged, [=](int index) {
    stackedWidget->setCurrentIndex(index);
});

connect(stackedWidget, QOverload<int>::of(&QStackedWidget::currentChanged),
        [=](int index) {
    tabBar->setCurrentIndex(index);
});
```

### 向导式界面

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 创建向导页面
QStringList steps = {"步骤1：基本信息", "步骤2：详细信息", "步骤3：确认"};
for (const QString& step : steps) {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel(step));
    layout->addStretch();
    stackedWidget->addWidget(page);
}

// 创建导航按钮
NPushButton* prevBtn = new NPushButton("上一步");
NPushButton* nextBtn = new NPushButton("下一步");
NPushButton* finishBtn = new NPushButton("完成");

// 处理导航
connect(prevBtn, &NPushButton::clicked, [=]() {
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex > 0) {
        stackedWidget->doPageSwitch(
            NNavigationType::PageTransitionType::SlideRight,
            currentIndex - 1,
            true,
            300
        );
    }
});

connect(nextBtn, &NPushButton::clicked, [=]() {
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex < stackedWidget->count() - 1) {
        stackedWidget->doPageSwitch(
            NNavigationType::PageTransitionType::SlideLeft,
            currentIndex + 1,
            false,
            300
        );
    }
});

connect(finishBtn, &NPushButton::clicked, [=]() {
    qDebug() << "向导完成";
});

// 更新按钮状态
auto updateButtons = [=]() {
    int currentIndex = stackedWidget->currentIndex();
    prevBtn->setEnabled(currentIndex > 0);
    nextBtn->setVisible(currentIndex < stackedWidget->count() - 1);
    finishBtn->setVisible(currentIndex == stackedWidget->count() - 1);
};

connect(stackedWidget, QOverload<int>::of(&QStackedWidget::currentChanged),
        [=](int index) {
    updateButtons();
});

updateButtons();
```

### 自定义样式

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 设置背景色
stackedWidget->setBackGroundColor(QColor(255, 255, 255));

// 设置圆角
stackedWidget->setBorderRadius(8);

// 添加页面
QWidget* page1 = new QWidget();
QVBoxLayout* layout1 = new QVBoxLayout(page1);
layout1->addWidget(new QLabel("页面 1"));

QWidget* page2 = new QWidget();
QVBoxLayout* layout2 = new QVBoxLayout(page2);
layout2->addWidget(new QLabel("页面 2"));

stackedWidget->addWidget(page1);
stackedWidget->addWidget(page2);
```

### 动态页面管理

```cpp
// 创建堆栈控件
NStackedWidget* stackedWidget = new NStackedWidget();

// 添加初始页面
QWidget* homePage = new QWidget();
QVBoxLayout* homeLayout = new QVBoxLayout(homePage);
homeLayout->addWidget(new QLabel("首页"));
stackedWidget->addWidget(homePage);

// 动态添加页面
NPushButton* addPageBtn = new NPushButton("添加页面");
connect(addPageBtn, &NPushButton::clicked, [=]() {
    static int pageCount = 1;
    
    QWidget* newPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(newPage);
    layout->addWidget(new QLabel(QString("动态页面 %1").arg(pageCount++)));
    
    int index = stackedWidget->addWidget(newPage);
    stackedWidget->setCurrentIndex(index);
});

// 删除当前页面
NPushButton* removePageBtn = new NPushButton("删除页面");
connect(removePageBtn, &NPushButton::clicked, [=]() {
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex > 0) {  // 保留首页
        QWidget* widget = stackedWidget->widget(currentIndex);
        stackedWidget->removeWidget(widget);
        delete widget;
        stackedWidget->setCurrentIndex(currentIndex - 1);
    }
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NStackedWidget(QWidget* parent = nullptr)` | 创建堆栈控件 |

### 页面管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `addWidget(QWidget* widget)` | 添加页面 | 返回页面索引 |
| `insertWidget(int index, QWidget* widget)` | 在指定位置插入页面 | 返回页面索引 |
| `removeWidget(QWidget* widget)` | 移除页面 | - |
| `widget(int index)` | 获取指定索引的页面 | 返回 `QWidget*` |
| `currentWidget()` | 获取当前页面 | 返回 `QWidget*` |
| `currentIndex()` | 获取当前页面索引 | 返回 `int` |
| `setCurrentIndex(int index)` | 设置当前页面 | 页面索引 |
| `setCurrentWidget(QWidget* widget)` | 设置当前页面 | 页面控件 |
| `count()` | 获取页面总数 | 返回 `int` |

### 页面切换动画

| 方法 | 说明 | 参数 |
|------|------|------|
| `doPageSwitch(NNavigationType::PageTransitionType transitionType, int targetIndex, bool isRouteBack, int duration)` | 带动画的页面切换 | 动画类型、目标索引、是否返回、动画时长 |

### 样式

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setBackGroundColor(QColor background)` | 设置背景色 | 颜色对象 |
| `backgroundColor()` | 获取背景色 | 返回 `QColor` |
| `setBorderRadius(int radius)` | 设置圆角半径 | 半径像素值 |
| `borderRadius()` | 获取圆角半径 | 返回 `int` |

### 继承的 API

NStackedWidget 继承自 `QStackedWidget`，支持所有标准 QStackedWidget 的方法：

- `currentChanged()` - 当前页面改变信号
- `widgetRemoved()` - 页面被移除信号

## 主题定制

堆栈控件会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
