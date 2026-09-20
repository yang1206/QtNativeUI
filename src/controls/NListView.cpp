//
// Created by Yang1206 on 2025/12/29.
//

#include <QAbstractItemModel>
#include <QCoreApplication>
#include <QItemSelection>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NScrollBar.h>
#include <QtNativeUI/NTheme.h>

#include "../private/naccessible_p.h"
#include "../private/nlistitemdelegate_p.h"
#include "../private/nlistview_p.h"
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

    d->_pItemHeight                = 36;
    d->_pItemBorderRadius          = 4;
    d->_pBorderRadius              = 8;
    d->_pBorderVisible             = true;
    d->_pBackgroundVisible         = true;
    d->_pSelectionIndicatorVisible = true;

    d->isDark    = nTheme->isDarkMode();
    d->themeMode = nTheme->themeMode();
    d->initStyle();

    setFrameShape(QFrame::NoFrame);
    setAutoFillBackground(false);
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    viewport()->setAutoFillBackground(false);
    setViewportMargins(4, 4, 4, 4);
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

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NListView);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        d->updateStyle();
    });
    connect(nTheme, &NTheme::accentColorChanged, this, [this]() {
        if (viewport())
            viewport()->update();
    });
    connect(nTheme, &NTheme::themeModeChanged, this, [this, d]() {
        if (d->selectMarkController)
            d->selectMarkController->onScrolled();
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
    if (d->selectMarkController)
        d->selectMarkController->updateFromSelection();
}

void NListView::scrollContentsBy(int dx, int dy) {
    Q_D(NListView);
    QListView::scrollContentsBy(dx, dy);
    if (d->selectMarkController)
        d->selectMarkController->onScrolled();
}

void NListView::mousePressEvent(QMouseEvent* event) {
    Q_D(NListView);
    d->pressedIndex = indexAt(event->pos());
    QListView::mousePressEvent(event);
    if (viewport())
        viewport()->update();
}

void NListView::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NListView);
    QListView::mouseReleaseEvent(event);
    d->pressedIndex = QModelIndex();
    if (viewport())
        viewport()->update();
}

void NListView::paintEvent(QPaintEvent* event) {
    Q_D(NListView);
    QListView::paintEvent(event);
    if (!viewport())
        return;
    if (isShowingPlaceholder()) {
        QPainter placeholder(viewport());
        if (placeholder.isActive()) {
            placeholder.setRenderHint(QPainter::Antialiasing);
            placeholder.setPen(d->placeholderTextColor());
            placeholder.drawText(viewport()->rect().adjusted(24, 24, -24, -24),
                                 Qt::AlignCenter | Qt::TextWordWrap,
                                 getPlaceholderText());
        }
        return;
    }
    if (d->selectMarkController)
        d->selectMarkController->paintViewportOverlay(viewport());
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
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, refresh));
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
