#ifndef NNAVIGATIONBARPRIVATE_H
#define NNAVIGATIONBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPointer>
#include "nnavigationnode_p.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NAutoSuggestBox;
class NMenu;
class NBaseListView;
class QLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class NNavigationBar;
class NNavigationNode;
class NNavigationModel;
class NNavigationTreeView;

class QListView;
class NNavigationFooterModel;
class NNavigationFooterDelegate;
class NToolButton;
class NNavigationRouter;
class QPropertyAnimation;

class NNavigationBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NNavigationBar)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE_D(bool, IsSearchVisible)

    Q_PROPERTY_CREATE(int, NavigationViewWidth);
    Q_PROPERTY_CREATE(int, PaneAnimatedWidth);

  public:
    explicit NNavigationBarPrivate(QObject* parent = nullptr);
    ~NNavigationBarPrivate() override;

    Q_SLOT void onNavigationButtonClicked();

    // 核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void syncSelection(const QString& key);
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    NThemeType::ThemeMode             _themeMode;
    QMap<QString, QString>            _suggestKeyMap;
    NNavigationRouter* _router{nullptr};
    QVBoxLayout*                      _navigationLayout{nullptr};

    QPointer<QWidget>          _headerWidget{nullptr};
    QVBoxLayout*               _headerLayout{nullptr};
    NToolButton*               _navigationButton{nullptr};
    NToolButton*               _searchButton{nullptr};
    NNavigationModel*          _navigationModel{nullptr};
    NNavigationTreeView*       _navigationView{nullptr};
    NBaseListView*             _footerView{nullptr};
    NNavigationFooterModel*    _footerModel{nullptr};
    NNavigationFooterDelegate* _footerDelegate{nullptr};
    NAutoSuggestBox*           _navigationSuggestBox{nullptr};

    QStringList _lastExpandedNodeKeys;

    NNavigationType::NavigationDisplayMode _currentDisplayMode{NNavigationType::NavigationDisplayMode::Maximal};

    QPointer<NNavigationNode> _displayedNode;
    QPropertyAnimation* _widthAnimation{nullptr};
    QPropertyAnimation* _viewWidthAnimation{nullptr};

    QVBoxLayout* _navigationButtonLayout{nullptr};  // 导航按钮布局
    QVBoxLayout* _navigationSuggestLayout{nullptr}; // 搜索框布局

    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _expandOrCollpaseExpanderNode(NNavigationNode* node, bool isExpand);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);
    void _raiseNavigationBar();
    void _showSearchAndFocus();
    void _showCompactMenu(NNavigationNode* node);

    void _doComponentAnimation(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave);

    void _doNavigationBarWidthAnimation(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _doNavigationViewWidthAnimation(bool isAnimation);
};

#endif // NNAVIGATIONBARPRIVATE_H
