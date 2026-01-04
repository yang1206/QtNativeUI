#ifndef QTNATIVEUI_NPIVOT_P_H
#define QTNATIVEUI_NPIVOT_P_H

#include <QObject>
#include <QPropertyAnimation>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NPivot.h>

struct NPivotItem {
    QString text;
    QIcon   icon;
    int     iconSize{16};
    bool    enabled{true};
    int     badge{0};
    bool    showBadge{false};
};

struct NPivotRightWidget {
    QWidget* widget{nullptr};
    int      spacing{8};
};

class NPivotPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NPivot)
    Q_PROPERTY_CREATE_D(int, ItemSpacing)
    Q_PROPERTY_CREATE_D(int, IconSpacing)
    Q_PROPERTY_CREATE_D(int, IndicatorHeight)
    Q_PROPERTY_CREATE_D(int, IndicatorRadius)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
    Q_PROPERTY_CREATE_D(int, BadgeSize)
    Q_PROPERTY_CREATE_D(NPivotType::Alignment, Alignment)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColorSelected)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColorSelected)
    Q_PROPERTY_CREATE_D(QColor, LightTextColorHover)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColorHover)
    Q_PROPERTY_CREATE_D(QColor, LightTextColorDisabled)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColorDisabled)
    Q_PROPERTY_CREATE_D(QColor, LightIndicatorColor)
    Q_PROPERTY_CREATE_D(QColor, DarkIndicatorColor)
    Q_PROPERTY_CREATE_D(QColor, LightBadgeColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBadgeColor)
    Q_PROPERTY_CREATE_D(QColor, LightBadgeTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBadgeTextColor)

  public:
    explicit NPivotPrivate(QObject* parent = nullptr);
    ~NPivotPrivate();

    QColor getCurrentColor(const QColor& lightColor, const QColor& darkColor) const;
    int    rightWidgetsTotalWidth() const;

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    QList<NPivotItem>        _items;
    QList<NPivotRightWidget> _rightWidgets;
    int                      _currentIndex{-1};
    int                      _hoverIndex{-1};

    QFont _itemFont;
    QFont _selectedItemFont;
    bool  _useCustomSelectedFont{false};

    qreal               _indicatorX{0};
    qreal               _indicatorWidth{0};
    QPropertyAnimation* _posAnimation{nullptr};
    QPropertyAnimation* _widthAnimation{nullptr};
};

#endif // QTNATIVEUI_NPIVOT_P_H
