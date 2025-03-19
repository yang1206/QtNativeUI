#ifndef QTNATIVEUI_NMENU_P_H
#define QTNATIVEUI_NMENU_P_H

#include <QProxyStyle>
#include <QtNativeUI/NMenu.h>

#include "QtNativeUI/NEnums.h"

class NMenuPrivate;
class NMenuPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, LightSeparatorColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSeparatorColor)

  public:
    // 内部样式类
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

      private:
        NMenuPrivate* d; // 指向父对象的指针
    };

    explicit NMenuPrivate(QObject* parent = nullptr);
    ~NMenuPrivate();
    Q_D_CREATE(NMenu)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;
    int                   _shadowBorderWidth{5};
    int                   _itemHeight{28};
    int                   _itemPadding{12};
    int                   _iconTextSpacing{8};
    int                   _checkmarkWidth{20};

    // 自定义样式
    Style* _menuStyle{nullptr};

    // 更新主题颜色
    void updateThemeColors();
};

#endif // QTNATIVEUI_NMENU_P_H