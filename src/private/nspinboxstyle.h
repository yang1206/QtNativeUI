#ifndef NSPINBOXSTYLE_H
#define NSPINBOXSTYLE_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class NSpinBoxStyleInterface {
  public:
    virtual ~NSpinBoxStyleInterface() {}

    // 文本框部分颜色
    virtual QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const = 0;
    virtual QColor borderColorForState(bool isDark, bool isEnabled) const                                   = 0;
    virtual QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const                = 0;

    // 按钮部分颜色
    virtual QColor buttonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const = 0;

    virtual int  borderRadius() const                 = 0;
    virtual int  borderWidth() const                  = 0;
    virtual int  bottomLineWidth(bool hasFocus) const = 0;
    virtual bool isDarkMode() const                   = 0;
    virtual int  shadowBorderWidth() const            = 0;
};

class NSpinBoxStyle : public QProxyStyle {
  public:
    NSpinBoxStyle(const NSpinBoxStyleInterface* styleInterface, QStyle* style = nullptr);
    ~NSpinBoxStyle() {}

    void drawComplexControl(ComplexControl             control,
                            const QStyleOptionComplex* option,
                            QPainter*                  painter,
                            const QWidget*             widget = nullptr) const override;

    QRect subControlRect(ComplexControl             cc,
                         const QStyleOptionComplex* opt,
                         SubControl                 sc,
                         const QWidget*             widget = nullptr) const override;

    int pixelMetric(PixelMetric         metric,
                    const QStyleOption* option = nullptr,
                    const QWidget*      widget = nullptr) const override;

  private:
    const NSpinBoxStyleInterface* m_styleInterface;
};

#endif // NSPINBOXSTYLE_H