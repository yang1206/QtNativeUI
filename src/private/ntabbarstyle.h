#ifndef QTNATIVEUI_NTABBARSTYLE_H
#define QTNATIVEUI_NTABBARSTYLE_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class QStyleOptionToolButton;
void        nPaintTabToolButton(QPainter* painter, const QStyleOptionToolButton& option, const QWidget* widget);
inline bool nShouldPaintFocusVisual(QStyle::State state) {
    return state.testFlag(QStyle::State_HasFocus) && state.testFlag(QStyle::State_KeyboardFocusChange);
}

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
    int   pixelMetric(PixelMetric         metric,
                      const QStyleOption* option = nullptr,
                      const QWidget*      widget = nullptr) const override;
    int   styleHint(StyleHint           hint,
                    const QStyleOption* option     = nullptr,
                    const QWidget*      widget     = nullptr,
                    QStyleHintReturn*   returnData = nullptr) const override;
};

#endif // QTNATIVEUI_NTABBARSTYLE_H
