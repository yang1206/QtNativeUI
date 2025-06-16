#include "NNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>

#include "NNavigationModel.h"
#include "NNavigationNode.h"
#include "NNavigationStyle.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolTip.h"

NNavigationView::NNavigationView(QWidget* parent) : QTreeView(parent) {
    setObjectName("NNavigationView");
    setStyleSheet("#NNavigationView{background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setExpandsOnDoubleClick(false);
    setAutoScroll(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    // 滚动条设置
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    NScrollBar* vScrollBar = new NScrollBar(this);
    connect(vScrollBar, &NScrollBar::rangeAnimationFinished, this, [=]() { doItemsLayout(); });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    NScrollBar* floatVScrollBar = new NScrollBar(verticalScrollBar(), this);
    floatVScrollBar->installEventFilter(this);

    _navigationStyle = new NNavigationStyle(this->style());
    _navigationStyle->setNavigationView(this);
    setStyle(_navigationStyle);

    // 设置滚动手势
    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller*          scroller   = QScroller::scroller(this->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed) {
            _navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            viewport()->update();
        } else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive) {
            _navigationStyle->setPressIndex(QModelIndex());
        }
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &NNavigationView::customContextMenuRequested, this, &NNavigationView::onCustomContextMenuRequested);

    _compactToolTip = new NToolTip();
}

NNavigationView::~NNavigationView() {}

void NNavigationView::navigationNodeStateChange(QVariantMap data) {
    this->_navigationStyle->navigationNodeStateChange(data);
}

void NNavigationView::onCustomContextMenuRequested(const QPoint& pos) {
    QModelIndex posIndex = indexAt(pos);
    if (!posIndex.isValid()) {
        return;
    }

    NNavigationNode* posNode = static_cast<NNavigationNode*>(posIndex.internalPointer());
    if (!posNode->getIsExpanderNode()) {
        QMenu    menu(this);
        QAction* openAction = menu.addAction("在新窗口中打开");
        connect(
            openAction, &QAction::triggered, this, [=]() { Q_EMIT navigationOpenNewWindow(posNode->getNodeKey()); });
        menu.exec(mapToGlobal(pos));
    }
}

void NNavigationView::mouseMoveEvent(QMouseEvent* event) {
    if (width() <= 60) {
        QModelIndex posIndex = indexAt(event->pos());
        if (!posIndex.isValid()) {
            _compactToolTip->hide();
            return;
        }

        NNavigationNode* posNode = static_cast<NNavigationNode*>(posIndex.internalPointer());
        _compactToolTip->setText(posNode->getNodeTitle());
        _compactToolTip->move(mapToGlobal(QPoint(width() + 5, event->pos().y() - _compactToolTip->height() / 2)));
        _compactToolTip->show();
    } else {
        _compactToolTip->hide();
    }

    QTreeView::mouseMoveEvent(event);
}

void NNavigationView::mouseDoubleClickEvent(QMouseEvent* event) {
    _navigationStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QTreeView::mouseDoubleClickEvent(event);
}

void NNavigationView::mouseReleaseEvent(QMouseEvent* event) {
    QTreeView::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            Q_EMIT navigationClicked(index);
        }

        _navigationStyle->setPressIndex(QModelIndex());
    }
}

bool NNavigationView::eventFilter(QObject* watched, QEvent* event) {
    switch (event->type()) {
        case QEvent::MouseMove:
        case QEvent::HoverMove: {
            if (width() <= 60) {
                QModelIndex posIndex = indexAt(mapFromGlobal(QCursor::pos()));
                if (!posIndex.isValid()) {
                    _compactToolTip->hide();
                    break;
                }

                NNavigationNode* posNode = static_cast<NNavigationNode*>(posIndex.internalPointer());
                _compactToolTip->setText(posNode->getNodeTitle());
                _compactToolTip->move(
                    mapToGlobal(QPoint(width() + 5, QCursor::pos().y() - _compactToolTip->height() / 2)));
                _compactToolTip->show();
            } else {
                _compactToolTip->hide();
            }
            break;
        }
        default: {
            break;
        }
    }

    return QAbstractItemView::eventFilter(watched, event);
}