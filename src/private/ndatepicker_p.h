#ifndef QTNATIVEUI_NDATEPICKER_P_H
#define QTNATIVEUI_NDATEPICKER_P_H

#include <QDate>
#include <QHBoxLayout>
#include <QObject>
#include <QWidget>
#include <QtNativeUI/NDatePicker.h>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NPicker.h>

class NDatePickerContainer : public QWidget {
    Q_OBJECT
    Q_PROPERTY_CREATE(int, AnimationPixOffsetY)
    Q_PRIVATE_CREATE(int, ButtonAreaHeight)

  public:
    explicit NDatePickerContainer(QWidget* parent = nullptr);
    ~NDatePickerContainer() override;

    void doPickerAnimation();

  signals:
    void confirmButtonClicked();
    void cancelButtonClicked();

  protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

  private:
    friend class NDatePicker;
    friend class NDatePickerPrivate;

    int                   _buttonMargin{5};
    int                   _buttonSpacing{5};
    QPixmap               _animationPix;
    QRect                 _confirmButtonRect;
    QRect                 _cancelButtonRect;
    bool                  _isConfirmButtonHover{false};
    bool                  _isCancelButtonHover{false};
    bool                  _isConfirmButtonClicked{false};
    QList<NPicker*>       _pickerList;
    QList<int>            _historyIndexList;
    NThemeType::ThemeMode _themeMode;

    void _handleSaveOrReset(bool isSave);
};

class NDatePickerPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QDate, Date)
    Q_PROPERTY_CREATE_D(QDate, MinimumDate)
    Q_PROPERTY_CREATE_D(QDate, MaximumDate)
    Q_PROPERTY_CREATE_D(QLocale, Locale)

  public:
    explicit NDatePickerPrivate(QObject* parent = nullptr);
    ~NDatePickerPrivate() override;
    Q_D_CREATE(NDatePicker)

    void onDatePickerClicked();
    void onConfirmButtonClicked();
    void onCancelButtonClicked();
    void onYearChanged();
    void onMonthChanged();

    void addPicker(const QStringList& itemList, bool isEnableLoop = true, int width = 60);
    void updatePickersFromDate();
    void rebuildPickers();
    void updateDayPicker();

    int                   _pickerHeight{245};
    NThemeType::ThemeMode _themeMode;
    QHBoxLayout*          _containerLayout{nullptr};
    NDatePickerContainer* _datePickerContainer{nullptr};

    NPicker* _yearPicker{nullptr};
    NPicker* _monthPicker{nullptr};
    NPicker* _dayPicker{nullptr};

    int _getPickerTotalWidth() const;
};

#endif // QTNATIVEUI_NDATEPICKER_P_H
