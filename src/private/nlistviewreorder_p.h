#ifndef NLISTVIEWREORDER_P_H
#define NLISTVIEWREORDER_P_H

class QAbstractItemModel;

namespace NListViewReorder {

bool moveRow(QAbstractItemModel* model, int sourceRow, int insertBefore);

} // namespace NListViewReorder

#endif
