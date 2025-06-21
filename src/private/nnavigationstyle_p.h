#ifndef NNAVIGATIONSTYLE_H
#define NNAVIGATIONSTYLE_H

#include <QModelIndex>
#include <QProxyStyle>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NNavigationNode;
class NNavigationTreeView;
class QPropertyAnimation;

class NNavigationStyle : public QProxyStyle {
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Opacity)
    Q_PROPERTY_CREATE(qreal, Rotate)
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PRIVATE_CREATE(NNavigationTreeView*, NavigationView)
    Q_PROPERTY_CREATE(qreal, LastSelectMarkTop)
    Q_PROPERTY_CREATE(qreal, LastSelectMarkBottom)
    Q_PROPERTY_CREATE(qreal, SelectMarkTop)
    Q_PROPERTY_CREATE(qreal, SelectMarkBottom)
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)

  public:
    explicit NNavigationStyle(QStyle* style = nullptr);
    ~NNavigationStyle();

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

    void navigationNodeStateChange(QVariantMap data);

  private:
    NThemeType::ThemeMode _themeMode{NThemeType::Light};
    bool                  _isSelectMarkDisplay{true};
    int                   _leftPadding{11};
    int                   _iconAreaWidth{40};
    int                   _textRightSpacing{3};
    int                   _indicatorIconAreaWidth{34};
    NNavigationNode*      _opacityAnimationTargetNode{nullptr};
    NNavigationNode*      _expandAnimationTargetNode{nullptr};
    NNavigationNode*      _lastSelectedNode{nullptr};
    QPropertyAnimation*   _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation*   _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation*   _selectMarkTopAnimation{nullptr};
    QPropertyAnimation*   _selectMarkBottomAnimation{nullptr};
    bool                  _compareItemY(NNavigationNode* node1, NNavigationNode* node2);
};

#endif // NNAVIGATIONSTYLE_H