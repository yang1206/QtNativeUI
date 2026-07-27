#include "MainWindowPage.h"
#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NMainWindow.h>
#include <QtNativeUI/NPushButton.h>

MainWindowPage::MainWindowPage(QWidget* parent)
    : BasePage("MainWindow 主窗口",
               "基于 QWindowKit 的无边框主窗口，支持多种窗口效果和自定义标题栏。",
               parent) {

    QHBoxLayout* effectLayout = new QHBoxLayout();
    effectLayout->setSpacing(16);

    NPushButton* noneButton = new NPushButton("无效果窗口");
    noneButton->setFixedSize(140, 40);
    connect(noneButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("无效果主窗口");
        window->setWindowEffect(NMainWindow::None);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("这是一个无背景效果的 NMainWindow", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    NPushButton* blurButton = new NPushButton("Blur 效果窗口");
    blurButton->setFixedSize(140, 40);
    connect(blurButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("Blur 效果主窗口");
        window->setWindowEffect(NMainWindow::Blur);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("这是一个 Blur 背景效果的 NMainWindow", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    NPushButton* acrylicButton = new NPushButton("Acrylic 效果窗口");
    acrylicButton->setFixedSize(140, 40);
    connect(acrylicButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("Acrylic 效果主窗口");
        window->setWindowEffect(NMainWindow::Acrylic);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("这是一个 Acrylic 背景效果的 NMainWindow (Windows 10+)", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    NPushButton* micaButton = new NPushButton("Mica 效果窗口");
    micaButton->setFixedSize(140, 40);
    connect(micaButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("Mica 效果主窗口");
        window->setWindowEffect(NMainWindow::Mica);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("这是一个 Mica 背景效果的 NMainWindow (Windows 11+)", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    NPushButton* micaAltButton = new NPushButton("MicaAlt 效果窗口");
    micaAltButton->setFixedSize(140, 40);
    connect(micaAltButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("MicaAlt 效果主窗口");
        window->setWindowEffect(NMainWindow::MicaAlt);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("这是一个 MicaAlt 背景效果的 NMainWindow (Windows 11+)", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

#ifdef Q_OS_MAC
    NPushButton* glassRegularButton = new NPushButton("Glass Regular");
    glassRegularButton->setFixedSize(140, 40);
    connect(glassRegularButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("Glass Regular 主窗口");
        window->setWindowEffect(NMainWindow::GlassRegular);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("macOS 26+ Liquid Glass regular 效果", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    NPushButton* glassRoundedButton = new NPushButton("Glass 圆角");
    glassRoundedButton->setFixedSize(140, 40);
    connect(glassRoundedButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("Glass 圆角主窗口");
        window->setWindowEffect(NMainWindow::GlassRegular);
        window->setGlassCornerRadius(24);
        window->setGlassTintColor(QColor(255, 255, 255, 46));
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("Liquid Glass + 圆角 + 浅色 tint", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });
#endif

    effectLayout->addWidget(noneButton);
    effectLayout->addWidget(blurButton);
    effectLayout->addWidget(acrylicButton);
    effectLayout->addWidget(micaButton);
    effectLayout->addWidget(micaAltButton);
#ifdef Q_OS_MAC
    if (NMainWindow::isGlassEffectSupported()) {
        effectLayout->addWidget(glassRegularButton);
        effectLayout->addWidget(glassRoundedButton);
    }
#endif
    effectLayout->addStretch();

    addSection("窗口效果", effectLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(16);

    NPushButton* themeButton = new NPushButton("主题按钮示例");
    themeButton->setFixedSize(140, 40);
    connect(themeButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("主题按钮示例");
        window->setWindowEffect(NMainWindow::Mica);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("点击标题栏的主题按钮可以切换明暗主题", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    NPushButton* pinButton = new NPushButton("置顶按钮示例");
    pinButton->setFixedSize(140, 40);
    connect(pinButton, &NPushButton::clicked, this, []() {
        NMainWindow* window = new NMainWindow();
        window->setWindowTitle("置顶按钮示例");
        window->setWindowEffect(NMainWindow::Mica);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel("点击标题栏的置顶按钮可以将窗口置顶", central);
        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    buttonLayout->addWidget(themeButton);
    buttonLayout->addWidget(pinButton);
    buttonLayout->addStretch();

    addSection("系统按钮", buttonLayout);

    QHBoxLayout* customLayout = new QHBoxLayout();
    customLayout->setSpacing(16);

    NPushButton* customTitleBarButton = new NPushButton("自定义标题栏");
    customTitleBarButton->setFixedSize(140, 40);
    connect(customTitleBarButton, &NPushButton::clicked, this, []() {
        class CustomTitleBarWindow : public NMainWindow {
          public:
            CustomTitleBarWindow() : NMainWindow() {}

          protected:
            void showEvent(QShowEvent* event) override {
                NMainWindow::showEvent(event);
#ifdef Q_OS_MAC
                setNativeSystemButtonsVisible(false);
#endif
            }
        };

        CustomTitleBarWindow* window = new CustomTitleBarWindow();
        window->setWindowTitle("自定义标题栏");
        window->setWindowEffect(NMainWindow::Mica);
        window->resize(800, 600);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     titleBar       = new QWidget(window);
        QHBoxLayout* titleBarLayout = new QHBoxLayout(titleBar);
        titleBarLayout->setContentsMargins(10, 5, 5, 5);

        QLabel* titleLabel = new QLabel("自定义标题栏", titleBar);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        NPushButton* closeBtn = new NPushButton("×", titleBar);
        closeBtn->setFixedSize(30, 30);
        connect(closeBtn, &QPushButton::clicked, window, &QWidget::close);

        titleBarLayout->addWidget(titleLabel);
        titleBarLayout->addStretch();
        titleBarLayout->addWidget(closeBtn);

        titleBar->setFixedHeight(40);
        window->setTitleBarWidget(titleBar);
        window->setHitTestVisible(closeBtn, true);

        QWidget*     central = new QWidget(window);
        QVBoxLayout* layout  = new QVBoxLayout(central);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* label = new QLabel(
            "这个窗口使用了自定义标题栏，\n标题栏区域可以拖动窗口，\n关闭按钮可以正常点击。\n\n在macOS上，原生红绿灯已被隐藏。",
            central);
        label->setWordWrap(true);

        layout->addWidget(label);
        layout->addStretch();

        window->setCentralWidget(central);
        window->show();
    });

    customLayout->addWidget(customTitleBarButton);
    customLayout->addStretch();

    addSection("自定义标题栏", customLayout);
}
