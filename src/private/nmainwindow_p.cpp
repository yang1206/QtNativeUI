#include "nmainwindow_p.h"
#include "QtNativeUI/NTheme.h"

NMainWindowPrivate::NMainWindowPrivate(QObject* parent) : QObject(parent) {
    isDarkMode = nTheme->isDarkMode();
    updateBackgroundColor();
}

NMainWindowPrivate::~NMainWindowPrivate() {}

void NMainWindowPrivate::setBackdropEffect(NMainWindow::BackdropType type) {
    backdropEffect = type;
    if (backdropEffect == NMainWindow::None) {
        updateBackgroundColor();
    }
}

void NMainWindowPrivate::updateBackgroundColor() {
    backgroundColor = isDarkMode
                          ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                          : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);
}