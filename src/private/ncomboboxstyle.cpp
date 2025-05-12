#include "ncomboboxstyle.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPainterPath>
#include <QStyleOptionComboBox>
#include "QtNativeUI/NComboBox.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NComboBoxStyle::NComboBoxStyle(const NComboBoxStyleInterface* styleInterface, QStyle* style)
    : QProxyStyle(style), m_styleInterface(styleInterface) {}

void NComboBoxStyle::drawPrimitive(PrimitiveElement    element,
                                   const QStyleOption* option,
                                   QPainter*           painter,
                                   const QWidget*      widget) const {
    if (element == PE_FrameFocusRect && qobject_cast<const NComboBox*>(widget)) {
        return;
    }
#ifndef Q_OS_WIN
    if (element == PE_PanelMenu || PE_IndicatorArrowDown) {
        return;
    }
#endif

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NComboBoxStyle::drawComplexControl(ComplexControl             control,
                                        const QStyleOptionComplex* option,
                                        QPainter*                  painter,
                                        const QWidget*             widget) const {
    if (control == CC_ComboBox && qobject_cast<const NComboBox*>(widget)) {
        const QStyleOptionComboBox* comboOpt = qstyleoption_cast<const QStyleOptionComboBox*>(option);
        if (!comboOpt) {
            QProxyStyle::drawComplexControl(control, option, painter, widget);
            return;
        }

        QRect frameRect = subControlRect(control, comboOpt, SC_ComboBoxFrame, widget);
        QRect editRect  = subControlRect(control, comboOpt, SC_ComboBoxEditField, widget);
        QRect arrowRect = subControlRect(control, comboOpt, SC_ComboBoxArrow, widget);

        QRect foregroundRect(frameRect.x() + m_styleInterface->shadowBorderWidth(),
                             frameRect.y() + m_styleInterface->shadowBorderWidth(),
                             frameRect.width() - 2 * m_styleInterface->shadowBorderWidth(),
                             frameRect.height() - 2 * m_styleInterface->shadowBorderWidth());

        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);

        bool isDark       = m_styleInterface->isDarkMode();
        bool isEnabled    = comboOpt->state & QStyle::State_Enabled;
        bool hasFocus     = comboOpt->state & QStyle::State_HasFocus;
        bool hasHover     = comboOpt->state & QStyle::State_MouseOver;
        bool isPressed    = comboOpt->state & QStyle::State_Sunken;
        bool isArrowHover = (comboOpt->activeSubControls & SC_ComboBoxArrow) && hasHover;
        bool isEditable   = comboOpt->editable;

        QColor bgColor = m_styleInterface->backgroundColorForState(isDark, isEnabled, hasFocus, hasHover);
        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(foregroundRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

        QColor borderColor = m_styleInterface->borderColorForState(isDark, isEnabled);
        painter->setPen(QPen(borderColor, m_styleInterface->borderWidth()));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(foregroundRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

        if (!isPressed || isEditable) {
            QColor bottomLineColor =
                m_styleInterface->bottomLineColorForState(isDark, isEnabled, hasFocus && isEditable);
            int bottomLineWidth = m_styleInterface->bottomLineWidth(hasFocus && isEditable);

            int          bottomRectHeight = bottomLineWidth + m_styleInterface->borderRadius() / 2;
            QRect        bottomRect       = foregroundRect;
            QPainterPath clipPath;
            clipPath.addRect(
                QRect(0, foregroundRect.bottom() - bottomRectHeight + 4, widget->width(), bottomRectHeight * 2));
            painter->setClipPath(clipPath);
            painter->setPen(QPen(bottomLineColor, bottomLineWidth));
            painter->drawRoundedRect(bottomRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());
        }

        painter->restore();
        painter->save();

        QColor arrowBgColor =
            m_styleInterface->dropdownButtonBackgroundColor(isDark, isEnabled, isArrowHover, isPressed);

        painter->setPen(Qt::NoPen);
        painter->setBrush(arrowBgColor);

        QRect arrowBgRect = arrowRect.adjusted(0, 4, 0, -4);
        painter->drawRoundedRect(arrowBgRect, 4, 4);

        painter->restore();
        painter->save();

        QIcon arrowIcon           = nIcon->fromRegular(NRegularIconType::ChevronDown12Regular, 10);
        QRect arrowRectWithOffset = arrowRect;
        arrowRectWithOffset.translate(0, m_styleInterface->getArrowYOffset());
        arrowIcon.paint(painter, arrowRectWithOffset, Qt::AlignCenter, isEnabled ? QIcon::Normal : QIcon::Disabled);

        QColor textColor          = m_styleInterface->textColorForState(isDark, isEnabled);
        QColor selectionBgColor   = m_styleInterface->selectionBackgroundColor(isDark);
        QColor selectionTextColor = m_styleInterface->selectionTextColor(isDark);

        if (const NComboBox* comboBox = qobject_cast<const NComboBox*>(widget)) {
            NComboBox* nonConstComboBox = const_cast<NComboBox*>(comboBox);
            QPalette   pal              = nonConstComboBox->palette();
            pal.setColor(QPalette::Text, textColor);
            pal.setColor(QPalette::ButtonText, textColor);
            pal.setColor(QPalette::Highlight, selectionBgColor);
            pal.setColor(QPalette::HighlightedText, selectionTextColor);
            nonConstComboBox->setPalette(pal);

            if (comboBox->isEditable() && comboBox->getLineEdit()) {
                QLineEdit* lineEdit    = comboBox->getLineEdit();
                QPalette   lineEditPal = lineEdit->palette();
                lineEditPal.setColor(QPalette::Text, textColor);
                lineEditPal.setColor(QPalette::Highlight, selectionBgColor);
                lineEditPal.setColor(QPalette::HighlightedText, selectionTextColor);
                lineEdit->setPalette(lineEditPal);
            }
        }

        painter->restore();

        if (!comboOpt->editable) {
            QRect contentRect = editRect.adjusted(4, 0, -4, 0);
            QString text = comboOpt->currentText;
            painter->setPen(textColor);
            painter->drawText(contentRect, Qt::AlignVCenter | Qt::AlignLeft, text);
        }

        return;
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void NComboBoxStyle::drawControl(ControlElement      element,
                                 const QStyleOption* option,
                                 QPainter*           painter,
                                 const QWidget*      widget) const {
    if (element == CE_ShapedFrame) {
        if (widget->objectName() == "NComboBoxContainer") {
            int   _shadowBorderWidth = 6;
            int   borderRadius       = NDesignToken(NDesignTokenKey::CornerRadiusMedium).toInt();
            QRect viewRect           = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            QRect foregroundRect(viewRect.x() + _shadowBorderWidth,
                                 viewRect.y(),
                                 viewRect.width() - 2 * _shadowBorderWidth,
                                 viewRect.height() - _shadowBorderWidth);
            painter->setPen(NThemeColor(NFluentColorKey::ControlStrokeColorSecondary, nTheme->themeMode()));
            painter->setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, nTheme->themeMode()));
            painter->drawRoundedRect(foregroundRect, borderRadius, borderRadius);
            painter->restore();
        }
        return;
    }

    if (element == CE_ItemViewItem) {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
            int margin = 2;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(Qt::NoPen);
            QPainterPath path;
            QRect        optionRect = option->rect;
            optionRect.adjust(margin, margin, -margin, -margin);
#ifndef Q_OS_WIN
            optionRect.adjust(6, 0, -6, 0);
#endif
            path.addRoundedRect(optionRect, 5, 5);
            if (option->state & QStyle::State_Selected) {
                painter->setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, nTheme->themeMode()));
                painter->drawPath(path);
                painter->setPen(Qt::NoPen);
                painter->setBrush(nTheme->isDarkMode() ? nTheme->accentColor().lighter()
                                                       : nTheme->accentColor().darker());
                painter->drawRoundedRect(QRectF(optionRect.x() + 3,
                                                optionRect.y() + optionRect.height() * 0.2,
                                                3,
                                                optionRect.height() - +optionRect.height() * 0.4),
                                         2,
                                         2);
            } else {
                if (option->state & QStyle::State_MouseOver) {
                    painter->setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, nTheme->themeMode()));
                    painter->drawPath(path);
                }
            }
            painter->setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, nTheme->themeMode()));
            painter->drawText(
                QRect(option->rect.x() + 20, option->rect.y(), option->rect.width() - 20, option->rect.height()),
                Qt::AlignVCenter,
                vopt->text);
            painter->restore();
        }
        return;
    }
    if (element == CE_ComboBoxLabel || element == CE_PushButtonBevel) {
        if (qobject_cast<const NComboBox*>(widget)) {
            if (element == CE_ComboBoxLabel) {
                const QStyleOptionComboBox* comboOpt = qstyleoption_cast<const QStyleOptionComboBox*>(option);
                if (comboOpt && !comboOpt->editable) {
                    QRect editRect = subControlRect(CC_ComboBox, comboOpt, SC_ComboBoxEditField, widget);
                    QRect contentRect = editRect.adjusted(4, 0, -4, 0);
                    bool isDark = m_styleInterface->isDarkMode();
                    bool isEnabled = comboOpt->state & QStyle::State_Enabled;
                    QColor textColor = m_styleInterface->textColorForState(isDark, isEnabled);
                    painter->save();
                    painter->setPen(textColor);
                    painter->drawText(contentRect, Qt::AlignVCenter | Qt::AlignLeft, comboOpt->currentText);
                    painter->restore();
                    return;
                }
            }
            return;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

QRect NComboBoxStyle::subControlRect(ComplexControl             cc,
                                     const QStyleOptionComplex* opt,
                                     SubControl                 sc,
                                     const QWidget*             widget) const {
    if (cc == CC_ComboBox && qobject_cast<const NComboBox*>(widget)) {
        const QStyleOptionComboBox* comboOpt = qstyleoption_cast<const QStyleOptionComboBox*>(opt);
        if (!comboOpt) {
            return QProxyStyle::subControlRect(cc, opt, sc, widget);
        }

        QRect comboRect  = opt->rect;
        int   margin     = 4;
        int   arrowWidth = comboRect.height() - 4 * margin;

        switch (sc) {
            case SC_ComboBoxFrame:
                return comboRect;

            case SC_ComboBoxArrow:
                return QRect(comboRect.right() - arrowWidth - margin,
                             comboRect.top() + margin,
                             arrowWidth,
                             comboRect.height() - 2 * margin);

            case SC_ComboBoxEditField: {
                int leftMargin = margin + m_styleInterface->shadowBorderWidth();
                int editWidth  = comboRect.width() - arrowWidth - 2 * margin - leftMargin;
                return QRect(comboRect.left() + leftMargin,
                             comboRect.top() + margin,
                             editWidth,
                             comboRect.height() - 2 * margin);
            }

            case SC_ComboBoxListBoxPopup:
                return comboRect;

            default:
                break;
        }
    }

    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

QStyle::SubControl NComboBoxStyle::hitTestComplexControl(ComplexControl             control,
                                                         const QStyleOptionComplex* option,
                                                         const QPoint&              pos,
                                                         const QWidget*             widget) const {
    if (control == CC_ComboBox && qobject_cast<const NComboBox*>(widget)) {
        QRect arrowRect = subControlRect(control, option, SC_ComboBoxArrow, widget);
        QRect editRect  = subControlRect(control, option, SC_ComboBoxEditField, widget);

        if (arrowRect.contains(pos)) {
            return SC_ComboBoxArrow;
        } else if (editRect.contains(pos)) {
            return SC_ComboBoxEditField;
        } else {
            return SC_ComboBoxFrame;
        }
    }

    return QProxyStyle::hitTestComplexControl(control, option, pos, widget);
}

int NComboBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    if (metric == PM_DefaultFrameWidth && qobject_cast<const NComboBox*>(widget)) {
        return 0;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize NComboBoxStyle::sizeFromContents(ContentsType        type,
                                       const QStyleOption* option,
                                       const QSize&        size,
                                       const QWidget*      widget) const {
    if (type == CT_ItemViewItem) {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(35);
        return itemSize;
    }
    if (type == CT_ComboBox && qobject_cast<const NComboBox*>(widget)) {
        QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        newSize.setHeight(qMax(newSize.height(), 35));
        return newSize;
    }

    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
