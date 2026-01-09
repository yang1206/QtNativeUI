#include "ntranslation.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>

Q_SINGLETON_CREATE_CPP(NTranslation)

NTranslation::NTranslation(QObject* parent) : QObject(parent), m_currentLanguage("en_US") {}

NTranslation::~NTranslation() {
    QCoreApplication::removeTranslator(&m_translator);
    QCoreApplication::removeTranslator(&m_customTranslator);
}

bool NTranslation::setLanguage(const QString& locale) {
    QCoreApplication::removeTranslator(&m_translator);

    QString resourcePath = QString(":/translations/qtnativeui_%1.qm").arg(locale);
    bool    success      = m_translator.load(resourcePath);

    if (success) {
        QCoreApplication::installTranslator(&m_translator);
        m_currentLanguage = locale;
    } else {
        success = tryLoadEnglishFallback();
    }

    return success;
}

bool NTranslation::tryLoadEnglishFallback() {
    QString resourcePath = ":/translations/qtnativeui_en_US.qm";
    bool    success      = m_translator.load(resourcePath);

    if (success) {
        QCoreApplication::installTranslator(&m_translator);
        m_currentLanguage = "en_US";
    } else {
        qWarning() << "Failed to load any translation";
    }

    return success;
}

QString NTranslation::currentLanguage() const { return m_currentLanguage; }

QStringList NTranslation::availableLanguages() const {
    QStringList languages;
    QDir        resourceDir(":/translations");
    QStringList files = resourceDir.entryList(QStringList() << "qtnativeui_*.qm", QDir::Files);

    for (const QString& file : files) {
        QString lang = file.mid(11, file.length() - 14); // 去掉"qtnativeui_"和".qm"
        if (!languages.contains(lang)) {
            languages << lang;
        }
    }

    return languages;
}

bool NTranslation::loadCustomTranslation(const QString& filePath) {

    QCoreApplication::removeTranslator(&m_customTranslator);

    bool success = m_customTranslator.load(filePath);
    if (success) {
        QCoreApplication::installTranslator(&m_customTranslator);
    }

    return success;
}