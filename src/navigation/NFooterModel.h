#ifndef NFOOTERMODEL_H
#define NFOOTERMODEL_H

#include <QAbstractListModel>
#include "NNavigationNode.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NFooterModel : public QAbstractListModel {
    Q_OBJECT
    Q_PRIVATE_CREATE(NNavigationNode*, SelectedNode)

  public:
    explicit NFooterModel(QObject* parent = nullptr);
    ~NFooterModel();

    NNavigationType::NodeOperateReturnType
    addFooterNode(QString                footerTitle,
                  QString&               footerKey,
                  bool                   isHasFooterPage,
                  int                    keyPoints = 0,
                  NRegularIconType::Icon icon      = NRegularIconType::None);

    int              getFooterNodeCount() const;
    NNavigationNode* getNavigationNode(QString footerKey);
    void             removeNavigationNode(QString footerKey);

  protected:
    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

  private:
    QList<NNavigationNode*> _footerNodeList;
};

#endif // NFOOTERMODEL_H