#include "animation.h"
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QtNativeUI/NAnimation.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NScrollArea.h>

using namespace QtNativeUI;

AnimationExample::AnimationExample(QWidget* parent) : QWidget(parent) { initUI(); }

void AnimationExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 创建滚动区域
    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);

    // 创建内容容器
    QWidget*     contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(20);

    // 添加各种动画示例
    contentLayout->addWidget(createPropertyAnimations());

    // 设置滚动区域的内容
    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
}

QWidget* AnimationExample::createPropertyAnimations() {
    QGroupBox*   groupBox = new QGroupBox("属性动画", this);
    QVBoxLayout* layout   = new QVBoxLayout(groupBox);

    QLabel* description = new QLabel("各种流畅动画效果（注意观察不同动画类型的速度和缓动效果）:");
    description->setWordWrap(true);
    layout->addWidget(description);

    // 创建网格布局以展示各种动画
    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(15);

    // 行标题 - 动画属性
    QStringList propertyLabels = {"位置动画", "缩放动画", "角度动画", "透明度动画"};

    // 列标题 - 动画类型
    QStringList typeLabels = {"快速调用", "强调调用", "快速消失", "软消失", "点到点", "淡入淡出"};

    // 添加行标题
    for (int row = 0; row < propertyLabels.size(); ++row) {
        QLabel* label = new QLabel(propertyLabels[row]);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        grid->addWidget(label, row + 1, 0);
    }

    // 添加列标题
    for (int col = 0; col < typeLabels.size(); ++col) {
        QLabel* label = new QLabel(typeLabels[col]);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        grid->addWidget(label, 0, col + 1);
    }

    // 创建动画演示矩阵
    NAnimationType::Type types[] = {NAnimationType::FastInvoke,
                                    NAnimationType::StrongInvoke,
                                    NAnimationType::FastDismiss,
                                    NAnimationType::SoftDismiss,
                                    NAnimationType::PointToPoint,
                                    NAnimationType::FadeInOut};

    NAnimationProperty::Property properties[] = {NAnimationProperty::Position,
                                                 NAnimationProperty::Scale,
                                                 NAnimationProperty::Angle,
                                                 NAnimationProperty::Opacity};

    // 为每种动画类型和属性创建一个按钮和目标显示部件
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 6; ++col) {
            QWidget*     container       = new QWidget();
            QVBoxLayout* containerLayout = new QVBoxLayout(container);
            containerLayout->setContentsMargins(0, 0, 0, 0);

            // 创建一个按钮来触发动画
            NPushButton* button = new NPushButton("播放");
            button->setFixedSize(60, 30);

            // 创建一个用于显示动画效果的自定义部件
            AnimationDisplayWidget* displayWidget = new AnimationDisplayWidget();

            // 设置动画
            setupAnimation(button, displayWidget, types[col], properties[row]);

            containerLayout->addWidget(button);
            containerLayout->addWidget(displayWidget, 0, Qt::AlignCenter);

            grid->addWidget(container, row + 1, col + 1);
        }
    }

    layout->addLayout(grid);
    return groupBox;
}

void AnimationExample::setupAnimation(QPushButton*                 button,
                                      AnimationDisplayWidget*      displayWidget,
                                      NAnimationType::Type         type,
                                      NAnimationProperty::Property property) {
    // 创建动画
    QPropertyAnimation* animation = nullptr;

    // 根据动画类型创建对应的动画对象
    switch (type) {
        case NAnimationType::FastInvoke:
            animation = new NFastInvokeAnimation(displayWidget);
            break;
        case NAnimationType::StrongInvoke:
            animation = new NStrongInvokeAnimation(displayWidget);
            break;
        case NAnimationType::FastDismiss:
            animation = new NFastDismissAnimation(displayWidget);
            break;
        case NAnimationType::SoftDismiss:
            animation = new NSoftDismissAnimation(displayWidget);
            break;
        case NAnimationType::PointToPoint:
            animation = new NPointToPointAnimation(displayWidget);
            break;
        case NAnimationType::FadeInOut:
            animation = new NFadeInOutAnimation(displayWidget);
            break;
    }

    if (!animation)
        return;

    // 根据属性类型设置动画参数
    QVariant startValue, endValue;
    QString  propertyName;

    switch (property) {
        case NAnimationProperty::Position:
            propertyName = "position";
            startValue   = QPointF(0, 0);
            endValue     = QPointF(20, 20); // 更明显的位置变化
            break;
        case NAnimationProperty::Scale:
            propertyName = "scale";
            startValue   = 1.0;
            endValue     = 2.0; // 更明显的缩放
            break;
        case NAnimationProperty::Angle:
            propertyName = "angle";
            startValue   = 0.0;
            endValue     = 360.0;
            break;
        case NAnimationProperty::Opacity:
            propertyName = "opacity";
            startValue   = 1.0;
            endValue     = 0.1; // 更明显的透明度变化
            break;
    }

    animation->setTargetObject(displayWidget);
    animation->setPropertyName(propertyName.toLatin1());
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);

    // 连接按钮点击信号启动动画
    connect(button, &QPushButton::clicked, this, [animation, startValue, displayWidget, property, propertyName]() {
        // 停止当前运行的动画并重置到初始状态
        animation->stop();
        // 重置属性到初始值
        switch (property) {
            case NAnimationProperty::Position:
                displayWidget->setPosition(QPointF(0, 0));
                break;
            case NAnimationProperty::Scale:
                displayWidget->setScale(1.0);
                break;
            case NAnimationProperty::Angle:
                displayWidget->setAngle(0.0);
                break;
            case NAnimationProperty::Opacity:
                displayWidget->setOpacity(1.0);
                break;
        }
        // 启动动画
        animation->start();
        // 输出动画信息以便调试
        // qDebug() << "Starting animation:" << "Type:" << reinterpret_cast<int>(animation->metaObject()->className())
        //          << "Property:" << propertyName << "Duration:" << animation->duration() << "ms";
    });

    // 完成后重置动画（对于某些类型的动画）
    if (property == NAnimationProperty::Opacity || property == NAnimationProperty::Scale) {
        connect(animation, &QPropertyAnimation::finished, this, [animation, startValue, displayWidget, property]() {
            QTimer::singleShot(1000, [animation, startValue, displayWidget, property]() {
                // 确保自动往回动画前重置方向
                animation->setDirection(QAbstractAnimation::Forward);

                // 设置开始和结束值（有时需要重新设置）
                switch (property) {
                    case NAnimationProperty::Scale:
                        animation->setStartValue(2.0);
                        animation->setEndValue(1.0);
                        break;
                    case NAnimationProperty::Opacity:
                        animation->setStartValue(0.1);
                        animation->setEndValue(1.0);
                        break;
                    default:
                        break;
                }

                animation->start();
            });
        });
    }
}