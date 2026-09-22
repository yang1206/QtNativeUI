//
// Created by Yang1206 on 2025/12/30.
//

#ifndef NTREEVIEW_P_H
#define NTREEVIEW_P_H

#include <QMetaObject>
#include <QPersistentModelIndex>
#include <QVector>

class QLabel;
class QPainter;
class QVariantAnimation;

#include <QtNativeUI/NTreeView.h>
#include "QtNativeUI/NEnums.h"
#include "nselectionindicator_p.h"
#include "ntreeviewstyle_p.h"

class NTreeItemDelegate;

class NTreeViewPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemPressedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemPressedColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightPlaceholderTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPlaceholderTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, ItemBorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, Indentation)
    Q_PROPERTY_CREATE_D(QString, PlaceholderText)
    Q_PROPERTY_CREATE_D(QString, HeaderText)
    Q_PROPERTY_CREATE_D(bool, BorderVisible)
    Q_PROPERTY_CREATE_D(bool, BackgroundVisible)
    Q_PROPERTY_CREATE_D(bool, SelectionIndicatorVisible)

  public:
    explicit NTreeViewPrivate(QObject* parent = nullptr);
    ~NTreeViewPrivate() override;
    Q_D_CREATE(NTreeView)

  public:
    NTreeViewStyle*                 style{nullptr};
    NTreeItemDelegate*              itemDelegate{nullptr};
    NSelectionIndicatorSession*     selectMark{nullptr};
    QLabel*                         headerLabel{nullptr};
    QVariantAnimation*              expandRevealAnim{nullptr};
    QPersistentModelIndex           pressedIndex;
    QPersistentModelIndex           currentIndicatorIndex;
    QPersistentModelIndex           animParent;
    qreal                           animSubtreeHeight{0.0};
    bool                            animExpanding{true};
    bool                            pendingCollapseFinalize{false};
    bool                            revealAnimationsEnabled{true};
    bool                            isDark{false};
    NThemeType::ThemeMode           themeMode{NThemeType::Light};
    QString                         automaticAccessibleDescription;
    QVector<QMetaObject::Connection> modelConnections;

    QColor itemHoverColor() const;
    QColor itemSelectedColor() const;
    QColor itemPressedColor() const;
    QColor textColor() const;
    QColor placeholderTextColor() const;
    QColor backgroundColor() const;
    QColor borderColor() const;
    int    itemHeight() const { return _pItemHeight; }
    int    itemBorderRadius() const { return _pItemBorderRadius; }
    bool   selectionIndicatorVisible() const { return _pSelectionIndicatorVisible; }

    QRectF selectionMarkRect(const QModelIndex& index) const;
    void   updateSelectionMark(const QModelIndex& previousHint = QModelIndex());
    void   paintSelectionMark(QPainter* painter) const;
    void   updateStyle();
    void   initStyle();
    void   layoutHeader();
    void   updateHeaderStyle();

    bool   isDescendantOf(const QModelIndex& candidate, const QModelIndex& ancestor) const;
    qreal  computeSubtreeHeight(const QModelIndex& parent) const;
    qreal  chevronRotation(const QModelIndex& index) const;
    bool   isRevealRunning() const;
    void   startExpandReveal(const QModelIndex& parent);
    void   startCollapseReveal(const QModelIndex& parent);
    void   finalizeDeferredCollapse();
    void   clearExpandRevealState();
    void   completeActiveExpandReveal();
    void   toggleExpanded(const QModelIndex& index);
};

#endif // NTREEVIEW_P_H
