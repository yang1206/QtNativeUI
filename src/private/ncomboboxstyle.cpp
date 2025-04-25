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
    // 完全阻止绘制焦点框
    if (element == PE_FrameFocusRect && qobject_cast<const NComboBox*>(widget)) {
        return; // 不绘制任何内容
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NComboBoxStyle::drawComplexControl(ComplexControl             control,
                                        const QStyleOptionComplex* option,
                                        QPainter*                  painter,
                                        const QWidget*             widget) const {
    if (control == CC_ComboBox && qobject_cast<const NComboBox*>(widget)) {
        // 稍后实现绘制逻辑
        // ncomboboxstyle.cpp (继续)
        const QStyleOptionComboBox* comboOpt = qstyleoption_cast<const QStyleOptionComboBox*>(option);
        if (!comboOpt) {
            QProxyStyle::drawComplexControl(control, option, painter, widget);
            return;
        }

        QRect frameRect = subControlRect(control, comboOpt, SC_ComboBoxFrame, widget);
        QRect editRect  = subControlRect(control, comboOpt, SC_ComboBoxEditField, widget);
        QRect arrowRect = subControlRect(control, comboOpt, SC_ComboBoxArrow, widget);

        // 添加阴影边距
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

        // 绘制背景
        QColor bgColor = m_styleInterface->backgroundColorForState(isDark, isEnabled, hasFocus, hasHover);
        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(foregroundRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

        // 绘制边框
        QColor borderColor = m_styleInterface->borderColorForState(isDark, isEnabled);
        painter->setPen(QPen(borderColor, m_styleInterface->borderWidth()));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(foregroundRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

        // 绘制底边线
        QColor bottomLineColor = m_styleInterface->bottomLineColorForState(isDark, isEnabled, hasFocus);
        int    bottomLineWidth = m_styleInterface->bottomLineWidth(hasFocus);

        // 使用裁剪区域绘制底边线
        int          bottomRectHeight = bottomLineWidth + m_styleInterface->borderRadius() / 2;
        QRect        bottomRect       = foregroundRect;
        QPainterPath clipPath;
        clipPath.addRect(
            QRect(0, foregroundRect.bottom() - bottomRectHeight + 4, widget->width(), bottomRectHeight * 2));
        painter->setClipPath(clipPath);
        painter->setPen(QPen(bottomLineColor, bottomLineWidth));
        painter->drawRoundedRect(bottomRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

        painter->restore();
        painter->save();

        // 绘制箭头按钮区域
        QColor arrowBgColor =
            m_styleInterface->dropdownButtonBackgroundColor(isDark, isEnabled, isArrowHover, isPressed);

        painter->setPen(Qt::NoPen);
        painter->setBrush(arrowBgColor);

        // 创建一个圆角矩形，右侧与ComboBox对齐
        QRect arrowBgRect = arrowRect.adjusted(0, 4, -4, -4);
        painter->drawRoundedRect(arrowBgRect, 4, 4);

        // 绘制箭头图标
        QIcon arrowIcon =
            nIcon->fromRegular(m_styleInterface->isDropdownVisible() ? NRegularIconType::ChevronUp16Regular
                                                                     : NRegularIconType::ChevronDown16Regular,
                               12);

        arrowIcon.paint(painter, arrowRect, Qt::AlignCenter, isEnabled ? QIcon::Normal : QIcon::Disabled);

        // 设置文字颜色
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

        // 绘制当前文本 (对于非editable的combobox)
        if (!comboOpt->editable) {
            QRect   contentRect = editRect.adjusted(4, 0, -4, 0);
            QString text        = comboOpt->currentText;
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
    if (qobject_cast<const NComboBox*>(widget)) {
        if (element == CE_ComboBoxLabel) {
            const QStyleOptionComboBox* comboOpt = qstyleoption_cast<const QStyleOptionComboBox*>(option);
            if (comboOpt) {
                QRect  editRect    = subControlRect(CC_ComboBox, comboOpt, SC_ComboBoxEditField, widget);
                QRect  contentRect = editRect.adjusted(4, 0, -4, 0);
                bool   isDark      = m_styleInterface->isDarkMode();
                bool   isEnabled   = comboOpt->state & QStyle::State_Enabled;
                QColor textColor   = m_styleInterface->textColorForState(isDark, isEnabled);
                painter->save();
                painter->setPen(textColor);
                painter->drawText(contentRect, Qt::AlignVCenter | Qt::AlignLeft, comboOpt->currentText);
                painter->restore();
                return;
            }
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
        int   arrowWidth = comboRect.height() - 2 * margin;

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
    if (type == CT_ComboBox && qobject_cast<const NComboBox*>(widget)) {
        QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        newSize.setHeight(qMax(newSize.height(), 35)); // 最小高度
        return newSize;
    }

    return QProxyStyle::sizeFromContents(type, option, size, widget);
}