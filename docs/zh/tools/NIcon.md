# NIcon Fluent 图标系统

### 介绍

NIcon 提供了 Microsoft Fluent Design 系统的图标库。包含两种风格的图标：Regular（常规）和 Filled（填充），每种风格都包含数百个图标。图标会自动适应应用主题（明亮/暗黑），并支持自定义颜色和大小。

### 图标风格

NIcon 提供两种图标风格：

- **Regular** - 常规风格，线条风格的图标
- **Filled** - 填充风格，实心的图标

### 基本用法

```cpp
#include <QtNativeUI/NIcon.h>

// 获取全局图标实例（单例）
NIcon* icon = nIcon;

// 创建常规风格的图标
QIcon searchIcon = nIcon->fromRegular(NRegularIconType::Search24Regular);

// 创建填充风格的图标
QIcon checkIcon = nIcon->fromFilled(NFilledIconType::CheckmarkCircle24Filled);

// 在按钮上使用图标
NPushButton* button = new NPushButton("搜索");
button->setIcon(searchIcon);
```

### 常规图标（Regular）

常规图标是线条风格，适合大多数 UI 场景：

```cpp
// 基本用法
QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular);

// 指定颜色
QIcon coloredIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    QColor(0, 120, 215)
);

// 指定大小
QIcon largeIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    32  // 32x32 像素
);

// 指定大小和颜色
QIcon customIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    32,
    QColor(255, 0, 0)
);

// 指定宽度和高度（非正方形）
QIcon rectIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    24,  // 字体大小
    32,  // 宽度
    24   // 高度
);

// 指定宽度、高度和颜色
QIcon customRectIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    24,
    32,
    24,
    QColor(0, 120, 215)
);
```

### 填充图标（Filled）

填充图标是实心风格，适合强调和突出：

```cpp
// 基本用法
QIcon icon = nIcon->fromFilled(NFilledIconType::CheckmarkCircle24Filled);

// 指定颜色
QIcon coloredIcon = nIcon->fromFilled(
    NFilledIconType::CheckmarkCircle24Filled,
    QColor(0, 200, 0)
);

// 指定大小
QIcon largeIcon = nIcon->fromFilled(
    NFilledIconType::CheckmarkCircle24Filled,
    32
);

// 指定大小和颜色
QIcon customIcon = nIcon->fromFilled(
    NFilledIconType::CheckmarkCircle24Filled,
    32,
    QColor(0, 200, 0)
);

// 指定宽度和高度
QIcon rectIcon = nIcon->fromFilled(
    NFilledIconType::CheckmarkCircle24Filled,
    24,
    32,
    24
);

// 指定宽度、高度和颜色
QIcon customRectIcon = nIcon->fromFilled(
    NFilledIconType::CheckmarkCircle24Filled,
    24,
    32,
    24,
    QColor(0, 200, 0)
);
```

### 主题感知

图标会自动适应应用主题。当主题改变时，图标颜色会自动更新：

```cpp
// 创建主题感知的图标（不指定颜色）
QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular);

// 在明亮主题中显示深色
// 在暗黑主题中显示浅色
// 自动适应

// 监听主题变化
connect(nTheme, &NTheme::darkModeChanged, [=](bool isDark) {
    // 图标会自动更新颜色
    qDebug() << "主题已改变，图标颜色已自动更新";
});
```

### 常见图标示例

```cpp
// 文件操作
QIcon newIcon = nIcon->fromRegular(NRegularIconType::DocumentAdd24Regular);
QIcon openIcon = nIcon->fromRegular(NRegularIconType::FolderOpen24Regular);
QIcon saveIcon = nIcon->fromRegular(NRegularIconType::Save24Regular);
QIcon deleteIcon = nIcon->fromRegular(NRegularIconType::Delete24Regular);

// 编辑操作
QIcon undoIcon = nIcon->fromRegular(NRegularIconType::ArrowUndo24Regular);
QIcon redoIcon = nIcon->fromRegular(NRegularIconType::ArrowRedo24Regular);
QIcon cutIcon = nIcon->fromRegular(NRegularIconType::Cut24Regular);
QIcon copyIcon = nIcon->fromRegular(NRegularIconType::Copy24Regular);
QIcon pasteIcon = nIcon->fromRegular(NRegularIconType::Clipboard24Regular);

// 搜索和导航
QIcon searchIcon = nIcon->fromRegular(NRegularIconType::Search24Regular);
QIcon homeIcon = nIcon->fromRegular(NRegularIconType::Home24Regular);
QIcon settingsIcon = nIcon->fromRegular(NRegularIconType::Settings24Regular);
QIcon helpIcon = nIcon->fromRegular(NRegularIconType::QuestionCircle24Regular);

// 状态指示
QIcon checkIcon = nIcon->fromFilled(NFilledIconType::CheckmarkCircle24Filled);
QIcon errorIcon = nIcon->fromFilled(NFilledIconType::ErrorCircle24Filled);
QIcon warningIcon = nIcon->fromFilled(NFilledIconType::Warning24Filled);
QIcon infoIcon = nIcon->fromFilled(NFilledIconType::Info24Filled);

// 通信
QIcon bellIcon = nIcon->fromRegular(NRegularIconType::Bell24Regular);
QIcon mailIcon = nIcon->fromRegular(NRegularIconType::Mail24Regular);
QIcon phoneIcon = nIcon->fromRegular(NRegularIconType::Phone24Regular);
```

### 工具栏示例

```cpp
// 创建工具栏
QToolBar* toolBar = new QToolBar();

// 添加带图标的工具栏按钮
QAction* newAction = toolBar->addAction(
    nIcon->fromRegular(NRegularIconType::DocumentAdd24Regular),
    "新建"
);

QAction* openAction = toolBar->addAction(
    nIcon->fromRegular(NRegularIconType::FolderOpen24Regular),
    "打开"
);

QAction* saveAction = toolBar->addAction(
    nIcon->fromRegular(NRegularIconType::Save24Regular),
    "保存"
);

toolBar->addSeparator();

QAction* undoAction = toolBar->addAction(
    nIcon->fromRegular(NRegularIconType::ArrowUndo24Regular),
    "撤销"
);

QAction* redoAction = toolBar->addAction(
    nIcon->fromRegular(NRegularIconType::ArrowRedo24Regular),
    "重做"
);

// 连接信号
connect(newAction, &QAction::triggered, [=]() {
    qDebug() << "新建文件";
});

connect(saveAction, &QAction::triggered, [=]() {
    qDebug() << "保存文件";
});
```

### 菜单示例

```cpp
// 创建菜单
NMenu* menu = new NMenu("文件");

// 添加带图标的菜单项
menu->addItem(
    "新建",
    nIcon->fromRegular(NRegularIconType::DocumentAdd24Regular)
);

menu->addItem(
    "打开",
    nIcon->fromRegular(NRegularIconType::FolderOpen24Regular)
);

menu->addItem(
    "保存",
    nIcon->fromRegular(NRegularIconType::Save24Regular)
);

menu->addSeparator();

menu->addItem(
    "退出",
    nIcon->fromRegular(NRegularIconType::SignOut24Regular)
);
```

### 按钮示例

```cpp
// 创建带图标的按钮
NPushButton* searchBtn = new NPushButton("搜索");
searchBtn->setIcon(nIcon->fromRegular(NRegularIconType::Search24Regular));

// 创建仅显示图标的按钮
NToolButton* settingsBtn = new NToolButton();
settingsBtn->setIcon(nIcon->fromRegular(NRegularIconType::Settings24Regular));

// 创建带彩色图标的按钮
NPushButton* deleteBtn = new NPushButton("删除");
deleteBtn->setIcon(nIcon->fromRegular(
    NRegularIconType::Delete24Regular,
    QColor(255, 0, 0)  // 红色
));
```

### 列表项示例

```cpp
// 创建带图标的列表项
NListView* listView = new NListView();
QStandardItemModel* model = new QStandardItemModel();

QStandardItem* item1 = new QStandardItem();
item1->setIcon(nIcon->fromRegular(NRegularIconType::Document24Regular));
item1->setText("文档1");

QStandardItem* item2 = new QStandardItem();
item2->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
item2->setText("文件夹1");

model->appendRow(item1);
model->appendRow(item2);

listView->setModel(model);
```

### 自定义大小和颜色

```cpp
// 创建不同大小的图标
QIcon smallIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, 16);
QIcon mediumIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, 24);
QIcon largeIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, 32);
QIcon extraLargeIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, 48);

// 创建不同颜色的图标
QIcon blueIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    QColor(0, 120, 215)
);

QIcon greenIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    QColor(0, 200, 0)
);

QIcon redIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    QColor(255, 0, 0)
);

// 创建半透明图标
QColor transparentColor(0, 120, 215, 128);  // 50% 透明
QIcon transparentIcon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    transparentColor
);
```

## API 参考

### Regular 图标方法

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `fromRegular(NRegularIconType::Icon icon)` | 创建常规图标 | 图标枚举 | `QIcon` |
| `fromRegular(NRegularIconType::Icon icon, const QColor& color)` | 创建指定颜色的常规图标 | 图标枚举、颜色 | `QIcon` |
| `fromRegular(NRegularIconType::Icon icon, int size)` | 创建指定大小的常规图标 | 图标枚举、大小 | `QIcon` |
| `fromRegular(NRegularIconType::Icon icon, int size, const QColor& color)` | 创建指定大小和颜色的常规图标 | 图标枚举、大小、颜色 | `QIcon` |
| `fromRegular(NRegularIconType::Icon icon, int size, int width, int height)` | 创建指定尺寸的常规图标 | 图标枚举、字体大小、宽度、高度 | `QIcon` |
| `fromRegular(NRegularIconType::Icon icon, int size, int width, int height, const QColor& color)` | 创建指定尺寸和颜色的常规图标 | 图标枚举、字体大小、宽度、高度、颜色 | `QIcon` |

### Filled 图标方法

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `fromFilled(NFilledIconType::Icon icon)` | 创建填充图标 | 图标枚举 | `QIcon` |
| `fromFilled(NFilledIconType::Icon icon, const QColor& color)` | 创建指定颜色的填充图标 | 图标枚举、颜色 | `QIcon` |
| `fromFilled(NFilledIconType::Icon icon, int size)` | 创建指定大小的填充图标 | 图标枚举、大小 | `QIcon` |
| `fromFilled(NFilledIconType::Icon icon, int size, const QColor& color)` | 创建指定大小和颜色的填充图标 | 图标枚举、大小、颜色 | `QIcon` |
| `fromFilled(NFilledIconType::Icon icon, int size, int width, int height)` | 创建指定尺寸的填充图标 | 图标枚举、字体大小、宽度、高度 | `QIcon` |
| `fromFilled(NFilledIconType::Icon icon, int size, int width, int height, const QColor& color)` | 创建指定尺寸和颜色的填充图标 | 图标枚举、字体大小、宽度、高度、颜色 | `QIcon` |

### 字体信息方法

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `getRegularFontFamily()` | 获取常规图标字体名称 | `QString` |
| `getFilledFontFamily()` | 获取填充图标字体名称 | `QString` |

## 最佳实践

### 1. 使用主题感知的图标

```cpp
// 推荐：不指定颜色，让图标自动适应主题
QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular);

// 避免：硬编码颜色
// QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular, QColor(0, 0, 0));
```

### 2. 选择合适的图标风格

```cpp
// 推荐：根据场景选择风格
// 常规风格用于大多数 UI
QIcon regularIcon = nIcon->fromRegular(NRegularIconType::Search24Regular);

// 填充风格用于强调和状态指示
QIcon filledIcon = nIcon->fromFilled(NFilledIconType::CheckmarkCircle24Filled);

// 避免：混乱地使用两种风格
```

### 3. 使用一致的图标大小

```cpp
// 推荐：使用设计令牌定义的大小
const int ICON_SIZE_SMALL = 16;
const int ICON_SIZE_MEDIUM = 24;
const int ICON_SIZE_LARGE = 32;

QIcon smallIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, ICON_SIZE_SMALL);
QIcon mediumIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, ICON_SIZE_MEDIUM);
QIcon largeIcon = nIcon->fromRegular(NRegularIconType::Search24Regular, ICON_SIZE_LARGE);

// 避免：随意使用不同的大小
```

### 4. 缓存图标

```cpp
// 推荐：缓存常用图标以提高性能
class IconCache {
private:
    QMap<QString, QIcon> cache;

public:
    QIcon getSearchIcon() {
        if (!cache.contains("search")) {
            cache["search"] = nIcon->fromRegular(NRegularIconType::Search24Regular);
        }
        return cache["search"];
    }
};

// 避免：每次都创建新的图标
// QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular);
```

## 常见问题

### Q: 如何找到我需要的图标？

A: 查看 `NIconEnums.h` 中的 `NRegularIconType` 和 `NFilledIconType` 枚举，或查看 Fluent Design 官方文档。

### Q: 图标为什么在暗黑主题中看不清？

A: 确保没有硬编码颜色。使用不指定颜色的方法，让图标自动适应主题：

```cpp
// 正确
QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular);

// 错误
QIcon icon = nIcon->fromRegular(NRegularIconType::Search24Regular, QColor(0, 0, 0));
```

### Q: 如何创建自定义大小的图标？

A: 使用带宽度和高度参数的方法：

```cpp
QIcon icon = nIcon->fromRegular(
    NRegularIconType::Search24Regular,
    24,  // 字体大小
    32,  // 宽度
    24   // 高度
);
```

### Q: 图标支持哪些格式？

A: NIcon 使用 TrueType 字体格式的 Fluent System Icons 字体。所有图标都是矢量的，可以无损缩放。
