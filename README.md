<p align="center">
  <img src="docs/public/logo.svg" width="120" alt="QtNativeUI Logo" />
</p>

<h1 align="center">QtNativeUI</h1>

<p align="center">
    <b>现代化的 Qt UI 组件库，基于 Microsoft Fluent Design 设计体系。</b>
</p>

<p align="center">
  <!-- <a href="https://github.com/yang1206/QtNativeUI/actions/workflows/windows.yml">
    <img src="https://github.com/yang1206/QtNativeUI/actions/workflows/windows.yml/badge.svg" alt="Build Status" />
  </a> -->
  <a href="https://github.com/yang1206/QtNativeUI/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/yang1206/QtNativeUI" alt="License" />
  </a>
  <a href="https://qt.io">
    <img src="https://img.shields.io/badge/Qt-6.6%2B-41CD52?logo=qt" alt="Qt Version" />
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-0078D7" alt="Platform" />
  </a>
</p>

<p align="center">
  <a href="#特性">特性</a> •
  <a href="#快速开始">快速开始</a> •
  <a href="#文档">文档</a> •
  <a href="#示例">示例</a> •
  <a href="#贡献">贡献</a>
</p>

---

**QtNativeUI** 是一个为 Qt 桌面应用提供现代化外观的 C++ 组件库。它遵循 Fluent Design 设计规范，提供了动画效果、亚克力/云母（Mica）材质以及完善的主题系统。

## ✨ 特性

-   🎨 **Fluent Design**: 实现 Fluent Design 设计规范，支持 Acrylic, Mica 等现代材质效果
-   🌓 **主题系统**: 内置亮色/暗色模式，支持跟随系统自动切换，支持自定义强调色
-   🧩 **丰富组件**: 提供 40+ 个常用控件，从基础按钮到复杂的导航视图
-   ⚡ **高性能**: 基于 Qt 原生绘制，优化的资源占用和流畅的动画效果
-   🎈 **易于使用**: 清晰的 API 设计，无需复杂的 QSS，开箱即用
-   🦀 **跨平台**: 在 Windows, macOS 和 Linux 上提供一致的现代化体验

## 📦 安装

### 前置要求

-   Qt 6.6 或更高版本 (推荐 Qt 6.8+)
-   C++17 编译器
-   CMake 3.20+ (支持 FetchContent 子模块功能)
-   Git (用于拉取子模块依赖)

### 方式一：CMake FetchContent (推荐)

最简单集成 QtNativeUI 的方法是使用 CMake 的 `FetchContent` 模块。由于项目依赖 QWindowKit 子模块，需要 CMake 3.20+ 版本支持。

```cmake
cmake_minimum_required(VERSION 3.20)  # 确保支持子模块

include(FetchContent)

FetchContent_Declare(
    QtNativeUI
    GIT_REPOSITORY https://github.com/yang1206/QtNativeUI.git
    GIT_TAG        main
    GIT_SUBMODULES_RECURSE ON  # 重要：递归拉取子模块
)

FetchContent_MakeAvailable(QtNativeUI)

target_link_libraries(your_app PRIVATE QtNativeUI::QtNativeUI)
```

### 方式二：手动编译

如果您需要离线使用或更精细的控制，可以手动编译 QtNativeUI。

```bash
# 克隆项目及其子模块
git clone --recursive https://github.com/yang1206/QtNativeUI.git

# 或者分步执行
git clone https://github.com/yang1206/QtNativeUI.git
cd QtNativeUI
git submodule update --init --recursive

# 编译
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## 🚀 快速示例

```cpp
#include <QApplication>
#include <QtNativeUI/NMainWindow.h>
#include <QtNativeUI/NPushButton.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    NMainWindow window;
    window.setWindowTitle("QtNativeUI Demo");
    window.resize(800, 600);
    
    // 设置现代窗口背景效果
    window.setBackdropType(NMainWindow::Mica);

    NPushButton *btn = new NPushButton("Hello Fluent Design");
    btn->setToolTip("这是一个现代风格按钮");
    window.setCentralWidget(btn);

    window.show();
    return app.exec();
}
```

## 📚 文档

完整的文档和 API 参考请访问我们的文档网站（如果有在线地址请替换，这里假设本地）：

- [快速开始](docs/guide/getting-started.md)
- [组件总览](docs/components/NLabel.md)
- [主题系统](docs/tools/NTheme.md)

或者在本地运行文档：

```bash
cd docs
pnpm install
pnpm docs:dev
```

## 🤝 贡献

我们非常欢迎社区贡献！如果您发现 Bug 或有新功能建议，请提交 Issue。如果您想贡献代码，请阅读 [贡献指南](docs/guide/contributing.md)。

## 📄 许可证

本项目基于 [LGPL-3.0](LICENSE) 许可证开源。

### 第三方依赖

本项目使用了以下第三方库：

- **QWindowKit** - 跨平台窗口管理库
  - 许可证：Apache License 2.0
  - 作者：Stdware Collections, wangwenx190 (Yuhang Zhao)
  - 项目地址：https://github.com/stdware/qwindowkit

- **Fluent UI System Icons** - Microsoft Fluent 设计系统图标库
  - 许可证：MIT License
  - 作者：Microsoft Corporation
  - 项目地址：https://github.com/microsoft/fluentui-system-icons

详细的第三方许可证信息请查看 [NOTICE](NOTICE) 文件。

## 🙏 致谢

本项目的设计和实现受到了以下优秀开源项目的启发：

- [QWindowKit](https://github.com/stdware/qwindowkit) - 提供了强大的跨平台窗口管理能力
- [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) - 优秀的 Qt Fluent Design 组件库实现
- [PyQt-Fluent-Widgets](https://github.com/zhiyiYo/PyQt-Fluent-Widgets) - Python 版本的 Fluent Design 组件库
- [Microsoft WinUI](https://github.com/microsoft/microsoft-ui-xaml) - 官方的 Fluent Design 设计规范和实现
- [Fluent UI System Icons](https://github.com/microsoft/fluentui-system-icons) - 提供了完整的 Fluent 风格图标集

感谢这些项目的开发者们为开源社区做出的贡献！
