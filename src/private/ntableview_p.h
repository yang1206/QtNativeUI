#ifndef NTABLEVIEW_P_H
#define NTABLEVIEW_P_H

#include <QColor>
#include <QMetaObject>
#include <QObject>
#include <QVector>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NTableView.h"

class NScrollBar;
class QWidget;

class NTableViewPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderPressedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderPressedColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightPlaceholderTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPlaceholderTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightDividerColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDividerColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(int, RowHeight)
    Q_PROPERTY_CREATE_D(int, HeaderHeight)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, CellHorizontalPadding)
    Q_PROPERTY_CREATE_D(QString, PlaceholderText)
    Q_PROPERTY_CREATE_D(bool, BorderVisible)
    Q_PROPERTY_CREATE_D(bool, BackgroundVisible)

  public:
    explicit NTableViewPrivate(QObject* parent = nullptr);
    ~NTableViewPrivate() override;
    Q_D_CREATE(NTableView)

  public:
    QString automaticAccessibleDescription;
    int     hoveredRow{-1};
    QWidget* chromeUnderlay{nullptr};
    QWidget* borderOverlay{nullptr};
    NScrollBar* overlayVerticalScrollBar{nullptr};
    NScrollBar* overlayHorizontalScrollBar{nullptr};
    QVector<QMetaObject::Connection> modelConnections;
    bool isDark{false};
    NThemeType::ThemeMode themeMode{NThemeType::ThemeMode::Light};

    QColor backgroundColor() const;
    QColor headerBackgroundColor() const;
    QColor headerHoverColor() const;
    QColor headerPressedColor() const;
    QColor itemHoverColor() const;
    QColor itemSelectedColor() const;
    QColor textColor() const;
    QColor headerTextColor() const;
    QColor placeholderTextColor() const;
    QColor dividerColor() const;
    QColor borderColor() const;

    void updatePalette();
};

#endif
