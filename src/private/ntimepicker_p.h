#ifndef QTNATIVEUI_NTIMEPICKER_P_H
#define QTNATIVEUI_NTIMEPICKER_P_H

#include <QHBoxLayout>
#include <QObject>
#include <QTime>
#include <QWidget>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NPicker.h>
#include <QtNativeUI/NTimePicker.h>

class NTimePickerContainer : public QWidget {
    Q_OBJECT
    Q_PROPERTY_CREATE(int, AnimationPixOffsetY)
    Q_PRIVATE_CREATE(int, ButtonAreaHeight)

  public:
    explicit NTimePickerContainer(QWidget* parent = nullptr);
    ~NTimePickerContainer() override;

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
    friend class NTimePicker;
    friend class NTimePickerPrivate;

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

class NTimePickerPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QTime, Time)
    Q_PROPERTY_CREATE_D(bool, Use24HourFormat)
    Q_PROPERTY_CREATE_D(bool, ShowSeconds)

  public:
    explicit NTimePickerPrivate(QObject* parent = nullptr);
    ~NTimePickerPrivate() override;
    Q_D_CREATE(NTimePicker)

    void onTimePickerClicked();
    void onConfirmButtonClicked();
    void onCancelButtonClicked();

    void addPicker(const QStringList& itemList, bool isEnableLoop = true);
    void updatePickersFromTime();
    void rebuildPickers();

    int                   _pickerHeight{245};
    NThemeType::ThemeMode _themeMode;
    QHBoxLayout*          _containerLayout{nullptr};
    NTimePickerContainer* _timePickerContainer{nullptr};

    int _getPickerTotalWidth() const;
};

#endif // QTNATIVEUI_NTIMEPICKER_P_H
