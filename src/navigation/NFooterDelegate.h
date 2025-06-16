#ifndef NFOOTERDELEGATE_H
#define NFOOTERDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>
#include "../private/nbaselistview.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NNavigationNode;
class QPropertyAnimation;

class NFooterDelegate : public QStyledItemDelegate {
    Q_OBJECT
    Q_PRIVATE_CREATE(NBaseListView*, ListView)
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)

  public:
    explicit NFooterDelegate(QObject* parent = nullptr);
    ~NFooterDelegate();

    void navigationNodeStateChange(QVariantMap data);

  protected:
    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  private:
    int                   _leftPadding{11};
    int                   _iconAreaWidth{40};
    int                   _textRightSpacing{3};
    int                   _indicatorIconAreaWidth{24};
    qreal                 _lastSelectMarkTop{10};
    qreal                 _lastSelectMarkBottom{10};
    qreal                 _selectMarkTop{10};
    qreal                 _selectMarkBottom{10};
    bool                  _isSelectMarkDisplay{true};
    NNavigationNode*      _lastSelectedNode{nullptr};
    NThemeType::ThemeMode _themeMode{NThemeType::Light};
    QPropertyAnimation*   _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation*   _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation*   _selectMarkTopAnimation{nullptr};
    QPropertyAnimation*   _selectMarkBottomAnimation{nullptr};
    bool                  _compareItemY(NNavigationNode* node1, NNavigationNode* node2);
};

#endif // NFOOTERDELEGATE_H