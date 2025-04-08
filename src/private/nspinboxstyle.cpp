#include "nspinboxstyle.h"
#include <QLineEdit>
#include <QPainterPath>
#include <QStyleOptionSpinBox>
#include "QtNativeUI/NDoubleSpinBox.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NSpinBox.h"
#include "QtNativeUI/NTheme.h"

NSpinBoxStyle::NSpinBoxStyle(const NSpinBoxStyleInterface* styleInterface, QStyle* style)
    : QProxyStyle(style), m_styleInterface(styleInterface) {}

void NSpinBoxStyle::drawPrimitive(PrimitiveElement    element,
                                  const QStyleOption* option,
                                  QPainter*           painter,
                                  const QWidget*      widget) const {
    // 完全阻止绘制焦点框
    if (element == PE_FrameFocusRect &&
        (qobject_cast<const NSpinBox*>(widget) || qobject_cast<const NDoubleSpinBox*>(widget))) {
        return; // 不绘制任何内容
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NSpinBoxStyle::drawComplexControl(ComplexControl             control,
                                       const QStyleOptionComplex* option,
                                       QPainter*                  painter,
                                       const QWidget*             widget) const {
    if (control == CC_SpinBox &&
        (qobject_cast<const NSpinBox*>(widget) || qobject_cast<const NDoubleSpinBox*>(widget))) {
        const QStyleOptionSpinBox* spinOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if (!spinOpt) {
            QProxyStyle::drawComplexControl(control, option, painter, widget);
            return;
        }
        QRect frameRect = subControlRect(control, spinOpt, SC_SpinBoxFrame, widget);
        QRect upRect    = subControlRect(control, spinOpt, SC_SpinBoxUp, widget);
        QRect downRect  = subControlRect(control, spinOpt, SC_SpinBoxDown, widget);
        // 添加阴影边距
        QRect foregroundRect(frameRect.x() + m_styleInterface->shadowBorderWidth(),
                             frameRect.y() + m_styleInterface->shadowBorderWidth(),
                             frameRect.width() - 2 * m_styleInterface->shadowBorderWidth(),
                             frameRect.height() - 2 * m_styleInterface->shadowBorderWidth());

        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);

        bool isDark    = m_styleInterface->isDarkMode();
        bool isEnabled = spinOpt->state & QStyle::State_Enabled;
        bool hasFocus  = spinOpt->state & QStyle::State_HasFocus;
        bool hasHover  = spinOpt->state & QStyle::State_MouseOver;

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

        if ((spinOpt->subControls & SC_SpinBoxUp) || (spinOpt->subControls & SC_ScrollBarAddLine)) {
            bool upEnabled = isEnabled && (spinOpt->stepEnabled & QAbstractSpinBox::StepUpEnabled);
            bool upHover   = (spinOpt->activeSubControls & SC_SpinBoxUp) && (spinOpt->state & QStyle::State_MouseOver);
            bool upPressed = upHover && (spinOpt->state & QStyle::State_Sunken);

            QColor upBgColor = m_styleInterface->buttonBackgroundColor(isDark, upEnabled, upHover, upPressed);
            painter->save();
            painter->setPen(Qt::NoPen);
            painter->setBrush(upBgColor);
            painter->drawRoundedRect(upRect, 4, 4);
            QIcon upIcon = nIcon->fromRegular(NRegularIconType::ChevronUp20Regular, 8);
            upIcon.paint(painter, upRect, Qt::AlignCenter, upEnabled ? QIcon::Normal : QIcon::Disabled);
            painter->restore();
        }

        if ((spinOpt->subControls & SC_SpinBoxDown) || (spinOpt->subControls & SC_ScrollBarSubLine)) {
            bool downEnabled = isEnabled && (spinOpt->stepEnabled & QAbstractSpinBox::StepDownEnabled);
            bool downHover =
                (spinOpt->activeSubControls & SC_SpinBoxDown) && (spinOpt->state & QStyle::State_MouseOver);
            bool downPressed = downHover && (spinOpt->state & QStyle::State_Sunken);

            QColor downBgColor = m_styleInterface->buttonBackgroundColor(isDark, downEnabled, downHover, downPressed);

            // 绘制减号按钮背景
            painter->save();
            painter->setPen(Qt::NoPen);
            painter->setBrush(downBgColor);
            painter->drawRoundedRect(downRect, 4, 4);
            QIcon downIcon = nIcon->fromRegular(NRegularIconType::ChevronDown20Regular, 8);
            downIcon.paint(painter, downRect, Qt::AlignCenter, downEnabled ? QIcon::Normal : QIcon::Disabled);

            painter->restore();
        }
        // 设置文字颜色
        QColor textColor = m_styleInterface->textColorForState(isDark, isEnabled);
        if (const NSpinBox* spinBox = qobject_cast<const NSpinBox*>(widget)) {
            NSpinBox* nonConstSpinBox = const_cast<NSpinBox*>(spinBox);
            QPalette  pal             = nonConstSpinBox->palette();
            pal.setColor(QPalette::Text, textColor);
            pal.setColor(QPalette::ButtonText, textColor);
            nonConstSpinBox->setPalette(pal);

            if (QLineEdit* lineEdit = nonConstSpinBox->getLineEdit()) {
                QPalette lineEditPal = lineEdit->palette();
                lineEditPal.setColor(QPalette::Text, textColor);
                lineEditPal.setColor(QPalette::HighlightedText, textColor);
                lineEdit->setPalette(lineEditPal);
            }
        } else if (const NDoubleSpinBox* doubleSpinBox = qobject_cast<const NDoubleSpinBox*>(widget)) {
            NDoubleSpinBox* nonConstDoubleSpinBox = const_cast<NDoubleSpinBox*>(doubleSpinBox);
            QPalette        pal                   = nonConstDoubleSpinBox->palette();
            pal.setColor(QPalette::Text, textColor);
            pal.setColor(QPalette::ButtonText, textColor);
            nonConstDoubleSpinBox->setPalette(pal);

            if (QLineEdit* lineEdit = nonConstDoubleSpinBox->getLineEdit()) {
                QPalette lineEditPal = lineEdit->palette();
                lineEditPal.setColor(QPalette::Text, textColor);
                lineEditPal.setColor(QPalette::HighlightedText, textColor);
                lineEdit->setPalette(lineEditPal);
            }
        }

        return;
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect NSpinBoxStyle::subControlRect(ComplexControl             cc,
                                    const QStyleOptionComplex* opt,
                                    SubControl                 sc,
                                    const QWidget*             widget) const {
    if (cc == CC_SpinBox && (qobject_cast<const NSpinBox*>(widget) || qobject_cast<const NDoubleSpinBox*>(widget))) {
        const QStyleOptionSpinBox* spinOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt);
        if (!spinOpt) {
            return QProxyStyle::subControlRect(cc, opt, sc, widget);
        }

        QRect spinBoxRect      = opt->rect;
        int   margin           = 5;
        int   buttonHeight     = (spinBoxRect.height() - 2 * margin) / 1.2;
        int   buttonWidth      = buttonHeight;
        int   spacing          = 8;
        int   totalButtonWidth = 2 * buttonWidth + spacing;

        // 使用滚动条控件ID代替微调框控件ID
        if (sc == SC_SpinBoxUp || sc == SC_ScrollBarAddLine) {
            return QRect(spinBoxRect.right() - buttonWidth - margin * 2,
                         spinBoxRect.top() + (spinBoxRect.height() - buttonHeight) / 2,
                         buttonWidth,
                         buttonHeight);
        } else if (sc == SC_SpinBoxDown || sc == SC_ScrollBarSubLine) {
            return QRect(spinBoxRect.right() - 2 * buttonWidth - spacing - margin,
                         spinBoxRect.top() + (spinBoxRect.height() - buttonHeight) / 2,
                         buttonWidth,
                         buttonHeight);
        } else if (sc == SC_SpinBoxEditField) {
            return QRect(spinBoxRect.left(),
                         spinBoxRect.top(),
                         spinBoxRect.width() - totalButtonWidth - spacing,
                         spinBoxRect.height());
        } else if (sc == SC_SpinBoxFrame) {
            return spinBoxRect;
        }
    }

    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

int NSpinBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    // 默认边框宽度
    if (metric == PM_DefaultFrameWidth) {
        return 0;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

QStyle::SubControl NSpinBoxStyle::hitTestComplexControl(ComplexControl             control,
                                                        const QStyleOptionComplex* option,
                                                        const QPoint&              pos,
                                                        const QWidget*             widget) const {
    if (control == CC_SpinBox &&
        (qobject_cast<const NSpinBox*>(widget) || qobject_cast<const NDoubleSpinBox*>(widget))) {
        // 获取按钮区域
        QRect upRect   = subControlRect(control, option, SC_SpinBoxUp, widget);
        QRect downRect = subControlRect(control, option, SC_SpinBoxDown, widget);
        QRect editRect = subControlRect(control, option, SC_SpinBoxEditField, widget);

        // 首先检查是否在自定义按钮区域内
        if (upRect.contains(pos)) {
            return SC_SpinBoxUp;
        } else if (downRect.contains(pos)) {
            return SC_SpinBoxDown;
        } else if (editRect.contains(pos)) {
            return SC_SpinBoxEditField;
        } else {
            return SC_SpinBoxFrame;
        }
    }

    return QProxyStyle::hitTestComplexControl(control, option, pos, widget);
}