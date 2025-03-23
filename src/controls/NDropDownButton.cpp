#include "QtNativeUI/NDropDownButton.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include "../private/ndropdownbutton_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NDropDownButton::NDropDownButton(QWidget* parent) : NPushButton(parent), d_ptr(new NDropDownButtonPrivate()) { init(); }

NDropDownButton::NDropDownButton(const QString& text, QWidget* parent) : NDropDownButton(parent) { setText(text); }

NDropDownButton::~NDropDownButton() {}

void NDropDownButton::init() {
    Q_D(NDropDownButton);
    d->q_ptr = this;

    setShowArrow(true);

    connect(this, &NPushButton::clicked, this, &NDropDownButton::showMenu);
    d->_horizontalPadding   = NDesignToken(NDesignTokenKey::SpacingL).toInt();
    d->_contentArrowSpacing = NDesignToken(NDesignTokenKey::SpacingM).toInt();
}

void NDropDownButton::setMenu(NMenu* menu) {
    Q_D(NDropDownButton);
    d->_menu = menu;
}

QMenu* NDropDownButton::menu() const {
    Q_D(const NDropDownButton);
    return d->_menu;
}

void NDropDownButton::setShowArrow(bool show) {
    Q_D(NDropDownButton);
    if (d->_showArrow != show) {
        d->_showArrow = show;
        update();
    }
}

bool NDropDownButton::showArrow() const {
    Q_D(const NDropDownButton);
    return d->_showArrow;
}

void NDropDownButton::showMenu() {
    Q_D(NDropDownButton);
    if (!d->_menu) {
        return;
    }

    QPoint pos = mapToGlobal(QPoint(0, height()));
    d->_menu->exec(pos);
}

void NDropDownButton::paintEvent(QPaintEvent* event) {
    Q_D(NDropDownButton);

    NPushButton::paintEvent(event);

    if (!d->_showArrow) {
        return;
    }

    drawDropDownArrow();
}

void NDropDownButton::drawDropDownArrow() {
    Q_D(NDropDownButton);

    QColor textColor = isEnabled() ? NThemeColor(NFluentColorKey::TextFillColorPrimary, nTheme->themeMode())
                                   : NThemeColor(NFluentColorKey::TextFillColorDisabled, nTheme->themeMode());

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    int contentWidth = 0;
    int textWidth    = 0;
    int iconWidth    = 0;

    if (!text().isEmpty()) {
        textWidth = painter.fontMetrics().horizontalAdvance(text());
        contentWidth += textWidth;
    }

    if (!icon().isNull()) {
        iconWidth = iconSize().width();
        if (contentWidth > 0) {
            contentWidth += d->_iconTextSpacing;
        }
        contentWidth += iconWidth;
    }

    if (contentWidth > 0) {
        contentWidth += d->_contentArrowSpacing;
    }
    contentWidth += d->_arrowSize;

    int contentStartX = (width() - contentWidth) / 2;

    int arrowX;
    if (!text().isEmpty() || !icon().isNull()) {
        arrowX = contentStartX + contentWidth - d->_arrowSize;
    } else {
        arrowX = (width() - d->_arrowSize) / 2;
    }

    int arrowY = (height() - d->_arrowSize) / 2;

    QRect availableRect = rect().adjusted(d->_horizontalPadding, 0, -d->_horizontalPadding, 0);

    QRect contentRect;
    if (contentWidth > d->_arrowSize + d->_contentArrowSpacing) {
        contentRect = QRect(contentStartX,
                            0,
                            contentWidth - d->_arrowSize - (contentWidth > d->_arrowSize ? d->_contentArrowSpacing : 0),
                            height());
    } else {
        contentRect = availableRect;
    }

    setProperty("_nContentRect", QVariant::fromValue(contentRect));
    setProperty("_nIconTextSpacing", d->_iconTextSpacing);

    QIcon chevronIcon = nIcon->fromRegular(NRegularIconType::ChevronDown12Regular, d->_arrowSize, textColor);
    chevronIcon.paint(&painter, QRect(arrowX, arrowY, d->_arrowSize, d->_arrowSize), Qt::AlignCenter);
}

void NDropDownButton::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::LanguageChange) {
        update();
    }
    NPushButton::changeEvent(event);
}
