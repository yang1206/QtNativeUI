#include "nspinboxstyle.h"
#include <QPainterPath>
#include <QStyleOptionSpinBox>

#include "QtNativeUI/NDoubleSpinBox.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NSpinBox.h"
#include "QtNativeUI/NTheme.h"

NSpinBoxStyle::NSpinBoxStyle(const NSpinBoxStyleInterface* styleInterface, QStyle* style)
    : QProxyStyle(style), m_styleInterface(styleInterface) {}

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

        if (spinOpt->subControls & SC_SpinBoxUp) {
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

        if (spinOpt->subControls & SC_SpinBoxDown) {
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

        switch (sc) {
            case SC_SpinBoxUp:
                return QRect(spinBoxRect.right() - buttonWidth - margin * 2,
                             spinBoxRect.top() + (spinBoxRect.height() - buttonHeight) / 2,
                             buttonWidth,
                             buttonHeight);

            case SC_SpinBoxDown:
                return QRect(spinBoxRect.right() - 2 * buttonWidth - spacing - margin,
                             spinBoxRect.top() + (spinBoxRect.height() - buttonHeight) / 2,
                             buttonWidth,
                             buttonHeight);

            case SC_SpinBoxEditField:
                return QRect(spinBoxRect.left(),
                             spinBoxRect.top(),
                             spinBoxRect.width() - totalButtonWidth - spacing,
                             spinBoxRect.height());

            case SC_SpinBoxFrame:
                return spinBoxRect;

            default:
                break;
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