#ifndef QTNATIVEUI_NCALENDARWIDGETSTYLE_H
#define QTNATIVEUI_NCALENDARWIDGETSTYLE_H

#include <QProxyStyle>

class QWidget;
class QPainter;
class QStyleOption;

class NCalendarWidgetStyleInterface {
  public:
    virtual ~NCalendarWidgetStyleInterface() {}

    virtual QColor backgroundColorForState(bool isDark) const                                      = 0;
    virtual QColor headerBackgroundColorForState(bool isDark) const                                = 0;
    virtual QColor dateTextColorForState(bool isDark, bool isEnabled) const                        = 0;
    virtual QColor dateBackgroundColorForState(bool isDark, bool isSelected, bool isHovered) const = 0;
    virtual QColor headerTextColorForState(bool isDark) const                                      = 0;
    virtual QColor weekdayTextColorForState(bool isDark) const                                     = 0;
    virtual QColor weekendTextColorForState(bool isDark) const                                     = 0;
    virtual QColor todayBackgroundColorForState(bool isDark) const                                 = 0;
    virtual int    borderRadius() const                                                            = 0;
    virtual bool   isDarkMode() const                                                              = 0;
};

class NCalendarWidgetStyle : public QProxyStyle {
  public:
    NCalendarWidgetStyle(const NCalendarWidgetStyleInterface* styleInterface, QStyle* style = nullptr);

    void drawPrimitive(PrimitiveElement    element,
                       const QStyleOption* option,
                       QPainter*           painter,
                       const QWidget*      widget = nullptr) const override;

    void drawControl(ControlElement      element,
                     const QStyleOption* option,
                     QPainter*           painter,
                     const QWidget*      widget = nullptr) const override;

    void drawComplexControl(ComplexControl             control,
                            const QStyleOptionComplex* option,
                            QPainter*                  painter,
                            const QWidget*             widget = nullptr) const override;

    QSize sizeFromContents(ContentsType        type,
                           const QStyleOption* option,
                           const QSize&        size,
                           const QWidget*      widget = nullptr) const override;

  private:
    const NCalendarWidgetStyleInterface* m_styleInterface;
};

#endif // QTNATIVEUI_NCALENDARWIDGETSTYLE_H