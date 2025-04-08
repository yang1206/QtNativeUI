#include "neditstyle.h"
#include <QLineEdit>
#include <QPainterPath>
#include <QPlainTextEdit>
#include <QStyleOptionFrame>
#include "QtNativeUI/NTheme.h"

NEditStyle::NEditStyle(const NEditStyleInterface* styleInterface, QStyle* style)
    : QProxyStyle(style), m_styleInterface(styleInterface) {}

void NEditStyle::drawPrimitive(PrimitiveElement    element,
                               const QStyleOption* option,
                               QPainter*           painter,
                               const QWidget*      widget) const {
    if ((element == PE_PanelLineEdit || element == PE_FrameLineEdit || element == PE_Frame) && widget) {
        bool isTextInput = qobject_cast<const QLineEdit*>(widget) || qobject_cast<const QTextEdit*>(widget) ||
                           qobject_cast<const QPlainTextEdit*>(widget);

        if (isTextInput && m_styleInterface) {
            if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option)) {
                QRect foregroundRect(m_styleInterface->shadowBorderWidth(),
                                     m_styleInterface->shadowBorderWidth(),
                                     widget->width() - 2 * m_styleInterface->shadowBorderWidth(),
                                     widget->height() - 2 * m_styleInterface->shadowBorderWidth());
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing);

                bool isEnabled = fopt->state & QStyle::State_Enabled;
                bool hasFocus  = fopt->state & QStyle::State_HasFocus;
                bool hasHover  = fopt->state & QStyle::State_MouseOver;
                bool isDark    = m_styleInterface->isDarkMode();

                // 绘制背景
                QColor bgColor = m_styleInterface->backgroundColorForState(isDark, isEnabled, hasFocus, hasHover);
                painter->setPen(Qt::NoPen);
                painter->setBrush(bgColor);
                painter->drawRoundedRect(
                    foregroundRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

                if (const QTextEdit* textEdit = qobject_cast<const QTextEdit*>(widget)) {
                    // 动态设置 viewport 的背景色，确保与控件背景色一致
                    QTextEdit* nonConstTextEdit = const_cast<QTextEdit*>(textEdit);
                    QPalette   pal              = nonConstTextEdit->viewport()->palette();
                    pal.setColor(QPalette::Base, bgColor);
                    nonConstTextEdit->viewport()->setPalette(pal);
                } else if (const QPlainTextEdit* plainTextEdit = qobject_cast<const QPlainTextEdit*>(widget)) {
                    QPlainTextEdit* nonConstPlainTextEdit = const_cast<QPlainTextEdit*>(plainTextEdit);
                    QPalette        pal                   = nonConstPlainTextEdit->viewport()->palette();
                    pal.setColor(QPalette::Base, bgColor);
                    nonConstPlainTextEdit->viewport()->setPalette(pal);
                }

                // 绘制边框
                QColor borderColor = m_styleInterface->borderColorForState(isDark, isEnabled);
                painter->setPen(QPen(borderColor, m_styleInterface->borderWidth()));
                painter->setBrush(Qt::NoBrush);
                painter->drawRoundedRect(
                    foregroundRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

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
                painter->drawRoundedRect(
                    bottomRect, m_styleInterface->borderRadius(), m_styleInterface->borderRadius());

                QColor textColor = m_styleInterface->textColorForState(isDark, isEnabled);

                if (const QLineEdit* lineEdit = qobject_cast<const QLineEdit*>(widget)) {
                    QLineEdit* nonConstLineEdit = const_cast<QLineEdit*>(lineEdit);
                    QPalette   pal              = nonConstLineEdit->palette();
                    pal.setColor(QPalette::Text, textColor);
                    pal.setColor(QPalette::HighlightedText, textColor);
                    nonConstLineEdit->setPalette(pal);
                } else if (const QTextEdit* textEdit = qobject_cast<const QTextEdit*>(widget)) {
                    QTextEdit* nonConstTextEdit = const_cast<QTextEdit*>(textEdit);
                    QPalette   pal              = nonConstTextEdit->palette();
                    pal.setColor(QPalette::Text, textColor);
                    pal.setColor(QPalette::HighlightedText, textColor);
                    nonConstTextEdit->setPalette(pal);
                    nonConstTextEdit->viewport()->setPalette(pal);
                } else if (const QPlainTextEdit* plainTextEdit = qobject_cast<const QPlainTextEdit*>(widget)) {
                    QPlainTextEdit* nonConstPlainTextEdit = const_cast<QPlainTextEdit*>(plainTextEdit);
                    QPalette        pal                   = nonConstPlainTextEdit->palette();
                    pal.setColor(QPalette::Text, textColor);
                    pal.setColor(QPalette::HighlightedText, textColor);
                    nonConstPlainTextEdit->setPalette(pal);
                    nonConstPlainTextEdit->viewport()->setPalette(pal);
                }

                painter->restore();
                return;
            }
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int NEditStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    // 禁用默认边框
    if (metric == PM_DefaultFrameWidth && widget) {
        bool isTextInput = qobject_cast<const QLineEdit*>(widget) || qobject_cast<const QTextEdit*>(widget) ||
                           qobject_cast<const QPlainTextEdit*>(widget);

        if (isTextInput) {
            return 0;
        }
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}