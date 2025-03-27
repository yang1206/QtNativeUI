#include "QtNativeUI/NConfig.h"

#include <QStyle>
#include <QStyleFactory>
#include <QSysInfo>
Q_SINGLETON_CREATE_CPP(NConfig)

NConfig::NConfig(QObject* parent) : QObject(parent) {}

NConfig::~NConfig() = default;

void NConfig::initialize(QApplication* app) {
#ifdef Q_OS_WIN
    app->setStyle("Fusion");
#endif
}
