#include "nscrollarea_p.h"
#include "QtNativeUI/NScrollArea.h"

NScrollAreaPrivate::NScrollAreaPrivate(QObject* parent) : QObject(parent) {}

NScrollAreaPrivate::~NScrollAreaPrivate() {}

void NScrollAreaPrivate::updateContentRect() {
    if (!q_ptr) {
        return;
    }

    QWidget* viewport = q_ptr->viewport();
    if (!viewport) {
        return;
    }

    // 获取视口的内容区域
    _contentRect = viewport->rect();

    // 应用边距
    _contentRect.adjust(
        _viewportMargins.left(), _viewportMargins.top(), -_viewportMargins.right(), -_viewportMargins.bottom());
}