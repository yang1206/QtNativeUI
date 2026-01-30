#include "DialogPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NDialog.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>

DialogPage::DialogPage(QWidget* parent)
    : BasePage("Dialog 无边框对话框", "基于 QWindowKit 的无边框对话框，支持多种窗口效果。", parent) {

    QHBoxLayout* effectLayout = new QHBoxLayout();
    effectLayout->setSpacing(16);

    NPushButton* noneButton = new NPushButton("无效果");
    noneButton->setFixedSize(120, 40);
    connect(noneButton, &NPushButton::clicked, this, [this]() {
        NDialog* dialog = new NDialog(this);
        dialog->setWindowTitle("无效果对话框");
        dialog->setWindowEffect(NDialog::None);
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     content   = new QWidget(dialog);
        QVBoxLayout* dlgLayout = new QVBoxLayout(content);
        dlgLayout->setContentsMargins(20, 10, 20, 20);
        QLabel*      label     = new QLabel("这是一个无背景效果的 NDialog", content);
        NPushButton* closeBtn  = new NPushButton("关闭", content);
        closeBtn->setFixedSize(100, 36);
        connect(closeBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        dlgLayout->addWidget(label);
        dlgLayout->addStretch();
        dlgLayout->addWidget(closeBtn, 0, Qt::AlignRight);
        dialog->setContentWidget(content);

        dialog->exec();
    });

    NPushButton* blurButton = new NPushButton("Blur 效果");
    blurButton->setFixedSize(120, 40);
    connect(blurButton, &NPushButton::clicked, this, [this]() {
        NDialog* dialog = new NDialog(this);
        dialog->setWindowTitle("Blur 效果对话框");
        dialog->setWindowEffect(NDialog::Blur);
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     content   = new QWidget(dialog);
        QVBoxLayout* dlgLayout = new QVBoxLayout(content);
        dlgLayout->setContentsMargins(20, 10, 20, 20);
        QLabel*      label     = new QLabel("这是一个 Blur 背景效果的 NDialog", content);
        NPushButton* closeBtn  = new NPushButton("关闭", content);
        closeBtn->setFixedSize(100, 36);
        connect(closeBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        dlgLayout->addWidget(label);
        dlgLayout->addStretch();
        dlgLayout->addWidget(closeBtn, 0, Qt::AlignRight);
        dialog->setContentWidget(content);

        dialog->exec();
    });

    NPushButton* acrylicButton = new NPushButton("Acrylic 效果");
    acrylicButton->setFixedSize(120, 40);
    connect(acrylicButton, &NPushButton::clicked, this, [this]() {
        NDialog* dialog = new NDialog(this);
        dialog->setWindowTitle("Acrylic 效果对话框");
        dialog->setWindowEffect(NDialog::Acrylic);
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     content   = new QWidget(dialog);
        QVBoxLayout* dlgLayout = new QVBoxLayout(content);
        dlgLayout->setContentsMargins(20, 10, 20, 20);
        QLabel*      label     = new QLabel("这是一个 Acrylic 背景效果的 NDialog (Windows 10+)", content);
        NPushButton* closeBtn  = new NPushButton("关闭", content);
        closeBtn->setFixedSize(100, 36);
        connect(closeBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        dlgLayout->addWidget(label);
        dlgLayout->addStretch();
        dlgLayout->addWidget(closeBtn, 0, Qt::AlignRight);
        dialog->setContentWidget(content);

        dialog->exec();
    });

    NPushButton* micaButton = new NPushButton("Mica 效果");
    micaButton->setFixedSize(120, 40);
    connect(micaButton, &NPushButton::clicked, this, [this]() {
        NDialog* dialog = new NDialog(this);
        dialog->setWindowTitle("Mica 效果对话框");
        dialog->setWindowEffect(NDialog::Mica);
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     content   = new QWidget(dialog);
        QVBoxLayout* dlgLayout = new QVBoxLayout(content);
        dlgLayout->setContentsMargins(20, 10, 20, 20);
        QLabel*      label     = new QLabel("这是一个 Mica 背景效果的 NDialog (Windows 11+)", content);
        NPushButton* closeBtn  = new NPushButton("关闭", content);
        closeBtn->setFixedSize(100, 36);
        connect(closeBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        dlgLayout->addWidget(label);
        dlgLayout->addStretch();
        dlgLayout->addWidget(closeBtn, 0, Qt::AlignRight);
        dialog->setContentWidget(content);

        dialog->exec();
    });

    NPushButton* micaAltButton = new NPushButton("MicaAlt 效果");
    micaAltButton->setFixedSize(120, 40);
    connect(micaAltButton, &NPushButton::clicked, this, [this]() {
        NDialog* dialog = new NDialog(this);
        dialog->setWindowTitle("MicaAlt 效果对话框");
        dialog->setWindowEffect(NDialog::MicaAlt);
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     content   = new QWidget(dialog);
        QVBoxLayout* dlgLayout = new QVBoxLayout(content);
        dlgLayout->setContentsMargins(20, 10, 20, 20);
        QLabel*      label     = new QLabel("这是一个 MicaAlt 背景效果的 NDialog (Windows 11+)", content);
        NPushButton* closeBtn  = new NPushButton("关闭", content);
        closeBtn->setFixedSize(100, 36);
        connect(closeBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        dlgLayout->addWidget(label);
        dlgLayout->addStretch();
        dlgLayout->addWidget(closeBtn, 0, Qt::AlignRight);
        dialog->setContentWidget(content);

        dialog->exec();
    });

    effectLayout->addWidget(noneButton);
    effectLayout->addWidget(blurButton);
    effectLayout->addWidget(acrylicButton);
    effectLayout->addWidget(micaButton);
    effectLayout->addWidget(micaAltButton);
    effectLayout->addStretch();

    addSection("窗口效果", effectLayout);

    QHBoxLayout* customLayout = new QHBoxLayout();
    customLayout->setSpacing(16);

    NPushButton* customTitleBarButton = new NPushButton("自定义标题栏");
    customTitleBarButton->setFixedSize(140, 40);
    connect(customTitleBarButton, &NPushButton::clicked, this, [this]() {
        class CustomTitleBarDialog : public NDialog {
          public:
            CustomTitleBarDialog(QWidget* parent) : NDialog(parent) {}

          protected:
            void showEvent(QShowEvent* event) override {
                NDialog::showEvent(event);
#ifdef Q_OS_MAC
                setNativeSystemButtonsVisible(false);
#endif
            }
        };

        CustomTitleBarDialog* dialog = new CustomTitleBarDialog(this);
        dialog->setWindowEffect(NDialog::Mica);
        dialog->resize(450, 350);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QWidget*     titleBar       = new QWidget(dialog);
        QHBoxLayout* titleBarLayout = new QHBoxLayout(titleBar);
        titleBarLayout->setContentsMargins(10, 5, 5, 5);

        QLabel* titleLabel = new QLabel("自定义标题栏", titleBar);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        NPushButton* closeBtn = new NPushButton("×", titleBar);
        closeBtn->setFixedSize(30, 30);
        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::reject);

        titleBarLayout->addWidget(titleLabel);
        titleBarLayout->addStretch();
        titleBarLayout->addWidget(closeBtn);

        titleBar->setFixedHeight(40);
        dialog->setTitleBar(titleBar);
        dialog->setHitTestVisible(closeBtn, true);

        QWidget*     contentWidget = new QWidget(dialog);
        QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->setContentsMargins(20, 10, 20, 20);

        QLabel* contentLabel = new QLabel(
            "这个对话框使用了自定义标题栏，\n标题栏区域可以拖动窗口，\n关闭按钮可以正常点击。\n\n在macOS上，原生红绿灯已被隐藏。",
            contentWidget);
        contentLabel->setWordWrap(true);

        NPushButton* okBtn = new NPushButton("确定", contentWidget);
        okBtn->setFixedSize(100, 36);
        connect(okBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        contentLayout->addWidget(contentLabel);
        contentLayout->addStretch();
        contentLayout->addWidget(okBtn, 0, Qt::AlignRight);

        dialog->setContentWidget(contentWidget);

        dialog->exec();
    });

    NPushButton* noTitleBarButton = new NPushButton("无标题栏");
    noTitleBarButton->setFixedSize(140, 40);
    connect(noTitleBarButton, &NPushButton::clicked, this, [this]() {
        class NoTitleBarDialog : public NDialog {
          public:
            NoTitleBarDialog(QWidget* parent) : NDialog(parent) {}

          protected:
            void showEvent(QShowEvent* event) override {
                NDialog::showEvent(event);
#ifdef Q_OS_MAC
                setNativeSystemButtonsVisible(false);
#endif
            }
        };

        NoTitleBarDialog* dialog = new NoTitleBarDialog(this);
        dialog->setWindowEffect(NDialog::Mica);
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        dialog->setWindowBarVisible(false);

        QWidget*     contentWidget = new QWidget(dialog);
        QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->setContentsMargins(20, 20, 20, 20);

        QLabel* contentLabel = new QLabel(
            "这是一个没有标题栏的对话框，\n适用于需要完全自定义布局的场景。\n\n可以通过 setWindowBarVisible(false) 隐藏默认标题栏。\n\n在macOS上，原生红绿灯已被隐藏。",
            contentWidget);
        contentLabel->setWordWrap(true);

        QHBoxLayout* btnLayout = new QHBoxLayout();
        NPushButton* cancelBtn = new NPushButton("取消", contentWidget);
        NPushButton* okBtn     = new NPushButton("确定", contentWidget);
        cancelBtn->setFixedSize(100, 36);
        okBtn->setFixedSize(100, 36);
        connect(cancelBtn, &NPushButton::clicked, dialog, &QDialog::reject);
        connect(okBtn, &NPushButton::clicked, dialog, &QDialog::accept);

        btnLayout->addStretch();
        btnLayout->addWidget(cancelBtn);
        btnLayout->addWidget(okBtn);

        contentLayout->addWidget(contentLabel);
        contentLayout->addStretch();
        contentLayout->addLayout(btnLayout);

        dialog->setContentWidget(contentWidget);

        dialog->exec();
    });

    customLayout->addWidget(customTitleBarButton);
    customLayout->addWidget(noTitleBarButton);
    customLayout->addStretch();

    addSection("自定义标题栏", customLayout);
}
