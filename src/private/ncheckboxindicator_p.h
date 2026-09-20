#ifndef NCHECKBOXINDICATOR_P_H
#define NCHECKBOXINDICATOR_P_H

#include <QRect>
#include <Qt>

class QPainter;

namespace NCheckBoxIndicator {
void paint(QPainter* painter, const QRect& rect, Qt::CheckState state, bool enabled, bool hovered,
           bool isDark, int borderRadius = 4);
}

#endif
