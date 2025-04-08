#ifndef QTNATIVEUI_NTEXTINPUTSTYLE_H
#define QTNATIVEUI_NTEXTINPUTSTYLE_H

#include <QProxyStyle>

class QWidget;
class QPainter;
class QStyleOption;

class NEditStyleInterface {
  public:
    virtual ~NEditStyleInterface() {}

    virtual QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const = 0;

    virtual QColor borderColorForState(bool isDark, bool isEnabled) const = 0;

    virtual QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const = 0;
    virtual QColor textColorForState(bool isDark, bool isEnabled) const                      = 0;

    virtual int borderRadius() const                 = 0;
    virtual int borderWidth() const                  = 0;
    virtual int bottomLineWidth(bool hasFocus) const = 0;

    virtual bool isDarkMode() const = 0;

    virtual int shadowBorderWidth() const = 0;
};

class NEditStyle : public QProxyStyle {
  public:
    NEditStyle(const NEditStyleInterface* styleInterface, QStyle* style = nullptr);

    void drawPrimitive(PrimitiveElement    element,
                       const QStyleOption* option,
                       QPainter*           painter,
                       const QWidget*      widget = nullptr) const override;

    int pixelMetric(PixelMetric         metric,
                    const QStyleOption* option = nullptr,
                    const QWidget*      widget = nullptr) const override;

  private:
    const NEditStyleInterface* m_styleInterface;
};

#endif // QTNATIVEUI_NTEXTINPUTSTYLE_H