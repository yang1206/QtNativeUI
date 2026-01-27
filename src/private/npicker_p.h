#ifndef QTNATIVEUI_NPICKER_P_H
#define QTNATIVEUI_NPICKER_P_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QTimer>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NPicker.h>

class QPropertyAnimation;
class NPickerPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ScrollOffset)
    Q_PROPERTY_CREATE_D(QStringList, Items)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, VisibleItemCount)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(bool, Container)
    Q_PROPERTY_CREATE_D(bool, LoopEnabled)

  public:
    explicit NPickerPrivate(QObject* parent = nullptr);
    ~NPickerPrivate() override;
    Q_D_CREATE(NPicker)

  private:
    NThemeType::ThemeMode _themeMode;
    QPropertyAnimation*   _scrollAnimation{nullptr};
    int                   _penBorderWidth{1};
    int                   _indicatorMargin{3};
    bool                  _isUpArrowPress{false};
    bool                  _isDownArrowPress{false};
    QRect                 _upArrowRect;
    QRect                 _downArrowRect;
    QTimer*               _pressSustainTimer{nullptr};
    QTimer*               _repeatScrollTimer{nullptr};
    qreal                 _targetScrollOffset{0};
    QPoint                _mousePoint;

    void _scroll(int delta);
};

#endif // QTNATIVEUI_NPICKER_P_H
