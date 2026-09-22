//
// Created by Yang1206 on 2025/12/30.
//

#include "ntreeview_p.h"

#include <QEasingCurve>
#include <QItemSelectionModel>
#include <QLabel>
#include <QPainter>
#include <QVariantAnimation>

#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NTheme.h"
#include "ntreeitemdelegate_p.h"

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
    if (!index.isValid() || !q->viewport())
        return {};
    const QRect itemRect = q->visualRect(index);
    if (itemRect.isEmpty())
        return {};
    const QRectF bg(2, itemRect.top() + 2, q->viewport()->width() - 4, itemRect.height() - 4);
    return QRectF(itemRect.left() + kTreeMarkInset,
                  bg.center().y() - kTreeMarkHeight / 2.0,
                  kTreeMarkWidth,
                  kTreeMarkHeight);
}

void NTreeViewPrivate::updateSelectionMark(const QModelIndex& previousHint) {
    Q_Q(NTreeView);
    if (!selectMark || !_pSelectionIndicatorVisible) {
        if (selectMark)
            selectMark->clear();
        currentIndicatorIndex = QModelIndex();
        return;
    }
    QModelIndex current;
    if (q->selectionModel()) {
        current = q->currentIndex();
        if (!current.isValid() || !q->selectionModel()->isSelected(current)) {
            const QModelIndexList rows = q->selectionModel()->selectedIndexes();
            current = rows.isEmpty() ? QModelIndex() : rows.first();
        }
    }
    if (!current.isValid()) {
        selectMark->clear();
        currentIndicatorIndex = QModelIndex();
        return;
    }
    const QModelIndex previous =
        previousHint.isValid() ? previousHint : QModelIndex(currentIndicatorIndex);
    auto currentRect = [this, current]() { return selectionMarkRect(current); };
    if (q->selectionMode() == QAbstractItemView::SingleSelection && previous.isValid()
        && previous != current && selectionMarkRect(previous).isValid()) {
        const QRectF prevRect = selectionMarkRect(previous);
        const QRectF curRect  = selectionMarkRect(current);
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
    if (!painter || !_pSelectionIndicatorVisible)
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
    if (!q->selectionModel())
        return;
    for (const QModelIndex& index : q->selectionModel()->selectedIndexes()) {
        if (index.column() != 0)
            continue;
        const QRectF mark = selectionMarkRect(index);
        if (!mark.isEmpty())
            NSelectionIndicator::paintMark(painter, mark, accent);
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
    if (!candidate.isValid() || !ancestor.isValid())
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
    if (!parent.isValid() || !q->isExpanded(parent))
        return 0.0;
    qreal height = 0.0;
    QModelIndex walk = q->indexBelow(parent);
    while (walk.isValid() && isDescendantOf(walk, parent)) {
        const QRect rowRect = q->visualRect(walk);
        height += rowRect.height() > 0 ? rowRect.height() : _pItemHeight;
        walk = q->indexBelow(walk);
    }
    return height;
}

bool NTreeViewPrivate::isRevealRunning() const {
    return expandRevealAnim && expandRevealAnim->state() == QAbstractAnimation::Running && animParent.isValid();
}

qreal NTreeViewPrivate::chevronRotation(const QModelIndex& index) const {
    Q_Q(const NTreeView);
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
    if (parent.isValid())
        q->setExpanded(parent, false);
    if (q->viewport())
        q->viewport()->update();
}

void NTreeViewPrivate::completeActiveExpandReveal() {
    Q_Q(NTreeView);
    if (!isRevealRunning())
        return;
    const bool shouldCollapse = !animExpanding || pendingCollapseFinalize;
    expandRevealAnim->stop();
    if (shouldCollapse) {
        finalizeDeferredCollapse();
        return;
    }
    clearExpandRevealState();
    if (q->viewport())
        q->viewport()->update();
}

void NTreeViewPrivate::startExpandReveal(const QModelIndex& parent) {
    Q_Q(NTreeView);
    if (!expandRevealAnim || !parent.isValid())
        return;
    if (!q->isVisible() || !revealAnimationsEnabled) {
        clearExpandRevealState();
        return;
    }
    const qreal resume = (animParent.isValid() && QModelIndex(animParent) == parent && isRevealRunning())
                             ? qBound(0.0, expandRevealAnim->currentValue().toReal(), 1.0)
                             : 0.0;
    expandRevealAnim->stop();
    animExpanding           = true;
    pendingCollapseFinalize = false;
    animParent              = parent;
    animSubtreeHeight       = computeSubtreeHeight(parent);
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
    if (!expandRevealAnim || !parent.isValid()) {
        if (parent.isValid())
            q->setExpanded(parent, false);
        return;
    }
    if (!revealAnimationsEnabled) {
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
    if (!index.isValid())
        return;
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
    if (q->isExpanded(index)) {
        if (revealAnimationsEnabled && q->isVisible() && computeSubtreeHeight(index) > 0.0)
            startCollapseReveal(index);
        else
            q->setExpanded(index, false);
    } else {
        q->setExpanded(index, true);
    }
}

void NTreeViewPrivate::initStyle() {
    Q_Q(NTreeView);
    style = new NTreeViewStyle();
    q->setStyle(style);
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
