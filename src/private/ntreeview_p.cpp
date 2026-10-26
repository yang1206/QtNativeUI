//
// Created by Yang1206 on 2025/12/30.
//

#include "ntreeview_p.h"

#include <QEasingCurve>
#include <QItemSelectionModel>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>
#include <QVariantAnimation>
#include <QtMath>

#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NTheme.h"
#include "ntreeitemdelegate_p.h"
#include "ntreecheckcontroller_p.h"

namespace {
constexpr qreal kTreeMarkWidth  = 3.0;
constexpr qreal kTreeMarkHeight = 16.0;
constexpr qreal kTreeMarkInset  = 4.0;
constexpr int   kExpandRevealMs = 150;
} // namespace

NTreeViewPrivate::NTreeViewPrivate(QObject* parent) : QObject(parent) {}

NTreeViewPrivate::~NTreeViewPrivate() = default;

QColor NTreeViewPrivate::itemHoverColor() const { return isDark ? _pDarkItemHoverColor : _pLightItemHoverColor; }

QColor NTreeViewPrivate::itemSelectedColor() const {
    return isDark ? _pDarkItemSelectedColor : _pLightItemSelectedColor;
}

QColor NTreeViewPrivate::itemPressedColor() const {
    return isDark ? _pDarkItemPressedColor : _pLightItemPressedColor;
}

QColor NTreeViewPrivate::textColor() const { return isDark ? _pDarkTextColor : _pLightTextColor; }

QColor NTreeViewPrivate::placeholderTextColor() const {
    return isDark ? _pDarkPlaceholderTextColor : _pLightPlaceholderTextColor;
}

QColor NTreeViewPrivate::backgroundColor() const {
    return isDark ? _pDarkBackgroundColor : _pLightBackgroundColor;
}

QColor NTreeViewPrivate::borderColor() const { return isDark ? _pDarkBorderColor : _pLightBorderColor; }

QRectF NTreeViewPrivate::selectionMarkRect(const QModelIndex& index) const {
    Q_Q(const NTreeView);
    if (modelChangeDepth > 0 || !index.isValid() || index.model() != q->model() || !q->viewport())
        return {};
    const QRect itemRect = q->visualRect(index);
    if (itemRect.isEmpty())
        return {};
    const QRectF bg(2, itemRect.top() + 2, q->viewport()->width() - 4, itemRect.height() - 4);
    return QRectF(itemRect.left() + kTreeMarkInset,
                  bg.center().y() - kTreeMarkHeight / 2.0,
                  kTreeMarkWidth,
                  kTreeMarkHeight).intersected(QRectF(revealClipRect(index)));
}

bool NTreeViewPrivate::usesCheckState(const QModelIndex& index) const {
    Q_Q(const NTreeView);
    return q->selectionMode() != QAbstractItemView::SingleSelection
           && q->selectionMode() != QAbstractItemView::NoSelection && index.isValid()
           && (index.siblingAtColumn(0).flags() & Qt::ItemIsUserCheckable);
}

bool NTreeViewPrivate::isItemSelected(const QModelIndex& index) const {
    Q_Q(const NTreeView);
    if (q->selectionMode() == QAbstractItemView::NoSelection || !index.isValid())
        return false;
    if (usesCheckState(index))
        return index.siblingAtColumn(0).data(Qt::CheckStateRole).toInt() == Qt::Checked;
    return q->selectionModel() && q->selectionModel()->model() == q->model()
           && q->selectionModel()->isSelected(index);
}

QRect NTreeViewPrivate::revealClipRect(const QModelIndex& index) const {
    Q_Q(const NTreeView);
    QRect clip = q->viewport()->rect();
    if (isRevealRunning() && isDescendantOf(index, animParent)) {
        const QRect parentRect = q->visualRect(animParent);
        const qreal progress = qBound(0.0, expandRevealAnim->currentValue().toReal(), 1.0);
        clip.setTop(qMax(clip.top(), parentRect.bottom() + 1));
        clip.setBottom(qMin(clip.bottom(), qFloor(parentRect.bottom() + animSubtreeHeight * progress)));
    }
    return clip.isValid() ? clip : QRect();
}

void NTreeViewPrivate::paintRowBackground(QPainter* painter, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    Q_Q(const NTreeView);
    if (!q->isEnabled() || !(index.flags() & Qt::ItemIsEnabled))
        return;
    const bool selected = isItemSelected(index);
    const bool pressed = pressedIndex.isValid()
                         && pressedIndex.sibling(pressedIndex.row(), 0) == index.siblingAtColumn(0);
    QColor color(Qt::transparent);
    if (pressed)
        color = itemPressedColor();
    else if (selected)
        color = itemSelectedColor();
    else if ((option.state & QStyle::State_MouseOver)
             || (hoveredIndex.isValid() && hoveredIndex == index.siblingAtColumn(0)))
        color = itemHoverColor();
    if (color.alpha() == 0)
        return;
    QPainterPath path;
    path.addRoundedRect(QRectF(2, option.rect.top() + 2, q->viewport()->width() - 4, option.rect.height() - 4),
                        _pItemBorderRadius, _pItemBorderRadius);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(path, color);
    painter->restore();
}

bool NTreeViewPrivate::affectsReveal(const QModelIndex& parent, int first, int last, bool removing) const {
    if (!animParent.isValid())
        return false;
    if (parent == animParent || isDescendantOf(parent, animParent))
        return true;
    if (!removing)
        return false;
    QModelIndex branch(animParent);
    while (branch.isValid() && branch.parent() != parent)
        branch = branch.parent();
    return branch.isValid() && branch.row() >= first && branch.row() <= last;
}

void NTreeViewPrivate::clearSelectionMark() {
    if (selectMark)
        selectMark->clear();
    currentIndicatorIndex = QModelIndex();
}

void NTreeViewPrivate::clearTransientState() {
    pressedIndex = QModelIndex();
    hoveredIndex = QModelIndex();
    ignoreRevealRelease = false;
    clearSelectionMark();
    clearExpandRevealState();
}

void NTreeViewPrivate::updateSelectionMark() {
    Q_Q(NTreeView);
    if (modelChangeDepth > 0)
        return;
    const auto* selection = q->selectionModel();
    if (!selectMark || !_pSelectionIndicatorVisible
        || q->selectionMode() != QAbstractItemView::SingleSelection || !selection
        || selection->model() != q->model()) {
        clearSelectionMark();
        return;
    }
    QPersistentModelIndex current(q->currentIndex());
    if (current.isValid() && q->selectionBehavior() == QAbstractItemView::SelectRows)
        current = current.sibling(current.row(), 0);
    if (!current.isValid() || !selection->isSelected(current)) {
        const QModelIndexList rows = selection->selectedIndexes();
        current = rows.isEmpty() ? QModelIndex() : rows.first();
        if (current.isValid() && q->selectionBehavior() == QAbstractItemView::SelectRows)
            current = current.sibling(current.row(), 0);
    }
    if (!current.isValid()) {
        clearSelectionMark();
        return;
    }
    // 滚动、重复选择通知只改变几何位置，不应重启或打断正在运行的选中动画。
    if (currentIndicatorIndex == current)
        return;
    // 两端都必须持有持久索引，普通 QModelIndex 的 isValid() 无法识别已被模型删除的项。
    const QPersistentModelIndex previous(currentIndicatorIndex);
    auto currentRect = [this, current]() { return selectionMarkRect(current); };
    const QRectF prevRect = selectionMarkRect(previous);
    const QRectF curRect  = selectionMarkRect(current);
    if (!prevRect.isEmpty() && !curRect.isEmpty()) {
        const auto direction  = prevRect.center().y() < curRect.center().y()
                                   ? NSelectionIndicatorMotion::Direction::Down
                                   : NSelectionIndicatorMotion::Direction::Up;
        auto previousRectFn = [this, previous]() { return selectionMarkRect(previous); };
        selectMark->transition(previousRectFn, currentRect, direction);
    } else {
        selectMark->showAt(currentRect);
    }
    currentIndicatorIndex = current;
}

void NTreeViewPrivate::paintSelectionMark(QPainter* painter) const {
    Q_Q(const NTreeView);
    if (!painter || modelChangeDepth > 0 || !_pSelectionIndicatorVisible
        || q->selectionMode() == QAbstractItemView::NoSelection)
        return;
    const QColor accent = nTheme->accentColor().normal();
    if (q->selectionMode() == QAbstractItemView::SingleSelection) {
        if (!selectMark)
            return;
        const QRectF mark = selectMark->currentRect();
        if (!mark.isEmpty())
            NSelectionIndicator::paintMark(painter, mark, accent);
        return;
    }
    const auto* selection = q->selectionModel();
    if (!selection || selection->model() != q->model() || q->isColumnHidden(0))
        return;
    // 只检查视口内的行，避免多选大量节点时每帧生成全部选中单元格列表。
    QModelIndex index = q->QTreeView::indexAt(QPoint(0, 0));
    if (index.isValid())
        index = index.siblingAtColumn(0);
    while (index.isValid()) {
        const QRect rowRect = q->visualRect(index);
        if (rowRect.top() >= q->viewport()->height())
            break;
        if (isItemSelected(index)) {
            const QRectF mark = selectionMarkRect(index);
            if (!mark.isEmpty())
                NSelectionIndicator::paintMark(painter, mark, accent);
        }
        index = q->indexBelow(index);
    }
}

void NTreeViewPrivate::updateHeaderStyle() {
    if (!headerLabel)
        return;
    const QColor color =
        NThemeColor(NFluentColorKey::TextFillColorSecondary, isDark ? NThemeType::Dark : NThemeType::Light);
    QFont font = headerLabel->font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
    font.setWeight(QFont::DemiBold);
    headerLabel->setFont(font);
    headerLabel->setStyleSheet(QStringLiteral("background:transparent;color:%1;").arg(color.name()));
}

void NTreeViewPrivate::layoutHeader() {
    Q_Q(NTreeView);
    constexpr int kSideMargin   = 4;
    constexpr int kHeaderHeight = 28;
    int           topMargin     = kSideMargin;
    const int     innerWidth    = qMax(0, q->width() - kSideMargin * 2);
    if (headerLabel) {
        if (_pHeaderText.isEmpty()) {
            headerLabel->hide();
        } else {
            headerLabel->setText(_pHeaderText);
            headerLabel->setGeometry(kSideMargin, kSideMargin, innerWidth, kHeaderHeight);
            headerLabel->show();
            topMargin += kHeaderHeight;
        }
    }
    q->setViewportMargins(kSideMargin, topMargin, kSideMargin, kSideMargin);
}

bool NTreeViewPrivate::isDescendantOf(const QModelIndex& candidate, const QModelIndex& ancestor) const {
    if (!candidate.isValid() || !ancestor.isValid() || candidate.model() != ancestor.model())
        return false;
    QModelIndex walk = candidate.parent();
    while (walk.isValid()) {
        if (walk == ancestor)
            return true;
        walk = walk.parent();
    }
    return false;
}

qreal NTreeViewPrivate::computeSubtreeHeight(const QModelIndex& parent) const {
    Q_Q(const NTreeView);
    if (!parent.isValid() || parent.model() != q->model() || !q->isExpanded(parent))
        return 0.0;
    const QRect parentRect = q->visualRect(parent);
    if (!parentRect.intersects(q->viewport()->rect()))
        return 0.0;
    const int bottom = q->viewport()->height();
    qreal height = 0.0;
    int visited = 0;
    QModelIndex walk = q->indexBelow(parent);
    // 动效只影响视口；无需为屏幕之外的成千上万行计算完整子树高度。
    while (walk.isValid() && visited++ <= bottom + 1 && isDescendantOf(walk, parent)) {
        const QRect rowRect = q->visualRect(walk);
        if (rowRect.top() >= bottom)
            break;
        height = qMax(height, qreal(qMin(rowRect.bottom() + 1, bottom) - parentRect.bottom() - 1));
        if (rowRect.bottom() + 1 >= bottom)
            break;
        walk = q->indexBelow(walk);
    }
    return height;
}

bool NTreeViewPrivate::isRevealRunning() const {
    Q_Q(const NTreeView);
    return expandRevealAnim && expandRevealAnim->state() == QAbstractAnimation::Running
           && animParent.isValid() && animParent.model() == q->model();
}

qreal NTreeViewPrivate::chevronRotation(const QModelIndex& index) const {
    Q_Q(const NTreeView);
    if (!index.isValid() || index.model() != q->model())
        return 0.0;
    if (isRevealRunning() && QModelIndex(animParent) == index)
        return qBound(0.0, expandRevealAnim->currentValue().toReal(), 1.0);
    return q->isExpanded(index) ? 1.0 : 0.0;
}

void NTreeViewPrivate::clearExpandRevealState() {
    Q_Q(NTreeView);
    if (expandRevealAnim)
        expandRevealAnim->stop();
    animParent                = QModelIndex();
    animExpanding             = true;
    animSubtreeHeight         = 0.0;
    pendingCollapseFinalize   = false;
    if (q->viewport())
        q->viewport()->update();
}

void NTreeViewPrivate::finalizeDeferredCollapse() {
    Q_Q(NTreeView);
    const QModelIndex parent = QModelIndex(animParent);
    pendingCollapseFinalize  = false;
    animParent               = QModelIndex();
    animExpanding            = true;
    animSubtreeHeight        = 0.0;
    if (parent.isValid() && parent.model() == q->model())
        q->setExpanded(parent, false);
    if (q->viewport())
        q->viewport()->update();
}

void NTreeViewPrivate::completeActiveExpandReveal() {
    if (!isRevealRunning())
        return;
    const bool shouldCollapse = !animExpanding || pendingCollapseFinalize;
    expandRevealAnim->stop();
    if (shouldCollapse) {
        finalizeDeferredCollapse();
        return;
    }
    clearExpandRevealState();
}

void NTreeViewPrivate::startExpandReveal(const QModelIndex& parent) {
    Q_Q(NTreeView);
    if (modelChangeDepth > 0 || !expandRevealAnim || !parent.isValid() || parent.model() != q->model())
        return;
    const QPersistentModelIndex target(parent);
    if (isRevealRunning() && animParent != parent)
        completeActiveExpandReveal();
    if (!target.isValid() || target.model() != q->model() || !q->isExpanded(target))
        return;
    if (!q->isVisible() || !revealAnimationsEnabled || !revealAnimationEnabled) {
        clearExpandRevealState();
        return;
    }
    const qreal resume = (animParent == target && isRevealRunning())
                             ? qBound(0.0, expandRevealAnim->currentValue().toReal(), 1.0)
                             : 0.0;
    expandRevealAnim->stop();
    animExpanding           = true;
    pendingCollapseFinalize = false;
    animParent              = target;
    animSubtreeHeight       = computeSubtreeHeight(target);
    if (animSubtreeHeight <= 0.0) {
        clearExpandRevealState();
        return;
    }
    const int duration = qMax(1, qRound(kExpandRevealMs * (1.0 - resume)));
    expandRevealAnim->setDuration(duration);
    expandRevealAnim->setStartValue(resume);
    expandRevealAnim->setEndValue(1.0);
    expandRevealAnim->start();
    if (q->viewport())
        q->viewport()->update();
}

void NTreeViewPrivate::startCollapseReveal(const QModelIndex& parent) {
    Q_Q(NTreeView);
    if (modelChangeDepth > 0 || !parent.isValid() || parent.model() != q->model())
        return;
    if (!expandRevealAnim) {
        if (parent.isValid())
            q->setExpanded(parent, false);
        return;
    }
    if (!q->isVisible() || !revealAnimationsEnabled || !revealAnimationEnabled) {
        q->setExpanded(parent, false);
        return;
    }
    const bool resuming = animParent.isValid() && QModelIndex(animParent) == parent && isRevealRunning();
    const qreal resume  = resuming ? qBound(0.0, expandRevealAnim->currentValue().toReal(), 1.0) : 1.0;
    expandRevealAnim->stop();
    animExpanding           = false;
    pendingCollapseFinalize = true;
    animParent              = parent;
    if (!resuming)
        animSubtreeHeight = computeSubtreeHeight(parent);
    if (animSubtreeHeight <= 0.0) {
        pendingCollapseFinalize = false;
        clearExpandRevealState();
        q->setExpanded(parent, false);
        return;
    }
    const int duration = qMax(1, qRound(kExpandRevealMs * resume));
    expandRevealAnim->setDuration(duration);
    expandRevealAnim->setStartValue(resume);
    expandRevealAnim->setEndValue(0.0);
    expandRevealAnim->start();
    if (q->viewport())
        q->viewport()->update();
}

void NTreeViewPrivate::toggleExpanded(const QModelIndex& index) {
    Q_Q(NTreeView);
    if (modelChangeDepth > 0 || !index.isValid() || index.model() != q->model())
        return;
    if (q->QTreeView::isAnimated())
        q->QTreeView::setAnimated(false);
    const QPersistentModelIndex target(index);
    if (animParent.isValid() && QModelIndex(animParent) == index && isRevealRunning()) {
        if (animExpanding)
            startCollapseReveal(index);
        else {
            pendingCollapseFinalize = false;
            startExpandReveal(index);
        }
        return;
    }
    if (isRevealRunning())
        completeActiveExpandReveal();
    if (!target.isValid() || target.model() != q->model())
        return;
    if (q->isExpanded(target)) {
        if (revealAnimationsEnabled && revealAnimationEnabled && q->isVisible())
            startCollapseReveal(target);
        else
            q->setExpanded(target, false);
    } else {
        q->setExpanded(target, true);
    }
}

void NTreeViewPrivate::initStyle() {
    Q_Q(NTreeView);
    style = new NTreeViewStyle();
    style->setParent(q);
    q->setStyle(style);
    checks = new NTreeCheckController([q]() {
        if (q->viewport())
            q->viewport()->update();
    }, this);
    itemDelegate = new NTreeItemDelegate(q, this, q);
    q->setItemDelegate(itemDelegate);
    selectMark = new NSelectionIndicatorSession([q]() {
        if (q->viewport())
            q->viewport()->update();
    }, q);
    headerLabel = new QLabel(q);
    headerLabel->setObjectName(QStringLiteral("NTreeViewHeader"));
    headerLabel->hide();
    expandRevealAnim = new QVariantAnimation(this);
    expandRevealAnim->setDuration(kExpandRevealMs);
    expandRevealAnim->setEasingCurve(QEasingCurve::InOutSine);
    connect(expandRevealAnim, &QVariantAnimation::valueChanged, this, [q](const QVariant&) {
        if (q->viewport())
            q->viewport()->update();
    });
    connect(expandRevealAnim, &QVariantAnimation::finished, this, [this]() {
        if (pendingCollapseFinalize) {
            finalizeDeferredCollapse();
            return;
        }
        clearExpandRevealState();
    });
    connect(q, &QTreeView::expanded, this, [this](const QModelIndex& index) {
        if (!revealAnimationsEnabled)
            return;
        startExpandReveal(index);
    });
    connect(q, &QTreeView::collapsed, this, [this](const QModelIndex& index) {
        if (animParent.isValid()
            && (animParent == index || isDescendantOf(QModelIndex(animParent), index))
            && !pendingCollapseFinalize) {
            clearExpandRevealState();
        }
    });
    updateHeaderStyle();
    layoutHeader();
    updateStyle();
}

void NTreeViewPrivate::updateStyle() {
    if (!style)
        return;
    style->setBackgroundColor(backgroundColor());
    style->setBorderColor(borderColor());
    style->setBorderRadius(_pBorderRadius);
    style->setItemHeight(_pItemHeight);
    style->setBorderVisible(_pBorderVisible);
    style->setBackgroundVisible(_pBackgroundVisible);
    updateHeaderStyle();
    Q_Q(NTreeView);
    q->update();
    if (q->viewport())
        q->viewport()->update();
}
