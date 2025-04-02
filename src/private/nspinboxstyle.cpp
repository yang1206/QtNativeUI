#include "nspinboxstyle.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSpinBox>
#include "QtNativeUI/NTheme.h"

NSpinBoxStyle::NSpinBoxStyle(const NSpinBoxStyleInterface* styleInterface, QStyle* style)
    : QProxyStyle(style), m_styleInterface(styleInterface) {}

void NSpinBoxStyle::drawComplexControl(ComplexControl             control,
                                       const QStyleOptionComplex* option,
                                       QPainter*                  painter,
                                       const QWidget*             widget) const {
    if (control == CC_SpinBox) {
        const QStyleOptionSpinBox* spinOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if (!spinOpt) {
            QProxyStyle::drawComplexControl(control, option, painter, widget);
            return;
        }
        // 获取编辑区域
        QRect editRect = subControlRect(control, spinOpt, SC_SpinBoxEditField, widget);

        // 添加阴影边距
        QRect foregroundRect(editRect.x() + m_styleInterface->shadowBorderWidth(),
                             editRect.y() + m_styleInterface->shadowBorderWidth(),
                             editRect.width() - 2 * m_styleInterface->shadowBorderWidth(),
                             editRect.height() - 2 * m_styleInterface->shadowBorderWidth());

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

        // 使用默认方式绘制按钮部分
        // QStyleOptionSpinBox buttonOption = *spinOpt;
        // buttonOption.rect = option->rect;
        // QProxyStyle::drawPrimitive(PE_IndicatorSpinUp, &buttonOption, painter, widget);
        // QProxyStyle::drawPrimitive(PE_IndicatorSpinDown, &buttonOption, painter, widget);
        //
        return;
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect NSpinBoxStyle::subControlRect(ComplexControl             cc,
                                    const QStyleOptionComplex* opt,
                                    SubControl                 sc,
                                    const QWidget*             widget) const {
    if (cc == CC_SpinBox) {
        const QStyleOptionSpinBox* spinOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt);
        if (!spinOpt) {
            return QProxyStyle::subControlRect(cc, opt, sc, widget);
        }

        QRect spinBoxRect = opt->rect;
        int   buttonWidth = spinBoxRect.height() / 2;

        switch (sc) {
            case SC_SpinBoxUp:
                return QRect(spinBoxRect.right() - buttonWidth, 0, buttonWidth, spinBoxRect.height() / 2);

            case SC_SpinBoxDown:
                return QRect(
                    spinBoxRect.right() - buttonWidth, spinBoxRect.height() / 2, buttonWidth, spinBoxRect.height() / 2);

            case SC_SpinBoxEditField:
                return QRect(0, 0, spinBoxRect.width() - buttonWidth, spinBoxRect.height());

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