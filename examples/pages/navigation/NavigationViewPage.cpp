#include "NavigationViewPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NNavigationView.h>
#include <QtNativeUI/NPushButton.h>

NavigationViewPage::NavigationViewPage(QWidget* parent)
    : BasePage("NavigationView 导航视图",
               "提供左侧导航栏和右侧内容区的布局，支持多级导航和页面切换。",
               parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NPushButton* basicButton = new NPushButton("基本导航视图");
    basicButton->setFixedSize(140, 40);
    connect(basicButton, &NPushButton::clicked, this, []() {
        QWidget* window = new QWidget();
        window->setWindowTitle("基本导航视图");
        window->resize(900, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout* layout = new QVBoxLayout(window);
        layout->setContentsMargins(0, 0, 0, 0);

        NNavigationView* navView = new NNavigationView(window);

        QString group1;
        navView->addExpanderNode("分组 1", group1, NRegularIconType::Folder16Regular);
        navView->addPageNode("页面 1-1", new QLabel("这是页面 1-1 的内容"), group1, NRegularIconType::Document16Regular);
        navView->addPageNode("页面 1-2", new QLabel("这是页面 1-2 的内容"), group1, NRegularIconType::Document16Regular);

        QString group2;
        navView->addExpanderNode("分组 2", group2, NRegularIconType::Folder16Regular);
        navView->addPageNode("页面 2-1", new QLabel("这是页面 2-1 的内容"), group2, NRegularIconType::Document16Regular);
        navView->addPageNode("页面 2-2", new QLabel("这是页面 2-2 的内容"), group2, NRegularIconType::Document16Regular);

        navView->addPageNode("独立页面", new QLabel("这是独立页面的内容"), NRegularIconType::Home16Regular);

        layout->addWidget(navView);
        window->show();
    });

    NPushButton* displayModeButton = new NPushButton("显示模式切换");
    displayModeButton->setFixedSize(140, 40);
    connect(displayModeButton, &NPushButton::clicked, this, []() {
        QWidget* window = new QWidget();
        window->setWindowTitle("显示模式切换");
        window->resize(900, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout* layout = new QVBoxLayout(window);
        layout->setContentsMargins(0, 0, 0, 0);

        NNavigationView* navView = new NNavigationView(window);

        QString group;
        navView->addExpanderNode("示例分组", group, NRegularIconType::Folder16Regular);
        navView->addPageNode("页面 1", new QLabel("页面 1 内容"), group, NRegularIconType::Document16Regular);
        navView->addPageNode("页面 2", new QLabel("页面 2 内容"), group, NRegularIconType::Document16Regular);

        QWidget*     controlWidget = new QWidget();
        QHBoxLayout* controlLayout = new QHBoxLayout(controlWidget);

        NPushButton* autoBtn = new NPushButton("Auto", controlWidget);
        NPushButton* minimalBtn = new NPushButton("Minimal", controlWidget);
        NPushButton* compactBtn = new NPushButton("Compact", controlWidget);
        NPushButton* maximalBtn = new NPushButton("Maximal", controlWidget);

        autoBtn->setFixedSize(100, 32);
        minimalBtn->setFixedSize(100, 32);
        compactBtn->setFixedSize(100, 32);
        maximalBtn->setFixedSize(100, 32);

        connect(autoBtn, &NPushButton::clicked, [navView]() {
            navView->setDisplayMode(NNavigationType::Auto);
        });
        connect(minimalBtn, &NPushButton::clicked, [navView]() {
            navView->setDisplayMode(NNavigationType::Minimal);
        });
        connect(compactBtn, &NPushButton::clicked, [navView]() {
            navView->setDisplayMode(NNavigationType::Compact);
        });
        connect(maximalBtn, &NPushButton::clicked, [navView]() {
            navView->setDisplayMode(NNavigationType::Maximal);
        });

        controlLayout->addWidget(new QLabel("显示模式:"));
        controlLayout->addWidget(autoBtn);
        controlLayout->addWidget(minimalBtn);
        controlLayout->addWidget(compactBtn);
        controlLayout->addWidget(maximalBtn);
        controlLayout->addStretch();

        QWidget*     contentWidget = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->addWidget(controlWidget);
        contentLayout->addWidget(new QLabel("点击按钮切换导航栏显示模式"));
        contentLayout->addStretch();

        navView->addPageNode("控制面板", contentWidget, NRegularIconType::Settings16Regular);

        layout->addWidget(navView);
        window->show();
    });

    basicLayout->addWidget(basicButton);
    basicLayout->addWidget(displayModeButton);
    basicLayout->addStretch();

    addSection("基本示例", basicLayout);

    QHBoxLayout* transitionLayout = new QHBoxLayout();
    transitionLayout->setSpacing(16);

    NPushButton* transitionButton = new NPushButton("页面过渡动画");
    transitionButton->setFixedSize(140, 40);
    connect(transitionButton, &NPushButton::clicked, this, []() {
        QWidget* window = new QWidget();
        window->setWindowTitle("页面过渡动画");
        window->resize(900, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout* layout = new QVBoxLayout(window);
        layout->setContentsMargins(0, 0, 0, 0);

        NNavigationView* navView = new NNavigationView(window);

        QWidget*     controlWidget = new QWidget();
        QVBoxLayout* controlLayout = new QVBoxLayout(controlWidget);

        QLabel* titleLabel = new QLabel("选择过渡动画类型:");
        titleLabel->setStyleSheet("font-weight: bold;");

        QHBoxLayout* btnLayout = new QHBoxLayout();

        NPushButton* noTransBtn = new NPushButton("无动画", controlWidget);
        NPushButton* popupBtn = new NPushButton("弹出", controlWidget);
        NPushButton* scaleBtn = new NPushButton("缩放", controlWidget);
        NPushButton* flipBtn = new NPushButton("翻转", controlWidget);

        noTransBtn->setFixedSize(100, 32);
        popupBtn->setFixedSize(100, 32);
        scaleBtn->setFixedSize(100, 32);
        flipBtn->setFixedSize(100, 32);

        connect(noTransBtn, &NPushButton::clicked, [navView]() {
            navView->setPageTransitionType(NNavigationType::NoTransition);
        });
        connect(popupBtn, &NPushButton::clicked, [navView]() {
            navView->setPageTransitionType(NNavigationType::PopupTransition);
        });
        connect(scaleBtn, &NPushButton::clicked, [navView]() {
            navView->setPageTransitionType(NNavigationType::ScaleTransition);
        });
        connect(flipBtn, &NPushButton::clicked, [navView]() {
            navView->setPageTransitionType(NNavigationType::FlipTransition);
        });

        btnLayout->addWidget(noTransBtn);
        btnLayout->addWidget(popupBtn);
        btnLayout->addWidget(scaleBtn);
        btnLayout->addWidget(flipBtn);
        btnLayout->addStretch();

        controlLayout->addWidget(titleLabel);
        controlLayout->addLayout(btnLayout);
        controlLayout->addWidget(new QLabel("\n点击按钮设置过渡动画，然后切换页面查看效果"));
        controlLayout->addStretch();

        navView->addPageNode("动画控制", controlWidget, NRegularIconType::Settings16Regular);

        for (int i = 1; i <= 5; i++) {
            QLabel* pageLabel = new QLabel(QString("这是页面 %1 的内容").arg(i));
            navView->addPageNode(QString("页面 %1").arg(i), pageLabel, NRegularIconType::Document16Regular);
        }

        layout->addWidget(navView);
        window->show();
    });

    transitionLayout->addWidget(transitionButton);
    transitionLayout->addStretch();

    addSection("页面过渡", transitionLayout);

    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(8);

    QLabel* infoLabel = new QLabel(
        "NNavigationView 特性：\n\n"
        "• 支持多级导航结构（分组 + 页面）\n"
        "• 四种显示模式：Auto（自动）、Minimal（最小化）、Compact（紧凑）、Maximal（最大化）\n"
        "• 多种页面过渡动画：弹出、缩放、翻转等\n"
        "• 支持搜索功能\n"
        "• 支持页脚节点\n"
        "• 支持路由系统和导航历史\n"
        "• 支持关键点标记（keyPoints）\n\n"
        "本示例程序就是使用 NNavigationView 构建的！",
        this);
    infoLabel->setWordWrap(true);

    infoLayout->addWidget(infoLabel);

    addSection("组件说明", infoLayout);
}
