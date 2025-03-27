
#ifndef QTNATIVEUI_NCONFIG_H
#define QTNATIVEUI_NCONFIG_H

#include <QApplication>

#include "singleton.h"
#include "stdafx.h"

class QTNATIVEUI_EXPORT NConfig : public QObject {
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NConfig)

  public:
    enum Platform { Windows, MacOS, Linux, Unknown };
    Q_ENUM(Platform)

    void initialize(QApplication* app);

    void useDefaultStyleOnPlatform(Platform platform, bool use);

    bool isUsingDefaultStyle() const;

    bool isUsingDefaultStyle(Platform platform) const;

    static Platform currentPlatform();

  signals:
    void platformStyleChanged(Platform platform);

  private:
    explicit NConfig(QObject* parent = nullptr);
    ~NConfig();

    QApplication*         m_app{nullptr};
    QHash<Platform, bool> m_platformDefaultStyles;
};

#define nConfig NConfig::getInstance()

#endif // QTNATIVEUI_NCONFIG_H