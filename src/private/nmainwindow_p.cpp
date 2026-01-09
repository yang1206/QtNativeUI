#include "nmainwindow_p.h"
#include "nframelesshelper_p.h"

#include "QtNativeUI/NWindowBar.h"
#include "QtNativeUI/NWindowButton.h"

NMainWindowPrivate::NMainWindowPrivate()  = default;
NMainWindowPrivate::~NMainWindowPrivate() = default;

void NMainWindowPrivate::setupFrameless()
{
    Q_Q(NMainWindow);

    frameless = new NFramelessHelper(q, q);
    frameless->setup();
}

void NMainWindowPrivate::registerSystemButtons(NWindowBar* bar)
{
    if constexpr (!NFramelessHelper::useNativeSystemButtons()) {
        frameless->setSystemButton(NFramelessHelper::Minimize,
                                   bar->systemButton(NWindowButton::Minimize));
        frameless->setSystemButton(NFramelessHelper::Maximize,
                                   bar->systemButton(NWindowButton::Maximize));
        frameless->setSystemButton(NFramelessHelper::Close,
                                   bar->systemButton(NWindowButton::Close));

        auto* themeBtn = bar->systemButton(NWindowButton::Theme);
        auto* pinBtn   = bar->systemButton(NWindowButton::Pin);
        if (themeBtn)
            frameless->setHitTestVisible(themeBtn, true);
        if (pinBtn)
            frameless->setHitTestVisible(pinBtn, true);
    } else {
        frameless->setWindowAttribute(QStringLiteral("no-system-buttons"), false);
    }
}
