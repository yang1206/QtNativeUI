#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NPushButton.h>
#include "../private/npushbutton_p.h"

NPushButton::NPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new NPushButtonPrivate(this)), m_backgroundColor(100, 100, 100) {
    Q_D(NPushButton);
    m_colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
    m_colorAnimation->setDuration(200); // 设置动画持续时间为200毫秒

    m_rippleAnimation = new QPropertyAnimation(this, "rippleProgress", this);
    m_rippleAnimation->setDuration(1000);
    m_rippleAnimation->setStartValue(0.0);
    m_rippleAnimation->setEndValue(1.0);
    m_rippleAnimation->setEasingCurve(QEasingCurve::OutQuad);
}

NPushButton::NPushButton(const QString& text, QWidget* parent) : QPushButton(text, parent) {
    m_colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
    m_colorAnimation->setDuration(200); // 设置动画持续时间为200毫秒

    m_rippleAnimation = new QPropertyAnimation(this, "rippleProgress", this);
    m_rippleAnimation->setDuration(1000);
    m_rippleAnimation->setStartValue(0.0);
    m_rippleAnimation->setEndValue(1.0);
    m_rippleAnimation->setEasingCurve(QEasingCurve::OutQuad);
    setText(text);
}

NPushButton::~NPushButton() {}

void NPushButton::setBackgroundColor(const QColor& color) {
    m_backgroundColor = color;
    update(); // 更新绘制
}

void NPushButton::setRippleProgress(qreal progress) {
    m_rippleProgress = progress;
    update();
}

void NPushButton::paintEvent(QPaintEvent* event) {
    // Q_D(NPushButton);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect buttonRect = rect();
    painter.setPen(Qt::NoPen);

    if (!isEnabled()) {
        // 禁用状态使用灰色
        painter.setBrush(QColor(200, 200, 200));
    } else {
        painter.setBrush(m_backgroundColor);
    }

    painter.drawRoundedRect(buttonRect, 10, 10);

    if (m_rippleProgress > 0.0 && m_rippleProgress <= 1.0) {
        QPainterPath clipPath;
        clipPath.addRoundedRect(rect(), 10, 10);
        painter.setClipPath(clipPath);

        painter.setOpacity(0.3 * (1.0 - m_rippleProgress));
        painter.setBrush(QColor(255, 255, 255));

        qreal maxRadius = sqrt(pow(width(), 2) + pow(height(), 2));
        qreal radius    = m_rippleProgress * maxRadius;

        painter.drawEllipse(m_rippleCenter, radius, radius);
    }

    painter.setOpacity(1.0);
    painter.setPen(isEnabled() ? Qt::white : QColor(150, 150, 150));
    painter.drawText(buttonRect, Qt::AlignCenter, text());
}

void NPushButton::enterEvent(QEnterEvent* event) {
    if (isEnabled()) {
        m_isHovered = true;
        m_colorAnimation->stop();
        m_colorAnimation->setStartValue(m_backgroundColor);
        m_colorAnimation->setEndValue(QColor(120, 120, 120));
        m_colorAnimation->start();
    }
    QPushButton::enterEvent(event);
}

void NPushButton::leaveEvent(QEvent* event) {
    if (isEnabled()) {
        m_isHovered = false;
        m_colorAnimation->stop();
        m_colorAnimation->setStartValue(m_backgroundColor);
        m_colorAnimation->setEndValue(QColor(100, 100, 100));
        m_colorAnimation->start();
    }
    QPushButton::leaveEvent(event);
}

void NPushButton::mousePressEvent(QMouseEvent* event) {
    if (isEnabled()) {
        m_isPressed = true;
        m_colorAnimation->stop();
        m_colorAnimation->setStartValue(m_backgroundColor);
        m_colorAnimation->setEndValue(QColor(80, 80, 80));
        m_colorAnimation->start();

        // 记录点击位置并启动涟漪动画
        m_rippleCenter = event->pos();
        m_rippleAnimation->stop();
        m_rippleProgress = 0.0;
        m_rippleAnimation->start();
    }
    QPushButton::mousePressEvent(event);
}

void NPushButton::mouseReleaseEvent(QMouseEvent* event) {
    if (isEnabled()) {
        m_isPressed = false;
        m_colorAnimation->stop();
        m_colorAnimation->setStartValue(m_backgroundColor);
        m_colorAnimation->setEndValue(m_isHovered ? QColor(120, 120, 120) : QColor(100, 100, 100));
        m_colorAnimation->start();
    }
    QPushButton::mouseReleaseEvent(event);
}