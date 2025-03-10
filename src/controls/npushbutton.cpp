#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtCore/qcoreevent.h>
#include <QtMath>
#include <QtNativeUI/NPushButton.h>
#include <QMouseEvent>
#include "../private/npushbutton_p.h"


namespace QtNativeUI {

// 实现 buttonStyle 属性
void NPushButton::setButtonStyle(NPushButtonStyle value)
{
    Q_D(NPushButton);
    if (d->_buttonStyle != value) {
        d->_buttonStyle = value;
        d->initStyleColors();
        update();
        Q_EMIT buttonStyleChanged();
    }
}

NPushButton::NPushButtonStyle NPushButton::buttonStyle() const
{
    Q_D(const NPushButton);
    return d->_buttonStyle;
}

// 构造函数
NPushButton::NPushButton(QWidget* parent)
    : QPushButton(parent)
    , d_ptr(new NPushButtonPrivate(this))
{
    // 设置无边框风格，因为我们要自己绘制
    setFlat(true);
    
    // 焦点策略
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置默认字体
    QFont font = this->font();
    font.setPixelSize(14);
    setFont(font);
    
    // 属性初始化
    setCursor(Qt::PointingHandCursor);
    
    // 更新外观
    updateFromTheme();
}

NPushButton::NPushButton(const QString& text, QWidget* parent)
    : NPushButton(parent)
{
    setText(text);
}

NPushButton::~NPushButton()
{
    // QScopedPointer 会自动清理 d_ptr
}

void NPushButton::resetAppearance()
{
    Q_D(NPushButton);
    
    // 重置自定义颜色
    _backgroundColor = QColor();
    _textColor = QColor();
    _borderColor = QColor();
    
    // 重新从主题获取颜色
    d->initStyleColors();
    update();
    
    emit backgroundColorChanged();
    emit textColorChanged();
    emit borderColorChanged();
}

void NPushButton::updateFromTheme()
{
    Q_D(NPushButton);
    d->initStyleColors();
    update();
}

QSize NPushButton::sizeHint() const
{
    // 计算文本尺寸
    QFontMetrics fm(font());
    QSize textSize = fm.size(Qt::TextShowMnemonic, text());
    
    // 基本尺寸
    int baseWidth = textSize.width() + 32;  // 左右各16像素的内边距
    int baseHeight = textSize.height() + 16; // 上下各8像素的内边距
    
    // 确保最小尺寸
    return QSize(qMax(baseWidth, 80), qMax(baseHeight, 32));
}

void NPushButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(NPushButton);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect rect = this->rect();
    qreal radius = _cornerRadius;
    
    // 创建圆角矩形路径
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    
    // 绘制阴影 (如果启用)
    if (_elevationEnabled && isEnabled() && d->_buttonState != NPushButtonPrivate::ButtonState::Pressed) {
        QColor shadowColor = NTheme::instance()->color(NColorRole::Shadow);
        int shadowOffsetY = 2;
        int shadowBlurRadius = 4;
        
        // 简单阴影效果
        if (d->_buttonStyle == NPushButtonStyle::Primary ||
            d->_buttonStyle == NPushButtonStyle::Secondary ||
            d->_buttonStyle == NPushButtonStyle::Danger) {
            
            QPainterPath shadowPath;
            shadowPath.addRoundedRect(rect.adjusted(0, 0, 0, shadowOffsetY), radius, radius);
            
            QColor blurColor(shadowColor);
            for (int i = shadowBlurRadius; i > 0; i--) {
                blurColor.setAlpha(10 + i * 5);
                painter.setPen(Qt::NoPen);
                painter.setBrush(blurColor);
                painter.drawPath(shadowPath.translated(0, i * 0.5));
            }
        }
    }
    
    // 绘制背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->currentBackgroundColor());
    painter.drawPath(path);
    
    // 绘制边框 (如果不是透明)
    if (d->currentBorderColor() != Qt::transparent) {
        painter.setPen(QPen(d->currentBorderColor(), 1.0));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }
    
    // 绘制文本
    painter.setPen(d->currentTextColor());
    painter.setBrush(Qt::NoBrush);
    painter.drawText(rect, Qt::AlignCenter, text());
    
    // 绘制图标 (如果有)
    if (!icon().isNull()) {
        QIcon::Mode mode = isEnabled() ? QIcon::Normal : QIcon::Disabled;
        QIcon::State state = isChecked() ? QIcon::On : QIcon::Off;
        QSize iconSize = this->iconSize();
        
        // 文本对齐
        Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter;
        
        // 计算图标位置
        int iconSpacing = 8;  // 图标和文本之间的间距
        int textWidth = painter.fontMetrics().horizontalAdvance(text());
        int contentWidth = iconSize.width() + iconSpacing + textWidth;
        int x = (rect.width() - contentWidth) / 2;
        QRect iconRect(x, (rect.height() - iconSize.height()) / 2, 
                       iconSize.width(), iconSize.height());
        
        // 绘制图标
        icon().paint(&painter, iconRect, alignment, mode, state);
        
        // 重新绘制文本（考虑图标）
        QRect textRect(x + iconSize.width() + iconSpacing, 0, 
                       textWidth, rect.height());
        painter.drawText(textRect, Qt::AlignVCenter, text());
    }
}

void NPushButton::enterEvent(QEnterEvent* event)
{
    Q_D(NPushButton);
    d->_buttonState = NPushButtonPrivate::ButtonState::Hover;
    update();
    QPushButton::enterEvent(event);
}

void NPushButton::leaveEvent(QEvent* event)
{
    Q_D(NPushButton);
    d->_buttonState = NPushButtonPrivate::ButtonState::Normal;
    update();
    QPushButton::leaveEvent(event);
}

void NPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(NPushButton);
    if (event->button() == Qt::LeftButton) {
        d->_buttonState = NPushButtonPrivate::ButtonState::Pressed;
        update();
    }
    QPushButton::mousePressEvent(event);
}

void NPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(NPushButton);
    if (event->button() == Qt::LeftButton) {
        QRect rect = this->rect();
        if (rect.contains(event->pos())) {
            d->_buttonState = NPushButtonPrivate::ButtonState::Hover;
        } else {
            d->_buttonState = NPushButtonPrivate::ButtonState::Normal;
        }
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void NPushButton::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::EnabledChange) {
        update();
    }
    QPushButton::changeEvent(event);
}

} // namespace QtNativeUI ·