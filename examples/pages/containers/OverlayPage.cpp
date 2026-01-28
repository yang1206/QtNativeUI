#include "OverlayPage.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NOverlay.h>
#include <QtNativeUI/NPushButton.h>

OverlayPage::OverlayPage(QWidget* parent)
    : BasePage("Overlay 遮罩层", "Overlay 提供遮罩层功能，用于加载状态、模态背景等场景。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NPushButton* showOverlayButton = new NPushButton("显示遮罩层", this);
    showOverlayButton->setFixedSize(120, 40);
    connect(showOverlayButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->showOverlay(this);

        QTimer::singleShot(2000, [overlay]() {
            overlay->hideOverlay();
            overlay->deleteLater();
        });
    });

    NPushButton* showWithContentButton = new NPushButton("显示带内容的遮罩", this);
    showWithContentButton->setFixedSize(150, 40);
    connect(showWithContentButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);

        QWidget* contentWidget = new QWidget(overlay);
        QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->addWidget(new NLabel("这是自定义内容", NLabelType::Title, contentWidget));
        contentLayout->addWidget(new NLabel("点击遮罩层关闭", NLabelType::Body, contentWidget));

        overlay->setContentWidget(contentWidget);
        overlay->setClickToClose(true);
        overlay->showOverlay(this);
    });

    basicLayout->addWidget(showOverlayButton);
    basicLayout->addWidget(showWithContentButton);
    basicLayout->addStretch();

    addSection("基本遮罩层", basicLayout);

    QHBoxLayout* loadingLayout = new QHBoxLayout();
    loadingLayout->setSpacing(16);

    NPushButton* showLoadingButton = new NPushButton("显示加载中", this);
    showLoadingButton->setFixedSize(120, 40);
    connect(showLoadingButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->showLoading(this, "加载中...");

        QTimer::singleShot(3000, [overlay]() {
            overlay->hideLoading();
            overlay->deleteLater();
        });
    });

    NPushButton* customLoadingButton = new NPushButton("自定义加载文本", this);
    customLoadingButton->setFixedSize(150, 40);
    connect(customLoadingButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->showLoading(this, "正在处理数据，请稍候...");

        QTimer::singleShot(3000, [overlay]() {
            overlay->hideLoading();
            overlay->deleteLater();
        });
    });

    loadingLayout->addWidget(showLoadingButton);
    loadingLayout->addWidget(customLoadingButton);
    loadingLayout->addStretch();

    addSection("加载状态", loadingLayout);

    QHBoxLayout* opacityLayout = new QHBoxLayout();
    opacityLayout->setSpacing(16);

    NPushButton* lowOpacityButton = new NPushButton("低透明度", this);
    lowOpacityButton->setFixedSize(120, 40);
    connect(lowOpacityButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->setOpacity(30);
        overlay->showOverlay(this);

        QTimer::singleShot(2000, [overlay]() {
            overlay->hideOverlay();
            overlay->deleteLater();
        });
    });

    NPushButton* highOpacityButton = new NPushButton("高透明度", this);
    highOpacityButton->setFixedSize(120, 40);
    connect(highOpacityButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->setOpacity(80);
        overlay->showOverlay(this);

        QTimer::singleShot(2000, [overlay]() {
            overlay->hideOverlay();
            overlay->deleteLater();
        });
    });

    opacityLayout->addWidget(lowOpacityButton);
    opacityLayout->addWidget(highOpacityButton);
    opacityLayout->addStretch();

    addSection("不同透明度", opacityLayout);

    QHBoxLayout* interactiveLayout = new QHBoxLayout();
    interactiveLayout->setSpacing(16);

    NPushButton* clickToCloseButton = new NPushButton("点击关闭", this);
    clickToCloseButton->setFixedSize(120, 40);
    connect(clickToCloseButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->setClickToClose(true);
        overlay->showOverlay(this);

        QWidget* contentWidget = new QWidget(overlay);
        QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->addWidget(new NLabel("点击遮罩层关闭", NLabelType::Body, contentWidget));
        overlay->setContentWidget(contentWidget);
    });

    NPushButton* noClickCloseButton = new NPushButton("禁止点击关闭", this);
    noClickCloseButton->setFixedSize(150, 40);
    connect(noClickCloseButton, &NPushButton::clicked, [this]() {
        NOverlay* overlay = new NOverlay(this);
        overlay->setClickToClose(false);
        overlay->showOverlay(this);

        QWidget* contentWidget = new QWidget(overlay);
        QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->addWidget(new NLabel("点击遮罩层无法关闭", NLabelType::Body, contentWidget));

        NPushButton* closeButton = new NPushButton("关闭", contentWidget);
        closeButton->setFixedSize(80, 32);
        connect(closeButton, &NPushButton::clicked, [overlay]() {
            overlay->hideOverlay();
            overlay->deleteLater();
        });
        contentLayout->addWidget(closeButton);

        overlay->setContentWidget(contentWidget);
    });

    interactiveLayout->addWidget(clickToCloseButton);
    interactiveLayout->addWidget(noClickCloseButton);
    interactiveLayout->addStretch();

    addSection("交互示例", interactiveLayout);
}
