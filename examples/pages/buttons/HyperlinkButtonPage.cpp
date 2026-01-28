#include "HyperlinkButtonPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NHyperlinkButton.h>

HyperlinkButtonPage::HyperlinkButtonPage(QWidget* parent)
    : BasePage("HyperlinkButton 超链接按钮", "HyperlinkButton 用于显示可点击的超链接，点击后打开URL。", parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(24);

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NHyperlinkButton* qtBtn = new NHyperlinkButton("Visit Qt Website");
    qtBtn->setUrl("https://www.qt.io/");

    NHyperlinkButton* githubBtn = new NHyperlinkButton("Visit GitHub");
    githubBtn->setUrl("https://github.com/");

    NHyperlinkButton* disabledBtn = new NHyperlinkButton("Disabled Link");
    disabledBtn->setUrl("https://example.com/");
    disabledBtn->setEnabled(false);

    basicLayout->addWidget(qtBtn);
    basicLayout->addWidget(githubBtn);
    basicLayout->addWidget(disabledBtn);
    basicLayout->addStretch();

    mainLayout->addWidget(new QLabel("基本超链接："));
    mainLayout->addLayout(basicLayout);

    QHBoxLayout* interactiveLayout = new QHBoxLayout();
    interactiveLayout->setSpacing(16);

    NHyperlinkButton* customBtn = new NHyperlinkButton("Custom Link");
    customBtn->setUrl("https://doc.qt.io/");

    QLabel* clickLabel = new QLabel("未点击");

    connect(customBtn, &NHyperlinkButton::linkActivated, [clickLabel](const QUrl& url) {
        clickLabel->setText("已点击: " + url.toString());
    });

    interactiveLayout->addWidget(customBtn);
    interactiveLayout->addWidget(clickLabel);
    interactiveLayout->addStretch();

    mainLayout->addWidget(new QLabel("交互示例："));
    mainLayout->addLayout(interactiveLayout);

    addSection("HyperlinkButton 完整示例", mainLayout);
}
