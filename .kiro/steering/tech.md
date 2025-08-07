# Technology Stack & Build System

## Core Technologies

- **Language**: C++17/C++20
- **Framework**: Qt5/Qt6 (Widgets module)
- **Build System**: CMake 3.16+
- **Generator**: Ninja Multi-Config (preferred)

## Dependencies

- **Qt Modules**: Widgets, LinguistTools
- **Platform Libraries**: 
  - Windows: dwmapi, uxtheme (for native window effects)
- **Build Tools**: CMake, Ninja, Qt's lupdate/lrelease for translations

## Supported Compilers & Platforms

- **Windows**: MSVC (cl.exe), MinGW (g++.exe)
- **macOS**: Clang++ 
- **Linux**: GCC (g++)

## Build Configuration

### CMake Options
- `BUILD_SHARED_LIBS`: Build as shared library (default: ON)
- `QtNativeUI_BUILD_EXAMPLES`: Build example applications (default: ON)
- `QtNativeUI_BUILD_TESTS`: Build test suite (default: ON)

### Common Build Commands

```bash
# Configure with preset
cmake --preset default

# Build debug version
cmake --build build --config Debug

# Build release version  
cmake --build build --config Release

# Platform-specific presets
cmake --preset windows-msvc    # Windows with MSVC
cmake --preset windows-mingw   # Windows with MinGW
cmake --preset macos           # macOS
cmake --preset linux           # Linux
```

### Development Setup

1. Install Qt5 or Qt6 with Widgets module
2. Set `QT_DIR` or `QT_MSVC_DIR` environment variable
3. Configure with appropriate preset
4. Build with CMake

## Code Standards

- **C++ Standard**: C++17 minimum, C++20 preferred
- **Formatting**: Uses .clang-format with LLVM base style
- **Encoding**: UTF-8 with BOM for C++ files
- **Line Length**: 120 characters maximum
- **Indentation**: 4 spaces, no tabs

## Translation System

- Uses Qt's internationalization system (lupdate/lrelease)
- Translation files in `translations/` directory
- Automatic QM file generation during build
- Supported languages: English (en_US), Chinese (zh_CN)