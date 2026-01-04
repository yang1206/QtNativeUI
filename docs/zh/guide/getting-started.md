# 快速开始

## 安装

### 前置要求

- Qt 5.15+ 或 Qt 6.0+
- CMake 3.16+
- C++17 编译器

### 从源代码构建

```bash
git clone https://github.com/Neutron12138/QtNativeUI.git
cd QtNativeUI
cmake --preset default
cmake --build build --config Release
```

### 集成到你的项目

在你的 `CMakeLists.txt` 中添加：

```cmake
find_package(QtNativeUI REQUIRED)
target_link_libraries(your_target QtNativeUI::QtNativeUI)
```

## 基本使用

### 创建第一个应用

```cpp
#include <QApplication>
#include <QtNativeUI/NMainWindow.h>
#include <QtNativeUI/NPushButton.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    NMainWindow window;
    window.setWindowTitle("QtNativeUI Demo");
    
    NPushButton *button = new NPushButton("Click Me");
    window.setCentralWidget(button);
    
    window.show();
    return app.exec();
}
```

## 主题系统

### 切换主题

```cpp
#include <QtNativeUI/NTheme.h>

// 切换到暗色主题
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);

// 切换到亮色主题
nTheme->setThemeMode(NThemeType::ThemeMode::Light);
```

### 自定义颜色

```cpp
#include <QtNativeUI/NTheme.h>

// 获取主题颜色
QColor color = NThemeColor(NFluentColorKey::AccentFillColorDefault, NThemeType::Light);
```

## 下一步

- 浏览 [组件库](/components/NLabel) 了解所有可用控件
- 查看 [示例代码](https://github.com/Neutron12138/QtNativeUI/tree/main/examples)
- 阅读 [API 文档](/components/)
