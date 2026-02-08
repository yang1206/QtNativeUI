#include "QtNativeUI/NGroupBox.h"
#include "../private/ngroupbox_p.h"
#include "QtNativeUI/NCheckBox.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

#include <QMouseEvent>
#include <QPainter>

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

NGroupBox::NGroupBox(QWidget* parent) : QWidget(parent), d_ptr(new NGroupBoxPrivate()) { init(); }

NGroupBox::NGroupBox(const QString& title, QWidget* parent) : NGroupBox(parent) { setTitle(title); }

NGroupBox::~NGroupBox() {}

void NGroupBox::init() {
    Q_D(NGroupBox);
    d->q_ptr = this;

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pBorderRadius         = NRadiusToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Dark);
    d->_pLightBorderColor     = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor      = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);
    d->_pLightTitleColor      = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTitleColor       = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pTitleSpacing         = NDesignToken(NDesignTokenKey::SpacingM).toInt();
    d->_pShowBorder           = true;
    d->_pContentMargin        = 8;
    d->_pTitleHeight          = 28;

    setObjectName("NGroupBox");

    d->_checkBox = new NCheckBox(this);
    d->_checkBox->hide();
    connect(d->_checkBox, &NCheckBox::toggled, this, [this](bool checked) {
        Q_D(NGroupBox);
        d->_checked = checked;
        updateChildrenEnabled();
        Q_EMIT toggled(checked);
    });

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NGroupBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
    
    int topMargin    = d->_pTitleHeight + 4 + d->_pContentMargin;
    int sideMargin   = d->_pContentMargin;
    int bottomMargin = d->_pContentMargin;
    setContentsMargins(sideMargin, topMargin, sideMargin, bottomMargin);
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

void NGroupBox::setTitle(const QString& title) {
    Q_D(NGroupBox);
    if (d->_title != title) {
        d->_title = title;
        update();
    }
}

QString NGroupBox::title() const {
    Q_D(const NGroupBox);
    return d->_title;
}

void NGroupBox::setAlignment(Qt::Alignment alignment) {
    Q_D(NGroupBox);
    if (d->_alignment != alignment) {
        d->_alignment = alignment;
        update();
    }
}

Qt::Alignment NGroupBox::alignment() const {
    Q_D(const NGroupBox);
    return d->_alignment;
}

void NGroupBox::setFlat(bool flat) {
    Q_D(NGroupBox);
    if (d->_flat != flat) {
        d->_flat = flat;
        update();
    }
}

bool NGroupBox::isFlat() const {
    Q_D(const NGroupBox);
    return d->_flat;
}

void NGroupBox::setCheckable(bool checkable) {
    Q_D(NGroupBox);
    if (d->_checkable != checkable) {
        d->_checkable = checkable;
        d->_checkBox->setVisible(checkable);
        if (checkable) {
            updateChildrenEnabled();
        } else {
            const QObjectList& children = this->children();
            for (QObject* child : children) {
                QWidget* widget = qobject_cast<QWidget*>(child);
                if (widget && widget != d->_checkBox) {
                    widget->setEnabled(true);
                }
            }
        }
        updateLayout();
        update();
    }
}

bool NGroupBox::isCheckable() const {
    Q_D(const NGroupBox);
    return d->_checkable;
}

void NGroupBox::setChecked(bool checked) {
    Q_D(NGroupBox);
    if (!d->_checkable || d->_checked == checked) {
        return;
    }
    d->_checked = checked;
    d->_checkBox->setChecked(checked);
    updateChildrenEnabled();
}

bool NGroupBox::isChecked() const {
    Q_D(const NGroupBox);
    return d->_checked;
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

void NGroupBox::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawBackground(&painter);
    drawBorder(&painter);
    drawTitle(&painter);
}

void NGroupBox::mousePressEvent(QMouseEvent* event) { QWidget::mousePressEvent(event); }

void NGroupBox::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange) {
        update();
    }
    QWidget::changeEvent(event);
}

void NGroupBox::resizeEvent(QResizeEvent* event) {
    updateLayout();
    QWidget::resizeEvent(event);
}

void NGroupBox::drawBackground(QPainter* painter) {
    Q_D(NGroupBox);

    if (d->_groupBoxStyle == NGroupBox::Card || d->_groupBoxStyle == NGroupBox::Standard) {
        QColor bgColor = d->getCurrentColor(d->_pLightBackgroundColor, d->_pDarkBackgroundColor);

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        
        QRect bgRect = rect().adjusted(0, d->_pTitleHeight + 4, -1, -1);
        
        painter->drawRoundedRect(bgRect, d->_pBorderRadius, d->_pBorderRadius);
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

    QRect borderRect = rect().adjusted(0, d->_pTitleHeight + 4, -1, -1);
    painter->drawRoundedRect(borderRect, d->_pBorderRadius, d->_pBorderRadius);
    painter->restore();
}

void NGroupBox::drawTitle(QPainter* painter) {
    Q_D(NGroupBox);

    if (d->_title.isEmpty()) {
        return;
    }

    QColor titleColor = d->getCurrentColor(d->_pLightTitleColor, d->_pDarkTitleColor);
    if (!isEnabled()) {
        titleColor = d->getCurrentColor(NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light),
                                        NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark));
    }

    QRect tRect = titleRect();

    painter->save();
    painter->setPen(titleColor);

    QFont titleFont = font();
    titleFont.setBold(true);
    painter->setFont(titleFont);

    int textX = tRect.x();

    if (d->_checkable) {
        textX += 20 + d->_pTitleSpacing / 2;
    }

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

    QRect textRect(textX, tRect.y(), tRect.width() - textX + tRect.x(), tRect.height());
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, d->_title);

    painter->restore();
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
    
    if (d->_checkable) {
        QRect tRect     = titleRect();
        int   checkBoxX = tRect.x();
        int   checkBoxY = tRect.y() + (tRect.height() - 20) / 2;
        d->_checkBox->setGeometry(checkBoxX, checkBoxY, 20, 20);
    }
}

void NGroupBox::updateTitleIcon() { update(); }

void NGroupBox::updateChildrenEnabled() {
    Q_D(NGroupBox);
    if (!d->_checkable) {
        return;
    }

    bool               enabled  = d->_checked;
    const QObjectList& children = this->children();
    for (QObject* child : children) {
        QWidget* widget = qobject_cast<QWidget*>(child);
        if (widget && widget != d->_checkBox) {
            widget->setEnabled(enabled);
        }
    }
}
