#ifndef QTNATIVEUI_NICON_H
#define QTNATIVEUI_NICON_H

#include <QIcon>
#include <QIconEngine>
#include <QtCore/qmap.h>

#include "NIconEnums.h"
#include "singleton.h"

class NFluentIconEngine : public QIconEngine {
  public:
    NFluentIconEngine(bool isRegular, uint32_t iconCode, int size, const QColor& customColor = QColor());
    ~NFluentIconEngine() override;

    void         paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
    QIconEngine* clone() const override;
    QSize        actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) override;
    QPixmap      pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

  private:
    bool     isRegular;
    uint32_t iconCode;
    int      iconSize;
    QColor   customColor;

    QColor getEffectiveColor(QIcon::Mode mode) const;
};

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

    QIcon createIcon(const QString& fontFamily,
                     uint32_t       unicode,
                     int            size   = 24,
                     const QColor&  color  = QColor(),
                     int            width  = 0,
                     int            height = 0);

    QIcon createThemeAwareIcon(bool isRegular, uint32_t unicode, int size = 24, const QColor& customColor = QColor());

    QColor getThemeBasedColor() const;

  public:
    QIcon fromRegular(NRegularIconType::Icon icon);
    QIcon fromRegular(NRegularIconType::Icon icon, const QColor& color);
    QIcon fromRegular(NRegularIconType::Icon icon, int size);
    QIcon fromRegular(NRegularIconType::Icon icon, int size, const QColor& color);
    QIcon fromRegular(NRegularIconType::Icon icon, int size, int width, int height);
    QIcon fromRegular(NRegularIconType::Icon icon, int size, int width, int height, const QColor& color);

    QIcon fromFilled(NFilledIconType::Icon icon);
    QIcon fromFilled(NFilledIconType::Icon icon, const QColor& color);
    QIcon fromFilled(NFilledIconType::Icon icon, int size);
    QIcon fromFilled(NFilledIconType::Icon icon, int size, const QColor& color);
    QIcon fromFilled(NFilledIconType::Icon icon, int size, int width, int height);
    QIcon fromFilled(NFilledIconType::Icon icon, int size, int width, int height, const QColor& color);

    QString getRegularFontFamily() const { return fontConfigs["Regular"].fontFamily; }
    QString getFilledFontFamily() const { return fontConfigs["Filled"].fontFamily; }
    QString getIconText(uint32_t unicode);
};

#define nIcon NIcon::getInstance()

#endif // QTNATIVEUI_NICON_H