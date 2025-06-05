#include "QtNativeUI/NConfig.h"
#include "../private/ntranslation.h"

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
    font.setFamilies(QStringList() << "Segoe UI" << "Microsoft YaHei" << "PingFang SC");
    font.setPixelSize(13);
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
#endif
    nTranslation->setLanguage(QLocale::system().name());
}

bool NConfig::setLanguage(const QString& locale) { return nTranslation->setLanguage(locale); }

QString NConfig::currentLanguage() const { return nTranslation->currentLanguage(); }

QStringList NConfig::availableLanguages() const { return nTranslation->availableLanguages(); }

bool NConfig::loadCustomTranslation(const QString& filePath) { return nTranslation->loadCustomTranslation(filePath); }
