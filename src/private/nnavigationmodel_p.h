#ifndef NNAVIGATIONMODEL_H
#define NNAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"
#include "nnavigationnode_p.h"

class NNavigationModel : public QAbstractItemModel {
    Q_OBJECT
    Q_PRIVATE_CREATE(NNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE(NNavigationNode*, SelectedExpandedNode)

  public:
    explicit NNavigationModel(QObject* parent = nullptr);
    ~NNavigationModel() override;

    [[nodiscard]] QModelIndex parent(const QModelIndex& child) const override;
    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] int         rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] int         columnCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant    data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addExpanderNode(QString                expanderTitle,
                                                           QString&               expanderKey,
                                                           QString                targetExpanderKey,
                                                           NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QString& pageKey, NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QString& pageKey, int keyPoints, NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageNode(QString                pageTitle,
                                                       QString&               pageKey,
                                                       QString                targetExpanderKey,
                                                       int                    keyPoints,
                                                       NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QString& pageKey, int keyPoints, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageNode(QString               pageTitle,
                                                       QString&              pageKey,
                                                       QString               targetExpanderKey,
                                                       int                   keyPoints,
                                                       NFilledIconType::Icon icon);

    QStringList removeNavigationNode(QString nodeKey);

    NNavigationNode*        getNavigationNode(QString nodeKey) const;
    QList<NNavigationNode*> getRootExpanderNodes() const;
    QList<NNavigationNode*> getRootExpandedNodes() const;

  private:
    QMap<QString, NNavigationNode*> _nodesMap;
    NNavigationNode*                _rootNode{nullptr};
};

#endif // NNAVIGATIONMODEL_H