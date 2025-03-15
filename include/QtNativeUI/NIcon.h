#ifndef QTNATIVEUI_NICON_H
#define QTNATIVEUI_NICON_H

#include <QIcon>
#include <QtCore/qmap.h>

#include "NIconEnums.h"
#include "singleton.h"

class QTNATIVEUI_EXPORT NIcon {
    Q_SINGLETON_CREATE_H(NIcon)

  private:
    NIcon();
    ~NIcon();

    struct FontConfig {
        QString fontFamily;
        int     defaultSize;
        FontConfig(const QString& family = "", int size = 24) : fontFamily(family), defaultSize(size) {}
    };

    QMap<QString, FontConfig> fontConfigs;
    void                      initFontConfigs();

    // 通用的图标生成方法
    QIcon createIcon(const QString& fontFamily,
                     unsigned int   unicode,
                     int            size   = 24,
                     const QColor&  color  = Qt::black,
                     int            width  = 0,
                     int            height = 0);

  public:
    // Regular 图标
    QIcon fromRegular(NRegularIconType::Icon icon);
    QIcon fromRegular(NRegularIconType::Icon icon, const QColor& color);
    QIcon fromRegular(NRegularIconType::Icon icon, int size);
    QIcon fromRegular(NRegularIconType::Icon icon, int size, const QColor& color);
    QIcon fromRegular(NRegularIconType::Icon icon, int size, int width, int height);
    QIcon fromRegular(NRegularIconType::Icon icon, int size, int width, int height, const QColor& color);

    // Filled 图标
    QIcon fromFilled(NFilledIconType::Icon icon);
    QIcon fromFilled(NFilledIconType::Icon icon, const QColor& color);
    QIcon fromFilled(NFilledIconType::Icon icon, int size);
    QIcon fromFilled(NFilledIconType::Icon icon, int size, const QColor& color);
    QIcon fromFilled(NFilledIconType::Icon icon, int size, int width, int height);
    QIcon fromFilled(NFilledIconType::Icon icon, int size, int width, int height, const QColor& color);
};

#define nIcon NIcon::getInstance()

#endif // QTNATIVEUI_NICON_H