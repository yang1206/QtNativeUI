# 部署指南

开发完成后，您需要将应用程序打包分发给用户。由于 Qt 应用程序依赖于众多的动态链接库（DLL），正确部署是确保程序在目标机器上运行的关键。

## Windows 部署

在 Windows 上，Qt 提供了 `windeployqt` 工具来自动复制所需的依赖项。

### 1. 编译 Release 版本

首先确保您已经编译了 Release 版本的应用程序。

```bash
cd build
cmake --build . --config Release
```

### 2. 使用 windeployqt

找到您的可执行文件（例如 `MyApp.exe`），然后运行：

```powershell
# 假设 Qt 环境变量已配置
windeployqt --release --compiler-runtime "path\to\MyApp.exe"
```

这将会把 Qt 的核心库（QtCore, QtGui, QtWidgets 等）以及插件复制到 exe 同级目录。

### 3. 处理 QtNativeUI 依赖

如果 QtNativeUI 是动态链接的（默认），您还需要将 `QtNativeUI.dll` 复制到应用程序目录。

- 如果您使用 `FetchContent` 或手动编译安装，请找到生成的 `QtNativeUI.dll` 并复制。

### 4. 打包

现在您可以将整个文件夹打包成 zip 或使用安装程序制作工具（如 Inno Setup, NSIS）生成安装包。

## macOS 部署

在 macOS 上，使用 `macdeployqt` 工具。

```bash
# 编译
cmake --build . --config Release

# 部署
macdeployqt MyApp.app -dmg
```

这会将依赖库嵌入到 `.app` 包中，并生成一个 `.dmg` 磁盘镜像文件。

## Linux 部署

Linux 部署相对复杂，因为发行版众多。推荐使用 `AppImage` 或 `Flatpak`。

### 使用 linuxdeployqt (生成 AppImage)

1.  下载 `linuxdeployqt` AppImage。
2.  确保使用了老版本的系统（如 Ubuntu 20.04）进行编译，以保证 glibc 兼容性。
3.  运行：

```bash
# 必须先设置 QMAKE 路径
export PATH=/path/to/qt/bin:$PATH

linuxdeployqt MyApp -appimage
```

## CMake 安装规则

为了简化部署，建议在您的 `CMakeLists.txt` 中添加安装规则：

```cmake
install(TARGETS MyApp
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# 如果静态链接 QtNativeUI，不需要额外步骤
# 如果动态链接，需要安装 DLL
if(WIN32)
    install(FILES $<TARGET_FILE:QtNativeUI> DESTINATION bin)
endif()
```

这样您可以使用 `cmake --install .` 将所有文件导出到一个干净的目录，方便通过脚本处理。
