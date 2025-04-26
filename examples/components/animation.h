// examples/components/animation.h
#ifndef ANIMATION_EXAMPLE_H
#define ANIMATION_EXAMPLE_H

#include <QPainter>
#include <QWidget>
#include <QtNativeUI/NAnimation.h>

class QPushButton;
class QLabel;
class QScrollArea;

class AnimationDisplayWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

  public:
    AnimationDisplayWidget(QWidget* parent = nullptr)
        : QWidget(parent), m_position(0, 0), m_scale(1.0), m_angle(0), m_opacity(1.0) {
        setMinimumSize(60, 60);
        setFixedSize(60, 60);
        setStyleSheet("background-color: #3498db; border-radius: 5px;");
    }

    QPointF position() const { return m_position; }
    void    setPosition(const QPointF& pos) {
        m_position = pos;
        update();
    }

    qreal scale() const { return m_scale; }
    void  setScale(qreal scale) {
        m_scale = scale;
        update();
    }

    qreal angle() const { return m_angle; }
    void  setAngle(qreal angle) {
        m_angle = angle;
        update();
    }

    qreal opacity() const { return m_opacity; }
    void  setOpacity(qreal opacity) {
        m_opacity = opacity;
        update();
    }

  protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 应用透明度
        painter.setOpacity(m_opacity);

        // 应用旋转和缩放
        painter.translate(width() / 2, height() / 2);
        painter.rotate(m_angle);
        painter.scale(m_scale, m_scale);
        painter.translate(-width() / 2, -height() / 2);

        // 应用位置偏移
        painter.translate(m_position.x(), m_position.y());

        // 绘制矩形
        painter.fillRect(0, 0, width(), height(), QColor("#3498db"));
    }

  private:
    QPointF m_position;
    qreal   m_scale;
    qreal   m_angle;
    qreal   m_opacity;
};

class AnimationExample : public QWidget {
    Q_OBJECT
  public:
    explicit AnimationExample(QWidget* parent = nullptr);

  private:
    void     initUI();
    QWidget* createPropertyAnimations();

    void setupAnimation(QPushButton*                             button,
                        AnimationDisplayWidget*                  targetWidget,
                        QtNativeUI::NAnimationType::Type         type,
                        QtNativeUI::NAnimationProperty::Property property);

    QScrollArea* m_scrollArea;
};

#endif // ANIMATION_EXAMPLE_H