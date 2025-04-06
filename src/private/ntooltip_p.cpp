//
// Created by Yang1206 on 2025/4/6.
//

#include "ntooltip_p.h"
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolTip.h"

QPoint NToolTipPositionManager::position(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = calculatePosition(tooltip, parent);
    int    x = pos.x(), y = pos.y();

    QScreen* screen = QApplication::screenAt(pos);
    if (!screen) {
        screen = QApplication::primaryScreen();
    }

    QRect screenGeometry = screen->availableGeometry();
    x                    = qMax(screenGeometry.left(), qMin(x, screenGeometry.right() - tooltip->width() - 4));
    y                    = qMax(screenGeometry.top(), qMin(y, screenGeometry.bottom() - tooltip->height() - 4));

    return QPoint(x, y);
}

NToolTipPositionManager* NToolTipPositionManager::create(NToolTipPosition position) {
    switch (position) {
        case NToolTipPosition::TOP:
            return new TopToolTipManager();
        case NToolTipPosition::BOTTOM:
            return new BottomToolTipManager();
        case NToolTipPosition::LEFT:
            return new LeftToolTipManager();
        case NToolTipPosition::RIGHT:
            return new RightToolTipManager();
        case NToolTipPosition::TOP_LEFT:
            return new TopLeftToolTipManager();
        case NToolTipPosition::TOP_RIGHT:
            return new TopRightToolTipManager();
        case NToolTipPosition::BOTTOM_LEFT:
            return new BottomLeftToolTipManager();
        case NToolTipPosition::BOTTOM_RIGHT:
            return new BottomRightToolTipManager();
        default:
            return new TopToolTipManager();
    }
}

QPoint TopToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() + parent->width() / 2 - tooltip->width() / 2;
    int    y   = pos.y() - tooltip->height();
    return QPoint(x, y);
}

QPoint BottomToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() + parent->width() / 2 - tooltip->width() / 2;
    int    y   = pos.y() + parent->height();
    return QPoint(x, y);
}

QPoint LeftToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() - tooltip->width();
    int    y   = pos.y() + (parent->height() - tooltip->height()) / 2;
    return QPoint(x, y);
}

QPoint RightToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() + parent->width();
    int    y   = pos.y() + (parent->height() - tooltip->height()) / 2;
    return QPoint(x, y);
}

QPoint TopLeftToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() - tooltip->layout()->contentsMargins().left();
    int    y   = pos.y() - tooltip->height();
    return QPoint(x, y);
}

QPoint TopRightToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() + parent->width() - tooltip->width() + tooltip->layout()->contentsMargins().right();
    int    y   = pos.y() - tooltip->height();
    return QPoint(x, y);
}

QPoint BottomLeftToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() - tooltip->layout()->contentsMargins().left();
    int    y   = pos.y() + parent->height();
    return QPoint(x, y);
}

QPoint BottomRightToolTipManager::calculatePosition(NToolTip* tooltip, QWidget* parent) {
    QPoint pos = parent->mapToGlobal(QPoint());
    int    x   = pos.x() + parent->width() - tooltip->width() + tooltip->layout()->contentsMargins().right();
    int    y   = pos.y() + parent->height();
    return QPoint(x, y);
}

NToolTipPrivate::NToolTipPrivate(QObject* parent) : QObject(parent) {
    timer = new QTimer(this);
    timer->setSingleShot(true);

    opacityAnimation = new QPropertyAnimation(this);
    opacityAnimation->setDuration(150);

    shadowEffect = new QGraphicsDropShadowEffect(this);
}

NToolTipPrivate::~NToolTipPrivate() {}

void NToolTipPrivate::updateStyle() {
    if (!q_ptr)
        return;

    QColor bgColor     = isDark ? _pDarkBackgroundColor : _pLightBackgroundColor;
    QColor textColor   = isDark ? _pDarkTextColor : _pLightTextColor;
    QColor borderColor = isDark ? _pDarkBorderColor : _pLightBorderColor;

    QString containerStyle = QString("QWidget#container {"
                                     "  background-color: %1;"
                                     "  border: 1px solid %2;"
                                     "  border-radius: %3px;"
                                     "}")
                                 .arg(bgColor.name(), borderColor.name(), QString::number(_pBorderRadius));

    // 设置标签样式
    QString labelStyle = QString("QLabel#contentLabel {"
                                 "  color: %1;"
                                 "  font-size: %2px;"
                                 "  padding: %3px;"
                                 "}")
                             .arg(textColor.name(),
                                  QString::number(NDesignToken(NDesignTokenKey::FontSizeBody).toInt()),
                                  QString::number(_pContentPadding));

    container->setStyleSheet(containerStyle);
    label->setStyleSheet(labelStyle);

    shadowEffect->setColor(QColor(0, 0, 0, 50));
    shadowEffect->setBlurRadius(_pShadowRadius);
    shadowEffect->setOffset(0, 5);
}