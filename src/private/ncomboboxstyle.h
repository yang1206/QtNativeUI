// ncomboboxstyle.h
#ifndef NCOMBOBOXSTYLE_H
#define NCOMBOBOXSTYLE_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class NComboBoxStyleInterface {
public:
    virtual ~NComboBoxStyleInterface() {}

    // 文本框部分颜色
    virtual QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const = 0;
    virtual QColor borderColorForState(bool isDark, bool isEnabled) const = 0;
    virtual QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const = 0;
    virtual QColor dropdownButtonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const = 0;
    virtual QColor textColorForState(bool isDark, bool isEnabled) const = 0;
    virtual QColor selectionBackgroundColor(bool isDark) const = 0;
    virtual QColor selectionTextColor(bool isDark) const = 0;
    virtual int    borderRadius() const = 0;
    virtual int    borderWidth() const = 0;
    virtual int    bottomLineWidth(bool hasFocus) const = 0;
    virtual bool   isDarkMode() const = 0;
    virtual int    shadowBorderWidth() const = 0;
    virtual bool   isDropdownVisible() const = 0;
};

class NComboBoxStyle : public QProxyStyle {
public:
    NComboBoxStyle(const NComboBoxStyleInterface* styleInterface, QStyle* style = nullptr);
    ~NComboBoxStyle() {}

    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption* option,
                       QPainter* painter,
                       const QWidget* widget = nullptr) const override;

    void drawComplexControl(ComplexControl control,
                            const QStyleOptionComplex* option,
                            QPainter* painter,
                            const QWidget* widget = nullptr) const override;

    QRect subControlRect(ComplexControl cc,
                         const QStyleOptionComplex* opt,
                         SubControl sc,
                         const QWidget* widget = nullptr) const override;

    SubControl hitTestComplexControl(ComplexControl control,
                                     const QStyleOptionComplex* option,
                                     const QPoint& pos,
                                     const QWidget* widget) const override;

    int pixelMetric(PixelMetric metric,
                    const QStyleOption* option = nullptr,
                    const QWidget* widget = nullptr) const override;

    QSize sizeFromContents(ContentsType type,
                          const QStyleOption* option,
                          const QSize& size,
                          const QWidget* widget) const override;

private:
    const NComboBoxStyleInterface* m_styleInterface;
};

#endif // NCOMBOBOXSTYLE_H