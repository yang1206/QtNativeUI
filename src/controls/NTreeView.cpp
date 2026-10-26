//
// Created by Yang1206 on 2025/12/30.
//

#include <QAbstractItemModel>
#include <QCoreApplication>
#include <QCursor>
#include <QItemSelection>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QResizeEvent>
#include <QScrollBar>
#include <QStyleOptionViewItem>
#include <QVariantAnimation>
#include <QWheelEvent>
#include <QtMath>

#include <QtNativeUI/NScrollBar.h>
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NTreeView.h>

#include "../private/naccessible_p.h"
#include "../private/ntreeview_p.h"
#include "../private/ntreecheckcontroller_p.h"
#include "../private/ntreeitemdelegate_p.h"

Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightPlaceholderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkPlaceholderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QString, PlaceholderText)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, QString, HeaderText)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, bool, BorderVisible)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, bool, BackgroundVisible)
Q_PROPERTY_CREATE_Q_CPP(NTreeView, bool, SelectionIndicatorVisible)

void NTreeView::setItemHeight(int ItemHeight) {
    Q_D(NTreeView);
    const int height = qMax(1, ItemHeight);
    if (d->_pItemHeight == height)
        return;
    d->_pItemHeight = height;
    d->updateStyle();
    doItemsLayout();
    Q_EMIT pItemHeightChanged();
}
int NTreeView::getItemHeight() const { return d_ptr->_pItemHeight; }

void NTreeView::setItemBorderRadius(int ItemBorderRadius) {
    Q_D(NTreeView);
    d->_pItemBorderRadius = ItemBorderRadius;
    d->updateStyle();
    Q_EMIT pItemBorderRadiusChanged();
}
int NTreeView::getItemBorderRadius() const { return d_ptr->_pItemBorderRadius; }

void NTreeView::setBorderRadius(int BorderRadius) {
    Q_D(NTreeView);
    d->_pBorderRadius = BorderRadius;
    d->updateStyle();
    Q_EMIT pBorderRadiusChanged();
}
int NTreeView::getBorderRadius() const { return d_ptr->_pBorderRadius; }

void NTreeView::setIndentation(int Indentation) {
    Q_D(NTreeView);
    d->_pIndentation = Indentation;
    QTreeView::setIndentation(Indentation);
    Q_EMIT pIndentationChanged();
}
int NTreeView::getIndentation() const { return d_ptr->_pIndentation; }

NTreeView::NTreeView(QWidget* parent) : QTreeView(parent), d_ptr(new NTreeViewPrivate()) {
    Q_D(NTreeView);
    d->q_ptr = this;
    init();
}

NTreeView::~NTreeView() {
    Q_D(NTreeView);
    d->checks->cancel();
    for (const auto& connection : d->modelConnections)
        disconnect(connection);
    for (const auto& connection : d->selectionModelConnections)
        disconnect(connection);
    d->clearTransientState();
}

void NTreeView::init() {
    Q_D(NTreeView);
    setObjectName(QStringLiteral("NTreeView"));
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::LayerFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::LayerFillColorDefault, NThemeType::Dark);
    d->_pLightItemHoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHoverColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightItemSelectedColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemSelectedColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightItemPressedColor = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkItemPressedColor  = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightPlaceholderTextColor = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkPlaceholderTextColor  = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pItemHeight                = 32;
    d->_pItemBorderRadius          = 4;
    d->_pBorderRadius              = 8;
    d->_pIndentation               = 16;
    d->_pBorderVisible             = true;
    d->_pBackgroundVisible         = true;
    d->_pSelectionIndicatorVisible = true;
    d->isDark    = nTheme->isDarkMode();
    d->themeMode = nTheme->themeMode();
    d->initStyle();
    setFrameShape(QFrame::NoFrame);
    setMouseTracking(true);
    setAutoFillBackground(false);
    viewport()->setMouseTracking(true);
    viewport()->setAutoFillBackground(false);
    setIndentation(d->_pIndentation);
    setRootIsDecorated(false);
    setItemsExpandable(true);
    setExpandsOnDoubleClick(false);
    QTreeView::setAnimated(false);
    setUniformRowHeights(true);
    setHeaderHidden(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setVerticalScrollBar(new NScrollBar(this));
    setHorizontalScrollBar(new NScrollBar(this));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QPalette pal = palette();
    pal.setColor(QPalette::Base, Qt::transparent);
    pal.setColor(QPalette::Window, Qt::transparent);
    pal.setColor(QPalette::Text, d->textColor());
    pal.setColor(QPalette::Highlight, Qt::transparent);
    pal.setColor(QPalette::HighlightedText, d->textColor());
    setPalette(pal);
    setStyleSheet(QStringLiteral("NTreeView{background:transparent;border:none;outline:none;}"
                                 "NTreeView::item{background:transparent;border:none;padding:0px;color:transparent;}"
                                 "NTreeView::item:selected{background:transparent;color:transparent;}"
                                 "NTreeView::item:hover{background:transparent;}"
                                 "NTreeView::item:focus{outline:none;}"
                                 "NTreeView::branch{background:transparent;border:none;}"));
    const auto refreshChrome = [this]() {
        Q_D(NTreeView);
        d->updateStyle();
    };
    connect(this, &NTreeView::pLightBackgroundColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkBackgroundColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pLightItemHoverColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkItemHoverColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pLightItemSelectedColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkItemSelectedColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pLightItemPressedColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkItemPressedColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pLightTextColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkTextColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pLightPlaceholderTextColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkPlaceholderTextColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pLightBorderColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pDarkBorderColorChanged, this, refreshChrome);
    connect(this, &NTreeView::pBorderVisibleChanged, this, refreshChrome);
    connect(this, &NTreeView::pBackgroundVisibleChanged, this, refreshChrome);
    connect(this, &NTreeView::pHeaderTextChanged, this, [this]() {
        Q_D(NTreeView);
        d->layoutHeader();
    });
    connect(this, &NTreeView::pSelectionIndicatorVisibleChanged, this, [this]() {
        Q_D(NTreeView);
        d->updateSelectionMark();
        if (viewport())
            viewport()->update();
    });
    connect(this, &NTreeView::pPlaceholderTextChanged, this, [this]() {
        updateAutomaticAccessibleDescription();
        if (viewport())
            viewport()->update();
    });
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NTreeView);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        QPalette pal = palette();
        pal.setColor(QPalette::Text, d->textColor());
        pal.setColor(QPalette::HighlightedText, d->textColor());
        setPalette(pal);
        d->updateStyle();
    });
    connect(nTheme, &NTheme::accentColorChanged, this, [this]() {
        if (viewport())
            viewport()->update();
    });
    updateAutomaticAccessibleDescription();
}

void NTreeView::setModel(QAbstractItemModel* model) {
    Q_D(NTreeView);
    if (QTreeView::model() == model)
        return;
    for (const QMetaObject::Connection& connection : d->modelConnections)
        disconnect(connection);
    d->modelConnections.clear();
    for (const auto& connection : d->selectionModelConnections)
        disconnect(connection);
    d->selectionModelConnections.clear();
    d->checks->cancel();
    d->checkRepairs.clear();
    d->checkAncestorRepairs.clear();
    d->clearTransientState();
    d->modelChangeDepth = 1;
    QTreeView::setModel(model);
    d->modelChangeDepth = 0;
    d->updateSelectionMark();
    connectModelSignals(model);
    updateAutomaticAccessibleDescription();
    if (viewport())
        viewport()->update();
}

void NTreeView::setSelectionModel(QItemSelectionModel* selectionModel) {
    Q_D(NTreeView);
    if (selectionModel == QTreeView::selectionModel())
        return;
    if (!selectionModel || selectionModel->model() != model()) {
        QTreeView::setSelectionModel(selectionModel);
        return;
    }
    for (const auto& connection : d->selectionModelConnections)
        disconnect(connection);
    d->selectionModelConnections.clear();
    d->clearSelectionMark();
    d->pressedIndex = QModelIndex();
    QTreeView::setSelectionModel(selectionModel);
    d->selectionModelConnections.append(connect(selectionModel, &QObject::destroyed, this, [this]() {
        Q_D(NTreeView);
        d->clearSelectionMark();
        d->pressedIndex = QModelIndex();
        if (viewport())
            viewport()->update();
    }));
    d->selectionModelConnections.append(connect(selectionModel, &QItemSelectionModel::modelChanged, this, [this]() {
        Q_D(NTreeView);
        d->updateSelectionMark();
    }));
    d->updateSelectionMark();
}

void NTreeView::setRootIndex(const QModelIndex& index) {
    Q_D(NTreeView);
    if (index.isValid() && index.model() != model())
        return;
    d->clearTransientState();
    QTreeView::setRootIndex(index);
    d->updateSelectionMark();
    updateAutomaticAccessibleDescription();
    if (viewport())
        viewport()->update();
}

void NTreeView::reset() {
    Q_D(NTreeView);
    const auto repairs = d->checks->interrupt();
    d->clearTransientState();
    QTreeView::reset();
    d->updateSelectionMark();
    updateAutomaticAccessibleDescription();
    if (d->modelChangeDepth == 0) {
        for (const auto& repair : repairs) {
            if (repair.isValid() && repair.model() == model())
                d->checks->reconcile(repair);
        }
    }
}

void NTreeView::setAnimated(bool enable) { setRevealAnimationEnabled(enable); }

bool NTreeView::isAnimated() const { return isRevealAnimationEnabled(); }

void NTreeView::setRevealAnimationEnabled(bool enable) {
    Q_D(NTreeView);
    QTreeView::setAnimated(false);
    if (d->revealAnimationEnabled == enable)
        return;
    d->revealAnimationEnabled = enable;
    if (!enable) {
        d->completeActiveExpandReveal();
        d->clearExpandRevealState();
    }
}

bool NTreeView::isRevealAnimationEnabled() const { return d_ptr->revealAnimationEnabled; }

QModelIndex NTreeView::indexAt(const QPoint& point) const {
    Q_D(const NTreeView);
    const QModelIndex index = QTreeView::indexAt(point);
    return index.isValid() && d->revealClipRect(index).contains(point) ? index : QModelIndex();
}

bool NTreeView::isShowingPlaceholder() const {
    if (getPlaceholderText().isEmpty())
        return false;
    const QAbstractItemModel* m = model();
    return !m || m->rowCount(rootIndex()) == 0;
}

void NTreeView::connectModelSignals(QAbstractItemModel* model) {
    Q_D(NTreeView);
    if (!model)
        return;
    const auto beginChange = [this](bool clearAll, bool finishReveal, bool interruptChecks) {
        Q_D(NTreeView);
        ++d->modelChangeDepth;
        if (interruptChecks) {
            for (const auto& repair : d->checks->interrupt()) {
                if (!d->checkRepairs.contains(repair))
                    d->checkRepairs.append(repair);
            }
        }
        if (finishReveal)
            d->completeActiveExpandReveal();
        if (clearAll)
            d->clearTransientState();
    };
    const auto resetAboutToChange = [beginChange]() { beginChange(true, false, true); };
    const auto layoutAboutToChange = [beginChange]() { beginChange(true, true, true); };
    const auto rememberAncestor = [this](QModelIndex parent) {
        Q_D(NTreeView);
        while (parent.isValid() && !(parent.flags() & Qt::ItemIsUserCheckable))
            parent = parent.parent();
        const QPersistentModelIndex ancestor(parent);
        if (ancestor.isValid() && !d->checkAncestorRepairs.contains(ancestor))
            d->checkAncestorRepairs.append(ancestor);
    };
    const auto rowsAboutToChange = [this, beginChange, rememberAncestor](const QModelIndex& parent, int first,
                                                                        int last, bool removing) {
        Q_D(NTreeView);
        rememberAncestor(parent);
        beginChange(false, d->affectsReveal(parent, first, last, removing),
                    d->checks->affects(parent, first, last, removing));
    };
    const auto changed = [this]() {
        Q_D(NTreeView);
        if (d->modelChangeDepth > 0)
            --d->modelChangeDepth;
        if (d->modelChangeDepth == 0) {
            d->updateSelectionMark();
            updateAutomaticAccessibleDescription();
            const auto repairs = d->checkRepairs;
            d->checkRepairs.clear();
            for (const auto& repair : repairs) {
                if (repair.isValid() && repair.model() == this->model())
                    d->checks->reconcile(repair);
            }
            const auto ancestors = d->checkAncestorRepairs;
            d->checkAncestorRepairs.clear();
            for (const auto& ancestor : ancestors) {
                if (ancestor.isValid() && ancestor.model() == this->model())
                    d->checks->reconcileAncestors(ancestor);
            }
        }
    };
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelAboutToBeReset, this, resetAboutToChange));
    d->modelConnections.append(connect(model, &QAbstractItemModel::layoutAboutToBeChanged, this, layoutAboutToChange));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeInserted, this,
                                      [rowsAboutToChange](const QModelIndex& parent, int first, int last) {
        rowsAboutToChange(parent, first, last, false);
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, this,
                                      [rowsAboutToChange](const QModelIndex& parent, int first, int last) {
        rowsAboutToChange(parent, first, last, true);
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeMoved, this,
                                      [this, beginChange, rememberAncestor](const QModelIndex& source, int first, int last,
                                                          const QModelIndex& destination, int) {
        Q_D(NTreeView);
        rememberAncestor(source);
        rememberAncestor(destination);
        const bool reveal = d->affectsReveal(source, first, last, true)
                            || d->affectsReveal(destination, 0, 0, false);
        const bool checks = d->checks->affects(source, first, last, true)
                            || d->checks->affects(destination, 0, 0, false);
        beginChange(false, reveal, checks);
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsAboutToBeInserted, this, layoutAboutToChange));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsAboutToBeRemoved, this, layoutAboutToChange));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsAboutToBeMoved, this, layoutAboutToChange));
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsMoved, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsInserted, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsRemoved, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsMoved, this, changed));
    d->modelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this,
                                      [this](const QModelIndex& topLeft, const QModelIndex& bottomRight,
                                              const QList<int>& roles) {
        Q_D(NTreeView);
        if (d->checks->isApplying() || d->modelChangeDepth > 0
            || (!roles.isEmpty() && !roles.contains(Qt::CheckStateRole)))
            return;
        d->checks->externalChange(topLeft, bottomRight);
        QModelIndex ancestor = topLeft.parent();
        while (ancestor.isValid() && !(ancestor.flags() & Qt::ItemIsUserCheckable))
            ancestor = ancestor.parent();
        if (ancestor.isValid())
            d->checks->reconcileAncestors(ancestor);
        // 文本、图标交给 Qt 的局部更新；勾选状态影响整行背景和标记，只补可见行区域。
        QModelIndex row = QTreeView::indexAt(QPoint(0, 0));
        while (row.isValid()) {
            const QRect rect = visualRect(row);
            if (rect.top() >= viewport()->height())
                break;
            if (row.parent() == topLeft.parent() && row.row() >= topLeft.row() && row.row() <= bottomRight.row())
                viewport()->update(QRect(0, rect.top(), viewport()->width(), rect.height()));
            row = indexBelow(row);
        }
    }));
    d->modelConnections.append(connect(model, &QObject::destroyed, this, [this]() {
        Q_D(NTreeView);
        d->checks->cancel();
        d->checkRepairs.clear();
        d->checkAncestorRepairs.clear();
        d->modelChangeDepth = 0;
        d->clearTransientState();
        if (QCoreApplication::closingDown())
            return;
        NA11y::applyAutomaticAccessibleDescription(this, d->automaticAccessibleDescription, QString());
        if (viewport())
            viewport()->update();
    }));
}

void NTreeView::updateAutomaticAccessibleDescription() {
    Q_D(NTreeView);
    if (QCoreApplication::closingDown())
        return;
    const QString automatic = isShowingPlaceholder() ? getPlaceholderText() : QString();
    NA11y::applyAutomaticAccessibleDescription(this, d->automaticAccessibleDescription, automatic);
}

void NTreeView::expandAll() {
    Q_D(NTreeView);
    d->revealAnimationsEnabled = false;
    d->clearExpandRevealState();
    QTreeView::expandAll();
    d->revealAnimationsEnabled = true;
}

void NTreeView::collapseAll() {
    Q_D(NTreeView);
    d->revealAnimationsEnabled = false;
    d->clearExpandRevealState();
    QTreeView::collapseAll();
    d->revealAnimationsEnabled = true;
}

void NTreeView::toggleExpanded(const QModelIndex& index) {
    Q_D(NTreeView);
    d->toggleExpanded(index);
}

qreal NTreeView::chevronRotation(const QModelIndex& index) const {
    Q_D(const NTreeView);
    return d->chevronRotation(index);
}

void NTreeView::drawBranches(QPainter* painter, const QRect& rect, const QModelIndex& index) const {
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(index);
}

void NTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index) const {
    Q_D(const NTreeView);
    const QRect clip = d->revealClipRect(index);
    if (!clip.intersects(options.rect))
        return;
    // 保持 Qt 的真实行位置，仅裁剪子树 reveal；绘制、菜单命中与选择标记使用相同几何。
    painter->save();
    painter->setClipRect(clip, Qt::IntersectClip);
    d->paintRowBackground(painter, options, index);
    QTreeView::drawRow(painter, options, index);
    painter->restore();
}

void NTreeView::mousePressEvent(QMouseEvent* event) {
    // 右键只用于上下文菜单，保留原有选择和 currentIndex。
    if (event->button() == Qt::RightButton) {
        event->accept();
        return;
    }
    Q_D(NTreeView);
    d->ignoreRevealRelease = QTreeView::indexAt(event->position().toPoint()).isValid()
                             && !indexAt(event->position().toPoint()).isValid();
    if (d->ignoreRevealRelease) {
        event->accept();
        return;
    }
    // 视图事件的坐标已经属于 viewport；空白处不应把旧 currentIndex 标为按下。
    d->pressedIndex = event->button() == Qt::LeftButton ? indexAt(event->position().toPoint()) : QModelIndex();
    QTreeView::mousePressEvent(event);
    if (viewport())
        viewport()->update();
}

void NTreeView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        event->accept();
        return;
    }
    Q_D(NTreeView);
    if (d->ignoreRevealRelease) {
        d->ignoreRevealRelease = false;
        event->accept();
        return;
    }
    QTreeView::mouseReleaseEvent(event);
    d->pressedIndex = QModelIndex();
    if (viewport())
        viewport()->update();
}

void NTreeView::keyPressEvent(QKeyEvent* event) {
    Q_D(NTreeView);
    d->completeActiveExpandReveal();
    const QModelIndex index = currentIndex().siblingAtColumn(0);
    if (event->key() == Qt::Key_Space && event->modifiers() == Qt::NoModifier
        && index.isValid() && (index.flags() & Qt::ItemIsUserCheckable)) {
        if (!event->isAutoRepeat())
            d->checks->toggle(index);
        event->accept();
        return;
    }
    const bool collapseKey = event->key() == Qt::Key_Minus
                             || event->key() == (isRightToLeft() ? Qt::Key_Right : Qt::Key_Left);
    if (collapseKey && event->modifiers() == Qt::NoModifier && itemsExpandable()
        && index.isValid() && isExpanded(index)) {
        d->toggleExpanded(index);
        event->accept();
        return;
    }
    QTreeView::keyPressEvent(event);
}

void NTreeView::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NTreeView);
    const QPersistentModelIndex previous(d->hoveredIndex);
    const QModelIndex hit = indexAt(event->position().toPoint());
    d->hoveredIndex = hit.isValid() ? hit.siblingAtColumn(0) : QModelIndex();
    if (previous != d->hoveredIndex) {
        for (const auto& index : {previous, d->hoveredIndex}) {
            if (index.isValid()) {
                const QRect rect = visualRect(index);
                viewport()->update(QRect(0, rect.top(), viewport()->width(), rect.height()));
            }
        }
    }
    QTreeView::mouseMoveEvent(event);
}

bool NTreeView::viewportEvent(QEvent* event) {
    Q_D(NTreeView);
    if ((event->type() == QEvent::Leave || event->type() == QEvent::HoverLeave) && d->hoveredIndex.isValid()) {
        const QRect rect = visualRect(d->hoveredIndex);
        d->hoveredIndex = QModelIndex();
        viewport()->update(QRect(0, rect.top(), viewport()->width(), rect.height()));
    }
    return QTreeView::viewportEvent(event);
}

QItemSelectionModel::SelectionFlags NTreeView::selectionCommand(const QModelIndex& index,
                                                               const QEvent* event) const {
    Q_D(const NTreeView);
    // 多选勾选行由 CheckStateRole 表达已选择；currentIndex 仍用于键盘焦点和导航。
    if (d->usesCheckState(index))
        return QItemSelectionModel::NoUpdate;
    if (event && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)) {
        const auto* mouse = static_cast<const QMouseEvent*>(event);
        if (mouse->button() == Qt::RightButton)
            return QItemSelectionModel::NoUpdate;
        if (index.isValid() && (index.flags() & Qt::ItemIsUserCheckable)) {
            QStyleOptionViewItem option;
            option.rect = visualRect(index);
            option.index = index;
            if (d->itemDelegate->checkBoxRect(option).contains(mouse->position()))
                return QItemSelectionModel::NoUpdate;
        }
    }
    return QTreeView::selectionCommand(index, event);
}

void NTreeView::currentChanged(const QModelIndex& current, const QModelIndex& previous) {
    Q_D(NTreeView);
    QTreeView::currentChanged(current, previous);
    d->updateSelectionMark();
}

void NTreeView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_D(NTreeView);
    QTreeView::selectionChanged(selected, deselected);
    d->updateSelectionMark();
    if (viewport())
        viewport()->update();
}

void NTreeView::scrollContentsBy(int dx, int dy) {
    Q_D(NTreeView);
    d->completeActiveExpandReveal();
    QTreeView::scrollContentsBy(dx, dy);
    const QPersistentModelIndex previous(d->hoveredIndex);
    const QModelIndex hit = viewport()->underMouse()
                                ? indexAt(viewport()->mapFromGlobal(QCursor::pos())) : QModelIndex();
    d->hoveredIndex = hit.isValid() ? hit.siblingAtColumn(0) : QModelIndex();
    if (previous != d->hoveredIndex) {
        for (const auto& index : {previous, d->hoveredIndex}) {
            if (index.isValid()) {
                const QRect rect = visualRect(index);
                viewport()->update(QRect(0, rect.top(), viewport()->width(), rect.height()));
            }
        }
    }
}

void NTreeView::resizeEvent(QResizeEvent* event) {
    QTreeView::resizeEvent(event);
    Q_D(NTreeView);
    d->layoutHeader();
}

void NTreeView::paintEvent(QPaintEvent* event) {
    Q_D(NTreeView);
    if (viewport()->underMouse()) {
        const QModelIndex hit = indexAt(viewport()->mapFromGlobal(QCursor::pos()));
        d->hoveredIndex = hit.isValid() ? hit.siblingAtColumn(0) : QModelIndex();
    }
    if (selectionMode() == QAbstractItemView::SingleSelection)
        d->updateSelectionMark();
    QTreeView::paintEvent(event);
    if (!viewport())
        return;
    if (isShowingPlaceholder()) {
        QPainter painter(viewport());
        if (painter.isActive()) {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(d->placeholderTextColor());
            painter.drawText(viewport()->rect().adjusted(24, 24, -24, -24),
                             Qt::AlignCenter | Qt::TextWordWrap,
                             getPlaceholderText());
        }
        return;
    }
    QPainter markPainter(viewport());
    if (markPainter.isActive())
        d->paintSelectionMark(&markPainter);
}

void NTreeView::wheelEvent(QWheelEvent* event) {
    QTreeView::wheelEvent(event);
    const QScrollBar* vbar = verticalScrollBar();
    const QScrollBar* hbar = horizontalScrollBar();
    if ((vbar && vbar->maximum() > vbar->minimum())
        || (hbar && hbar->maximum() > hbar->minimum())) {
        event->accept();
    }
}
