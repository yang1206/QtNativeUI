//
// Created by Yang1206 on 2025/12/29.
//

#ifndef NLISTVIEWSTYLE_P_H
#define NLISTVIEWSTYLE_P_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class NListViewStyle : public QProxyStyle {
    Q_OBJECT

  public:
    explicit NListViewStyle(QStyle* style = nullptr);
    ~NListViewStyle();

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
    void setLeftPadding(int padding);

  private:
    QColor _backgroundColor;
    QColor _borderColor;
    int    _borderRadius{8};
    QColor _itemHoverColor;
    QColor _itemSelectedColor;
    QColor _itemPressedColor;
    QColor _textColor;
    QColor _accentColor;
    int    _itemHeight{36};
    int    _itemBorderRadius{4};
    int    _leftPadding{12};
};

#endif // NLISTVIEWSTYLE_P_H
