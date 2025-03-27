#include "QtNativeUI/NConfig.h"

#include <QStyle>
#include <QStyleFactory>
#include <QSysInfo>
Q_SINGLETON_CREATE_CPP(NConfig)

NConfig::NConfig(QObject* parent) : QObject(parent) {
    // 默认所有平台使用自定义样式
    m_platformDefaultStyles[Windows] = false;
    m_platformDefaultStyles[MacOS]   = false;
    m_platformDefaultStyles[Linux]   = false;
}

NConfig::~NConfig() = default;

void NConfig::initialize(QApplication* app) {
    m_app = app;
#ifdef Q_OS_WIN
    app->setStyle("Fusion");
#endif
}

void NConfig::useDefaultStyleOnPlatform(Platform platform, bool use) {
    if (!m_platformDefaultStyles.contains(platform)) {
        return;
    }

    if (m_platformDefaultStyles[platform] != use) {
        m_platformDefaultStyles[platform] = use;

        // 如果是当前平台，立即应用样式
        if (platform == currentPlatform() && m_app) {
            if (use) {
                // 恢复 Qt 默认样式
                QApplication::setStyle(QStyleFactory::create(m_app->style()->objectName()));
            }
            emit platformStyleChanged(platform);
        }
    }
}

bool NConfig::isUsingDefaultStyle() const { return isUsingDefaultStyle(currentPlatform()); }

bool NConfig::isUsingDefaultStyle(Platform platform) const { return m_platformDefaultStyles.value(platform, false); }

NConfig::Platform NConfig::currentPlatform() {
    const QString platform = QSysInfo::productType();

    if (platform == "windows") {
        return Windows;
    } else if (platform == "macos") {
        return MacOS;
    } else if (platform == "linux") {
        return Linux;
    }

    return Unknown;
}