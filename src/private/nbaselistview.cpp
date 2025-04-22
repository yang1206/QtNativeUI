//
// Created by Yang1206 on 2025/4/23.
//

#include "nbaselistview.h"
#include <QMouseEvent>
#include "QtNativeUI/NScrollBar.h"
NBaseListView::NBaseListView(QWidget* parent) : QListView(parent) {
    setObjectName("NBaseListView");
    setStyleSheet("NBaseListView{background-color: transparent;border:0px;}"
                  "NBaseListView::item{border:none;}");
    setAutoScroll(false);
    setFocusPolicy(Qt::NoFocus);
    setVerticalScrollBar(new NScrollBar(this));
    setHorizontalScrollBar(new NScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

NBaseListView::~NBaseListView() {}

void NBaseListView::mousePressEvent(QMouseEvent* event) {
    Q_EMIT mousePress(indexAt(event->pos()));
    QListView::mousePressEvent(event);
}

void NBaseListView::mouseReleaseEvent(QMouseEvent* event) {
    Q_EMIT mouseRelease(indexAt(event->pos()));
    QListView::mouseReleaseEvent(event);
}

void NBaseListView::mouseDoubleClickEvent(QMouseEvent* event) {
    Q_EMIT mouseDoubleClick(indexAt(event->pos()));
    QListView::mouseDoubleClickEvent(event);
}
