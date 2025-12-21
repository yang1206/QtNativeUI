# QWindowKit 引入分析文档

## 概述

本文档分析是否应该将 [QWindowKit](https://github.com/stdware/qwindowkit) 引入 QtNativeUI 项目，作为 `NMainWindow` 的底层实现。

## QWindowKit 简介

QWindowKit 是一个跨平台的 Qt 无边框窗口框架，由 `wangwenx190/FramelessHelper` 重构升级而来。

### 基本信息

| 项目 | 信息 |
|------|------|
| 仓库地址 | https://github.com/stdware/qwindowkit |
| 许可证 | Apache 2.0 |
| Qt 版本要求 | >= 5.12 |
| C++ 标准 | >= C++17 |
| CMake 版本 | >= 3.19 |
| 依赖 | qmsetup (构建工具) |

### 支持平台

| 平台 | 支持状态 |
|------|----------|
| Windows 11 | ✅ 完整支持 (Snap Layout, Mica, MicaAlt) |
| Windows 10 | ✅ 完整支持 (Acrylic, DWM) |
| Windows 7/8 | ✅ 基础支持 |
| macOS | ✅ 完整支持 (NSVisualEffectView) |
| Linux (X11) | ✅ 支持 |
| Linux (Wayland) | ⚠️ 部分支持 |

### 核心功能

1. **无边框窗口**：移除系统标题栏，保留原生窗口行为
2. **自定义标题栏**：支持自定义标题栏控件，正确处理拖拽和系统按钮
3. **窗口特效**：
   - Windows: Mica, MicaAlt, Acrylic, DWM Blur
   - macOS: NSVisualEffectView (Vibrant)
4. **Snap Layout**：Windows 11 窗口贴靠布局支持
5. **原生窗口行为**：拖拽、缩放、最大化/最小化动画
6. **窗口阴影**：各平台原生阴影效果
7. **Qt Widgets & Qt Quick**：同时支持两种 UI 框架

---

## 当前 NMainWindow 实现分析

### 现有功能

```
src/controls/NMainWindow.cpp
src/private/nmainwindow_p.cpp
src/private/nwindoweffect_win.cpp
src/private/nwindoweffect_win.h
```

| 功能 | 状态 |
|------|------|
| Windows Mica | ✅ |
| Windows MicaAlt | ✅ |
| Windows Acrylic | ✅ |
| Windows DWM Blur | ✅ |
| Windows 无边框 | ❌ |
| Windows Snap Layout | ❌ |
| macOS 特效 | ❌ |
| macOS 无边框 | ❌ |
| Linux 支持 | ❌ |
| 自定义标题栏 | ❌ |

### 现有实现的问题

1. **仅支持 Windows**：macOS 和 Linux 完全没有实现
2. **不支持无边框**：无法移除系统标题栏
3. **代码耦合**：平台特定代码与业务逻辑混合
4. **维护成本高**：需要持续跟踪 Windows API 变化

---

## 功能对比

### QtNativeUI 现有实现 vs QWindowKit

| 功能 | 现有实现 | QWindowKit |
|------|----------|------------|
| Windows Mica/Acrylic | ✅ | ✅ |
| Windows Snap Layout | ❌ | ✅ |
| Windows 无边框 + 原生行为 | ❌ | ✅ |
| macOS 原生特效 | ❌ | ✅ |
| macOS 无边框 | ❌ | ✅ |
| Linux X11 支持 | ❌ | ✅ |
| Linux Wayland 支持 | ❌ | ⚠️ 部分 |
| 自定义标题栏 | ❌ | ✅ |
| 窗口阴影 | ⚠️ 部分 | ✅ |
| Qt Quick 支持 | ❌ | ✅ |
| 系统按钮集成 | ❌ | ✅ |
| 窗口拖拽区域控制 | ❌ | ✅ |

---

## 引入收益

### 功能收益

1. **完整的跨平台支持**：一套代码支持 Windows、macOS、Linux
2. **无边框窗口**：支持自定义标题栏，实现现代化 UI
3. **原生体验**：Snap Layout、窗口动画、系统按钮等原生行为
4. **减少代码量**：删除 `nwindoweffect_win.cpp` 等平台特定代码

### 维护收益

1. **专业分工**：窗口系统由 QWindowKit 维护，QtNativeUI 专注 UI 控件
2. **社区支持**：QWindowKit 有活跃社区，持续修复平台兼容性问题
3. **减少踩坑**：避免自己处理各平台窗口系统的边缘情况

### 用户收益

1. **更好的用户体验**：原生窗口行为，符合平台规范
2. **更多功能**：无边框、自定义标题栏等高级功能
3. **跨平台一致性**：三平台统一的窗口体验

---

## 引入成本

### 依赖成本

1. **直接依赖**：QWindowKit 库本身
2. **间接依赖**：qmsetup (仅构建时需要)
3. **体积增加**：约 200-500KB (静态链接)

### 构建成本

1. **CMake 配置**：需要添加 submodule 和 CMake 配置
2. **构建时间**：首次构建增加约 1-2 分钟
3. **CI/CD 调整**：需要更新构建脚本

### 学习成本

1. **API 学习**：需要理解 QWindowKit 的 API 和概念
2. **调试复杂度**：问题可能涉及 QWindowKit 内部

### 风险成本

1. **上游依赖**：依赖上游项目的维护状态
2. **API 变化**：上游 API 可能变化
3. **Bug 传递**：上游 bug 会影响 QtNativeUI

---

## 引入方式

### 推荐方式：Git Submodule

```bash
# 添加 submodule
git submodule add https://github.com/stdware/qwindowkit.git third_party/qwindowkit
git submodule update --init --recursive
```

### CMake 配置

```cmake
# CMakeLists.txt

# QWindowKit 配置
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_WIDGETS ON CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_QUICK OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_DOCUMENTATIONS OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/qwindowkit)

# 链接到 QtNativeUI
target_link_libraries(QtNativeUI PRIVATE QWindowKit::Widgets)
```

### 目录结构

```
QtNativeUI/
├── third_party/
│   └── qwindowkit/          # Git Submodule
├── src/
│   ├── controls/
│   │   └── NMainWindow.cpp  # 简化后的实现
│   └── private/
│       └── nmainwindow_p.h  # 使用 QWindowKit
└── CMakeLists.txt           # 更新配置
```

---

## NMainWindow 重构方案

### 重构后的 API

```cpp
class NMainWindow : public QMainWindow {
    Q_OBJECT

public:
    enum BackdropType { None, Mica, MicaAlt, Acrylic, Blur };
    
    explicit NMainWindow(QWidget* parent = nullptr);
    ~NMainWindow();

    // 窗口特效
    void setBackdropEffect(BackdropType type);
    BackdropType backdropEffect() const;

    // 标题栏 (新增)
    void setTitleBar(QWidget* titleBar);
    QWidget* titleBar() const;
    
    // 系统按钮 (新增)
    void setSystemButton(SystemButton type, QWidget* button);
    
    // 拖拽区域 (新增)
    void setHitTestVisible(QWidget* widget, bool visible = true);

signals:
    void backdropEffectChanged(BackdropType type);
};
```

### 重构后的实现

```cpp
// NMainWindow.cpp
#include <QWKWidgets/widgetwindowagent.h>

NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent) {
    d_ptr->windowAgent = new QWK::WidgetWindowAgent(this);
    d_ptr->windowAgent->setup(this);
    
    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        d_ptr->updateWindowEffect();
    });
}

void NMainWindow::setBackdropEffect(BackdropType type) {
    Q_D(NMainWindow);
    // QWindowKit 处理所有平台差异
    d->windowAgent->setWindowAttribute(...);
}

void NMainWindow::setTitleBar(QWidget* titleBar) {
    Q_D(NMainWindow);
    d->windowAgent->setTitleBar(titleBar);
}
```

### 可删除的文件

引入 QWindowKit 后，以下文件可以删除：

- `src/private/nwindoweffect_win.cpp`
- `src/private/nwindoweffect_win.h`

---

## 已知问题和限制

### QWindowKit 已知问题

1. **Windows 10 顶部边框**：移除标题栏后顶部边框会消失，QWindowKit 有 workaround
2. **Vulkan 渲染**：Qt Quick + Vulkan 时顶部边框显示异常
3. **Linux Wayland**：部分功能受限于 Wayland 协议限制

### 兼容性注意事项

1. **Qt 版本**：需要 Qt 5.12+，推荐 Qt 6.x
2. **编译器**：需要 C++17 支持
3. **CMake**：需要 3.19+，推荐 3.20+

---

## 决策建议

### 建议引入的情况

- ✅ 目标是真正的跨平台 UI 库
- ✅ 需要支持 macOS 原生特效
- ✅ 需要无边框窗口功能
- ✅ 需要自定义标题栏
- ✅ 希望减少平台特定代码的维护

### 不建议引入的情况

- ❌ 仅针对 Windows 平台
- ❌ 不需要无边框窗口
- ❌ 希望保持零依赖
- ❌ 项目规模较小，不值得引入复杂依赖

---

## 结论

**对于 QtNativeUI 项目，建议引入 QWindowKit。**

理由：
1. QtNativeUI 的目标是"真正的跨平台原生 UI 库"
2. 自己实现 macOS 和 Linux 的窗口特效需要大量 Objective-C++ 和平台代码
3. QWindowKit 已经解决了这些问题，且许可证友好
4. 引入成本可控，主要是构建配置的调整

---

## 参考资料

- [QWindowKit GitHub](https://github.com/stdware/qwindowkit)
- [QWindowKit 示例代码](https://github.com/stdware/qwindowkit/tree/main/examples)
- [FramelessHelper (前身)](https://github.com/wangwenx190/framelesshelper)
- [qmsetup](https://github.com/stdware/qmsetup)

---

*文档创建日期: 2025-12-21*
*最后更新: 2025-12-21*
