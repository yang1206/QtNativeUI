#ifndef QTNATIVEUI_NTABBARSTYLE_H
#define QTNATIVEUI_NTABBARSTYLE_H

#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"

class NTabBarStyle : public QProxyStyle {
    Q_OBJECT

  public:
    explicit NTabBarStyle(QStyle* style = nullptr);
    ~NTabBarStyle();

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

  private:
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;
    int                   _topRadius = 7;
    int                   _margin    = 9;

    // 辅助函数：绘制特殊形状的Tab路径
    QPainterPath getTabPath(const QRect& tabRect) const;
};

#endif // QTNATIVEUI_NTABBARSTYLE_H