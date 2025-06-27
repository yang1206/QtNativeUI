#ifndef QTNATIVEUI_NCALENDARWIDGET_P_H
#define QTNATIVEUI_NCALENDARWIDGET_P_H

#include <QObject>

#include "QtNativeUI/NCalendarWidget.h"
#include "QtNativeUI/NEnums.h"

class NCalendarWidget;
class NBaseListView;
class NCalendarModel;
class NCalendarDelegate;
class NPushButton;

class NCalendarWidgetPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NCalendarWidget)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QDate, SelectedDate)
    Q_PROPERTY_CREATE(qreal, ZoomRatio)
    Q_PROPERTY_CREATE(qreal, PixOpacity)

  public:
    explicit NCalendarWidgetPrivate(QObject* parent = nullptr);
    ~NCalendarWidgetPrivate();

    Q_SLOT void onSwitchButtonClicked();
    Q_SLOT void onCalendarViewClicked(const QModelIndex& index);
    Q_SLOT void onUpButtonClicked();
    Q_SLOT void onDownButtonClicked();

    QPixmap                            _oldCalendarViewPix;
    QPixmap                            _newCalendarViewPix;
    int                                _lastSelectedYear{0};
    int                                _lastSelectedMonth{1};
    qreal                              _borderWidth{1.5};
    NThemeType::ThemeMode              _themeMode;
    bool                               _isDark{false};
    NBaseListView*                     _calendarView{nullptr};
    NCalendarModel*                    _calendarModel{nullptr};
    NCalendarDelegate*                 _calendarDelegate{nullptr};
    NBaseListView*                     _calendarTitleView{nullptr};
    NPushButton*                       _modeSwitchButton{nullptr};
    NPushButton*                       _upButton{nullptr};
    NPushButton*                       _downButton{nullptr};
    bool                               _isSwitchAnimationFinished{true};
    bool                               _isDrawNewPix{false};
    QLocale                            _locale;
    NCalendarWidget::DateSelectionMode _selectionMode{NCalendarWidget::SingleDate};
    QList<QDate>                       _selectedDates;
    QPair<QDate, QDate>                _selectedDateRange;

    void handleDateSelection(const QDate& date);
    void updateDateSelection();

    void _scrollToDate(QDate date);
    void _doSwitchAnimation(bool isZoomIn);
    void _updateSwitchButtonText();
};

#endif // QTNATIVEUI_NCALENDARWIDGET_P_H