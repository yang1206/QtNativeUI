#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QtNativeUI/NProgressBar.h>
#include "../private/nprogressbar_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, bool, AnimationEnabled)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, int, AnimationDuration)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightPausedColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkPausedColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, LightErrorColor)
Q_PROPERTY_CREATE_Q_CPP(NProgressBar, QColor, DarkErrorColor)

NProgressBar::NProgressBar(QWidget* parent) : QProgressBar(parent), d_ptr(new NProgressBarPrivate()) { init(); }

NProgressBar::NProgressBar(Qt::Orientation orientation, QWidget* parent)
    : QProgressBar(parent), d_ptr(new NProgressBarPrivate()) {
    setOrientation(orientation);
    init();
}

void NProgressBar::init() {
    Q_D(NProgressBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NProgressBar");

    d->_pLightTrackColor = QColor(0, 0, 0);
    d->_pDarkTrackColor  = QColor(255, 255, 255);

    d->_pLightProgressColor = nTheme->accentColor().normal();
    d->_pDarkProgressColor  = nTheme->accentColor().normal();

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Dark);

    d->_pLightDisabledTrackColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledTrackColor  = NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Dark);

    d->_pLightDisabledProgressColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledProgressColor  = NThemeColor(NFluentColorKey::AccentFillColorDisabled, NThemeType::Dark);

    d->_pLightPausedColor = NThemeColor(NFluentColorKey::SystemFillColorCaution, NThemeType::Light);
    d->_pDarkPausedColor  = NThemeColor(NFluentColorKey::SystemFillColorCaution, NThemeType::Dark);
    d->_pLightErrorColor  = NThemeColor(NFluentColorKey::SystemFillColorCritical, NThemeType::Light);
    d->_pDarkErrorColor   = NThemeColor(NFluentColorKey::SystemFillColorCritical, NThemeType::Dark);

    d->_pBorderRadius      = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pProgressThickness = 4;

    d->_pAnimationEnabled  = true;
    d->_pAnimationDuration = 200;

    if (orientation() == Qt::Horizontal) {
        setMinimumHeight(d->_pProgressThickness);
        setMinimumWidth(100);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    } else {
        setMinimumWidth(d->_pProgressThickness);
        setMinimumHeight(100);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    }

    setTextVisible(false);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NProgressBar);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NProgressBar);
        d->_pLightProgressColor = accentColor.normal();
        d->_pDarkProgressColor  = accentColor.normal();
        update();
    });
}

NProgressBar::~NProgressBar() {}

void NProgressBar::setValue(int value) {
    Q_D(NProgressBar);
    QProgressBar::setValue(value);

    if (d->_pAnimationEnabled && !(minimum() == maximum())) {
        qreal endValue = 0.0;
        if (maximum() > minimum()) {
            endValue = static_cast<qreal>(value - minimum()) / static_cast<qreal>(maximum() - minimum());
        }
        d->startProgressAnimation(endValue);
    } else {
        if (maximum() > minimum()) {
            d->setProgressPosition(static_cast<qreal>(value - minimum()) / static_cast<qreal>(maximum() - minimum()));
        } else {
            d->setProgressPosition(0);
        }
    }
}

void NProgressBar::setProgressPosition(qreal position) {
    Q_D(NProgressBar);
    d->setProgressPosition(position);
}

qreal NProgressBar::progressPosition() const {
    Q_D(const NProgressBar);
    return d->_progressPosition;
}

void NProgressBar::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (minimum() >= maximum()) {
        drawIndeterminateProgress(&painter);
    } else {
        drawTrack(&painter);
        drawProgress(&painter);
    }

    if (isTextVisible()) {
        drawText(&painter);
    }
}

void NProgressBar::drawTrack(QPainter* painter) {
    Q_D(NProgressBar);
    painter->save();
    painter->setPen(d->getTrackColor());

    if (orientation() == Qt::Horizontal) {
        int y = rect().y() + rect().height() / 2;
        painter->drawLine(0, y, width(), y);
    } else {
        int x = rect().x() + rect().width() / 2;
        painter->drawLine(x, 0, x, height() - 2);
    }

    painter->restore();
}

void NProgressBar::drawProgress(QPainter* painter) {
    Q_D(NProgressBar);

    QRect progressRect = d->getProgressRect();
    if (progressRect.isEmpty())
        return;

    painter->setPen(Qt::NoPen);
    painter->setBrush(d->getBarColorForState());
    painter->drawRoundedRect(progressRect, d->_pBorderRadius, d->_pBorderRadius);
}

void NProgressBar::drawIndeterminateProgress(QPainter* painter) {
    Q_D(NProgressBar);

    if (d->_pAnimationEnabled && !d->_isAnimationRunning && !d->isPaused() && !d->isError()) {
        d->startIndeterminateAnimations();
    }

    QRect trackRect = d->getTrackRect();

    painter->setPen(Qt::NoPen);
    painter->setBrush(d->getBarColorForState());

    painter->save();
    QPainterPath clipPath;
    clipPath.addRoundedRect(trackRect, d->_pBorderRadius, d->_pBorderRadius);
    painter->setClipPath(clipPath);

    if (orientation() == Qt::Horizontal) {
        int x = int((d->shortPos() - 0.4) * trackRect.width());
        int w = int(0.4 * trackRect.width());
        int r = d->_pBorderRadius; // 使用与轨道相同的圆角

        QPainterPath shortPath;
        QRect        shortRect(x + trackRect.x(), trackRect.y(), w, trackRect.height());
        shortPath.addRoundedRect(shortRect, r, r);
        painter->fillPath(shortPath, d->getBarColorForState());

        x = int((d->longPos() - 0.6) * trackRect.width());
        w = int(0.6 * trackRect.width());

        QPainterPath longPath;
        QRect        longRect(x + trackRect.x(), trackRect.y(), w, trackRect.height());
        longPath.addRoundedRect(longRect, r, r);
        painter->fillPath(longPath, d->getBarColorForState());
    } else {
        int y = int(trackRect.bottom() - (d->shortPos() * trackRect.height()));
        int h = int(0.4 * trackRect.height());
        int r = d->_pBorderRadius; // 使用与轨道相同的圆角

        QPainterPath shortPath;
        QRect        shortRect(trackRect.x(), y - h, trackRect.width(), h);
        shortPath.addRoundedRect(shortRect, r, r);
        painter->fillPath(shortPath, d->getBarColorForState());

        y = int(trackRect.bottom() - (d->longPos() * trackRect.height()));
        h = int(0.6 * trackRect.height());

        QPainterPath longPath;
        QRect        longRect(trackRect.x(), y - h, trackRect.width(), h);
        longPath.addRoundedRect(longRect, r, r);
        painter->fillPath(longPath, d->getBarColorForState());
    }

    painter->restore();
}

void NProgressBar::drawText(QPainter* painter) {
    Q_D(NProgressBar);

    QString text = format();
    if (text.isEmpty()) {
        int percent = 0;
        if (maximum() > minimum())
            percent = static_cast<int>((value() - minimum()) * 100.0 / (maximum() - minimum()));
        text = QString("%1%").arg(percent);
    } else {
        text = text.replace("%p", QString::number(value() * 100 / (maximum() - minimum())))
                   .replace("%v", QString::number(value()))
                   .replace("%m", QString::number(maximum()));
    }

    painter->save();
    painter->setPen(d->getTextColor());
    painter->setFont(font());

    QRect textRect = rect();
    painter->drawText(textRect, Qt::AlignCenter, text);

    painter->restore();
}

void NProgressBar::resizeEvent(QResizeEvent* event) {
    QProgressBar::resizeEvent(event);

    Q_D(NProgressBar);
    if (d->_pAnimationEnabled) {
        d->updateAnimations();
    }
}

void NProgressBar::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange) {
        update();
    } else if (event->type() == QEvent::OrientationChange) {
        Q_D(NProgressBar);
        if (orientation() == Qt::Horizontal) {
            setMinimumSize(100, d->_pProgressThickness);
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        } else {
            setMinimumSize(d->_pProgressThickness, 100);
            setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        }
    }

    QProgressBar::changeEvent(event);
}

void NProgressBar::pause() {
    Q_D(NProgressBar);
    d->setPaused(true);
}

void NProgressBar::resume() {
    Q_D(NProgressBar);
    d->setPaused(false);
    d->setError(false);
}

void NProgressBar::reset() {
    Q_D(NProgressBar);
    setValue(minimum());
    d->setProgressPosition(0);
    update();
}

void NProgressBar::setPaused(bool paused) {
    Q_D(NProgressBar);
    d->setPaused(paused);
}

bool NProgressBar::isPaused() const {
    Q_D(const NProgressBar);
    return d->isPaused();
}

void NProgressBar::error() {
    Q_D(NProgressBar);
    d->setError(true);
}

void NProgressBar::setError(bool error) {
    Q_D(NProgressBar);
    d->setError(error);
}

bool NProgressBar::isError() const {
    Q_D(const NProgressBar);
    return d->isError();
}

void NProgressBar::setProgressThickness(int thickness) {
    Q_D(NProgressBar);

    d->_pProgressThickness = thickness;

    if (orientation() == Qt::Horizontal) {
        setMinimumHeight(thickness);
    } else {
        setMinimumWidth(thickness);
    }

    updateGeometry();
    update();

    Q_EMIT trackThicknessChanged();
}

void NProgressBar::setBorderRadius(int radius) {
    Q_D(NProgressBar);

    d->_pBorderRadius = radius;

    update();

    Q_EMIT borderRadiusChanged();
}

int NProgressBar::getProgressThickness() const {
    Q_D(const NProgressBar);
    return d->_pProgressThickness;
}

int NProgressBar::getBorderRadius() const {
    Q_D(const NProgressBar);
    return d->_pBorderRadius;
}

bool NProgressBar::isIndeterminate() const { return minimum() >= maximum(); }

void NProgressBar::setIndeterminate(bool indeterminate) {
    if (indeterminate) {
        setRange(0, 0);
    } else {
        setRange(0, 100);
    }
}