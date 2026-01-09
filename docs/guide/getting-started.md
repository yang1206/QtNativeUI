# 快速开始

## 安装

### 前置要求

在开始使用 QtNativeUI 之前，请确保您的开发环境满足以下要求：

- **Qt 版本**:
  - **推荐**: Qt 6.8 或更高版本 (获得最佳性能和最新特性)
  - **最低要求**: Qt 6.6
- **编译器**: 支持 C++17 或更高版本的编译器 (MSVC 2019+, GCC 10+, Clang 11+)
- **构建工具**: CMake 3.20+ (支持 FetchContent 子模块功能)
- **Git**: 用于拉取子模块依赖

### 方式一：CMake FetchContent (推荐)

最简单集成 QtNativeUI 的方法是使用 CMake 的 `FetchContent` 模块。由于项目依赖 QWindowKit 子模块，需要 CMake 3.20+ 版本支持。

在您的项目的 `CMakeLists.txt` 中添加以下内容：

```cmake
cmake_minimum_required(VERSION 3.20)  # 确保支持子模块

include(FetchContent)

FetchContent_Declare(
    QtNativeUI
    GIT_REPOSITORY https://github.com/yang1206/QtNativeUI.git
    GIT_TAG        main  # 或者指定具体的 tag/commit hash，例如 v1.0.0
    GIT_SUBMODULES_RECURSE ON  # 重要：递归拉取子模块
)

FetchContent_MakeAvailable(QtNativeUI)

# 将 QtNativeUI 链接到您的目标
target_link_libraries(your_target PRIVATE QtNativeUI::QtNativeUI)
```

### 方式二：手动编译

如果您需要离线使用或更精细的控制，可以手动编译 QtNativeUI。

1.  **克隆仓库**:
    ```bash
    # 克隆项目及其子模块
    git clone --recursive https://github.com/yang1206/QtNativeUI.git
    cd QtNativeUI
    
    # 如果已经克隆但没有子模块，执行：
    # git submodule update --init --recursive
    ```

2.  **配置与构建**:
    ```bash
    # 配置项目
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    
    # 构建
    cmake --build build --config Release
    ```

3.  **安装** (可选):
    ```bash
    # 安装到指定目录
    cmake --install build --prefix "C:/path/to/install/QtNativeUI"
    ```

4.  **在项目中使用**:
    ```cmake
    # 如果安装到了自定义目录
    set(QtNativeUI_DIR "C:/path/to/install/QtNativeUI/lib/cmake/QtNativeUI")
    
    find_package(QtNativeUI REQUIRED)
    target_link_libraries(your_target PRIVATE QtNativeUI::QtNativeUI)
    ```

## 编写第一个应用

下面是一个最小的示例，展示如何创建一个包含现代窗口效果的主窗口。

### main.cpp

```cpp
#include <QApplication>
#include <QtNativeUI/NMainWindow.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTheme.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建主窗口
    NMainWindow window;
    window.setWindowTitle("QtNativeUI Demo");
    window.resize(800, 600);
    
    // 设置窗口背景效果 (Windows 11 Mica 效果)
    window.setBackdropType(NMainWindow::Mica);

    // 创建一个 Fluent 风格的按钮
    NPushButton *button = new NPushButton("Hello, Fluent Design!");
    button->setToolTip("这是一个现代风格的按钮");
    
    // 设置为主窗口的中心控件
    window.setCentralWidget(button);

    // 显示窗口
    window.show();
    
    return app.exec();
}
```

## 窗口背景效果

QtNativeUI 支持多种现代窗口背景效果，不同平台支持程度不同：

### Windows 平台

```cpp
NMainWindow window;

// Mica 效果 (Windows 11+)
window.setBackdropType(NMainWindow::Mica);

// Acrylic 效果 (Windows 10/11)
window.setBackdropType(NMainWindow::Acrylic);

// 传统模糊效果 (Windows Vista+)
window.setBackdropType(NMainWindow::Blur);

// 无效果
window.setBackdropType(NMainWindow::None);
```

### macOS 平台

```cpp
NMainWindow window;

// 模糊效果 (自动适配亮色/暗色主题)
window.setBackdropType(NMainWindow::Blur);

// 其他效果会自动回退到 Blur 或 None
```

### Linux 平台

```cpp
NMainWindow window;

// Linux 平台所有效果都会回退到 None
window.setBackdropType(NMainWindow::None);
```

## 主题系统

QtNativeUI 内置了完整的主题系统，支持亮色和暗色模式。

### 监听主题切换

```cpp
#include <QtNativeUI/NTheme.h>

NMainWindow window;

// 监听主题按钮点击
connect(&window, &NMainWindow::themeToggled, [](bool isDark) {
    qDebug() << "主题已切换为：" << (isDark ? "暗色" : "亮色");
});

// 获取当前主题
bool isDark = nTheme->isDarkMode();
```

### 手动切换主题

```cpp
// 切换到暗色模式
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);

// 切换到亮色模式
nTheme->setThemeMode(NThemeType::ThemeMode::Light);

// 跟随系统 (默认)
nTheme->setThemeMode(NThemeType::ThemeMode::Auto);
```

## 自定义标题栏

NMainWindow 提供了灵活的标题栏定制功能：

### 添加菜单栏

```cpp
NMainWindow window;

// 创建菜单栏
QMenuBar* menuBar = new QMenuBar();
QMenu* fileMenu = menuBar->addMenu("文件");
fileMenu->addAction("新建");
fileMenu->addAction("打开");

// 设置菜单栏 (会自动集成到标题栏)
window.setMenuBar(menuBar);
```

### 自定义系统按钮

```cpp
NMainWindow window;

// 获取主题切换按钮
NWindowButton* themeBtn = window.systemButton(NMainWindow::Theme);
if (themeBtn) {
    // 自定义按钮行为
    connect(themeBtn, &NWindowButton::clicked, []() {
        qDebug() << "主题按钮被点击";
    });
}

// 监听窗口操作
connect(&window, &NMainWindow::pinButtonToggled, [](bool pinned) {
    qDebug() << "窗口置顶状态：" << pinned;
});
```

## 常见问题

**Q: 为什么构建时提示找不到 Qt 模块？**
A: 请确保您已正确安装 Qt，并且 `CMAKE_PREFIX_PATH` 指向了 Qt 的安装目录 (例如 `C:\Qt\6.8.0\msvc2019_64`)。

**Q: FetchContent 方式提示子模块错误？**
A: 请确保使用 CMake 3.20+ 版本，并在 FetchContent_Declare 中添加 `GIT_SUBMODULES_RECURSE ON`。

**Q: 窗口效果在我的系统上不生效？**
A: 窗口效果依赖于系统支持。Windows 11 支持 Mica，Windows 10 支持 Acrylic，macOS 支持 Blur，Linux 暂不支持特殊效果。

**Q: 是否支持 Qt 5？**
A: 目前 QtNativeUI 主要针对 Qt 6 进行优化，不支持 Qt 5。建议使用 Qt 6.6+ 以获得完整功能支持。

## 下一步

- 浏览 [组件库](../components/NMainWindow.md) 查看所有可用控件
- 学习如何使用 [主题系统](../tools/NTheme.md) 进行深度定制
- 查看 [GitHub 仓库](https://github.com/yang1206/QtNativeUI) 获取更多示例代码