#include "QtNativeUI/NGroupBox.h"
#include "../private/ngroupbox_p.h"
#include "QtNativeUI/NAnimation.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "QtNativeUI/NLabel.h"

Q_PROPERTY_CREATE_Q_CPP(NGroupBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, QColor, LightTitleColor)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, QColor, DarkTitleColor)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, int, TitleSpacing)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, bool, ShowBorder)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, int, ContentMargin)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, int, TitleHeight)
Q_PROPERTY_CREATE_Q_CPP(NGroupBox, int, CollapseIndicatorSize)

NGroupBox::NGroupBox(QWidget* parent) : QGroupBox(parent), d_ptr(new NGroupBoxPrivate()) { init(); }

NGroupBox::NGroupBox(const QString& title, QWidget* parent) : NGroupBox(parent) { setTitle(title); }

NGroupBox::~NGroupBox() {}

void NGroupBox::init() {
    Q_D(NGroupBox);
    d->q_ptr = this;

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    // 初始化默认颜色
    d->_pBorderRadius         = NDesignToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Dark);
    d->_pLightBorderColor     = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor      = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);
    d->_pLightTitleColor      = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTitleColor       = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pTitleSpacing         = NDesignToken(NDesignTokenKey::SpacingM).toInt();
    d->_pShowBorder           = true;
    d->_pContentMargin        = 12;
    d->_pTitleHeight          = 32;
    d->_pCollapseIndicatorSize = 16;

    setObjectName("NGroupBox");
    setFlat(true);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NGroupBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

void NGroupBox::setGroupBoxStyle(GroupBoxStyle style) {
    Q_D(NGroupBox);
    if (d->_groupBoxStyle != style) {
        d->_groupBoxStyle = style;
        update();
    }
}

NGroupBox::GroupBoxStyle NGroupBox::groupBoxStyle() const {
    Q_D(const NGroupBox);
    return d->_groupBoxStyle;
}

void NGroupBox::setTitleIcon(NRegularIconType::Icon icon, int size) {
    Q_D(NGroupBox);
    d->_titleIcon.isRegular = true;
    d->_titleIcon.iconCode  = static_cast<quint32>(icon);
    d->_titleIcon.size      = size;
    d->_titleIcon.hasIcon   = true;
    updateTitleIcon();
    update();
}

void NGroupBox::setTitleIcon(NFilledIconType::Icon icon, int size) {
    Q_D(NGroupBox);
    d->_titleIcon.isRegular = false;
    d->_titleIcon.iconCode  = static_cast<quint32>(icon);
    d->_titleIcon.size      = size;
    d->_titleIcon.hasIcon   = true;
    updateTitleIcon();
    update();
}

void NGroupBox::clearTitleIcon() {
    Q_D(NGroupBox);
    d->_titleIcon.hasIcon = false;
    update();
}

void NGroupBox::setExpandedIcon(NRegularIconType::Icon icon) {
    Q_D(NGroupBox);
    d->_expandedIcon.isRegular     = true;
    d->_expandedIcon.iconCode      = static_cast<quint32>(icon);
    d->_expandedIcon.hasCustomIcon = true;
    update();
}

void NGroupBox::setExpandedIcon(NFilledIconType::Icon icon) {
    Q_D(NGroupBox);
    d->_expandedIcon.isRegular     = false;
    d->_expandedIcon.iconCode      = static_cast<quint32>(icon);
    d->_expandedIcon.hasCustomIcon = true;
    update();
}

void NGroupBox::setCollapsedIcon(NRegularIconType::Icon icon) {
    Q_D(NGroupBox);
    d->_collapsedIcon.isRegular     = true;
    d->_collapsedIcon.iconCode      = static_cast<quint32>(icon);
    d->_collapsedIcon.hasCustomIcon = true;
    update();
}

void NGroupBox::setCollapsedIcon(NFilledIconType::Icon icon) {
    Q_D(NGroupBox);
    d->_collapsedIcon.isRegular     = false;
    d->_collapsedIcon.iconCode      = static_cast<quint32>(icon);
    d->_collapsedIcon.hasCustomIcon = true;
    update();
}

void NGroupBox::setCollapsible(bool collapsible) {
    Q_D(NGroupBox);
    if (d->_isCollapsible != collapsible) {
        d->_isCollapsible = collapsible;
        if (!collapsible && d->_isCollapsed) {
            setCollapsed(false);
        }
        update();
    }
}

bool NGroupBox::isCollapsible() const {
    Q_D(const NGroupBox);
    return d->_isCollapsible;
}

void NGroupBox::setCollapsed(bool collapsed) {
    Q_D(NGroupBox);
    if (!d->_isCollapsible || d->_isCollapsed == collapsed || d->_isAnimating) {
        return;
    }

    d->_isAnimating = true;
    d->_isCollapsed = collapsed;

    int targetHeight;
    if (collapsed) {
        if (d->_expandedHeight == 0) {
            d->_expandedHeight = height();
        }
        targetHeight        = d->_pContentMargin + d->_pTitleHeight + d->_pContentMargin;
        d->_collapsedHeight = targetHeight;
    } else {
        setMinimumHeight(0);
        setMaximumHeight(QWIDGETSIZE_MAX);
        targetHeight = d->_expandedHeight > 0 ? d->_expandedHeight : sizeHint().height();
    }

    // 展开时立即显示子控件，折叠时延迟隐藏
    if (!collapsed) {
        updateChildrenVisibility(true);
    }

    QPropertyAnimation* heightAnimation = new QPropertyAnimation(this, "maximumHeight", this);
    heightAnimation->setDuration(300);
    heightAnimation->setEasingCurve(QtNativeUI::NFluentAnimation::createBezierCurve(0.1, 0.9, 0.2, 1.0));
    heightAnimation->setStartValue(height());
    heightAnimation->setEndValue(targetHeight);

    QPropertyAnimation* minHeightAnimation = new QPropertyAnimation(this, "minimumHeight", this);
    minHeightAnimation->setDuration(300);
    minHeightAnimation->setEasingCurve(QtNativeUI::NFluentAnimation::createBezierCurve(0.1, 0.9, 0.2, 1.0));
    minHeightAnimation->setStartValue(collapsed ? height() : d->_collapsedHeight);
    minHeightAnimation->setEndValue(targetHeight);

    connect(heightAnimation, &QPropertyAnimation::finished, this, [this, collapsed]() {
        Q_D(NGroupBox);
        d->_isAnimating = false;

        if (collapsed) {
            // 动画完成后隐藏子控件
            updateChildrenVisibility(false);
        } else {
            setMaximumHeight(QWIDGETSIZE_MAX);
            setMinimumHeight(0);
        }

        updateLayout();
        emit collapsedChanged(collapsed);
    });

    heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    minHeightAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool NGroupBox::isCollapsed() const {
    Q_D(const NGroupBox);
    return d->_isCollapsed;
}

QSize NGroupBox::sizeHint() const {
    Q_D(const NGroupBox);
    QSize hint = QGroupBox::sizeHint();

    if (d->_isCollapsed) {
        hint.setHeight(d->_pContentMargin + d->_pTitleHeight + d->_pContentMargin);
    }

    return hint;
}

QSize NGroupBox::minimumSizeHint() const {
    Q_D(const NGroupBox);
    QSize hint = QGroupBox::minimumSizeHint();

    if (d->_isCollapsed) {
        hint.setHeight(d->_pContentMargin + d->_pTitleHeight + d->_pContentMargin);
    }

    return hint;
}

void NGroupBox::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawBackground(&painter);
    drawBorder(&painter);
    drawTitle(&painter);

    if (isCollapsible()) {
        drawCollapseIndicator(&painter);
    }
}

void NGroupBox::mousePressEvent(QMouseEvent* event) {
    Q_D(NGroupBox);

    if (d->_isCollapsible && titleRect().contains(event->pos())) {
        setCollapsed(!d->_isCollapsed);
        event->accept();
        return;
    }

    QGroupBox::mousePressEvent(event);
}

void NGroupBox::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange) {
        update();
    }
    QGroupBox::changeEvent(event);
}

void NGroupBox::resizeEvent(QResizeEvent* event) {
    updateLayout();
    QGroupBox::resizeEvent(event);
}

void NGroupBox::drawBackground(QPainter* painter) {
    Q_D(NGroupBox);

    if (d->_groupBoxStyle == NGroupBox::Card || d->_groupBoxStyle == NGroupBox::Standard) {
        QColor bgColor = d->getCurrentColor(d->_pLightBackgroundColor, d->_pDarkBackgroundColor);

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
        painter->restore();
    }
}

void NGroupBox::drawBorder(QPainter* painter) {
    Q_D(NGroupBox);

    if (!d->_pShowBorder) {
        return;
    }

    QColor borderColor = d->getCurrentColor(d->_pLightBorderColor, d->_pDarkBorderColor);

    painter->save();
    painter->setPen(QPen(borderColor, d->_borderWidth));
    painter->setBrush(Qt::NoBrush);

    QRect borderRect = rect().adjusted(0, 0, -1, -1);
    painter->drawRoundedRect(borderRect, d->_pBorderRadius, d->_pBorderRadius);
    painter->restore();
}

void NGroupBox::drawTitle(QPainter* painter) {
    Q_D(NGroupBox);

    QString titleText = title();
    if (titleText.isEmpty()) {
        return;
    }

    QColor titleColor = d->getCurrentColor(d->_pLightTitleColor, d->_pDarkTitleColor);
    QRect  tRect      = titleRect();

    painter->save();
    painter->setPen(titleColor);

    QFont titleFont = font();
    titleFont.setBold(true);
    painter->setFont(titleFont);

    int textX = tRect.x() + d->_pTitleSpacing;

    // 绘制图标
    if (d->_titleIcon.hasIcon) {
        QIcon icon;
        if (d->_titleIcon.isRegular) {
            icon = nIcon->fromRegular(
                static_cast<NRegularIconType::Icon>(d->_titleIcon.iconCode), d->_titleIcon.size, titleColor);
        } else {
            icon = nIcon->fromFilled(
                static_cast<NFilledIconType::Icon>(d->_titleIcon.iconCode), d->_titleIcon.size, titleColor);
        }

        QRect iconRect(
            textX, tRect.y() + (tRect.height() - d->_titleIcon.size) / 2, d->_titleIcon.size, d->_titleIcon.size);
        icon.paint(painter, iconRect);
        textX += d->_titleIcon.size + d->_pTitleSpacing / 2;
    }

    // 绘制文本
    QRect textRect(textX, tRect.y(), tRect.width() - textX + tRect.x(), tRect.height());
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, titleText);

    painter->restore();
}

void NGroupBox::drawCollapseIndicator(QPainter* painter) {
    Q_D(NGroupBox);

    QRect tRect         = titleRect();
    QRect indicatorRect = d->calculateCollapseIndicatorRect(tRect);

    QColor titleColor = d->getCurrentColor(d->_pLightTitleColor, d->_pDarkTitleColor);

    QIcon collapseIcon;
    int   iconSize = d->_pCollapseIndicatorSize;

    if (d->_isCollapsed) {
        if (d->_collapsedIcon.hasCustomIcon) {
            if (d->_collapsedIcon.isRegular) {
                collapseIcon = nIcon->fromRegular(
                    static_cast<NRegularIconType::Icon>(d->_collapsedIcon.iconCode), iconSize, titleColor);
            } else {
                collapseIcon = nIcon->fromFilled(
                    static_cast<NFilledIconType::Icon>(d->_collapsedIcon.iconCode), iconSize, titleColor);
            }
        } else {
            collapseIcon = nIcon->fromRegular(NRegularIconType::ChevronRight16Regular, iconSize, titleColor);
        }
    } else {
        if (d->_expandedIcon.hasCustomIcon) {
            if (d->_expandedIcon.isRegular) {
                collapseIcon = nIcon->fromRegular(
                    static_cast<NRegularIconType::Icon>(d->_expandedIcon.iconCode), iconSize, titleColor);
            } else {
                collapseIcon = nIcon->fromFilled(
                    static_cast<NFilledIconType::Icon>(d->_expandedIcon.iconCode), iconSize, titleColor);
            }
        } else {
            collapseIcon = nIcon->fromRegular(NRegularIconType::ChevronDown16Regular, iconSize, titleColor);
        }
    }

    collapseIcon.paint(painter, indicatorRect);
}

QRect NGroupBox::titleRect() const {
    Q_D(const NGroupBox);
    return d->calculateTitleRect(rect());
}

QRect NGroupBox::contentRect() const {
    Q_D(const NGroupBox);
    return d->calculateContentRect(rect());
}

void NGroupBox::updateLayout() {
    Q_D(NGroupBox);

    if (d->_isCollapsed) {
        return;
    }

    QRect cRect = contentRect();
    setContentsMargins(cRect.x(), cRect.y(), width() - cRect.right(), height() - cRect.bottom());
}

void NGroupBox::updateChildrenVisibility(bool visible) {
    // 优化：只处理真正的内容控件，排除布局管理器等
    const QList<QWidget*> children = findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget* child : children) {
        if (child != this && 
            !qobject_cast<QLabel*>(child) && 
            child->objectName() != "qt_groupbox_checkbox") {
            child->setVisible(visible);
        }
    }
}

void NGroupBox::updateTitleIcon() { 
    update(); 
}