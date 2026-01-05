#include "QtNativeUI/NInfoBar.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

#include "../private/ninfobar_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NTheme.h"

NInfoBar::NInfoBar(NInfoBarType::PositionPolicy position,
                   NInfoBarType::InfoMode       severity,
                   QString&                     title,
                   QString&                     message,
                   int                          displayDuration,
                   QWidget*                     parent,
                   bool                         showCloseButton)
    : QWidget{parent}, d_ptr(new NInfoBarPrivate()) {
    Q_D(NInfoBar);
    d->q_ptr            = this;
    d->_borderRadius    = 6;
    d->_title           = title;
    d->_message         = message;
    d->_position        = position;
    d->_severity        = severity;
    d->_themeMode       = nTheme->themeMode();
    d->_showCloseButton = showCloseButton;
    setFixedHeight(60);
    setMouseTracking(true);
    d->_pOpacity = 1;
    parent->installEventFilter(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 10, 0);
    mainLayout->addStretch();
    if (d->_showCloseButton) {
        d->_closeButton = new NPushButton(this);
        d->_closeButton->setObjectName("closeButton");
        d->_closeButton->setTransparentBackground(true);
        d->_closeButton->setFixedSize(30, 30);
        d->_closeButton->setFlat(true);
        d->_closeButton->setIcon(nIcon->fromRegular(NRegularIconType::Dismiss12Regular));
        d->_closeButton->setIconSize(QSize(12, 12));

        switch (d->_severity) {
            case NInfoBarType::Success: {
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorSuccessBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorSuccessBackground, NThemeType::Dark));
                break;
            }
            case NInfoBarType::Warning: {
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCautionBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCautionBackground, NThemeType::Dark));
                break;
            }
            case NInfoBarType::Information: {
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorAttentionBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorAttentionBackground, NThemeType::Dark));
                break;
            }
            case NInfoBarType::Error: {
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCriticalBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCriticalBackground, NThemeType::Dark));
                break;
            }
        }

        d->_closeButton->setBorderRadius(5);
        connect(d->_closeButton, &NPushButton::clicked, d, &NInfoBarPrivate::onCloseButtonClicked);

        mainLayout->addWidget(d->_closeButton);
    } else {
        d->_closeButton = nullptr;
    }
    setObjectName("NInfoBar");
    setStyleSheet("#NInfoBar{background-color:transparent;}");
    d->_infoBarCreate(displayDuration);
}

NInfoBar::~NInfoBar() {}

void NInfoBar::information(NInfoBarType::PositionPolicy position,
                           QString                      title,
                           QString                      message,
                           int                          displayDuration,
                           QWidget*                     parent,
                           bool                         showCloseButton) {
    if (!parent) {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList) {
            if (widget->isVisible() && widget->windowType() != Qt::Desktop) {
                parent = widget;
                break;
            }
        }
        if (!parent) {
            return;
        }
    }

    NInfoBar* infoBar =
        new NInfoBar(position, NInfoBarType::Information, title, message, displayDuration, parent, showCloseButton);
    Q_UNUSED(infoBar);
}

NInfoBar::NInfoBar(NInfoBarType::InfoMode severity,
                   const QString&         title,
                   const QString&         message,
                   QWidget*               parent,
                   bool                   showCloseButton)
    : QWidget{parent}, d_ptr(new NInfoBarPrivate()) {
    Q_D(NInfoBar);
    d->q_ptr            = this;
    d->_borderRadius    = 6;
    d->_title           = title;
    d->_message         = message;
    d->_position        = NInfoBarType::Top;
    d->_severity        = severity;
    d->_themeMode       = nTheme->themeMode();
    d->_showCloseButton = showCloseButton;
    setMouseTracking(true);
    d->_pOpacity            = 1;
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 10, 0);

    d->_widgetsLayout = new QHBoxLayout();
    d->_widgetsLayout->setContentsMargins(0, 0, 0, 0);
    d->_widgetsLayout->setSpacing(8);

    mainLayout->addStretch();
    mainLayout->addLayout(d->_widgetsLayout);

    if (showCloseButton) {
        d->_closeButton = new NPushButton(this);
        d->_closeButton->setObjectName("closeButton");
        d->_closeButton->setTransparentBackground(true);
        d->_closeButton->setFixedSize(30, 30);
        d->_closeButton->setFlat(true);
        d->_closeButton->setIcon(nIcon->fromRegular(NRegularIconType::Dismiss12Regular));
        d->_closeButton->setIconSize(QSize(12, 12));
        switch (d->_severity) {
            case NInfoBarType::Success:
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorSuccessBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorSuccessBackground, NThemeType::Dark));
                break;
            case NInfoBarType::Warning:
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCautionBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCautionBackground, NThemeType::Dark));
                break;
            case NInfoBarType::Information:
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorAttentionBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorAttentionBackground, NThemeType::Dark));
                break;
            case NInfoBarType::Error:
                d->_closeButton->setLightHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCriticalBackground, NThemeType::Light));
                d->_closeButton->setDarkHoverColor(
                    NThemeColor(NFluentColorKey::SystemFillColorCriticalBackground, NThemeType::Dark));
                break;
        }
        d->_closeButton->setBorderRadius(5);
        connect(d->_closeButton, &NPushButton::clicked, this, &NInfoBar::close);
        mainLayout->addWidget(d->_closeButton);
    } else {
        d->_closeButton = nullptr;
    }
    setObjectName("NInfoBar");
    setStyleSheet("#NInfoBar{background-color:transparent;}");

    d->_initializeInfoBar();
}

void NInfoBar::addWidget(QWidget* widget) {
    Q_D(NInfoBar);
    if (widget && d->_widgetsLayout) {
        d->_additionalWidgets.append(widget);
        d->_widgetsLayout->addWidget(widget);
    }
}

void NInfoBar::success(NInfoBarType::PositionPolicy position,
                       QString                      title,
                       QString                      message,
                       int                          displayDuration,
                       QWidget*                     parent,
                       bool                         showCloseButton) {
    if (!parent) {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList) {
            if (widget->isVisible() && widget->windowType() != Qt::Desktop) {
                parent = widget;
                break;
            }
        }
        if (!parent) {
            return;
        }
    }

    NInfoBar* infoBar =
        new NInfoBar(position, NInfoBarType::Success, title, message, displayDuration, parent, showCloseButton);
    Q_UNUSED(infoBar);
}

void NInfoBar::warning(NInfoBarType::PositionPolicy position,
                       QString                      title,
                       QString                      message,
                       int                          displayDuration,
                       QWidget*                     parent,
                       bool                         showCloseButton) {
    if (!parent) {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList) {
            if (widget->isVisible() && widget->windowType() != Qt::Desktop) {
                parent = widget;
                break;
            }
        }
        if (!parent) {
            return;
        }
    }

    NInfoBar* infoBar =
        new NInfoBar(position, NInfoBarType::Warning, title, message, displayDuration, parent, showCloseButton);
    Q_UNUSED(infoBar);
}

void NInfoBar::error(NInfoBarType::PositionPolicy position,
                     QString                      title,
                     QString                      message,
                     int                          displayDuration,
                     QWidget*                     parent,
                     bool                         showCloseButton) {
    if (!parent) {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList) {
            if (widget->isVisible() && widget->windowType() != Qt::Desktop) {
                parent = widget;
                break;
            }
        }
        if (!parent) {
            return;
        }
    }

    NInfoBar* infoBar =
        new NInfoBar(position, NInfoBarType::Error, title, message, displayDuration, parent, showCloseButton);
    Q_UNUSED(infoBar);
}

void NInfoBar::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    Q_D(NInfoBar);
    QPainter painter(this);
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    nTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_borderRadius, NDesignTokenKey::ElevationRest);

    painter.save();
    painter.setPen(d->_themeMode == NThemeType::Light
                       ? NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light)
                       : NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark));

    switch (d->_severity) {
        case NInfoBarType::Success: {
            d->_drawSuccess(&painter);
            break;
        }
        case NInfoBarType::Warning: {
            d->_drawWarning(&painter);
            break;
        }
        case NInfoBarType::Information: {
            d->_drawInformation(&painter);
            break;
        }
        case NInfoBarType::Error: {
            d->_drawError(&painter);
            break;
        }
    }

    d->_drawText(&painter);

    painter.restore();
}

void NInfoBar::resizeEvent(QResizeEvent* event) {
    Q_D(NInfoBar);
    QWidget::resizeEvent(event);
    d->_calculateTextLayout();
    if (d->_closeButton) {
        int buttonX = width() - d->_closeButton->width() - 15;
        int buttonY = (height() >= 60) ? 15 : (height() - d->_closeButton->height()) / 2;
        d->_closeButton->move(buttonX, buttonY);
    }
}

bool NInfoBar::eventFilter(QObject* watched, QEvent* event) {
    Q_D(NInfoBar);
    if (watched == parentWidget()) {
        switch (event->type()) {
            case QEvent::Resize: {
                QResizeEvent* resizeEvent = dynamic_cast<QResizeEvent*>(event);
                QSize         offsetSize  = parentWidget()->size() - resizeEvent->oldSize();
                if (d->_isNormalDisplay) {
                    switch (d->_position) {
                        case NInfoBarType::Top: {
                            this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->y());
                            break;
                        }
                        case NInfoBarType::Bottom: {
                            this->move(parentWidget()->width() / 2 - minimumWidth() / 2,
                                       this->pos().y() + offsetSize.height());
                            break;
                        }
                        case NInfoBarType::Left:
                        case NInfoBarType::TopLeft: {
                            this->move(d->_messageBarHorizontalMargin, this->pos().y());
                            break;
                        }
                        case NInfoBarType::BottomLeft: {
                            this->move(d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                            break;
                        }
                        case NInfoBarType::Right:
                        case NInfoBarType::TopRight: {
                            this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin,
                                       this->y());
                            break;
                        }
                        case NInfoBarType::BottomRight: {
                            this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin,
                                       this->pos().y() + offsetSize.height());
                            break;
                        }
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}