#include "nlistviewreorder_p.h"

#include <QAbstractItemModel>
#include <QStandardItemModel>

namespace NListViewReorder {

bool moveRow(QAbstractItemModel* model, int sourceRow, int insertBefore) {
    if (!model)
        return false;
    if (sourceRow < 0 || sourceRow >= model->rowCount())
        return false;
    if (insertBefore < 0 || insertBefore > model->rowCount())
        return false;
    if (insertBefore == sourceRow || insertBefore == sourceRow + 1)
        return false;
    auto* standardModel = qobject_cast<QStandardItemModel*>(model);
    if (standardModel) {
        const QList<QStandardItem*> taken = standardModel->takeRow(sourceRow);
        int dest = insertBefore;
        if (dest > sourceRow)
            dest--;
        standardModel->insertRow(dest, taken);
        return true;
    }
    int destRow = insertBefore;
    if (destRow > sourceRow)
        destRow--;
    return model->moveRow(QModelIndex(), sourceRow, QModelIndex(), destRow);
}

} // namespace NListViewReorder
