#include "WindowBarPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>

WindowBarPage::WindowBarPage(QWidget* parent)
    : BasePage("WindowBar 窗口标题栏",
               "窗口标题栏组件，提供标题、图标、系统按钮等功能。通常与 NMainWindow 配合使用。",
               parent) {

    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(16);

    QLabel* descLabel = new QLabel(
        "NWindowBar 是一个完整的窗口标题栏实现，集成了窗口图标、标题文本、菜单栏和系统按钮。\n\n"
        "主要功能：\n"
        "• 窗口拖动：标题栏空白区域支持拖动移动窗口\n"
        "• 系统按钮：最小化、最大化、关闭、置顶、主题切换按钮\n"
        "• 菜单集成：可以将 QMenuBar 嵌入到标题栏中\n"
        "• 自定义组件：支持在标题栏中添加自定义组件\n"
        "• 响应式布局：自动适应窗口大小变化\n\n"
        "标题栏布局从左到右为：[图标] [标题] [菜单栏] [自定义组件] [系统按钮]",
        this);
    descLabel->setWordWrap(true);

    infoLayout->addWidget(descLabel);

    addSection("组件说明", infoLayout);

    QVBoxLayout* usageLayout = new QVBoxLayout();
    usageLayout->setSpacing(16);

    QLabel* usageLabel = new QLabel(
        "NWindowBar 通常不需要单独使用，它已经集成在 NMainWindow 中。\n\n"
        "如果需要自定义标题栏，可以通过以下方式：\n\n"
        "1. 获取默认标题栏并修改：\n"
        "   NWindowBar* bar = mainWindow->windowBar();\n"
        "   bar->setTitle(\"自定义标题\");\n"
        "   bar->setIconVisible(false);\n\n"
        "2. 设置自定义标题栏：\n"
        "   NWindowBar* customBar = new NWindowBar(mainWindow);\n"
        "   customBar->setTitle(\"自定义标题栏\");\n"
        "   mainWindow->setWindowBar(customBar);\n\n"
        "3. 添加菜单栏：\n"
        "   QMenuBar* menuBar = new QMenuBar();\n"
        "   mainWindow->setMenuBar(menuBar);\n\n"
        "4. 添加自定义组件：\n"
        "   NWindowBar* bar = mainWindow->windowBar();\n"
        "   bar->addWidget(customWidget);\n\n"
        "更多示例请参考 MainWindow 示例页面。",
        this);
    usageLabel->setWordWrap(true);

    usageLayout->addWidget(usageLabel);

    addSection("使用方法", usageLayout);
}
