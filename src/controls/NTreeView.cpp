//
// Created by Yang1206 on 2025/12/30.
//

#include <QAbstractItemModel>
#include <QCoreApplication>
#include <QItemSelection>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QResizeEvent>
#include <QScrollBar>
#include <QVariantAnimation>
#include <QWheelEvent>
#include <QtMath>

#include <QtNativeUI/NScrollBar.h>
#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NTreeView.h>

#include "../private/naccessible_p.h"
#include "../private/ntreeview_p.h"

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
    d->_pItemHeight = ItemHeight;
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

NTreeView::~NTreeView() = default;

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
    setAnimated(false);
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
    QTreeView::setModel(model);
    d->pressedIndex = QModelIndex();
    d->updateSelectionMark();
    connectModelSignals(model);
    updateAutomaticAccessibleDescription();
    if (viewport())
        viewport()->update();
}

bool NTreeView::isShowingPlaceholder() const {
    if (getPlaceholderText().isEmpty())
        return false;
    const QAbstractItemModel* m = model();
    return !m || m->rowCount() == 0;
}

void NTreeView::connectModelSignals(QAbstractItemModel* model) {
    Q_D(NTreeView);
    if (!model)
        return;
    const auto refresh = [this]() {
        if (QCoreApplication::closingDown())
            return;
        updateAutomaticAccessibleDescription();
        if (viewport())
            viewport()->update();
    };
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this, refresh));
    d->modelConnections.append(connect(model, &QObject::destroyed, this, [this]() {
        Q_D(NTreeView);
        if (QCoreApplication::closingDown())
            return;
        NA11y::applyAutomaticAccessibleDescription(this, d->automaticAccessibleDescription, QString());
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
    if (d->isRevealRunning()) {
        const QModelIndex parent = QModelIndex(d->animParent);
        const QRect parentRect = visualRect(parent);
        const qreal progress = qBound(0.0, d->expandRevealAnim->currentValue().toReal(), 1.0);
        const qreal boundaryY = parentRect.bottom() + 1 + d->animSubtreeHeight * progress;
        const qreal slideUp = d->animSubtreeHeight * (1.0 - progress);
        if (index != parent && d->isDescendantOf(index, parent)) {
            if (options.rect.top() >= boundaryY)
                return;
            QRect clip = options.rect;
            clip.setBottom(qMin(options.rect.bottom(), qFloor(boundaryY)));
            painter->save();
            painter->setClipRect(clip);
            QTreeView::drawRow(painter, options, index);
            painter->restore();
            return;
        }
        if (index != parent && options.rect.top() >= parentRect.bottom()) {
            painter->save();
            painter->translate(0, -slideUp);
            QTreeView::drawRow(painter, options, index);
            painter->restore();
            return;
        }
    }
    QTreeView::drawRow(painter, options, index);
}

void NTreeView::mousePressEvent(QMouseEvent* event) {
    Q_D(NTreeView);
    QTreeView::mousePressEvent(event);
    QModelIndex hitIndex = currentIndex();
    if (!hitIndex.isValid() || hitIndex.model() != model()) {
        const QPoint viewportPos = viewport() ? viewport()->mapFrom(this, event->position().toPoint())
                                              : event->position().toPoint();
        hitIndex = indexAt(viewportPos);
    }
    d->pressedIndex = hitIndex;
    if (viewport())
        viewport()->update();
}

void NTreeView::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NTreeView);
    QTreeView::mouseReleaseEvent(event);
    d->pressedIndex = QModelIndex();
    if (viewport())
        viewport()->update();
}

void NTreeView::currentChanged(const QModelIndex& current, const QModelIndex& previous) {
    Q_D(NTreeView);
    QTreeView::currentChanged(current, previous);
    d->updateSelectionMark(previous);
}

void NTreeView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_D(NTreeView);
    QTreeView::selectionChanged(selected, deselected);
    d->updateSelectionMark();
    if (viewport())
        viewport()->update();
}

void NTreeView::scrollContentsBy(int dx, int dy) {
    QTreeView::scrollContentsBy(dx, dy);
    Q_D(NTreeView);
    d->updateSelectionMark();
}

void NTreeView::resizeEvent(QResizeEvent* event) {
    QTreeView::resizeEvent(event);
    Q_D(NTreeView);
    d->layoutHeader();
}

void NTreeView::paintEvent(QPaintEvent* event) {
    Q_D(NTreeView);
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
