#ifndef QTNATIVEUI_NPUSHBUTTON_H
#define QTNATIVEUI_NPUSHBUTTON_H

#include <QPropertyAnimation>
#include <QPushButton>

#include "stdafx.h"

class NPushButtonPrivate;
class QTNATIVEUI_EXPORT NPushButton : public QPushButton {
    Q_OBJECT
    N_DECLARE_PRIVATE(NPushButton)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(qreal rippleProgress READ rippleProgress WRITE setRippleProgress)

  public:
    explicit NPushButton(QWidget* parent = nullptr);
    explicit NPushButton(const QString& text, QWidget* parent = nullptr);
    ~NPushButton();

    QColor backgroundColor() const { return m_backgroundColor; }
    void   setBackgroundColor(const QColor& color);
    qreal  rippleProgress() const { return m_rippleProgress; }
    void   setRippleProgress(qreal progress);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

  private:
    bool                m_isHovered = false;
    bool                m_isPressed = false;
    QColor              m_backgroundColor;
    QPropertyAnimation* m_colorAnimation;

    QPropertyAnimation* m_rippleAnimation;
    QPointF             m_rippleCenter;
    qreal               m_rippleProgress = 0.0;
};

#endif // QTNATIVEUI_NPUSHBUTTON_H