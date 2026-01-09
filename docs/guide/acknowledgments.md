# 致谢

QtNativeUI 的开发离不开开源社区的支持和贡献。我们在此向所有为本项目提供帮助的个人和组织表示诚挚的感谢。

## 第三方依赖

本项目使用了以下优秀的开源库：

### QWindowKit

- **项目地址**: [https://github.com/stdware/qwindowkit](https://github.com/stdware/qwindowkit)
- **许可证**: Apache License 2.0
- **作者**: Stdware Collections, wangwenx190 (Yuhang Zhao)
- **贡献**: 提供了强大的跨平台窗口管理能力，实现了 Windows Mica/Acrylic 效果、macOS 模糊效果等现代窗口特性

QWindowKit 是 QtNativeUI 的核心依赖，为我们提供了：

- 跨平台的无边框窗口实现
- Windows 11 Snap Layout 支持
- 原生窗口效果集成
- 精确的鼠标事件处理

### Fluent UI System Icons

- **项目地址**: [https://github.com/microsoft/fluentui-system-icons](https://github.com/microsoft/fluentui-system-icons)
- **许可证**: MIT License
- **作者**: Microsoft Corporation
- **贡献**: 提供了完整的 Fluent Design 风格图标集

我们使用了 Fluent UI System Icons 中的图标资源，确保了界面图标与 Microsoft Fluent Design 设计规范的一致性。

## 设计灵感

以下项目为 QtNativeUI 的设计和实现提供了重要启发：

### ElaWidgetTools

- **项目地址**: [https://github.com/Liniyous/ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools)
- **许可证**: MIT License
- **作者**: Liniyous
- **启发**: 优秀的 Qt Fluent Design 组件库实现，为我们的组件设计提供了重要参考

### PyQt-Fluent-Widgets

- **项目地址**: [https://github.com/zhiyiYo/PyQt-Fluent-Widgets](https://github.com/zhiyiYo/PyQt-Fluent-Widgets)
- **许可证**: GPLv3 (开源版本)
- **作者**: zhiyiYo
- **启发**: Python 版本的 Fluent Design 组件库，在组件 API 设计和主题系统方面给了我们很多灵感

### Microsoft WinUI

- **项目地址**: [https://github.com/microsoft/microsoft-ui-xaml](https://github.com/microsoft/microsoft-ui-xaml)
- **许可证**: MIT License
- **作者**: Microsoft Corporation
- **启发**: 官方的 Fluent Design 设计规范和实现，是我们设计组件外观和交互的重要参考

## 技术参考

### FramelessHelper

QWindowKit 项目继承了 [wangwenx190/framelesshelper](https://github.com/wangwenx190/framelesshelper) 的主要实现，我们也要感谢 FramelessHelper 项目为跨平台窗口管理所做的开创性工作。

### Qt Framework

- **官方网站**: [https://qt.io](https://qt.io)
- **许可证**: LGPL v3 / Commercial
- **贡献**: 提供了强大的跨平台 C++ 应用程序开发框架

QtNativeUI 基于 Qt 框架构建，充分利用了 Qt 的信号槽机制、绘制系统和跨平台能力。

## 许可证声明

本项目遵循各依赖库的许可证要求：

- **QWindowKit**: 遵循 Apache License 2.0，保留原作者版权声明
- **Fluent UI System Icons**: 遵循 MIT License，保留 Microsoft 版权声明
- **其他依赖**: 详细的许可证信息请查看项目根目录的 [NOTICE](https://github.com/yang1206/QtNativeUI/blob/main/NOTICE) 文件

## 社区贡献

感谢所有为 QtNativeUI 项目做出贡献的开发者：

- **核心开发**: yang1206
- **文档完善**: 社区贡献者们
- **问题反馈**: 所有提交 Issue 和建议的用户

如果您想为项目做出贡献，请查看我们的 [贡献指南](./contributing.md)。

## 特别感谢

- **Qt 社区**: 为跨平台 C++ 开发提供了优秀的框架和丰富的资源
- **Microsoft**: 为现代应用界面设计制定了 Fluent Design 标准
- **开源社区**: 所有为相关项目贡献代码、文档和想法的开发者们

## 联系我们

如果您在使用过程中发现任何问题，或者有改进建议，欢迎通过以下方式联系我们：

- **GitHub Issues**: [https://github.com/yang1206/QtNativeUI/issues](https://github.com/yang1206/QtNativeUI/issues)
- **项目主页**: [https://github.com/yang1206/QtNativeUI](https://github.com/yang1206/QtNativeUI)

---

_开源让世界变得更美好。感谢每一位为开源事业做出贡献的人！_
