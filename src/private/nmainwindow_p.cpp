#include "nmainwindow_p.h"

#include <QApplication>
#include <QPalette>

#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NWindowBar.h"
#include "QtNativeUI/NWindowButton.h"

#include <QWKWidgets/widgetwindowagent.h>

NMainWindowPrivate::NMainWindowPrivate()  = default;
NMainWindowPrivate::~NMainWindowPrivate() = default;

void NMainWindowPrivate::updateBackgroundColor() {
    backgroundColor = nTheme->isDarkMode()
                          ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                          : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);
}

void NMainWindowPrivate::setupWindowAgent() {
    Q_Q(NMainWindow);

    windowAgent = new QWK::WidgetWindowAgent(q);
    windowAgent->setup(q);

#ifdef Q_OS_MAC
    q->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
#endif
}

void NMainWindowPrivate::setupThemeConnection() {
    Q_Q(NMainWindow);

#ifdef Q_OS_MAC
    QObject::connect(nTheme, &NTheme::themeModeChanged, q, [this, q]() {
        if (backdropType == NMainWindow::Blur) {
            windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), nTheme->isDarkMode() ? "dark" : "light");
        }
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(q, &event);
    });
#elif defined(Q_OS_WIN)
    windowAgent->setWindowAttribute(QStringLiteral("dark-mode"), nTheme->isDarkMode());

    QObject::connect(nTheme, &NTheme::themeModeChanged, q, [this, q]() {
        windowAgent->setWindowAttribute(QStringLiteral("dark-mode"), nTheme->isDarkMode());

        if (backdropType == NMainWindow::None) {
            updateBackgroundColor();
            QPalette pal = q->palette();
            pal.setColor(QPalette::Window, backgroundColor);
            q->setPalette(pal);
            q->update();
        }

        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(q, &event);
    });
#endif
}

void NMainWindowPrivate::registerSystemButtons(NWindowBar* bar) {
    if constexpr (!useNativeSystemButtons()) {
        windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, bar->systemButton(NWindowButton::Minimize));
        windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, bar->systemButton(NWindowButton::Maximize));
        windowAgent->setSystemButton(QWK::WindowAgentBase::Close, bar->systemButton(NWindowButton::Close));

        auto* themeBtn = bar->systemButton(NWindowButton::Theme);
        auto* pinBtn   = bar->systemButton(NWindowButton::Pin);
        if (themeBtn)
            windowAgent->setHitTestVisible(themeBtn, true);
        if (pinBtn)
            windowAgent->setHitTestVisible(pinBtn, true);
    } else {
        windowAgent->setWindowAttribute(QStringLiteral("no-system-buttons"), false);
    }
}

void NMainWindowPrivate::applyBackdropEffect(NMainWindow::BackdropType type) {
    Q_Q(NMainWindow);
    bool needsRepaint = false;

#ifdef Q_OS_WIN
    static const QStringList effectKeys = {QString(),
                                           QStringLiteral("dwm-blur"),
                                           QStringLiteral("acrylic-material"),
                                           QStringLiteral("mica"),
                                           QStringLiteral("mica-alt")};

    for (int i = 1; i < effectKeys.size(); ++i) {
        windowAgent->setWindowAttribute(effectKeys[i], false);
    }

    if (type != NMainWindow::None && type < effectKeys.size()) {
        windowAgent->setWindowAttribute(effectKeys[type], true);
        q->setPalette(QPalette());
        if (backdropType == NMainWindow::None) {
            needsRepaint = true;
        }
    } else {
        updateBackgroundColor();
        QPalette pal = q->palette();
        pal.setColor(QPalette::Window, backgroundColor);
        q->setPalette(pal);
        needsRepaint = true;
    }

#elif defined(Q_OS_MAC)
    if (type == NMainWindow::Blur) {
        windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), nTheme->isDarkMode() ? "dark" : "light");
        q->setPalette(QPalette());
        if (backdropType == NMainWindow::None) {
            needsRepaint = true;
        }
    } else {
        windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), "none");
        updateBackgroundColor();
        QPalette pal = q->palette();
        pal.setColor(QPalette::Window, backgroundColor);
        q->setPalette(pal);
        needsRepaint = true;
    }
#else
    Q_UNUSED(type)
#endif

    if (needsRepaint) {
        q->update();
    }
}
