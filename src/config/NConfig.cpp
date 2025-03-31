#include "QtNativeUI/NConfig.h"

#include <QStyle>
Q_SINGLETON_CREATE_CPP(NConfig)

NConfig::NConfig(QObject* parent) : QObject(parent) {}

NConfig::~NConfig() {

};

void NConfig::initialize() {
#ifdef Q_OS_WIN
    qApp->setStyle("Fusion");
#endif
}
