#include "ndialog_p.h"
#include "nframelesshelper_p.h"

#include <QVBoxLayout>

#include "QtNativeUI/NWindowBar.h"
#include "QtNativeUI/NWindowButton.h"

NDialogPrivate::NDialogPrivate()  = default;
NDialogPrivate::~NDialogPrivate() = default;

void NDialogPrivate::setupFrameless()
{
    Q_Q(NDialog);

    frameless = new NFramelessHelper(q, q);
    frameless->setup();
}

void NDialogPrivate::setupLayout()
{
    Q_Q(NDialog);

    mainLayout = new QVBoxLayout(q);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
}

void NDialogPrivate::setupDefaultWindowBar()
{
    Q_Q(NDialog);

    windowBar = new NWindowBar(q);
    windowBar->setHostWidget(q);
    windowBar->setIconVisible(false);

    if (auto* minBtn = windowBar->systemButton(NWindowButton::Minimize))
        minBtn->hide();
    if (auto* maxBtn = windowBar->systemButton(NWindowButton::Maximize))
        maxBtn->hide();
    if (auto* pinBtn = windowBar->systemButton(NWindowButton::Pin))
        pinBtn->hide();
    if (auto* themeBtn = windowBar->systemButton(NWindowButton::Theme))
        themeBtn->hide();

    frameless->setTitleBar(windowBar);
    registerSystemButtons();

    mainLayout->addWidget(windowBar);
}

void NDialogPrivate::registerSystemButtons()
{
    if constexpr (!NFramelessHelper::useNativeSystemButtons()) {
        frameless->setSystemButton(NFramelessHelper::Close,
                                   windowBar->systemButton(NWindowButton::Close));
    } else {
        frameless->setWindowAttribute(QStringLiteral("no-system-buttons"), false);
    }
}
