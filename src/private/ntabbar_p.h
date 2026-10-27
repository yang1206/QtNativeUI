#ifndef QTNATIVEUI_NTABBAR_P_H
#define QTNATIVEUI_NTABBAR_P_H

#include <QObject>
#include <QPointer>
#include <QUuid>
#include <QVector>
#include <QtNativeUI/NTabBar.h>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NToolTip.h"

class QAbstractButton;

class NTabBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabBar)
    Q_PROPERTY_CREATE_D(int, BorderRadius)

    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderBackground)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderBackground)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderBackgroundSelected)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderBackgroundSelected)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderBackgroundPointerOver)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderBackgroundPointerOver)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderBackgroundPressed)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderBackgroundPressed)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderBackgroundDisabled)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderBackgroundDisabled)

    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderForeground)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderForeground)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderForegroundSelected)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderForegroundSelected)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderForegroundPointerOver)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderForegroundPointerOver)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderForegroundPressed)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderForegroundPressed)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderForegroundDisabled)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderForegroundDisabled)

    Q_PROPERTY_CREATE_D(QColor, LightItemIconForeground)
    Q_PROPERTY_CREATE_D(QColor, DarkItemIconForeground)
    Q_PROPERTY_CREATE_D(QColor, LightItemIconForegroundSelected)
    Q_PROPERTY_CREATE_D(QColor, DarkItemIconForegroundSelected)
    Q_PROPERTY_CREATE_D(QColor, LightItemIconForegroundPointerOver)
    Q_PROPERTY_CREATE_D(QColor, DarkItemIconForegroundPointerOver)
    Q_PROPERTY_CREATE_D(QColor, LightItemIconForegroundPressed)
    Q_PROPERTY_CREATE_D(QColor, DarkItemIconForegroundPressed)
    Q_PROPERTY_CREATE_D(QColor, LightItemIconForegroundDisabled)
    Q_PROPERTY_CREATE_D(QColor, DarkItemIconForegroundDisabled)

    Q_PROPERTY_CREATE_D(QColor, LightItemSeparator)
    Q_PROPERTY_CREATE_D(QColor, DarkItemSeparator)
    Q_PROPERTY_CREATE_D(QColor, LightItemHeaderBorderColorSelected)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHeaderBorderColorSelected)

  public:
    explicit NTabBarPrivate(QObject* parent = nullptr);
    ~NTabBarPrivate();

    NThemeType::ThemeMode           _themeMode;
    bool                            _isDark{false};
    int                             _hoverIndex{-1};
    NTabBar::TabWidthMode           _pTabWidthMode{NTabBar::TabWidthMode::Equal};
    NTabBar::CloseButtonDisplayMode _pCloseButtonDisplayMode{NTabBar::CloseButtonDisplayMode::Always};
    int                             _pTabMinimumWidth{100};
    int                             _pTabMaximumWidth{240};
    int                             _pTabHeight{32};
    bool                            _pTabDragEnabled{false};
    struct TabState {
        QUuid id{QUuid::createUuid()};
        bool  closable{true};
    };
    QVector<TabState>  tabs;
    QUuid              sourceId{QUuid::createUuid()};
    QUuid              pressedId;
    QPoint             pressPosition;
    bool               dragStarted{false};
    bool               layoutPending{false};
    bool               buttonsPending{false};
    bool               acceptDropsBeforeTabDrag{false};
    int                closeCurrentIndex{-1};
    QPointer<NToolTip> tooltip;
    QWidget*           tooltipAnchor{nullptr};

    bool             vertical() const;
    bool             compact(int index) const;
    bool             reserveCloseButton(int index) const;
    QAbstractButton* closeButton(int index) const;
    bool             isCloseButton(const QObject* object) const;
    int              closeButtonIndex(const QObject* button) const;
    bool             initializeCloseButton(int index);
    void             updateHover(int index);
    void             updateCloseButtons();
    void             updateCloseButton(int index);
    void             scheduleCloseButtons();
    void             scheduleLayout();
    void             showToolTip(int index, const QString& text = QString());
    void             hideToolTip();
    int              droppedIndex(const QMimeData* data) const;
};

#endif // QTNATIVEUI_NTABBAR_P_H
