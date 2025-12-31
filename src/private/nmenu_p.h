#ifndef QTNATIVEUI_NMENU_P_H
#define QTNATIVEUI_NMENU_P_H

#include <QProxyStyle>
#include <QtNativeUI/NMenu.h>

#include "QtNativeUI/NEnums.h"

class NMenuPrivate;
class NMenuPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NMenu)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightSeparatorColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSeparatorColor)

  public:
    class Style : public QProxyStyle {
      public:
        explicit Style(NMenuPrivate* parent, QStyle* style = nullptr);

        void drawControl(ControlElement      element,
                         const QStyleOption* option,
                         QPainter*           painter,
                         const QWidget*      widget = nullptr) const override;
        void drawPrimitive(PrimitiveElement    element,
                           const QStyleOption* option,
                           QPainter*           painter,
                           const QWidget*      widget = nullptr) const override;
        int  pixelMetric(PixelMetric         metric,
                         const QStyleOption* option = nullptr,
                         const QWidget*      widget = nullptr) const override;

        QSize sizeFromContents(ContentsType        type,
                               const QStyleOption* option,
                               const QSize&        size,
                               const QWidget*      widget) const override;

      private:
        NMenuPrivate* d;
    };

    explicit NMenuPrivate(QObject* parent = nullptr);
    ~NMenuPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;
    bool                  _enableDropDownAnimation{false};
    int                   _itemHeight{28};
    int                   _itemIconSize{18};
    mutable bool          _isAnyoneItemHasIcon{false};
    int                   _itemPadding{8};
    int                   _iconTextSpacing{8};
    int                   _checkmarkWidth{28};

    Style* _menuStyle{nullptr};

    QColor getCurrentColor(const QColor& lightColor, const QColor& darkColor) const {
        return _isDark ? darkColor : lightColor;
    }
};

#endif // QTNATIVEUI_NMENU_P_H