# Getting Started

## Installation

### Prerequisites

- Qt 5.15+ or Qt 6.0+
- CMake 3.16+
- C++17 compiler

### Build from Source

```bash
git clone https://github.com/Neutron12138/QtNativeUI.git
cd QtNativeUI
cmake --preset default
cmake --build build --config Release
```

### Integrate into Your Project

Add to your `CMakeLists.txt`:

```cmake
find_package(QtNativeUI REQUIRED)
target_link_libraries(your_target QtNativeUI::QtNativeUI)
```

## Basic Usage

### Create Your First Application

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

## Theme System

### Switch Themes

```cpp
#include <QtNativeUI/NTheme.h>

// Switch to dark theme
nTheme->setThemeMode(NThemeType::ThemeMode::Dark);

// Switch to light theme
nTheme->setThemeMode(NThemeType::ThemeMode::Light);
```

### Customize Colors

```cpp
#include <QtNativeUI/NTheme.h>

// Get theme color
QColor color = NThemeColor(NFluentColorKey::AccentFillColorDefault, NThemeType::Light);
```

## Next Steps

- Browse the [Component Library](/en/components/NLabel) to learn about all available controls
- Check out [Example Code](https://github.com/Neutron12138/QtNativeUI/tree/main/examples)
- Read the [API Documentation](/en/components/)
