# 快速开始

## 安装

### 前置要求

在开始使用 QtNativeUI 之前，请确保您的开发环境满足以下要求：

- **Qt 版本**:
  - **推荐**: Qt 6.8 或更高版本 (获得最佳性能和最新特性)
  - **最低要求**: Qt 6.6
- **编译器**: 支持 C++17 或更高版本的编译器 (MSVC 2019+, GCC 10+, Clang 11+)
- **构建工具**: CMake 3.16+

### 方式一：CMake 远程模块引入 (推荐)

最简单集成 QtNativeUI 的方法是使用 CMake 的 `FetchContent` 模块。这种方式会自动下载并配置项目，无需手动管理源代码。

在您的项目的 `CMakeLists.txt` 中添加以下内容：

```cmake
include(FetchContent)

FetchContent_Declare(
    QtNativeUI
    GIT_REPOSITORY https://github.com/yang1206/QtNativeUI.git
    GIT_TAG        main  # 或者指定具体的 tag/commit hash，例如 v1.0.0
)

FetchContent_MakeAvailable(QtNativeUI)

# 将 QtNativeUI 链接到您的目标
target_link_libraries(your_target PRIVATE QtNativeUI::QtNativeUI)
```

### 方式二：手动编译安装

如果您需要离线使用或更精细的控制，可以手动编译并安装 QtNativeUI。

1.  **克隆仓库**:
    ```bash
    git clone https://github.com/yang1206/QtNativeUI.git
    cd QtNativeUI
    ```

2.  **配置与构建**:
    ```bash
    # 创建构建目录
    mkdir build && cd build

    # 配置项目 (请确保 Qt 的 bin 目录在 PATH 中，或指定 CMAKE_PREFIX_PATH)
    cmake ..

    # 构建 Release 版本
    cmake --build . --config Release
    ```

3.  **安装**:
    ```bash
    # 需要管理员权限
    cmake --install . --prefix "C:/path/to/install/QtNativeUI"
    ```

4.  **在项目中使用**:
    在您的 `CMakeLists.txt` 中：
    ```cmake
    # 指定安装路径 (如果未安装到系统目录)
    set(QtNativeUI_DIR "C:/path/to/install/QtNativeUI/lib/cmake/QtNativeUI")

    find_package(QtNativeUI REQUIRED)
    target_link_libraries(your_target PRIVATE QtNativeUI::QtNativeUI)
    ```

## 编写第一个应用

下面是一个最小的示例，展示如何创建一个包含原生风格按钮的主窗口。

### main.cpp

```cpp
#include <QApplication>
#include <QtNativeUI/NMainWindow.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTheme.h>

int main(int argc, char *argv[])
{
    // 初始化 High DPI 支持 (Qt 6 默认开启，但显式声明是个好习惯)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    
    QApplication app(argc, argv);

    // 创建主窗口
    NMainWindow window;
    window.setWindowTitle("QtNativeUI Demo");
    window.resize(800, 600);

    // 创建一个 Fluent 风格的按钮
    NPushButton *button = new NPushButton("Hello, Fluent Design!");
    button->setToolTip("这是一个原生风格的按钮");
    
    // 设置为主窗口的中心控件
    window.setCentralWidget(button);

    // 显示窗口
    window.show();
    
    return app.exec();
}
```

## 主题配置

QtNativeUI 内置了完整的主题系统，支持亮色和暗色模式的自动切换（跟随系统）或手动切换。

### 手动切换主题

```cpp
#include <QtNativeUI/NTheme.h>

// 强制切换到暗色模式
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);

// 强制切换到亮色模式
nTheme->setThemeMode(NThemeType::ThemeMode::Light);

//跟随系统 (默认)
nTheme->setThemeMode(NThemeType::ThemeMode::Auto);
```

### 获取主题颜色

您可以方便地获取当前主题下的标准颜色，用于自定义绘制。

```cpp
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NFluentColors.h>

// 获取当前主题下的强调色
QColor accentColor = nTheme->getAccentColor();

// 或者获取特定的 Fluent 颜色资源
QColor errorColor = nTheme->getColor(NThemeType::ColorRole::SystemErrorColor);
```

## 常见问题

**Q: 为什么构建时提示找不到 Qt 模块？**
A: 请确保您已正确安装 Qt，并且 `CMAKE_PREFIX_PATH` 指向了 Qt 的安装目录 (例如 `C:\Qt\6.8.0\msvc2019_64`)。

**Q: 是否支持 Qt 5？**
A: 目前 QtNativeUI 主要针对 Qt 6 进行优化，虽然部分组件可能兼容 Qt 5.15，但我们**强烈建议**使用 Qt 6.6+ 以获得完整功能支持和 Fluent Design 效果。

## 下一步

- 浏览 [组件库](/components/NLabel) 查看所有可用控件。
- 学习如何使用 [主题系统](/tools/NTheme) 进行深度定制。
- 查看 [GitHub 仓库](https://github.com/yang1206/QtNativeUI) 获取更多示例代码。
