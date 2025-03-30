#include "nlineedit_p.h"
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionFrame>
#include "QtNativeUI/NTheme.h"

NLineEditPrivate::NLineEditPrivate(QObject* parent) : QObject(parent) {}

NLineEditPrivate::~NLineEditPrivate() {
    if (_lineEditStyle) {
        delete _lineEditStyle;
        _lineEditStyle = nullptr;
    }
}

NLineEditPrivate::Style::Style(NLineEditPrivate* d, QStyle* style) : QProxyStyle(style), d(d) {}

void NLineEditPrivate::Style::drawPrimitive(PrimitiveElement    element,
                                            const QStyleOption* option,
                                            QPainter*           painter,
                                            const QWidget*      widget) const {
    if (element == PE_PanelLineEdit) {
        if (widget && widget->objectName() == "NLineEdit") {
            if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option)) {
                QRect foregroundRect(d->_shadowBorderWidth,
                                     d->_shadowBorderWidth,
                                     widget->width() - 2 * (d->_shadowBorderWidth),
                                     widget->height() - 2 * d->_shadowBorderWidth);
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing);
                painter->setPen(Qt::NoPen);

                bool isEnabled = fopt->state & QStyle::State_Enabled;
                bool hasFocus  = fopt->state & QStyle::State_HasFocus;
                bool hasHover  = fopt->state & QStyle::State_MouseOver;

                QColor bgColor;
                if (!isEnabled) {
                    bgColor = d->_isDark ? d->_pDarkBackgroundDisabledColor : d->_pLightBackgroundDisabledColor;
                } else if (hasFocus) {
                    bgColor = d->_isDark ? d->_pDarkBackgroundFocusColor : d->_pLightBackgroundFocusColor;
                } else if (hasHover) {
                    bgColor = d->_isDark ? d->_pDarkBackgroundHoverColor : d->_pLightBackgroundHoverColor;
                } else {
                    bgColor = d->_isDark ? d->_pDarkBackgroundColor : d->_pLightBackgroundColor;
                }

                painter->setPen(Qt::NoPen);
                painter->setBrush(bgColor);
                painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

                QColor borderColor;
                if (!isEnabled) {
                    borderColor = d->_isDark ? d->_pDarkBorderDisabledColor : d->_pLightBorderDisabledColor;
                } else {
                    borderColor = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
                }

                painter->setPen(QPen(borderColor, d->_pBorderWidth));
                painter->setBrush(Qt::NoBrush);
                painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

                QColor bottomLineColor;
                int    bottomLineWidth;

                if (hasFocus && isEnabled) {
                    bottomLineColor = nTheme->accentColor().normal();
                    bottomLineWidth = 2;
                } else {
                    bottomLineColor = d->_isDark ? d->_pDarkBottomLineColor : d->_pLightBottomLineColor;
                    bottomLineWidth = 1;
                }
                painter->setPen(QPen(bottomLineColor, bottomLineWidth));
                int cornerOffset = d->_pBorderRadius / 2;
                int left         = foregroundRect.left() + cornerOffset;
                int right        = foregroundRect.right() - cornerOffset / 2;
                int bottom       = foregroundRect.bottom();
                painter->drawLine(left, bottom, right, bottom);
                painter->restore();
            }
            return;
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}