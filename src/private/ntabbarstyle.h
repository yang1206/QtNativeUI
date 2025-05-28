#ifndef QTNATIVEUI_NTABBARSTYLE_H
#define QTNATIVEUI_NTABBARSTYLE_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class NTabBarStyle : public QProxyStyle {
    Q_OBJECT

  public:
    explicit NTabBarStyle(QStyle* style = nullptr);
    ~NTabBarStyle() override;

    void drawPrimitive(PrimitiveElement    element,
                       const QStyleOption* option,
                       QPainter*           painter,
                       const QWidget*      widget = nullptr) const override;

    void drawControl(ControlElement      element,
                     const QStyleOption* option,
                     QPainter*           painter,
                     const QWidget*      widget = nullptr) const override;

    QSize sizeFromContents(ContentsType        type,
                           const QStyleOption* option,
                           const QSize&        size,
                           const QWidget*      widget) const override;

    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;
};

#endif // QTNATIVEUI_NTABBARSTYLE_H