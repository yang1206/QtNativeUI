//
// Created by Yang1206 on 2025/12/30.
//

#ifndef NTREEVIEWSTYLE_P_H
#define NTREEVIEWSTYLE_P_H

#include <QProxyStyle>

class NTreeViewStyle : public QProxyStyle {
    Q_OBJECT

  public:
    explicit NTreeViewStyle(QStyle* style = nullptr);
    ~NTreeViewStyle() override;

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
    void setItemHeight(int height);
    void setBorderVisible(bool visible);
    void setBackgroundVisible(bool visible);

  private:
    bool   m_borderVisible{true};
    bool   m_backgroundVisible{true};
    QColor _backgroundColor;
    QColor _borderColor;
    int    _borderRadius{8};
    int    _itemHeight{32};
};

#endif // NTREEVIEWSTYLE_P_H
