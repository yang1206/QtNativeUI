# NTheme 主题管理系统

### 介绍

NTheme 是 QtNativeUI 的核心主题管理系统，提供了完整的主题切换、颜色管理和设计令牌系统。它支持明亮/暗黑/系统三种主题模式，并能自动同步系统主题变化。

**重要提示：** NTheme 的功能在不同 Qt 版本中有显著差异。建议使用 **Qt 6.5 或更高版本** 以获得完整功能。

### Qt 版本支持情况

| 功能 | Qt 5.x | Qt 6.0-6.4 | Qt 6.5+ | Qt 6.8+ |
|------|--------|-----------|---------|---------|
| 基础主题切换 | ✓ | ✓ | ✓ | ✓ |
| 系统主题检测 | ✓ | ✓ | ✓ | ✓ |
| ColorScheme 同步 | ✗ | ✗ | ✓ | ✓ |
| 系统强调色检测 | ✓ | ✓ | ✓ | ✓ |
| 自动 ColorScheme 重置 | ✗ | ✗ | ✗ | ✓ |

### 基本用法

```cpp
#include <QtNativeUI/NTheme.h>

// 获取全局主题实例（单例）
NTheme* theme = nTheme;

// 检查当前是否为暗黑主题
if (theme->isDarkMode()) {
    qDebug() << "当前使用暗黑主题";
} else {
    qDebug() << "当前使用明亮主题";
}

// 监听主题变化
connect(theme, &NTheme::darkModeChanged, [](bool isDark) {
    qDebug() << "主题已切换为：" << (isDark ? "暗黑" : "明亮");
});
```

### 主题模式

NTheme 支持三种主题模式：

```cpp
// 设置为明亮主题
nTheme->setThemeMode(NThemeType::ThemeMode::Light);

// 设置为暗黑主题
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);

// 跟随系统主题（推荐）
nTheme->setThemeMode(NThemeType::ThemeMode::System);

// 获取当前主题模式
NThemeType::ThemeMode currentMode = nTheme->themeMode();
```

### 强调色管理

```cpp
// 获取当前强调色
NAccentColor accentColor = nTheme->accentColor();

// 设置强调色（使用 NAccentColor）
nTheme->setAccentColor(NColors::blue);

// 设置强调色（使用 QColor，自动转换）
nTheme->setAccentColor(QColor(0, 120, 215));

// 获取预定义的强调色
NAccentColor redAccent = nTheme->getAccentColor(NAccentColorType::Red);

// 获取根据当前主题调整的强调色
QColor themedAccent = nTheme->getThemedAccentColor(NAccentColorType::Blue);

// 获取强调色的特定变体
QColor darkVariant = nTheme->getAccentColorVariant(NAccentColorType::Blue, "dark");

// 监听强调色变化
connect(nTheme, &NTheme::accentColorChanged, [](const NAccentColor& color) {
    qDebug() << "强调色已更改";
});
```

### 系统强调色

```cpp
// 启用系统强调色（自动同步系统设置）
nTheme->useSystemAccentColor();

// 获取系统强调色
QColor systemAccent = nTheme->getSystemAccentColor();

// 监听系统强调色变化
connect(nTheme, &NTheme::systemAccentColorChanged, [](const QColor& color) {
    qDebug() << "系统强调色已更改为：" << color.name();
});
```

### 颜色管理

```cpp
// 获取 Fluent Design 颜色
QColor textColor = nTheme->getColor(NFluentColorKey::TextFillColorPrimary);
QColor backgroundColor = nTheme->getColor(NFluentColorKey::SolidBackgroundFillColorBase);

// 获取特定主题模式下的颜色
QColor darkModeText = nTheme->getColorForTheme(
    NFluentColorKey::TextFillColorPrimary,
    NThemeType::ThemeMode::Dark
);

// 自定义颜色
nTheme->setColor(NFluentColorKey::TextFillColorPrimary, QColor(255, 0, 0));

// 获取所有颜色键
QList<NFluentColorKey::Key> allKeys = nTheme->getAllColorKeys();

// 获取所有当前主题的颜色
QMap<NFluentColorKey::Key, QColor> allColors = nTheme->getAllColors();
```

### 设计令牌

设计令牌提供了一致的 UI 设计参数：

```cpp
// 圆角
int cornerRadius = nTheme->getRadius(NDesignTokenKey::CornerRadiusDefault).toInt();

// 间距
int spacing = nTheme->getSpacing(NDesignTokenKey::SpacingM).toInt();

// 字体大小
int fontSize = nTheme->getFontSize(NDesignTokenKey::FontSizeBody).toInt();

// 字重
int fontWeight = nTheme->getFontWeight(NDesignTokenKey::FontWeightMedium).toInt();

// 阴影层级
QVariantMap elevation = nTheme->getElevation(NDesignTokenKey::ElevationRest).toMap();

// 动画时长
int duration = nTheme->getAnimationDuration(NDesignTokenKey::AnimationNormal).toInt();

// 动画缓动
QString easing = nTheme->getAnimationEasing(NDesignTokenKey::EasingStandard).toString();
```

### Qt 6.5+ 特定功能

Qt 6.5 引入了 `Qt::ColorScheme`，NTheme 可以直接控制它：

```cpp
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
// 直接设置 Qt 的 ColorScheme
nTheme->setQtColorScheme(Qt::ColorScheme::Light);
nTheme->setQtColorScheme(Qt::ColorScheme::Dark);

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
// Qt 6.8+ 支持重置 ColorScheme 以跟随系统
nTheme->unsetQtColorScheme();
#endif
#endif
```

### 完整应用示例

```cpp
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NColor.h>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 创建主窗口
    QMainWindow window;
    window.setWindowTitle("NTheme 示例");

    QWidget* centralWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 主题模式标签
    QLabel* modeLabel = new QLabel();
    layout->addWidget(modeLabel);

    // 主题切换按钮
    QPushButton* lightBtn = new QPushButton("明亮主题");
    QPushButton* darkBtn = new QPushButton("暗黑主题");
    QPushButton* systemBtn = new QPushButton("系统主题");

    layout->addWidget(lightBtn);
    layout->addWidget(darkBtn);
    layout->addWidget(systemBtn);

    // 强调色标签
    QLabel* accentLabel = new QLabel();
    layout->addWidget(accentLabel);

    // 强调色按钮
    QPushButton* blueBtn = new QPushButton("蓝色");
    QPushButton* redBtn = new QPushButton("红色");
    QPushButton* greenBtn = new QPushButton("绿色");

    layout->addWidget(blueBtn);
    layout->addWidget(redBtn);
    layout->addWidget(greenBtn);

    // 更新标签
    auto updateLabels = [=]() {
        QString modeText = nTheme->isDarkMode() ? "暗黑主题" : "明亮主题";
        modeLabel->setText(QString("当前主题：%1").arg(modeText));

        QColor accentColor = nTheme->accentColor().normal();
        accentLabel->setText(QString("强调色：%1").arg(accentColor.name()));
    };

    // 连接信号
    connect(nTheme, &NTheme::darkModeChanged, [=](bool) {
        updateLabels();
    });

    connect(nTheme, &NTheme::accentColorChanged, [=](const NAccentColor&) {
        updateLabels();
    });

    // 连接按钮
    connect(lightBtn, &QPushButton::clicked, [=]() {
        nTheme->setThemeMode(NThemeType::ThemeMode::Light);
    });

    connect(darkBtn, &QPushButton::clicked, [=]() {
        nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
    });

    connect(systemBtn, &QPushButton::clicked, [=]() {
        nTheme->setThemeMode(NThemeType::ThemeMode::System);
    });

    connect(blueBtn, &QPushButton::clicked, [=]() {
        nTheme->setAccentColor(NColors::blue);
    });

    connect(redBtn, &QPushButton::clicked, [=]() {
        nTheme->setAccentColor(NColors::red);
    });

    connect(greenBtn, &QPushButton::clicked, [=]() {
        nTheme->setAccentColor(NColors::green);
    });

    // 初始化标签
    updateLabels();

    window.setCentralWidget(centralWidget);
    window.resize(400, 300);
    window.show();

    return app.exec();
}
```

### 主题感知的 UI 设计

```cpp
// 创建主题感知的按钮
NPushButton* button = new NPushButton("确定");

// 根据主题选择颜色
bool isDark = nTheme->isDarkMode();
QColor buttonColor = NColors::blue.defaultBrushFor(isDark);

// 监听主题变化并更新
connect(nTheme, &NTheme::darkModeChanged, [=](bool isDark) {
    QColor newColor = NColors::blue.defaultBrushFor(isDark);
    button->setStyleSheet(QString("background-color: %1;").arg(newColor.name()));
});
```

### 自定义颜色和令牌

```cpp
// 自定义 Fluent Design 颜色
nTheme->setColor(NFluentColorKey::TextFillColorPrimary, QColor(255, 0, 0));

// 自定义设计令牌
nTheme->setToken(NDesignTokenKey::CornerRadiusDefault, 12);
nTheme->setToken(NDesignTokenKey::SpacingM, 16);

// 重置为默认值
nTheme->resetToDefaults();
```

## API 参考

### 主题模式

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setThemeMode(NThemeType::ThemeMode mode)` | 设置主题模式 | Light/Dark/System | - |
| `themeMode()` | 获取当前主题模式 | - | `NThemeType::ThemeMode` |
| `isDarkMode()` | 判断是否为暗黑主题 | - | `bool` |

### 强调色

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setAccentColor(const NAccentColor& color)` | 设置强调色 | NAccentColor 对象 | - |
| `setAccentColor(const QColor& color)` | 设置强调色 | QColor 对象 | - |
| `accentColor()` | 获取当前强调色 | - | `NAccentColor` |
| `getAccentColor(NAccentColorType::Type type)` | 获取预定义强调色 | 强调色类型 | `NAccentColor` |
| `getThemedAccentColor(NAccentColorType::Type type)` | 获取主题感知的强调色 | 强调色类型 | `QColor` |
| `getAccentColorVariant(NAccentColorType::Type type, const QString& variant)` | 获取强调色变体 | 强调色类型、变体名称 | `QColor` |

### 系统强调色

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `useSystemAccentColor()` | 启用系统强调色同步 | - | - |
| `getSystemAccentColor()` | 获取系统强调色 | - | `QColor` |

### 颜色管理

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getColor(NFluentColorKey::Key key)` | 获取颜色 | 颜色键 | `QColor` |
| `setColor(NFluentColorKey::Key key, const QColor& color)` | 设置自定义颜色 | 颜色键、颜色值 | - |
| `getColorForTheme(NFluentColorKey::Key key, NThemeType::ThemeMode mode)` | 获取特定主题的颜色 | 颜色键、主题模式 | `QColor` |
| `getAllColorKeys()` | 获取所有颜色键 | - | `QList<NFluentColorKey::Key>` |
| `getAllColors()` | 获取所有当前主题颜色 | - | `QMap<NFluentColorKey::Key, QColor>` |

### 设计令牌

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getRadius(NDesignTokenKey::Radius key)` | 获取圆角令牌 | 圆角键 | `QVariant` |
| `getSpacing(NDesignTokenKey::Spacing key)` | 获取间距令牌 | 间距键 | `QVariant` |
| `getFontSize(NDesignTokenKey::FontSize key)` | 获取字体大小令牌 | 字体大小键 | `QVariant` |
| `getFontWeight(NDesignTokenKey::FontWeight key)` | 获取字重令牌 | 字重键 | `QVariant` |
| `getElevation(NDesignTokenKey::Elevation key)` | 获取阴影层级令牌 | 阴影键 | `QVariant` |
| `getAnimationDuration(NDesignTokenKey::AnimationDuration key)` | 获取动画时长令牌 | 动画时长键 | `QVariant` |
| `getAnimationEasing(NDesignTokenKey::AnimationEasing key)` | 获取动画缓动令牌 | 动画缓动键 | `QVariant` |

### Qt 6.5+ 特定方法

| 方法 | 说明 | 参数 | 返回值 | 最低版本 |
|------|------|------|--------|---------|
| `setQtColorScheme(Qt::ColorScheme scheme)` | 设置 Qt ColorScheme | Light/Dark | - | Qt 6.5+ |
| `unsetQtColorScheme()` | 重置 ColorScheme 跟随系统 | - | - | Qt 6.8+ |

### 其他方法

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `resetToDefaults()` | 重置所有自定义设置 | - | - |
| `drawEffectShadow(...)` | 绘制阴影效果 | 画笔、矩形、参数 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `darkModeChanged(bool isDark)` | 主题模式已改变 | 是否为暗黑主题 |
| `themeModeChanged(NThemeType::ThemeMode mode)` | 主题模式已改变 | 新的主题模式 |
| `accentColorChanged(const NAccentColor& color)` | 强调色已改变 | 新的强调色 |
| `systemAccentColorChanged(const QColor& color)` | 系统强调色已改变 | 新的系统强调色 |

## 最佳实践

### 1. 使用系统主题（推荐）

```cpp
// 推荐：跟随系统主题
nTheme->setThemeMode(NThemeType::ThemeMode::System);

// 避免：硬编码主题
// nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
```

### 2. 使用系统强调色

```cpp
// 推荐：使用系统强调色
nTheme->useSystemAccentColor();

// 避免：硬编码强调色
// nTheme->setAccentColor(NColors::blue);
```

### 3. 主题感知的 UI

```cpp
// 推荐：根据主题选择颜色
QColor textColor = nTheme->getColor(NFluentColorKey::TextFillColorPrimary);

// 避免：硬编码颜色
// QColor textColor = QColor(0, 0, 0);
```

### 4. 使用设计令牌

```cpp
// 推荐：使用设计令牌保持一致性
int radius = nTheme->getRadius(NDesignTokenKey::CornerRadiusDefault).toInt();
int spacing = nTheme->getSpacing(NDesignTokenKey::SpacingM).toInt();

// 避免：硬编码数值
// int radius = 8;
// int spacing = 16;
```

### 5. 监听主题变化

```cpp
// 推荐：监听主题变化并更新 UI
connect(nTheme, &NTheme::darkModeChanged, [this](bool isDark) {
    updateUIColors();
});

// 避免：忽略主题变化
```

## 版本迁移指南

### 从 Qt 5.x 升级到 Qt 6.5+

```cpp
// Qt 5.x 代码
nTheme->setThemeMode(NThemeType::ThemeMode::System);

// Qt 6.5+ 可以使用额外功能
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
nTheme->setQtColorScheme(Qt::ColorScheme::Dark);
#endif
```

### 条件编译示例

```cpp
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    // Qt 6.5+ 特定代码
    nTheme->setQtColorScheme(Qt::ColorScheme::Light);
#else
    // Qt 5.x 和 Qt 6.0-6.4 的代码
    nTheme->setThemeMode(NThemeType::ThemeMode::Light);
#endif
```

## 常见问题

### Q: 为什么我的应用在 Qt 5.x 上主题不同步？

A: Qt 5.x 没有原生的 ColorScheme 支持。NTheme 会通过监听 `QApplication::paletteChanged` 信号来检测系统主题变化，但可能不如 Qt 6.5+ 及时。建议升级到 Qt 6.5+。

### Q: 如何在运行时切换主题？

A: 使用 `setThemeMode()` 方法：
```cpp
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
```

### Q: 系统强调色为什么没有更新？

A: 确保已调用 `useSystemAccentColor()`。另外，某些平台可能不支持系统强调色检测。

### Q: 如何自定义颜色？

A: 使用 `setColor()` 方法：
```cpp
nTheme->setColor(NFluentColorKey::TextFillColorPrimary, QColor(255, 0, 0));
```

### Q: 设计令牌有什么用？

A: 设计令牌提供了一致的 UI 参数（圆角、间距、字体等），确保整个应用的视觉一致性。
