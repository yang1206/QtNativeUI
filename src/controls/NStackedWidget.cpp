//
// Created by Yang1206 on 2025/6/22.
//

#include "QtNativeUI/NStackedWidget.h"
#

NStackedWidget::NStackedWidget(QWidget* parent) : QStackedWidget(parent) {
    setObjectName("NStackedWidget");
    m_borderRadius = 10;
    m_background   = NThemeColor(NFluentColorKey::LayerFillColorDefault, nTheme->themeMode());
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        m_background = NThemeColor(NFluentColorKey::LayerFillColorDefault, mode);
        m_themeMode  = mode;
        update();
    });
}

NStackedWidget::~NStackedWidget() {}

void NStackedWidget::setBackGroundColor(QColor color) {
    m_background = color;
    update();
}

void NStackedWidget::setBorderRadius(int radius) {
    m_borderRadius = radius;
    update();
}

void NStackedWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QRect    targetRect = this->rect();
    targetRect.adjust(1, 1, 10, 10);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, m_themeMode), 1.5));
    painter.setBrush(m_background);
    if (m_borderRadius >= 1) {
        painter.drawRoundedRect(targetRect, m_borderRadius, m_borderRadius);
    } else {
        painter.drawRect(targetRect);
    }
    painter.restore();
    QStackedWidget::paintEvent(event);
}