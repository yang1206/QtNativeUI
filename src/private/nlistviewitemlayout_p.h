#ifndef NLISTVIEWITEMLAYOUT_P_H
#define NLISTVIEWITEMLAYOUT_P_H

#include <QModelIndex>
#include <QRect>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NListView.h"

namespace NListViewItemLayout {

constexpr int kDefaultSectionHeaderHeight = 28;

inline int sectionHeaderHeight(const NListView* view) {
    if (!view)
        return kDefaultSectionHeaderHeight;
    return qMax(0, view->getSectionHeaderHeight());
}

inline bool rowStartsSection(const NListView* view, const QModelIndex& index) {
    if (!view || !view->getSectionsEnabled() || !index.isValid())
        return false;
    const QString section = index.data(NListViewType::SectionRole).toString();
    if (section.isEmpty())
        return false;
    if (index.row() == 0)
        return true;
    const QModelIndex previous = index.sibling(index.row() - 1, index.column());
    return previous.data(NListViewType::SectionRole).toString() != section;
}

inline QRect rowContentRect(const NListView* view, const QModelIndex& index, const QRect& visualRect) {
    if (!rowStartsSection(view, index))
        return visualRect;
    return visualRect.adjusted(0, sectionHeaderHeight(view), 0, 0);
}

inline QRect rowBackgroundRect(const QRect& contentRect) { return contentRect.adjusted(2, 2, -2, -2); }

} // namespace NListViewItemLayout

#endif
