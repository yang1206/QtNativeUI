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
    // 移除现有翻译器
    QCoreApplication::removeTranslator(&m_translator);

    // 从资源加载翻译文件
    QString resourcePath = QString(":/translations/qtnativeui_%1.qm").arg(locale);
    bool    success      = m_translator.load(resourcePath);

    if (success) {
        QCoreApplication::installTranslator(&m_translator);
        m_currentLanguage = locale;
    } else {
        // 如果加载失败，尝试加载英语翻译作为后备
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
        // 从文件名提取语言代码
        QString lang = file.mid(11, file.length() - 14); // 去掉"qtnativeui_"和".qm"
        if (!languages.contains(lang)) {
            languages << lang;
        }
    }

    return languages;
}

bool NTranslation::loadCustomTranslation(const QString& filePath) {
    // 移除现有的自定义翻译
    QCoreApplication::removeTranslator(&m_customTranslator);

    // 加载新的自定义翻译
    bool success = m_customTranslator.load(filePath);
    if (success) {
        QCoreApplication::installTranslator(&m_customTranslator);
    }

    return success;
}