//
// Created by Yang1206 on 2025/12/30.
//

#ifndef NTREEVIEWSTYLE_P_H
#define NTREEVIEWSTYLE_P_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class NTreeViewStyle : public QProxyStyle {
    Q_OBJECT

  public:
    explicit NTreeViewStyle(QStyle* style = nullptr);
    ~NTreeViewStyle();

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
                           const QWidget*      widget = nullptr) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget = nullptr) const override;

    void setBackgroundColor(const QColor& color);
    void setBorderColor(const QColor& color);
    void setBorderRadius(int radius);
    void setItemHoverColor(const QColor& color);
    void setItemSelectedColor(const QColor& color);
    void setItemPressedColor(const QColor& color);
    void setTextColor(const QColor& color);
    void setAccentColor(const QColor& color);
    void setItemHeight(int height);
    void setItemBorderRadius(int radius);
    void setIndentation(int indentation);
    void setLeftPadding(int padding);
    void setHeaderMargin(int margin);

  private:
    QColor _backgroundColor;
    QColor _borderColor;
    int    _borderRadius{8};
    QColor _itemHoverColor;
    QColor _itemSelectedColor;
    QColor _itemPressedColor;
    QColor _textColor;
    QColor _accentColor;
    int    _itemHeight{32};
    int    _itemBorderRadius{4};
    int    _indentation{20};
    int    _leftPadding{10};
    int    _headerMargin{5};
};

#endif // NTREEVIEWSTYLE_P_H
