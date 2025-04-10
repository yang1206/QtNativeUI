#include <QEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QResizeEvent>
#include <QSequentialAnimationGroup>
#include <QtNativeUI/NProgressRing.h>
#include "../private/nprogressring_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, bool, AnimationEnabled)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, int, AnimationDuration)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightPausedColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkPausedColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, LightErrorColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressRing, QColor, DarkErrorColor)

NProgressRing::NProgressRing(QWidget* parent) : QWidget(parent), d_ptr(new NProgressRingPrivate()) { init(); }

NProgressRing::~NProgressRing() {}

void NProgressRing::init() {
    Q_D(NProgressRing);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NProgressRing");

    d->_pLightTrackColor = QColor(0, 0, 0, 34);
    d->_pDarkTrackColor  = QColor(255, 255, 255, 34);

    d->_pLightProgressColor = nTheme->accentColor().normal();
    d->_pDarkProgressColor  = nTheme->accentColor().normal();

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pLightDisabledTrackColor = QColor(0, 0, 0, 17);
    d->_pDarkDisabledTrackColor  = QColor(255, 255, 255, 17);

    d->_pLightDisabledProgressColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledProgressColor  = NThemeColor(NFluentColorKey::AccentFillColorDisabled, NThemeType::Dark);

    d->_pLightPausedColor = NThemeColor(NFluentColorKey::SystemFillColorCaution, NThemeType::Light);
    d->_pDarkPausedColor  = NThemeColor(NFluentColorKey::SystemFillColorCaution, NThemeType::Dark);
    d->_pLightErrorColor  = NThemeColor(NFluentColorKey::SystemFillColorCritical, NThemeType::Light);
    d->_pDarkErrorColor   = NThemeColor(NFluentColorKey::SystemFillColorCritical, NThemeType::Dark);

    d->_pStrokeWidth = 6;

    d->_pAnimationEnabled  = true;
    d->_pAnimationDuration = 200;

    d->_minimum     = 0;
    d->_maximum     = 100;
    d->_value       = 0;
    d->_textVisible = false;

    setFixedSize(100, 100);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NProgressRing);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NProgressRing);
        d->_pLightProgressColor = accentColor.normal();
        d->_pDarkProgressColor  = accentColor.normal();
        update();
    });
}

int NProgressRing::minimum() const {
    Q_D(const NProgressRing);
    return d->_minimum;
}

int NProgressRing::maximum() const {
    Q_D(const NProgressRing);
    return d->_maximum;
}

int NProgressRing::value() const {
    Q_D(const NProgressRing);
    return d->_value;
}

QString NProgressRing::format() const {
    Q_D(const NProgressRing);
    return d->_format;
}

bool NProgressRing::isTextVisible() const {
    Q_D(const NProgressRing);
    return d->_textVisible;
}

void NProgressRing::setMinimum(int minimum) {
    Q_D(NProgressRing);
    if (d->_minimum != minimum) {
        d->_minimum = minimum;
        if (d->_value < minimum)
            setValue(minimum);
        update();
    }
}

void NProgressRing::setMaximum(int maximum) {
    Q_D(NProgressRing);
    if (d->_maximum != maximum) {
        d->_maximum = maximum;
        if (d->_value > maximum)
            setValue(maximum);
        update();
    }
}

void NProgressRing::setRange(int minimum, int maximum) {
    Q_D(NProgressRing);
    if (d->_minimum != minimum || d->_maximum != maximum) {
        d->_minimum = minimum;
        d->_maximum = maximum;
        if (d->_value < minimum)
            setValue(minimum);
        else if (d->_value > maximum)
            setValue(maximum);
        update();
    }
}

void NProgressRing::setValue(int value) {
    Q_D(NProgressRing);
    if (d->_value != value) {
        d->_value = value;

        if (d->_pAnimationEnabled && !(d->_minimum == d->_maximum)) {
            qreal endValue = 0.0;
            if (d->_maximum > d->_minimum) {
                endValue = static_cast<qreal>(value - d->_minimum) / static_cast<qreal>(d->_maximum - d->_minimum);
            }
            d->startProgressAnimation(endValue);
        } else {
            if (d->_maximum > d->_minimum) {
                d->setProgressPosition(static_cast<qreal>(value - d->_minimum) /
                                       static_cast<qreal>(d->_maximum - d->_minimum));
            } else {
                d->setProgressPosition(0);
            }
        }

        emit valueChanged(value);
        update();
    }
}

void NProgressRing::setFormat(const QString& format) {
    Q_D(NProgressRing);
    if (d->_format != format) {
        d->_format = format;
        update();
    }
}

void NProgressRing::setTextVisible(bool visible) {
    Q_D(NProgressRing);
    if (d->_textVisible != visible) {
        d->_textVisible = visible;
        update();
    }
}

void NProgressRing::reset() {
    Q_D(NProgressRing);
    d->_value = d->_minimum;
    d->setProgressPosition(0);
    update();
}

void NProgressRing::pause() {
    Q_D(NProgressRing);
    d->setPaused(true);
}

void NProgressRing::resume() {
    Q_D(NProgressRing);
    d->setPaused(false);
    d->setError(false);
}

void NProgressRing::setPaused(bool paused) {
    Q_D(NProgressRing);
    d->setPaused(paused);
}

bool NProgressRing::isPaused() const {
    Q_D(const NProgressRing);
    return d->isPaused();
}

void NProgressRing::setProgressPosition(qreal position) {
    Q_D(NProgressRing);
    d->setProgressPosition(position);
}

qreal NProgressRing::progressPosition() const {
    Q_D(const NProgressRing);
    return d->progressPosition();
}

void NProgressRing::error() {
    Q_D(NProgressRing);
    d->setError(true);
}

void NProgressRing::setError(bool error) {
    Q_D(NProgressRing);
    d->setError(error);
}

bool NProgressRing::isError() const {
    Q_D(const NProgressRing);
    return d->isError();
}

void NProgressRing::setStrokeWidth(int width) {
    Q_D(NProgressRing);
    if (d->_pStrokeWidth != width) {
        d->_pStrokeWidth = width;
        update();
        emit strokeWidthChanged();
    }
}

bool NProgressRing::isIndeterminate() const { return minimum() >= maximum(); }

void NProgressRing::setIndeterminate(bool indeterminate) {
    if (indeterminate) {
        setRange(0, 0);
    } else {
        setRange(0, 100);
    }
}

int NProgressRing::getStrokeWidth() const {
    Q_D(const NProgressRing);
    return d->_pStrokeWidth;
}

void NProgressRing::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawTrack(&painter);

    if (minimum() >= maximum()) {
        drawIndeterminateProgress(&painter);
    } else {
        drawProgress(&painter);
    }

    if (isTextVisible()) {
        drawText(&painter);
    }
}

void NProgressRing::drawTrack(QPainter* painter) {
    Q_D(NProgressRing);

    int    cw = d->_pStrokeWidth;
    int    w  = qMin(width(), height()) - cw;
    QRectF arcRect(cw / 2.0, height() / 2.0 - w / 2.0, w, w);

    QPen pen(d->getTrackColor(), cw, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawArc(arcRect, 0, 360 * 16);
}

void NProgressRing::drawProgress(QPainter* painter) {
    Q_D(NProgressRing);

    if (maximum() <= minimum())
        return;

    int    cw = d->_pStrokeWidth;
    int    w  = qMin(width(), height()) - cw;
    QRectF arcRect(cw / 2.0, height() / 2.0 - w / 2.0, w, w);

    QPen pen(d->getBarColorForState(), cw, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);

    int degree = d->calculateAngle();
    painter->drawArc(arcRect, 90 * 16, -degree * 16);
}

void NProgressRing::drawIndeterminateProgress(QPainter* painter) {
    Q_D(NProgressRing);

    if (d->_pAnimationEnabled && !d->_isAnimationRunning && !d->isPaused() && !d->isError()) {
        d->startIndeterminateAnimations();
    }

    int    cw = d->_pStrokeWidth;
    int    w  = qMin(width(), height()) - cw;
    QRectF arcRect(cw / 2.0, height() / 2.0 - w / 2.0, w, w);

    QPen pen(d->getBarColorForState(), cw, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);

    // 动画位置计算
    int startAngle = (-d->startAngle() + 180) * 16; // 转换为16分之一度
    int spanAngle  = -d->spanAngle() * 16;

    // 确保角度在有效范围内
    startAngle = startAngle % (360 * 16);

    // 绘制圆弧
    painter->drawArc(arcRect, startAngle, spanAngle);
}

void NProgressRing::drawText(QPainter* painter) {
    Q_D(NProgressRing);

    QString text = format();

    int percent = 0;
    if (maximum() > minimum()) {
        percent = static_cast<int>((value() - minimum()) * 100.0 / (maximum() - minimum()));
    }

    if (text.isEmpty()) {
        text = QString("%1%").arg(percent);
    } else {
        // 替换占位符，确保不会有除零错误
        if (maximum() > minimum()) {
            text = text.replace("%p", QString::number(percent))
                       .replace("%v", QString::number(value()))
                       .replace("%m", QString::number(maximum()));
        } else {
            // 处理最大值等于最小值的情况
            text = text.replace("%p", "0")
                       .replace("%v", QString::number(value()))
                       .replace("%m", QString::number(maximum()));
        }
    }

    painter->save();
    painter->setPen(d->getTextColor());
    painter->setFont(font());

    QRect textRect = rect();
    painter->drawText(textRect, Qt::AlignCenter, text);

    painter->restore();
}

void NProgressRing::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    Q_D(NProgressRing);
    if (d->_pAnimationEnabled) {
        d->updateAnimations();
    }
}

void NProgressRing::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange) {
        update();
    }

    QWidget::changeEvent(event);
}
