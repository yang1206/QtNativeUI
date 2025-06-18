#ifndef NNAVIGATIONBARPRIVATE_H
#define NNAVIGATIONBARPRIVATE_H

#include <QMap>
#include <QObject>
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
class NNavigationView;

class QListView;
class NFooterModel;
class NFooterDelegate;
class NToolButton;

class NNavigationBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NNavigationBar)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE(int, NavigationViewWidth);

  public:
    explicit NNavigationBarPrivate(QObject* parent = nullptr);
    ~NNavigationBarPrivate() override;

    Q_SLOT void      onNavigationButtonClicked();
    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

    // 核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);

  private:
    NThemeType::ThemeMode             _themeMode;
    QMap<QString, QString>            _suggestKeyMap;
    QMap<QString, const QMetaObject*> _pageMetaMap;
    QMap<NNavigationNode*, NMenu*>    _compactMenuMap;
    QVBoxLayout*                      _navigationButtonLayout{nullptr};
    QHBoxLayout*                      _navigationSuggestLayout{nullptr};

    QWidget*          _headerWidget{nullptr};
    QVBoxLayout*      _headerLayout{nullptr};
    NToolButton*      _navigationButton{nullptr};
    NToolButton*      _searchButton{nullptr};
    NNavigationModel* _navigationModel{nullptr};
    NNavigationView*  _navigationView{nullptr};
    NBaseListView*    _footerView{nullptr};
    NFooterModel*     _footerModel{nullptr};
    NFooterDelegate*  _footerDelegate{nullptr};
    NAutoSuggestBox*  _navigationSuggestBox{nullptr};

    QList<NNavigationNode*> _lastExpandedNodesList;

    NNavigationType::NavigationDisplayMode _currentDisplayMode{NNavigationType::NavigationDisplayMode::Maximal};

    void _initNodeModelIndex(const QModelIndex& parentIndex);
    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _expandOrCollpaseExpanderNode(NNavigationNode* node, bool isExpand);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);
    void _raiseNavigationBar();

    void _doComponentAnimation(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave);
    void _handleMaximalToCompactLayout();
    void _handleCompactToMaximalLayout();
    void _resetLayout();

    void _doNavigationBarWidthAnimation(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _doNavigationViewWidthAnimation(bool isAnimation);
    void _doNavigationButtonAnimation(bool isCompact, bool isAnimation);
    void _doSearchButtonAnimation(bool isCompact, bool isAnimation);
};

#endif // NNAVIGATIONBARPRIVATE_H