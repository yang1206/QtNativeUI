#include "QtNativeUI/NExpander.h"
#include "../private/nexpander_p.h"
#include "QtNativeUI/NTheme.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QVariantAnimation>

Q_PROPERTY_CREATE_Q_CPP(NExpander, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NExpander, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NExpander, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NExpander, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NExpander, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NExpander, QColor, LightHeaderColor)
Q_PROPERTY_CREATE_Q_CPP(NExpander, QColor, DarkHeaderColor)
Q_PROPERTY_CREATE_Q_CPP(NExpander, int, HeaderHeight)
Q_PROPERTY_CREATE_Q_CPP(NExpander, int, ContentPadding)
Q_PROPERTY_CREATE_Q_CPP(NExpander, int, AnimationDuration)

NExpander::NExpander(QWidget* parent) : QWidget(parent), d_ptr(new NExpanderPrivate()) { init(); }

NExpander::NExpander(const QString& header, QWidget* parent) : NExpander(parent) { setHeader(header); }

NExpander::~NExpander() {}

void NExpander::init() {
    Q_D(NExpander);
    d->q_ptr = this;

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pBorderRadius         = NRadiusToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Dark);
    d->_pLightBorderColor     = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor      = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);
    d->_pLightHeaderColor     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkHeaderColor      = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pHeaderHeight         = 48;
    d->_pContentPadding       = 16;
    d->_pAnimationDuration    = 250;

    setObjectName("NExpander");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    d->_headerWidget = new NExpanderHeaderWidget(this);
    d->_headerWidget->setFixedHeight(d->_pHeaderHeight);

    d->_contentWidget = new NExpanderContentWidget(this);
    d->_contentWidget->setMaximumHeight(0);

    mainLayout->addWidget(d->_headerWidget);
    mainLayout->addWidget(d->_contentWidget);

    connect(d->_headerWidget, &NExpanderHeaderWidget::clicked, this, [this]() { setExpanded(!isExpanded()); });

    connect(nTheme, &NTheme::themeModeChanged, this, [this, d](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
        d->_headerWidget->update();
        d->_contentWidget->update();
    });
}

void NExpander::setHeader(const QString& header) {
    Q_D(NExpander);
    d->_headerWidget->setHeader(header);
}

QString NExpander::header() const {
    Q_D(const NExpander);
    return d->_headerWidget->header();
}

void NExpander::setExpanded(bool expanded) {
    Q_D(NExpander);
    if (d->_expanded == expanded) {
        return;
    }

    d->_expanded = expanded;
    d->_headerWidget->setExpanded(expanded);

    if (d->_isAnimating) {
        if (d->_heightAnimation) {
            d->_heightAnimation->stop();
            delete d->_heightAnimation;
            d->_heightAnimation = nullptr;
        }
        if (d->_chevronAnimation) {
            d->_chevronAnimation->stop();
            delete d->_chevronAnimation;
            d->_chevronAnimation = nullptr;
        }
        d->_isAnimating = false;
    }

    d->_contentWidget->adjustSize();
    d->_contentWidget->updateGeometry();
    
    int contentHeight = d->_contentWidget->sizeHint().height();
    int startHeight   = d->_contentWidget->height();
    int targetHeight  = expanded ? contentHeight : 0;

    if (startHeight == targetHeight) {
        if (!expanded) {
            d->_contentWidget->setMinimumHeight(0);
            d->_contentWidget->setMaximumHeight(0);
        } else {
            d->_contentWidget->setMinimumHeight(0);
            d->_contentWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        }
        d->_headerWidget->setChevronRotation(expanded ? 180.0 : 0.0);
        Q_EMIT expandedChanged(expanded);
        return;
    }

    d->_isAnimating = true;

    QVariantAnimation* heightAnim = new QVariantAnimation(this);
    heightAnim->setDuration(d->_pAnimationDuration);
    heightAnim->setEasingCurve(QEasingCurve::OutCubic);
    heightAnim->setStartValue(startHeight);
    heightAnim->setEndValue(targetHeight);

    QVariantAnimation* chevronAnim = new QVariantAnimation(this);
    chevronAnim->setDuration(d->_pAnimationDuration);
    chevronAnim->setEasingCurve(QEasingCurve::OutCubic);
    chevronAnim->setStartValue(expanded ? 0.0 : 180.0);
    chevronAnim->setEndValue(expanded ? 180.0 : 0.0);

    connect(heightAnim, &QVariantAnimation::valueChanged, this, [this, d](const QVariant& value) {
        if (d->_contentWidget) {
            int h = value.toInt();
            d->_contentWidget->setMinimumHeight(h);
            d->_contentWidget->setMaximumHeight(h);
        }
    });

    connect(chevronAnim, &QVariantAnimation::valueChanged, this, [this, d](const QVariant& value) {
        if (d->_headerWidget) {
            d->_headerWidget->setChevronRotation(value.toReal());
        }
    });

    connect(heightAnim, &QVariantAnimation::finished, this, [this, d, expanded]() {
        d->_isAnimating     = false;
        d->_heightAnimation = nullptr;
        if (expanded && d->_contentWidget) {
            d->_contentWidget->setMinimumHeight(0);
            d->_contentWidget->setMaximumHeight(QWIDGETSIZE_MAX);
        } else if (d->_contentWidget) {
            d->_contentWidget->setMinimumHeight(0);
            d->_contentWidget->setMaximumHeight(0);
        }
    });

    connect(chevronAnim, &QVariantAnimation::finished, this, [this, d]() { d->_chevronAnimation = nullptr; });

    d->_heightAnimation  = heightAnim;
    d->_chevronAnimation = chevronAnim;

    heightAnim->start(QAbstractAnimation::DeleteWhenStopped);
    chevronAnim->start(QAbstractAnimation::DeleteWhenStopped);

    Q_EMIT expandedChanged(expanded);
}

bool NExpander::isExpanded() const {
    Q_D(const NExpander);
    return d->_expanded;
}

void NExpander::setHeaderIcon(NRegularIconType::Icon icon, int size) {
    Q_D(NExpander);
    d->_headerWidget->setHeaderIcon(true, static_cast<quint32>(icon), size);
}

void NExpander::setHeaderIcon(NFilledIconType::Icon icon, int size) {
    Q_D(NExpander);
    d->_headerWidget->setHeaderIcon(false, static_cast<quint32>(icon), size);
}

void NExpander::clearHeaderIcon() {
    Q_D(NExpander);
    d->_headerWidget->clearHeaderIcon();
}

QWidget* NExpander::contentWidget() const {
    Q_D(const NExpander);
    return d->_contentWidget;
}

void NExpander::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}
