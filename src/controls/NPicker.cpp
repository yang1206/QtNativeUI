#include <QPainterPath>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QtMath>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NPicker.h>
#include <QtNativeUI/NTheme.h>
#include "../private/npicker_p.h"

Q_PROPERTY_CREATE_Q_CPP(NPicker, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NPicker, bool, Container)
Q_PROPERTY_CREATE_Q_CPP(NPicker, bool, LoopEnabled)

NPicker::NPicker(QWidget* parent) : QWidget{parent}, d_ptr(new NPickerPrivate()) {
    Q_D(NPicker);
    d->q_ptr             = this;
    d->_pScrollOffset    = 0;
    d->_pItemHeight      = 30;
    d->_pBorderRadius    = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pVisibleItemCount = 5;
    d->_pCurrentIndex    = 0;
    d->_pLoopEnabled    = true;
    d->_pContainer     = false;
    d->_themeMode        = nTheme->themeMode();

    setFixedSize(70, d->_pVisibleItemCount * d->_pItemHeight);
    setMouseTracking(true);
    setObjectName("NPicker");
    setStyleSheet("#NPicker{background-color:transparent;}");

    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    d->_scrollAnimation = new QPropertyAnimation(d, "pScrollOffset");
    connect(d->_scrollAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
    connect(d->_scrollAnimation, &QPropertyAnimation::finished, this, [=]() {
        while (d->_pScrollOffset < -d->_pItems.size() * d->_pItemHeight) {
            d->_pScrollOffset += d->_pItems.size() * d->_pItemHeight;
        }
        while (d->_pScrollOffset >= d->_pItems.size() * d->_pItemHeight) {
            d->_pScrollOffset -= d->_pItems.size() * d->_pItemHeight;
        }
        d->_targetScrollOffset = d->_pScrollOffset;

        int currentIndex = 0;
        if (d->_pScrollOffset >= 0) {
            currentIndex = d->_pScrollOffset / d->_pItemHeight;
        } else if (d->_pScrollOffset <= -d->_pItemHeight) {
            currentIndex = d->_pItems.count() + d->_pScrollOffset / d->_pItemHeight;
        }

        if (currentIndex != d->_pCurrentIndex) {
            d->_pCurrentIndex = currentIndex;
            Q_EMIT pCurrentIndexChanged();
            Q_EMIT currentDataChanged(getCurrentData());
        }
        update();
    });

    d->_scrollAnimation->setDuration(NDesignToken(NDesignTokenKey::AnimationNormal).toInt());
    d->_scrollAnimation->setEasingCurve(NDesignToken(NDesignTokenKey::EasingStandard).value<QEasingCurve>());

    d->_pressSustainTimer = new QTimer(this);
    d->_pressSustainTimer->setInterval(300);
    connect(d->_pressSustainTimer, &QTimer::timeout, this, [=]() { d->_repeatScrollTimer->start(); });

    d->_repeatScrollTimer = new QTimer(this);
    d->_repeatScrollTimer->setInterval(50);
    connect(d->_repeatScrollTimer, &QTimer::timeout, this, [=]() {
        if (d->_isUpArrowPress) {
            d->_scroll(120);
        }
        if (d->_isDownArrowPress) {
            d->_scroll(-120);
        }
    });

    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

NPicker::~NPicker() {}

void NPicker::setCurrentData(const QString& data) {
    Q_D(NPicker);
    if (d->_pItems.contains(data)) {
        setCurrentIndex(d->_pItems.indexOf(data));
    }
}

QString NPicker::getCurrentData() const {
    Q_D(const NPicker);
    if (d->_pCurrentIndex >= d->_pItems.count()) {
        return {};
    }
    return d->_pItems[d->_pCurrentIndex];
}

void NPicker::setItems(QStringList Items) {
    Q_D(NPicker);
    d->_pItems = Items;
    update();
    Q_EMIT pItemsChanged();
}

QStringList NPicker::getItems() const {
    Q_D(const NPicker);
    return d->_pItems;
}

void NPicker::setItemHeight(int itemHeight) {
    Q_D(NPicker);
    d->_pItemHeight = itemHeight;
    setFixedHeight(d->_pVisibleItemCount * d->_pItemHeight);
    update();
    Q_EMIT pItemHeightChanged();
}

int NPicker::getItemHeight() const {
    Q_D(const NPicker);
    return d->_pItemHeight;
}

void NPicker::setVisibleItemCount(int VisibleItemCount) {
    Q_D(NPicker);
    d->_pVisibleItemCount = VisibleItemCount;
    setFixedHeight(d->_pVisibleItemCount * d->_pItemHeight);
    update();
    Q_EMIT pVisibleItemCountChanged();
}

int NPicker::getVisibleItemCount() const {
    Q_D(const NPicker);
    return d->_pVisibleItemCount;
}

void NPicker::setCurrentIndex(int currentIndex) {
    Q_D(NPicker);
    if (currentIndex >= d->_pItems.count()) {
        return;
    }
    d->_pCurrentIndex      = currentIndex;
    d->_pScrollOffset      = d->_pItemHeight * currentIndex;
    d->_targetScrollOffset = d->_pScrollOffset;
    update();
}

int NPicker::getCurrentIndex() const {
    Q_D(const NPicker);
    return d->_pCurrentIndex;
}

void NPicker::wheelEvent(QWheelEvent* event) {
    Q_D(NPicker);
    d->_scroll(event->angleDelta().y());
    event->accept();
}

void NPicker::mousePressEvent(QMouseEvent* event) {
    Q_D(NPicker);
    if (d->_pContainer && d->_pVisibleItemCount >= 5) {
        auto currentPos = event->pos();
        if (d->_upArrowRect.contains(currentPos)) {
            d->_isUpArrowPress = true;
            d->_scroll(120);
            d->_pressSustainTimer->start();
        } else if (d->_downArrowRect.contains(currentPos)) {
            d->_isDownArrowPress = true;
            d->_scroll(-120);
            d->_pressSustainTimer->start();
        }
    }
    QWidget::mousePressEvent(event);
}

void NPicker::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NPicker);
    int centerIndex = d->_pVisibleItemCount / 2;
    int clickIndex  = event->pos().y() / d->_pItemHeight;
    int jumpCount   = abs(d->_pVisibleItemCount / 2 - clickIndex);

    if (d->_isUpArrowPress || d->_isDownArrowPress) {
        d->_pressSustainTimer->stop();
        d->_repeatScrollTimer->stop();
        d->_isUpArrowPress   = false;
        d->_isDownArrowPress = false;
        update();
    } else {
        if (clickIndex > centerIndex) {
            for (int i = 0; i < jumpCount; i++) {
                d->_scroll(-120);
            }
        } else if (clickIndex < centerIndex) {
            for (int i = 0; i < jumpCount; i++) {
                d->_scroll(120);
            }
        }
    }
    update();
}

void NPicker::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NPicker);
    d->_mousePoint = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void NPicker::leaveEvent(QEvent* event) {
    Q_D(NPicker);
    d->_mousePoint = QPoint();
    update();
    QWidget::leaveEvent(event);
}

void NPicker::paintEvent([[maybe_unused]] QPaintEvent* event) {
    Q_D(NPicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect foregroundRect(d->_penBorderWidth,
                         d->_penBorderWidth,
                         width() - 2 * d->_penBorderWidth,
                         height() - 2 * d->_penBorderWidth - 1);

    if (!d->_pContainer) {
        painter.setPen(
            QPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, d->_themeMode), d->_penBorderWidth));
        painter.setBrush(isEnabled() ? NThemeColor(NFluentColorKey::ControlFillColorDefault, d->_themeMode)
                                     : NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode));
        painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    }

    painter.setClipRect(foregroundRect);

    painter.setPen(Qt::NoPen);
    painter.setBrush(nTheme->accentColor().normal());

    bool isEven = d->_pVisibleItemCount % 2 == 0;
    if (isEven) {
        painter.drawRoundedRect(QRect(d->_indicatorMargin,
                                      d->_pItemHeight * d->_pVisibleItemCount / 2 + d->_indicatorMargin,
                                      width() - 2 * d->_indicatorMargin,
                                      d->_pItemHeight - 2 * d->_indicatorMargin),
                                d->_pBorderRadius,
                                d->_pBorderRadius);
    } else {
        painter.drawRoundedRect(QRect(d->_indicatorMargin,
                                      (height() - d->_pItemHeight) / 2 + d->_indicatorMargin,
                                      width() - 2 * d->_indicatorMargin,
                                      d->_pItemHeight - 2 * d->_indicatorMargin),
                                d->_pBorderRadius,
                                d->_pBorderRadius);
    }

    int yStart           = -d->_pItemHeight;
    int yEnd             = height();
    int centerYStart     = isEven ? d->_pItemHeight * d->_pVisibleItemCount / 2 : height() / 2 - d->_pItemHeight;
    int centerYEnd       = isEven ? d->_pItemHeight * d->_pVisibleItemCount / 2 + d->_pItemHeight / 2 : height() / 2;
    int startIndexOffset = d->_pVisibleItemCount / 2;

    for (int i = 0; i < d->_pItems.size(); i++) {
        qreal y = (i + startIndexOffset) * d->_pItemHeight - d->_pScrollOffset;

        if (d->_pLoopEnabled) {
            while (y <= yStart) {
                y += d->_pItems.size() * d->_pItemHeight;
            }
            while (y >= yEnd) {
                y -= d->_pItems.size() * d->_pItemHeight;
            }
        }

        if (y >= yStart && y <= yEnd) {
            painter.save();
            painter.translate(0, y);

            if (y >= centerYStart && y <= centerYEnd) {
                painter.setPen(NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode));
            } else {
                if (!d->_mousePoint.isNull() && d->_scrollAnimation->state() == QPropertyAnimation::State::Stopped) {
                    QRectF itemRect = QRectF(0, y, width(), d->_pItemHeight);
                    if (itemRect.contains(d->_mousePoint)) {
                        painter.setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, d->_themeMode));
                        painter.drawRoundedRect(QRect(d->_indicatorMargin,
                                                      d->_indicatorMargin,
                                                      width() - 2 * d->_indicatorMargin,
                                                      d->_pItemHeight - 2 * d->_indicatorMargin),
                                                d->_pBorderRadius,
                                                d->_pBorderRadius);
                    }
                }
                painter.setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, d->_themeMode));
            }

            painter.drawText(
                QRect(0, 0, width(), d->_pItemHeight), Qt::AlignCenter | Qt::TextSingleLine, d->_pItems[i]);
            painter.restore();
        }
    }

    if (d->_pContainer && d->_pVisibleItemCount >= 5 && underMouse()) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, d->_themeMode));

        d->_upArrowRect   = QRect(foregroundRect.x(), foregroundRect.y(), foregroundRect.width(), d->_pItemHeight);
        d->_downArrowRect = QRect(
            foregroundRect.x(), foregroundRect.bottom() - d->_pItemHeight + 2, foregroundRect.width(), d->_pItemHeight);

        QPainterPath upPath;
        upPath.moveTo(d->_upArrowRect.bottomLeft());
        upPath.lineTo(d->_upArrowRect.topLeft() + QPointF(0, d->_pBorderRadius));
        upPath.arcTo(QRectF(d->_upArrowRect.topLeft(), QSizeF(d->_pBorderRadius * 2, d->_pBorderRadius * 2)), 180, -90);
        upPath.lineTo(d->_upArrowRect.topRight() - QPointF(d->_pBorderRadius, 0));
        upPath.arcTo(QRectF(d->_upArrowRect.topRight() - QPointF(d->_pBorderRadius * 2, 0),
                            QSizeF(d->_pBorderRadius * 2, d->_pBorderRadius * 2)),
                     90,
                     -90);
        upPath.lineTo(d->_upArrowRect.bottomRight());
        upPath.closeSubpath();
        painter.drawPath(upPath);

        QPainterPath downPath;
        downPath.moveTo(d->_downArrowRect.topLeft());
        downPath.lineTo(d->_downArrowRect.bottomLeft() - QPointF(0, d->_pBorderRadius));
        downPath.arcTo(QRectF(d->_downArrowRect.bottomLeft() - QPointF(0, d->_pBorderRadius * 2),
                              QSizeF(d->_pBorderRadius * 2, d->_pBorderRadius * 2)),
                       180,
                       90);
        downPath.lineTo(d->_downArrowRect.bottomRight() - QPointF(d->_pBorderRadius, 0));
        downPath.arcTo(QRectF(d->_downArrowRect.bottomRight() - QPointF(d->_pBorderRadius * 2, d->_pBorderRadius * 2),
                              QSizeF(d->_pBorderRadius * 2, d->_pBorderRadius * 2)),
                       270,
                       90);
        downPath.lineTo(d->_downArrowRect.topRight());
        downPath.closeSubpath();
        painter.drawPath(downPath);

        painter.setBrush(NThemeColor(NFluentColorKey::TextFillColorPrimary, d->_themeMode));

        QColor iconColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, d->_themeMode);

        int   upIconSize = d->_isUpArrowPress ? 16 : 20;
        QIcon upIcon     = nIcon->fromFilled(NFilledIconType::CaretUp20Filled, upIconSize, iconColor);
        QRect upIconRect(d->_upArrowRect.center().x() - upIconSize / 2,
                         d->_upArrowRect.center().y() - upIconSize / 2,
                         upIconSize,
                         upIconSize);
        upIcon.paint(&painter, upIconRect);

        int   downIconSize = d->_isDownArrowPress ? 16 : 20;
        QIcon downIcon     = nIcon->fromFilled(NFilledIconType::CaretDown20Filled, downIconSize, iconColor);
        QRect downIconRect(d->_downArrowRect.center().x() - downIconSize / 2,
                           d->_downArrowRect.center().y() - downIconSize / 2,
                           downIconSize,
                           downIconSize);
        downIcon.paint(&painter, downIconRect);
    }

    painter.restore();
}

