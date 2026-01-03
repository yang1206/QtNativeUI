# NPivot 透视导航

### 介绍

透视导航用于在相关内容之间进行切换，类似于选项卡但具有更现代的外观。NPivot 提供了流畅的指示器动画和丰富的自定义选项。

### 基本用法

```cpp
// 创建透视导航
NPivot* pivot = new NPivot();

// 添加导航项
pivot->addItem("首页");
pivot->addItem("消息");
pivot->addItem("设置");

// 设置当前选中项
pivot->setCurrentIndex(0);

// 监听选择变化
connect(pivot, &NPivot::currentChanged, [](int index) {
    qDebug() << "切换到索引：" << index;
});
```

### 图标支持

支持为导航项添加 Fluent 图标，可以使用 Regular 或 Filled 样式。

```cpp
NPivot* iconPivot = new NPivot();

// 添加带图标的项目
iconPivot->addItem("首页", NRegularIconType::Home24Regular);
iconPivot->addItem("搜索", NRegularIconType::Search24Regular);
iconPivot->addItem("设置", NRegularIconType::Settings24Regular);

// 也可以使用 Filled 图标
iconPivot->addItem("收藏", NFilledIconType::Heart24Filled);
```

### 徽章功能

支持为导航项添加徽章，用于显示未读消息数量等。

```cpp
NPivot* badgePivot = new NPivot();

badgePivot->addItem("消息");
badgePivot->addItem("通知");

// 设置徽章
badgePivot->setBadge(0, "5");    // 消息有5条未读
badgePivot->setBadge(1, "99+");  // 通知超过99条

// 清除徽章
badgePivot->clearBadge(0);
```

### 对齐方式

支持左对齐、居中对齐和右对齐三种布局方式。

```cpp
NPivot* alignPivot = new NPivot();
alignPivot->addItem("项目1");
alignPivot->addItem("项目2");
alignPivot->addItem("项目3");

// 设置对齐方式
alignPivot->setAlignment(NPivotType::Center);  // 居中对齐
// alignPivot->setAlignment(NPivotType::Left);    // 左对齐（默认）
// alignPivot->setAlignment(NPivotType::Right);   // 右对齐
```

### 右侧扩展控件

可以在导航栏右侧添加额外的控件，如搜索框、按钮等。

```cpp
NPivot* extendedPivot = new NPivot();
extendedPivot->addItem("文档");
extendedPivot->addItem("图片");
extendedPivot->addItem("视频");

// 添加搜索框到右侧
NLineEdit* searchBox = new NLineEdit();
searchBox->setPlaceholderText("搜索...");
searchBox->addAction(NRegularIconType::Search24Regular, QLineEdit::LeadingPosition);
extendedPivot->addRightWidget(searchBox);

// 添加更多按钮
NPushButton* moreBtn = new NPushButton();
moreBtn->setFluentIcon(NRegularIconType::MoreHorizontal24Regular, 16);
extendedPivot->addRightWidget(moreBtn);
```

### 自定义字体

支持分别设置普通项目和选中项目的字体。

```cpp
NPivot* fontPivot = new NPivot();
fontPivot->addItem("标准");
fontPivot->addItem("粗体");

// 设置普通项目字体
QFont itemFont = fontPivot->font();
itemFont.setPointSize(14);
fontPivot->setItemFont(itemFont);

// 设置选中项目字体
QFont selectedFont = itemFont;
selectedFont.setBold(true);
fontPivot->setSelectedItemFont(selectedFont);
```

### 键盘导航

支持完整的键盘导航操作。

```cpp
// 支持的键盘操作：
// - 左/右方向键：切换项目
// - Home：跳转到第一项
// - End：跳转到最后一项
// - Enter/Space：激活当前项目

// 启用键盘焦点
pivot->setFocusPolicy(Qt::StrongFocus);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NPivot(QWidget* parent = nullptr)` | 创建透视导航控件 |

### 项目管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `addItem(const QString& text)` | 添加文本项目 | 项目文本 |
| `addItem(const QString& text, NRegularIconType::Icon icon)` | 添加带 Regular 图标的项目 | 文本、图标 |
| `addItem(const QString& text, NFilledIconType::Icon icon)` | 添加带 Filled 图标的项目 | 文本、图标 |
| `removeItem(int index)` | 移除指定索引的项目 | 项目索引 |
| `clear()` | 清除所有项目 | - |
| `count()` | 获取项目数量 | - |
| `itemText(int index)` | 获取项目文本 | 项目索引 |
| `setItemText(int index, const QString& text)` | 设置项目文本 | 索引、文本 |

### 选择控制

| 方法 | 说明 | 参数 |
|------|------|------|
| `setCurrentIndex(int index)` | 设置当前选中项 | 项目索引 |
| `currentIndex()` | 获取当前选中项索引 | - |

### 徽章管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBadge(int index, const QString& text)` | 设置徽章文本 | 索引、徽章文本 |
| `badge(int index)` | 获取徽章文本 | 项目索引 |
| `clearBadge(int index)` | 清除徽章 | 项目索引 |

### 布局控制

| 方法 | 说明 | 参数 |
|------|------|------|
| `setAlignment(NPivotType::Alignment alignment)` | 设置对齐方式 | 对齐方式枚举 |
| `alignment()` | 获取对齐方式 | - |
| `addRightWidget(QWidget* widget)` | 添加右侧控件 | 控件指针 |

### 字体设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setItemFont(const QFont& font)` | 设置项目字体 | 字体对象 |
| `itemFont()` | 获取项目字体 | - |
| `setSelectedItemFont(const QFont& font)` | 设置选中项字体 | 字体对象 |
| `selectedItemFont()` | 获取选中项字体 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `currentChanged(int index)` | 当前项改变时触发 | 新的索引 |
| `itemClicked(int index)` | 项目被点击时触发 | 点击的索引 |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightSelectedTextColor` | 明亮主题选中文本色 | `QColor` |
| `darkSelectedTextColor` | 暗黑主题选中文本色 | `QColor` |
| `lightIndicatorColor` | 明亮主题指示器色 | `QColor` |
| `darkIndicatorColor` | 暗黑主题指示器色 | `QColor` |

### 枚举

#### NPivotType::Alignment

| 值 | 说明 |
|----|------|
| `Left` | 左对齐（默认） |
| `Center` | 居中对齐 |
| `Right` | 右对齐 |

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 项目高度 | `48px` | 导航项目高度 |
| 项目间距 | `24px` | 项目之间的间距 |
| 指示器高度 | `2px` | 底部指示器高度 |
| 动画时长 | `250ms` | 指示器滑动动画时长 |
| 徽章大小 | `16px` | 徽章圆形直径 |

### 主题适配

```cpp
// 透视导航会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 指示器和选中状态颜色会自动更新
});
```