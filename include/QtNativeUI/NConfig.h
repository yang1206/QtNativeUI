
#ifndef QTNATIVEUI_NCONFIG_H
#define QTNATIVEUI_NCONFIG_H

#include <QApplication>
#include <QPointer>
#include "singleton.h"
#include "stdafx.h"

class QTNATIVEUI_EXPORT NConfig : public QObject {
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NConfig)

  public:
    void initialize();

  private:
    explicit NConfig(QObject* parent = nullptr);
    ~NConfig();
};

#define nConfig NConfig::getInstance()

#endif // QTNATIVEUI_NCONFIG_H