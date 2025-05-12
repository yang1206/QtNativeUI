#include "QtNativeUI/NConfig.h"

#include <QStyle>
#include <qfont.h>
Q_SINGLETON_CREATE_CPP(NConfig)

NConfig::NConfig(QObject* parent) : QObject(parent) {}

NConfig::~NConfig() {

};

void NConfig::initialize() {
#ifdef Q_OS_WIN
    // qApp->setStyle("Fusion");

    QFont font = qApp->font();
    font.setPixelSize(13);
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
#endif
}
