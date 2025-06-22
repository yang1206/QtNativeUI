#ifndef NNavigationFooterModel_H
#define NNavigationFooterModel_H

#include <QAbstractListModel>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"
#include "nnavigationnode_p.h"

class NNavigationFooterModel : public QAbstractListModel {
    Q_OBJECT
    Q_PRIVATE_CREATE(NNavigationNode*, SelectedNode)

  public:
    explicit NNavigationFooterModel(QObject* parent = nullptr);
    ~NNavigationFooterModel();

    NNavigationType::NodeOperateReturnType addFooterNode(QString                footerTitle,
                                                         QString&               footerKey,
                                                         bool                   isHasFooterPage,
                                                         int                    keyPoints = 0,
                                                         NRegularIconType::Icon icon      = NRegularIconType::None);

    NNavigationType::NodeOperateReturnType addFooterNode(QString               footerTitle,
                                                         QString&              footerKey,
                                                         bool                  isHasFooterPage,
                                                         int                   keyPoints = 0,
                                                         NFilledIconType::Icon icon      = NFilledIconType::None);

    int              getFooterNodeCount() const;
    NNavigationNode* getNavigationNode(QString footerKey);
    void             removeNavigationNode(QString footerKey);

  protected:
    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

  private:
    QList<NNavigationNode*> _footerNodeList;
};

#endif // NNavigationFooterModel_H