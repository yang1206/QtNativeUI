//
// Created by Yang1206 on 2025/12/29.
//

#include <QAbstractItemModel>
#include <QCoreApplication>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>
#include <QWheelEvent>

#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NScrollBar.h>
#include <QtNativeUI/NTheme.h>

#include "../private/naccessible_p.h"
#include "../private/nlistitemdelegate_p.h"
#include "../private/nlistview_p.h"
#include "../private/nlistviewdragreorder_p.h"
#include "../private/nlistviewreorder_p.h"
#include "../private/nlistviewselectmark_p.h"

Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkItemPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightPlaceholderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkPlaceholderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, ItemHeight)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, ItemBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NListView, QString, PlaceholderText)
Q_PROPERTY_CREATE_Q_CPP(NListView, bool, BorderVisible)
Q_PROPERTY_CREATE_Q_CPP(NListView, bool, BackgroundVisible)
Q_PROPERTY_CREATE_Q_CPP(NListView, bool, SelectionIndicatorVisible)
Q_PROPERTY_CREATE_Q_CPP(NListView, QString, HeaderText)
Q_PROPERTY_CREATE_Q_CPP(NListView, QString, FooterText)
Q_PROPERTY_CREATE_Q_CPP(NListView, bool, SectionsEnabled)
Q_PROPERTY_CREATE_Q_CPP(NListView, int, SectionHeaderHeight)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, LightSectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QColor, DarkSectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NListView, QFont, SectionHeaderFont)
Q_PROPERTY_CREATE_Q_CPP(NListView, bool, SelectionIndicatorAnimated)
Q_PROPERTY_CREATE_Q_CPP(NListView, bool, ReorderEnabled)

NListView::NListView(QWidget* parent) : QListView(parent), d_ptr(new NListViewPrivate()) {
    Q_D(NListView);
    d->q_ptr = this;
    init();
}

NListView::~NListView() {
    Q_D(NListView);
    for (const QMetaObject::Connection& connection : d->modelConnections)
        disconnect(connection);
    d->modelConnections.clear();
}

void NListView::init() {
    Q_D(NListView);

    setObjectName(QStringLiteral("NListView"));
    setFocusPolicy(Qt::StrongFocus);
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::LayerFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::LayerFillColorDefault, NThemeType::Dark);

    d->_pLightItemHoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHoverColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);

    d->_pLightItemSelectedColor = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkItemSelectedColor  = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);

    d->_pLightItemPressedColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemPressedColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pLightPlaceholderTextColor = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkPlaceholderTextColor  = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);

    d->_pLightBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pLightSectionTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkSectionTextColor  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);

    d->_pItemHeight                = 36;
    d->_pItemBorderRadius          = 4;
    d->_pBorderRadius              = 8;
    d->_pBorderVisible             = true;
    d->_pBackgroundVisible         = true;
    d->_pSelectionIndicatorVisible    = true;
    d->_pSectionsEnabled            = false;
    d->_pSectionHeaderHeight        = 28;
    QFont sectionFont = font();
    sectionFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
    d->_pSectionHeaderFont = sectionFont;
    d->_pSelectionIndicatorAnimated = true;
    d->_pReorderEnabled             = false;

    d->headerLabel  = new QLabel(this);
    d->footerLabel  = new QLabel(this);
    d->headerLabel->setObjectName(QStringLiteral("NListViewHeader"));
    d->footerLabel->setObjectName(QStringLiteral("NListViewFooter"));

    d->isDark    = nTheme->isDarkMode();
    d->themeMode = nTheme->themeMode();
    d->initStyle();

    setFrameShape(QFrame::NoFrame);
    setAutoFillBackground(false);
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    viewport()->setAutoFillBackground(false);
    d->updateHeaderFooterStyle();
    d->layoutHeaderFooter();
    setVerticalScrollBar(new NScrollBar(this));
    setHorizontalScrollBar(new NScrollBar(this));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setStyleSheet(QStringLiteral("NListView{background:transparent;border:none;outline:none;}"
                                 "NListView::item{background:transparent;border:none;padding:0px;}"
                                 "NListView::item:selected{background:transparent;}"
                                 "NListView::item:focus{outline:none;}"));

    d->itemDelegate = new NListItemDelegate(this, d, this);
    QListView::setItemDelegate(d->itemDelegate);
    d->selectMarkController = new NListViewSelectMarkController(this, this);
    d->dragReorder          = new NListViewDragReorderController(this, this);
    d->applyReorderConfiguration();

    connect(this, &NListView::pPlaceholderTextChanged, this, [this]() {
        updateAutomaticAccessibleDescription();
        if (viewport())
            viewport()->update();
        update();
    });
    connect(this, &NListView::pBorderVisibleChanged, this, [this]() {
        Q_D(NListView);
        d->updateStyle();
    });
    connect(this, &NListView::pBackgroundVisibleChanged, this, [this]() {
        Q_D(NListView);
        d->updateStyle();
    });
    connect(this, &NListView::pSelectionIndicatorVisibleChanged, this, [this]() {
        Q_D(NListView);
        if (d->selectMarkController)
            d->selectMarkController->resetState();
        if (viewport())
            viewport()->update();
    });
    connect(this, &NListView::pSelectionIndicatorAnimatedChanged, this, [this]() {
        if (viewport())
            viewport()->update();
    });
    connect(this, &NListView::pReorderEnabledChanged, this, [this]() {
        Q_D(NListView);
        d->applyReorderConfiguration();
    });
    const auto relayoutChrome = [this]() {
        Q_D(NListView);
        d->layoutHeaderFooter();
        if (viewport())
            viewport()->update();
    };
    connect(this, &NListView::pHeaderTextChanged, this, relayoutChrome);
    connect(this, &NListView::pFooterTextChanged, this, relayoutChrome);
    connect(this, &NListView::pSectionsEnabledChanged, this, [this]() {
        doItemsLayout();
        if (viewport())
            viewport()->update();
    });
    connect(this, &NListView::pSectionHeaderHeightChanged, this, [this]() {
        doItemsLayout();
        if (viewport())
            viewport()->update();
    });
    connect(this, &NListView::pSectionHeaderFontChanged, this, [this]() {
        if (viewport())
            viewport()->update();
    });
    const auto refreshChrome = [this]() {
        Q_D(NListView);
        d->updateStyle();
        if (viewport())
            viewport()->update();
    };
    connect(this, &NListView::pItemHeightChanged, this, refreshChrome);
    connect(this, &NListView::pItemBorderRadiusChanged, this, refreshChrome);
    connect(this, &NListView::pBorderRadiusChanged, this, refreshChrome);
    connect(this, &NListView::pLightBackgroundColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkBackgroundColorChanged, this, refreshChrome);
    connect(this, &NListView::pLightItemHoverColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkItemHoverColorChanged, this, refreshChrome);
    connect(this, &NListView::pLightItemSelectedColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkItemSelectedColorChanged, this, refreshChrome);
    connect(this, &NListView::pLightItemPressedColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkItemPressedColorChanged, this, refreshChrome);
    connect(this, &NListView::pLightTextColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkTextColorChanged, this, refreshChrome);
    connect(this, &NListView::pLightBorderColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkBorderColorChanged, this, refreshChrome);
    connect(this, &NListView::pLightSectionTextColorChanged, this, refreshChrome);
    connect(this, &NListView::pDarkSectionTextColorChanged, this, refreshChrome);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NListView);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        d->updateStyle();
        d->updateHeaderFooterStyle();
        if (d->selectMarkController)
            d->selectMarkController->onScrolled();
    });
    connect(nTheme, &NTheme::accentColorChanged, this, [this]() {
        if (viewport())
            viewport()->update();
    });
    updateAutomaticAccessibleDescription();
}

void NListView::setModel(QAbstractItemModel* model) {
    Q_D(NListView);
    if (QListView::model() == model)
        return;
    for (const QMetaObject::Connection& connection : d->modelConnections)
        disconnect(connection);
    d->modelConnections.clear();
    QListView::setModel(model);
    d->pressedIndex = QModelIndex();
    if (d->selectMarkController) {
        d->selectMarkController->resetState();
        d->selectMarkController->updateFromSelection();
    }
    connectModelSignals(model);
    updateAutomaticAccessibleDescription();
    if (viewport())
        viewport()->update();
}

void NListView::setSelectionModel(QItemSelectionModel* selectionModel) {
    Q_D(NListView);
    QListView::setSelectionModel(selectionModel);
    if (d->selectMarkController) {
        d->selectMarkController->resetState();
        d->selectMarkController->updateFromSelection();
    }
    if (viewport())
        viewport()->update();
}

bool NListView::isShowingPlaceholder() const {
    if (QCoreApplication::closingDown())
        return false;
    if (getPlaceholderText().isEmpty())
        return false;
    QAbstractItemModel* itemModel = model();
    if (!itemModel)
        return true;
    return itemModel->rowCount() == 0;
}

void NListView::currentChanged(const QModelIndex& current, const QModelIndex& previous) {
    Q_D(NListView);
    QListView::currentChanged(current, previous);
    if (d->selectMarkController)
        d->selectMarkController->updateFromSelection(previous);
}

void NListView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_D(NListView);
    QListView::selectionChanged(selected, deselected);
    if (d->selectMarkController) {
        d->selectMarkController->updateFromSelection();
        d->selectMarkController->syncMultiSelection(selected, deselected);
    }
}

void NListView::scrollContentsBy(int dx, int dy) {
    Q_D(NListView);
    QListView::scrollContentsBy(dx, dy);
    if (d->selectMarkController)
        d->selectMarkController->onScrolled();
}

QPoint NListView::viewportPosFromMouseEvent(const QMouseEvent* event) const {
    if (!event)
        return {};
    const QPoint viewPos = event->position().toPoint();
    return viewport() ? viewport()->mapFrom(this, viewPos) : viewPos;
}

void NListView::mousePressEvent(QMouseEvent* event) {
    Q_D(NListView);
    if (getReorderEnabled() && d->dragReorder && viewMode() == QListView::ListMode)
        d->dragReorder->reset();
    const QPoint viewportPos = viewportPosFromMouseEvent(event);
    QListView::mousePressEvent(event);
    QModelIndex hitIndex = currentIndex();
    if (!hitIndex.isValid() || hitIndex.model() != model())
        hitIndex = indexAt(viewportPos);
    d->pressedIndex = hitIndex;
    if (getReorderEnabled() && d->dragReorder && viewMode() == QListView::ListMode
        && event->button() == Qt::LeftButton && hitIndex.isValid())
        d->dragReorder->onPress(viewportPos, hitIndex.row());
    if (viewport())
        viewport()->update();
}

void NListView::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NListView);
    const QPoint viewportPos = viewportPosFromMouseEvent(event);
    if (getReorderEnabled() && d->dragReorder && viewMode() == QListView::ListMode
        && d->dragReorder->onMove(viewportPos, event->buttons())) {
        event->accept();
        return;
    }
    QListView::mouseMoveEvent(event);
}

void NListView::resizeEvent(QResizeEvent* event) {
    QListView::resizeEvent(event);
    Q_D(NListView);
    d->layoutHeaderFooter();
}

QRect NListView::layoutVisualRect(const QModelIndex& index) const {
    return QListView::visualRect(index);
}

QRect NListView::visualRect(const QModelIndex& index) const {
    QRect rect = QListView::visualRect(index);
    Q_D(const NListView);
    if (d->dragReorder && d->dragReorder->isDragging() && d->dragReorder->paintingWithOffsets())
        return d->dragReorder->adjustedVisualRect(index, rect);
    return rect;
}

QRect NListView::indicatorItemRect(const QModelIndex& index) const {
    Q_D(const NListView);
    if (d->dragReorder && d->dragReorder->isDragging() && index.isValid()
        && index.row() == d->dragReorder->sourceRow()) {
        const QRect ghost = d->dragReorder->ghostRect();
        if (!ghost.isEmpty())
            return ghost;
    }
    return visualRect(index);
}

void NListView::finishReorderDrop() {
    Q_D(NListView);
    if (d->dragReorder)
        d->dragReorder->reset();
    d->pressedIndex = QModelIndex();
    setState(QAbstractItemView::NoState);
    if (d->selectMarkController) {
        d->selectMarkController->resetState();
        d->selectMarkController->updateFromSelection();
    }
    doItemsLayout();
    if (viewport())
        viewport()->update();
}

void NListView::startDrag(Qt::DropActions supportedActions) {
    if (getReorderEnabled())
        return;
    QListView::startDrag(supportedActions);
}

void NListView::wheelEvent(QWheelEvent* event) {
    QListView::wheelEvent(event);
    const QScrollBar* vbar = verticalScrollBar();
    const QScrollBar* hbar = horizontalScrollBar();
    if ((vbar && vbar->maximum() > vbar->minimum())
        || (hbar && hbar->maximum() > hbar->minimum())) {
        event->accept();
    }
}

void NListView::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NListView);
    if (getReorderEnabled() && d->dragReorder && event->button() == Qt::LeftButton
        && viewMode() == QListView::ListMode) {
        if (d->dragReorder->isDragging() && model()) {
            const QPoint viewportPos = viewportPosFromMouseEvent(event);
            const int    sourceRow   = d->dragReorder->sourceRow();
            int          insertBefore = 0;
            d->dragReorder->onRelease(viewportPos, event->button(), &insertBefore);
            if (NListViewReorder::moveRow(model(), sourceRow, insertBefore)) {
                const int destRow = insertBefore > sourceRow ? insertBefore - 1 : insertBefore;
                const QModelIndex moved = model()->index(destRow, 0);
                if (moved.isValid()) {
                    setCurrentIndex(moved);
                    if (selectionModel()) {
                        selectionModel()->select(moved,
                                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                    }
                }
                Q_EMIT rowsReordered(sourceRow, destRow);
            }
            finishReorderDrop();
            event->accept();
            return;
        }
        if (d->dragReorder->sourceRow() >= 0)
            d->dragReorder->reset();
    }
    QListView::mouseReleaseEvent(event);
    d->pressedIndex = QModelIndex();
    if (viewport())
        viewport()->update();
}

void NListView::paintEvent(QPaintEvent* event) {
    Q_D(NListView);
    const bool dragging = d->dragReorder && d->dragReorder->isDragging();
    if (dragging)
        d->dragReorder->setPaintingWithOffsets(true);
    QListView::paintEvent(event);
    if (!viewport()) {
        if (d->dragReorder)
            d->dragReorder->setPaintingWithOffsets(false);
        return;
    }
    if (isShowingPlaceholder()) {
        QPainter placeholder(viewport());
        if (placeholder.isActive()) {
            placeholder.setRenderHint(QPainter::Antialiasing);
            placeholder.setPen(d->placeholderTextColor());
            placeholder.drawText(viewport()->rect().adjusted(24, 24, -24, -24),
                                 Qt::AlignCenter | Qt::TextWordWrap,
                                 getPlaceholderText());
        }
        if (d->dragReorder)
            d->dragReorder->setPaintingWithOffsets(false);
        return;
    }
    if (dragging)
        d->dragReorder->paintOverlay(viewport());
    if (d->selectMarkController)
        d->selectMarkController->paintViewportOverlay(viewport());
    if (!dragging && d->itemDelegate)
        d->itemDelegate->paintStickySection(viewport());
    if (d->dragReorder)
        d->dragReorder->setPaintingWithOffsets(false);
}

void NListView::connectModelSignals(QAbstractItemModel* model) {
    Q_D(NListView);
    if (!model)
        return;
    const auto refresh = [this]() {
        if (QCoreApplication::closingDown())
            return;
        updateAutomaticAccessibleDescription();
        if (viewport())
            viewport()->update();
    };
    const auto refreshIndicator = [this, d]() {
        if (QCoreApplication::closingDown() || !d->selectMarkController)
            return;
        d->selectMarkController->updateFromSelection();
    };
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelAboutToBeReset, this, [this, d]() {
        if (d->selectMarkController)
            d->selectMarkController->onModelAboutToReset();
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, [this, d, refresh]() {
        if (d->selectMarkController)
            d->selectMarkController->onModelReset();
        refresh();
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, [this, d, refresh]() {
        if (d->selectMarkController)
            d->selectMarkController->onLayoutChanged();
        refresh();
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, this, [this, d]() {
        if (d->selectMarkController)
            d->selectMarkController->onRowsAboutToBeRemoved();
    }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, refreshIndicator));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsMoved, this,
                                       [this, d, refresh](const QModelIndex& parent, int start, int end,
                                                          const QModelIndex& destinationParent, int destinationRow) {
                                           if (d->selectMarkController)
                                               d->selectMarkController->onRowsMoved();
                                           refresh();
                                           if (!getReorderEnabled() || parent != destinationParent || start != end)
                                               return;
                                           int destinationIndex = destinationRow;
                                           if (destinationIndex > start)
                                               destinationIndex -= 1;
                                           Q_EMIT rowsReordered(start, destinationIndex);
                                       }));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this, refresh));
    d->modelConnections.append(connect(model, &QObject::destroyed, this, [this]() {
        Q_D(NListView);
        if (QCoreApplication::closingDown())
            return;
        NA11y::applyAutomaticAccessibleDescription(this, d->automaticAccessibleDescription, QString());
    }));
}

void NListView::updateAutomaticAccessibleDescription() {
    Q_D(NListView);
    if (QCoreApplication::closingDown())
        return;
    const QString automatic = isShowingPlaceholder() ? getPlaceholderText() : QString();
    NA11y::applyAutomaticAccessibleDescription(this, d->automaticAccessibleDescription, automatic);
}
