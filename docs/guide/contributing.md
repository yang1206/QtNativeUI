# 贡献指南

感谢您有兴趣为 QtNativeUI 做出贡献！我们需要社区的力量来让这个项目变得更好。

## 行为准则

我们致力于提供一个友好、安全和包容的环境。请在参与社区讨论和贡献代码时保持尊重和专业。

## 如何贡献

### 1. 报告 Bug

如果您发现了 bug，请在 GitHub Issues 中提交报告。请包含以下信息：

-   Qt 版本和操作系统版本。
-   复现步骤的详细描述。
-   （可选）相关的代码片段或截图。

### 2. 提交功能建议

如果您有新的功能想法，欢迎在 Issues 中发起讨论。请描述该功能解决了什么问题，以及您期望的 API 设计。

### 3. 提交代码 (Pull Request)

1.  **Fork** 本仓库到您的 GitHub 账户。
2.  **Clone** 您的 Fork 到本地。
3.  创建一个新的分支进行开发：`git checkout -b my-new-feature`。
4.  提交您的更改。请确保代码符合项目的编码规范。
5.  **Push** 到您的 Fork：`git push origin my-new-feature`。
6.  在 GitHub 上发起 **Pull Request**。

## 开发环境设置

请参考 [快速开始](/guide/getting-started) 文档搭建开发环境。我们建议使用 Qt 6.6+ 和 CMake 3.20+。

## 编码规范

-   **代码风格**: 我们使用 `clang-format` 来保持代码风格一致。请在提交前运行格式化工具。
-   **命名规范**:
    -   类名使用 `PascalCase` (如 `NLabel`)。
    -   函数名和变量名使用 `camelCase` (如 `setText`)。
    -   私有成员变量以 `m_` 开头 (如 `m_text`).
-   **注释**: 请为新的类和公共方法添加 Doxygen 风格的注释。

## 文档编写

文档基于 VitePress。如果您修改了 API，请同步更新 `docs/` 目录下对应的文档。

```bash
# 预览文档
cd docs
pnpm install
pnpm docs:dev
```

## 许可证

通过提交代码，您同意您的代码将遵循项目的 LICENSE (LGPL-3.0) 进行分发。
