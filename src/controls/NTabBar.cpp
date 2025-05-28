#include <QApplication>
#include <QDrag>
#include <QHBoxLayout>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionTab>
#include <QtNativeUI/NTabBar.h>
#include "../private/ntabbar_p.h"
#include "../private/ntabbarstyle.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolTip.h"

Q_PROPERTY_CREATE_Q_CPP(NTabBar, int, BorderRadius)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemSeparator)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemSeparator)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBorderColorSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBorderColorSelected)

NTabBar::NTabBar(QWidget* parent) : QTabBar(parent), d_ptr(new NTabBarPrivate()) { init(); }

NTabBar::~NTabBar() {}

void NTabBar::init() {
    Q_D(NTabBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NTabBar");
    setStyleSheet("#NTabBar{background-color:transparent;}");
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);
    setUsesScrollButtons(true);
    d->_pLightItemHeaderBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundSelected =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTertiary, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundSelected =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundPointerOver =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundPointerOver =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundPressed =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundPressed =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundDisabled =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundDisabled =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);

    d->_pLightItemHeaderForeground            = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderForeground             = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundSelected    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundSelected     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundPointerOver = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundPointerOver  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundPressed     = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundPressed      = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundDisabled    = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkItemHeaderForegroundDisabled     = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightItemIconForeground            = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemIconForeground             = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemIconForegroundSelected    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkItemIconForegroundSelected     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightItemIconForegroundPointerOver = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemIconForegroundPointerOver  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemIconForegroundPressed     = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkItemIconForegroundPressed      = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightItemIconForegroundDisabled    = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkItemIconForegroundDisabled     = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightItemSeparator = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkItemSeparator  = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    d->_pLightItemHeaderBorderColorSelected =
        NThemeColor(NFluentColorKey::SubtleFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBorderColorSelected = NThemeColor(NFluentColorKey::SubtleFillColorTransparent, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

    setStyle(new NTabBarStyle(style()));

    QTabBar::setTabsClosable(false);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NTabBar);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();

        update();
    });
}

int NTabBar::hoverIndex() const {
    Q_D(const NTabBar);
    return d->_hoverIndex;
}

bool NTabBar::tabsClosable() const {
    Q_D(const NTabBar);
    return d->_tabsClosable;
}

void NTabBar::setTabsClosable(bool closable) {
    Q_D(NTabBar);
    if (d->_tabsClosable == closable)
        return;

    d->_tabsClosable = closable;

    if (closable) {
        for (int i = 0; i < count(); ++i) {
            setupCustomCloseButton(i);
        }
    } else {
        for (int i = 0; i < count(); ++i) {
            setTabButton(i, QTabBar::RightSide, nullptr);
        }
    }

    update();
}

void NTabBar::tabInserted(int index) {
    Q_D(NTabBar);
    QTabBar::tabInserted(index);

    if (d->_tabsClosable) {
        setupCustomCloseButton(index);
    }

    // 重置工具提示状态，避免索引错误
    d->_tooltipTabIndex = -1;
    if (d->_tooltipTimer) {
        d->_tooltipTimer->stop();
    }
    if (d->_currentTooltip) {
        d->_currentTooltip->hide();
        d->_currentTooltip->deleteLater();
        d->_currentTooltip = nullptr;
    }

    // 确保标签布局更新后计算正确的矩形
    QTimer::singleShot(0, this, [this]() { update(); });
}

void NTabBar::tabRemoved(int index) {
    Q_D(NTabBar);
    QTabBar::tabRemoved(index);

    d->_tooltipTabIndex = -1;
    if (d->_tooltipTimer) {
        d->_tooltipTimer->stop();
    }
    if (d->_currentTooltip) {
        d->_currentTooltip->hide();
        d->_currentTooltip->deleteLater();
        d->_currentTooltip = nullptr;
    }
}

void NTabBar::setupCustomCloseButton(int index) {
    QTabBar::Shape tabShape   = shape();
    bool           isVertical = tabShape == QTabBar::RoundedWest || tabShape == QTabBar::RoundedEast ||
                      tabShape == QTabBar::TriangularWest || tabShape == QTabBar::TriangularEast;

    // 创建关闭按钮
    NPushButton* closeBtn = new NPushButton(this);
    closeBtn->setFluentIcon(NRegularIconType::Dismiss12Regular, 16);
    closeBtn->setTransparentBackground(true);
    closeBtn->setObjectName("tabCloseButton");

    if (isVertical) {
        QWidget*     container = new QWidget(this);
        QHBoxLayout* layout    = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        closeBtn->setProperty("_nContentRect", QRect(0, 0, 24, 24));
        closeBtn->setFixedSize(24, 24);

        if (tabShape == QTabBar::RoundedWest || tabShape == QTabBar::TriangularWest) {
            layout->addWidget(closeBtn);
            layout->addStretch();
        } else {
            layout->addStretch();
            layout->addWidget(closeBtn);
        }

        container->setStyleSheet("background: transparent;");
        container->setFixedHeight(24);

        setTabButton(index, QTabBar::LeftSide, container);

        if (tabButton(index, QTabBar::RightSide)) {
            setTabButton(index, QTabBar::RightSide, nullptr);
        }
    } else {
        closeBtn->setProperty("_nContentRect", QRect(0, 0, 24, 24));
        closeBtn->setFixedSize(24, 24);

        setTabButton(index, QTabBar::RightSide, closeBtn);
    }

    connect(closeBtn, &NPushButton::clicked, this, &NTabBar::onCloseButtonClicked);

    closeBtn->setProperty("original_index", index);
}

void NTabBar::onCloseButtonClicked() {
    NPushButton* btn = qobject_cast<NPushButton*>(sender());
    if (!btn)
        return;

    for (int i = 0; i < count(); ++i) {
        if (tabButton(i, QTabBar::RightSide) == btn || tabButton(i, QTabBar::LeftSide) == btn) {
            emit tabCloseRequested(i);
            return;
        }
    }
}

void NTabBar::mousePressEvent(QMouseEvent* event) {
    Q_D(NTabBar);

    // 不再需要检查关闭按钮点击
    QTabBar::mousePressEvent(event);
    d->_lastDragPix = grab(tabRect(currentIndex()));
    Q_EMIT tabBarPressed(currentIndex());
}

void NTabBar::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NTabBar);

    // 拖拽相关代码保持不变
    QRect moveRect = rect();
    moveRect.adjust(0, -height(), 0, height());
    QPoint currentPos = event->pos();

    if (currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y()) {
        QDrag* drag = new QDrag(this);
        drag->setPixmap(d->_lastDragPix);
        qreal  pixRatio  = d->_lastDragPix.devicePixelRatioF();
        QPoint pixCenter = d->_lastDragPix.rect().center();
        drag->setHotSpot(pixCenter / pixRatio);

        QMimeData* dragData = new QMimeData;
        dragData->setProperty("DragType", "NTabBarDrag");
        dragData->setProperty("TabIndex", currentIndex());
        dragData->setProperty("TabIcon", tabIcon(currentIndex()));
        dragData->setProperty("TabText", tabText(currentIndex()));
        dragData->setProperty("NTabBarObject", QVariant::fromValue(this));
        dragData->setProperty("QDragObject", QVariant::fromValue(drag));

        drag->setMimeData(dragData);
        Q_EMIT tabDragStarted(drag);
    }

    int oldHoverIndex = d->_hoverIndex;
    d->_hoverIndex    = tabAt(event->pos());

    if (oldHoverIndex != d->_hoverIndex) {
        update();
    }

    int tabIndex = tabAt(event->pos());
    if (tabIndex >= 0 && tabIndex < count()) {
        QString tip = tabToolTip(tabIndex);
        if (!tip.isEmpty() && tabIndex != d->_tooltipTabIndex) {
            d->_tooltipTabIndex = tabIndex;

            if (d->_currentTooltip) {
                d->_currentTooltip->hide();
                d->_currentTooltip->deleteLater();
                d->_currentTooltip = nullptr;
            }

            d->_currentTooltip = NToolTip::createToolTip(tip, this);
            d->_currentTooltip->setDuration(5000);

            QRect tabRect = this->tabRect(tabIndex);

            NToolTipPosition position;
            QPoint           globalPos;

            if (shape() == QTabBar::RoundedNorth || shape() == QTabBar::TriangularNorth) {
                position  = NToolTipPosition::BOTTOM;
                globalPos = mapToGlobal(QPoint(tabRect.center().x(), tabRect.bottom()));
            } else if (shape() == QTabBar::RoundedSouth || shape() == QTabBar::TriangularSouth) {
                position  = NToolTipPosition::TOP;
                globalPos = mapToGlobal(QPoint(tabRect.center().x(), tabRect.top()));
            } else if (shape() == QTabBar::RoundedWest || shape() == QTabBar::TriangularWest) {
                position  = NToolTipPosition::RIGHT;
                globalPos = mapToGlobal(QPoint(tabRect.right(), tabRect.center().y()));
            } else {
                position  = NToolTipPosition::LEFT;
                globalPos = mapToGlobal(QPoint(tabRect.left(), tabRect.center().y()));
            }

            QWidget* anchor = new QWidget(this);
            anchor->setGeometry(QRect(mapFromGlobal(globalPos), QSize(1, 1)));
            anchor->show();

            d->_currentTooltip->adjustPosition(anchor, position);
            d->_currentTooltip->show();

            QTimer::singleShot(100, anchor, &QWidget::deleteLater);
        }
    } else if (d->_tooltipTabIndex != -1) {
        d->_tooltipTabIndex = -1;
        if (d->_currentTooltip) {
            d->_currentTooltip->hide();
            d->_currentTooltip->deleteLater();
            d->_currentTooltip = nullptr;
        }
    }

    QTabBar::mouseMoveEvent(event);
}

void NTabBar::mouseReleaseEvent(QMouseEvent* event) { QTabBar::mouseReleaseEvent(event); }

void NTabBar::leaveEvent(QEvent* event) {
    Q_D(NTabBar);

    if (d->_hoverIndex != -1) {
        d->_hoverIndex = -1;
        update();
    }
    if (d->_currentTooltip) {
        d->_currentTooltip->hide();
        d->_currentTooltip->deleteLater();
        d->_currentTooltip = nullptr;
    }

    QTabBar::leaveEvent(event);
}

void NTabBar::changeEvent(QEvent* event) {
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange ||
        event->type() == QEvent::PaletteChange) {
        update();
    }

    QTabBar::changeEvent(event);
}

bool NTabBar::event(QEvent* event) {
    Q_D(NTabBar);

    if (event->type() == QEvent::ToolTip) {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        int         tabIndex  = tabAt(helpEvent->pos());

        if (tabIndex >= 0 && tabIndex < count()) {
            QString toolTipText = tabToolTip(tabIndex);
            if (!toolTipText.isEmpty()) {
                if (d->_currentTooltip) {
                    d->_currentTooltip->hide();
                    d->_currentTooltip->deleteLater();
                    d->_currentTooltip = nullptr;
                }
                d->_currentTooltip = NToolTip::createToolTip(toolTipText, window());
                d->_currentTooltip->setDuration(5000);
                QRect tabRect = this->tabRect(tabIndex);

                NToolTipPosition position;
                QPoint           anchorPoint;

                if (shape() == QTabBar::RoundedNorth || shape() == QTabBar::TriangularNorth) {
                    position    = NToolTipPosition::BOTTOM;
                    anchorPoint = QPoint(tabRect.center().x(), tabRect.bottom());
                } else if (shape() == QTabBar::RoundedSouth || shape() == QTabBar::TriangularSouth) {
                    position    = NToolTipPosition::TOP;
                    anchorPoint = QPoint(tabRect.center().x(), tabRect.top());
                } else if (shape() == QTabBar::RoundedWest || shape() == QTabBar::TriangularWest) {
                    position    = NToolTipPosition::RIGHT;
                    anchorPoint = QPoint(tabRect.right(), tabRect.center().y());
                } else {
                    position    = NToolTipPosition::LEFT;
                    anchorPoint = QPoint(tabRect.left(), tabRect.center().y());
                }
                QWidget* anchor = new QWidget(this);
                anchor->setGeometry(QRect(anchorPoint, QSize(1, 1)));
                anchor->show();

                QTimer::singleShot(0, this, [this, d, anchor, position]() {
                    if (d->_currentTooltip) {
                        d->_currentTooltip->adjustPosition(anchor, position);
                        d->_currentTooltip->show();
                        QTimer::singleShot(100, anchor, &QWidget::deleteLater);
                    } else {
                        anchor->deleteLater();
                    }
                });

                return true;
            }
        }
    } else if (event->type() == QEvent::Leave || event->type() == QEvent::MouseButtonPress) {
        if (d->_currentTooltip) {
            d->_currentTooltip->hide();
            d->_currentTooltip->deleteLater();
            d->_currentTooltip = nullptr;
        }
        d->_tooltipTabIndex = -1;
        if (d->_tooltipTimer) {
            d->_tooltipTimer->stop();
        }
    }

    return QTabBar::event(event);
}

void NTabBar::setTabToolTip(int index, const QString& tip) {
    QTabBar::setTabToolTip(index, tip);
    Q_D(NTabBar);
    if (d->_hoverIndex == index && !tip.isEmpty()) {
        // 清理现有工具提示
        if (d->_currentTooltip) {
            d->_currentTooltip->hide();
            d->_currentTooltip->deleteLater();
            d->_currentTooltip = nullptr;
        }

        d->_currentTooltip = NToolTip::createToolTip(tip, this);
        d->_currentTooltip->setDuration(5000);

        QRect tabRect = this->tabRect(index);

        NToolTipPosition position;
        QPoint           globalPos;

        if (shape() == QTabBar::RoundedNorth || shape() == QTabBar::TriangularNorth) {
            position  = NToolTipPosition::BOTTOM;
            globalPos = mapToGlobal(QPoint(tabRect.center().x(), tabRect.bottom()));
        } else if (shape() == QTabBar::RoundedSouth || shape() == QTabBar::TriangularSouth) {
            position  = NToolTipPosition::TOP;
            globalPos = mapToGlobal(QPoint(tabRect.center().x(), tabRect.top()));
        } else if (shape() == QTabBar::RoundedWest || shape() == QTabBar::TriangularWest) {
            position  = NToolTipPosition::RIGHT;
            globalPos = mapToGlobal(QPoint(tabRect.right(), tabRect.center().y()));
        } else {
            position  = NToolTipPosition::LEFT;
            globalPos = mapToGlobal(QPoint(tabRect.left(), tabRect.center().y()));
        }

        QWidget* anchor = new QWidget(this);
        anchor->setGeometry(QRect(mapFromGlobal(globalPos), QSize(1, 1)));
        anchor->show();

        d->_currentTooltip->adjustPosition(anchor, position);
        d->_currentTooltip->show();

        QTimer::singleShot(100, anchor, &QWidget::deleteLater);
    }
}