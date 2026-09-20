#include "QtNativeUI/NConfig.h"
#include "../private/ntranslation.h"

#include <QFont>
#include <QtGlobal>

Q_SINGLETON_CREATE_CPP(NConfig)

void NConfig::prepareQtEnvironment() {
#if defined(Q_OS_WIN)
    if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "windows:fontengine=freetype");
#endif
}

NConfig::NConfig(QObject* parent) : QObject(parent) {}

NConfig::~NConfig() {}

void NConfig::initialize() {
#ifdef Q_OS_WIN
    if (qApp) {
        QFont font = qApp->font();
        font.setHintingPreference(QFont::PreferNoHinting);
        qApp->setFont(font);
    }
#endif
    nTranslation->setLanguage(QLocale::system().name());
}

bool NConfig::setLanguage(const QString& locale) { return nTranslation->setLanguage(locale); }

QString NConfig::currentLanguage() const { return nTranslation->currentLanguage(); }

QStringList NConfig::availableLanguages() const { return nTranslation->availableLanguages(); }

bool NConfig::loadCustomTranslation(const QString& filePath) { return nTranslation->loadCustomTranslation(filePath); }
