# NConfig 配置和国际化管理

### 介绍

NConfig 是 QtNativeUI 的配置管理系统，主要负责应用的初始化、国际化（i18n）和语言管理。它提供了简单的 API 来切换应用语言和加载自定义翻译。

### 基本用法

```cpp
#include <QtNativeUI/NConfig.h>

// 获取全局配置实例（单例）
NConfig* config = nConfig;

// 初始化配置（应在应用启动时调用）
nConfig->initialize();

// 获取当前语言
QString currentLang = nConfig->currentLanguage();
qDebug() << "当前语言：" << currentLang;
```

### 初始化

```cpp
#include <QApplication>
#include <QtNativeUI/NConfig.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 初始化 NConfig
    // 这会：
    // 1. 在 Windows 上设置合适的字体（Segoe UI Variable 或 Microsoft YaHei）
    // 2. 根据系统语言加载对应的翻译
    nConfig->initialize();

    // 创建主窗口
    QMainWindow window;
    window.show();

    return app.exec();
}
```

### 语言管理

#### 设置语言

```cpp
// 设置为中文
bool success = nConfig->setLanguage("zh_CN");
if (success) {
    qDebug() << "语言已切换为中文";
} else {
    qDebug() << "语言切换失败，使用英文作为备选";
}

// 设置为英文
nConfig->setLanguage("en_US");

// 设置为其他语言
nConfig->setLanguage("zh_TW");  // 繁体中文
nConfig->setLanguage("ja_JP");  // 日文
```

#### 获取当前语言

```cpp
// 获取当前语言代码
QString currentLang = nConfig->currentLanguage();
qDebug() << "当前语言：" << currentLang;

// 根据语言进行不同的处理
if (currentLang.startsWith("zh")) {
    qDebug() << "使用中文";
} else if (currentLang.startsWith("en")) {
    qDebug() << "使用英文";
}
```

#### 获取可用语言

```cpp
// 获取所有可用的语言列表
QStringList languages = nConfig->availableLanguages();

qDebug() << "可用语言：";
for (const QString& lang : languages) {
    qDebug() << "  " << lang;
}

// 创建语言选择菜单
QMenu* languageMenu = new QMenu("语言");
for (const QString& lang : languages) {
    QAction* action = languageMenu->addAction(lang);
    connect(action, &QAction::triggered, [=]() {
        nConfig->setLanguage(lang);
    });
}
```

### 自定义翻译

#### 加载自定义翻译文件

```cpp
// 加载自定义翻译文件
QString translationPath = "path/to/custom_translation.qm";
bool success = nConfig->loadCustomTranslation(translationPath);

if (success) {
    qDebug() << "自定义翻译已加载";
} else {
    qDebug() << "加载自定义翻译失败";
}
```

#### 翻译文件格式

翻译文件应该是 Qt 的 `.qm` 格式（编译后的翻译文件）。可以通过以下步骤创建：

1. 创建 `.ts` 文件（翻译源文件）
2. 使用 `lupdate` 工具提取字符串
3. 使用 Qt Linguist 编辑翻译
4. 使用 `lrelease` 工具编译为 `.qm` 文件

### 完整应用示例

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QtNativeUI/NConfig.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 初始化配置
    nConfig->initialize();

    // 创建主窗口
    QMainWindow window;
    window.setWindowTitle(QCoreApplication::translate("main", "Language Settings"));
    window.resize(400, 300);

    // 创建中心控件
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 语言标签
    QLabel* langLabel = new QLabel(QCoreApplication::translate("main", "Select Language:"));
    layout->addWidget(langLabel);

    // 语言选择下拉框
    QComboBox* langCombo = new QComboBox();
    QStringList languages = nConfig->availableLanguages();
    langCombo->addItems(languages);

    // 设置当前语言
    int currentIndex = languages.indexOf(nConfig->currentLanguage());
    if (currentIndex >= 0) {
        langCombo->setCurrentIndex(currentIndex);
    }

    layout->addWidget(langCombo);

    // 当前语言显示
    QLabel* currentLangLabel = new QLabel();
    auto updateCurrentLang = [=]() {
        currentLangLabel->setText(
            QCoreApplication::translate("main", "Current Language: %1")
                .arg(nConfig->currentLanguage())
        );
    };
    updateCurrentLang();
    layout->addWidget(currentLangLabel);

    // 连接信号
    connect(langCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        if (index >= 0 && index < languages.size()) {
            nConfig->setLanguage(languages[index]);
            updateCurrentLang();
            // 刷新 UI 以显示新的翻译
            window.setWindowTitle(QCoreApplication::translate("main", "Language Settings"));
            langLabel->setText(QCoreApplication::translate("main", "Select Language:"));
        }
    });

    layout->addStretch();

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
```

### 字体配置

NConfig 在初始化时会自动配置应用字体：

#### Windows

在 Windows 上，NConfig 会按以下优先级选择字体：
1. Segoe UI Variable（Windows 11 推荐）
2. Microsoft YaHei（中文支持）
3. PingFang SC（中文支持）
4. Segoe UI（备选）

```cpp
// 初始化时自动设置
nConfig->initialize();

// 字体已自动配置，无需手动设置
```

#### macOS 和 Linux

在 macOS 和 Linux 上，使用系统默认字体。

### 国际化最佳实践

#### 1. 使用 QCoreApplication::translate()

```cpp
// 推荐：使用 translate 函数
QString text = QCoreApplication::translate("context", "Hello World");

// 避免：硬编码字符串
// QString text = "Hello World";
```

#### 2. 标记可翻译的字符串

```cpp
// 推荐：使用 QT_TRANSLATE_NOOP 标记字符串
const char* messages[] = {
    QT_TRANSLATE_NOOP("context", "Message 1"),
    QT_TRANSLATE_NOOP("context", "Message 2"),
    QT_TRANSLATE_NOOP("context", "Message 3")
};

// 在使用时翻译
QString message = QCoreApplication::translate("context", messages[0]);
```

#### 3. 处理复数形式

```cpp
// 推荐：使用 QCoreApplication::translate 处理复数
int count = 5;
QString text = QCoreApplication::translate("context", "%n file(s)", "", count);
```

#### 4. 动态语言切换

```cpp
// 推荐：在语言切换时刷新 UI
connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
    nConfig->setLanguage(languages[index]);
    
    // 刷新所有 UI 文本
    updateUITexts();
});

void updateUITexts() {
    window->setWindowTitle(QCoreApplication::translate("main", "Application Title"));
    button->setText(QCoreApplication::translate("main", "Click Me"));
    label->setText(QCoreApplication::translate("main", "Label Text"));
}
```

## API 参考

### 初始化

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `initialize()` | 初始化配置系统 | - | - |

### 语言管理

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setLanguage(const QString& locale)` | 设置应用语言 | 语言代码（如 "zh_CN"） | `bool` - 成功返回 true |
| `currentLanguage()` | 获取当前语言 | - | `QString` - 语言代码 |
| `availableLanguages()` | 获取可用语言列表 | - | `QStringList` - 语言代码列表 |

### 自定义翻译

| 方法 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `loadCustomTranslation(const QString& filePath)` | 加载自定义翻译文件 | 翻译文件路径 | `bool` - 成功返回 true |

## 支持的语言

QtNativeUI 内置支持以下语言：

| 语言代码 | 语言名称 |
|---------|---------|
| `en_US` | English (United States) |
| `zh_CN` | 简体中文 |
| `zh_TW` | 繁體中文 |

其他语言可以通过加载自定义翻译文件来支持。

## 常见问题

### Q: 如何添加新的语言支持？

A: 按以下步骤操作：

1. 创建 `.ts` 翻译源文件
2. 使用 `lupdate` 提取字符串
3. 使用 Qt Linguist 编辑翻译
4. 使用 `lrelease` 编译为 `.qm` 文件
5. 将 `.qm` 文件添加到资源文件
6. 使用 `loadCustomTranslation()` 加载

### Q: 为什么语言切换后 UI 没有更新？

A: 需要手动刷新 UI 文本。语言切换只会影响新创建的字符串，已显示的文本不会自动更新。

```cpp
connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
    nConfig->setLanguage(languages[index]);
    updateUITexts();  // 手动刷新 UI
});
```

### Q: 如何在启动时使用特定语言？

A: 在 `initialize()` 之后调用 `setLanguage()`：

```cpp
nConfig->initialize();
nConfig->setLanguage("zh_CN");  // 使用中文
```

### Q: 自定义翻译文件应该放在哪里？

A: 可以放在以下位置：
- 应用资源文件中（推荐）
- 应用目录下的 `translations` 文件夹
- 任何可访问的文件系统路径

### Q: 如何处理缺失的翻译？

A: NConfig 会自动回退到英文。如果某个字符串没有翻译，会显示英文版本。
