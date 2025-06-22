#ifndef NSUGGESTBOXPRIVATE_H
#define NSUGGESTBOXPRIVATE_H

#include <QModelIndex>
#include <QObject>
#include <QVector>

#include "QtNativeUI/NAutoSuggestBox.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NIconEnums.h"
#include "QtNativeUI/stdafx.h"

class NAutoSuggestion : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE(NRegularIconType::Icon, Icon)
    Q_PROPERTY_CREATE(NFilledIconType::Icon, FilledIcon)
    Q_PROPERTY_CREATE(QString, Text)
    Q_PROPERTY_CREATE(QString, Key)
    Q_PROPERTY_CREATE(QVariantMap, Data)

  public:
    explicit NAutoSuggestion(QObject* parent = nullptr);
    ~NAutoSuggestion();
};

class QVBoxLayout;
class NLineEdit;
class NBaseListView;
class NAutoSuggestModel;
class NAutoSuggestDelegate;
class QWidget;
class QPropertyAnimation;

// 下拉列表容器
class NAutoSuggestPopup : public QWidget {
    Q_OBJECT

  public:
    explicit NAutoSuggestPopup(QWidget* parent = nullptr);
    ~NAutoSuggestPopup();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    NThemeType::ThemeMode _themeMode{NThemeType::Light};
};

class NAutoSuggestBoxPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NAutoSuggestBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)

  public:
    explicit NAutoSuggestBoxPrivate(QObject* parent = nullptr);
    ~NAutoSuggestBoxPrivate();
    bool eventFilter(QObject* watched, QEvent* event) override;

  public slots:
    void onTextChanged(const QString& text);
    void onSuggestionSelected(const QModelIndex& index);

  private:
    void setupUI();
    void selectNextSuggestion();
    void selectPreviousSuggestion();
    void applySelectedSuggestion();

    void _startSizeAnimation(QSize oldSize, QSize newSize);
    void _startExpandAnimation();
    void _startCloseAnimation();

    NThemeType::ThemeMode _themeMode{NThemeType::Light};
    NLineEdit*            _lineEdit{nullptr};
    NAutoSuggestPopup*    _popup{nullptr};
    NBaseListView*        _listView{nullptr};
    NAutoSuggestModel*    _model{nullptr};
    NAutoSuggestDelegate* _delegate{nullptr};
    QVBoxLayout*          _popupLayout{nullptr};

    QVector<NAutoSuggestion*> _suggestions;

    QSize _lastSize;
    bool  _isExpandAnimationFinished{true};
    bool  _isCloseAnimationFinished{true};

    NAutoSuggestBox::FilterMode _filterMode{NAutoSuggestBox::Contains};
    int                         _currentIndex{-1};
};

#endif // NSUGGESTBOXPRIVATE_H