#ifndef NTRANSLATION_H
#define NTRANSLATION_H

#include <QObject>
#include <QTranslator>
#include "QtNativeUI/singleton.h"

class NTranslation : public QObject {
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NTranslation)

public:
    bool setLanguage(const QString& locale);
    QString currentLanguage() const;
    QStringList availableLanguages() const;
    bool loadCustomTranslation(const QString& filePath);

private:
    explicit NTranslation(QObject* parent = nullptr);
    ~NTranslation();
    
    bool tryLoadEnglishFallback();
    
    QTranslator m_translator;
    QTranslator m_customTranslator;
    QString m_currentLanguage;
};

#define nTranslation NTranslation::getInstance()

#endif // NTRANSLATION_H