#ifndef NSUGGESTBOXPRIVATE_H
#define NSUGGESTBOXPRIVATE_H

#include <QModelIndex>
#include <QObject>
#include <QVariantMap>
#include <QVector>

#include <QWidget>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NIconEnums.h"
#include "QtNativeUI/stdafx.h"

class NSuggestion : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE(NRegularIconType::Icon, Icon)
    Q_PROPERTY_CREATE(QString, Text)
    Q_PROPERTY_CREATE(QString, Key)
    Q_PROPERTY_CREATE(QVariantMap, Data)

  public:
    explicit NSuggestion(QObject* parent = nullptr);
    ~NSuggestion();
};

class QVBoxLayout;
class NLineEdit;
class QListView;
class NSuggestModel;
class NSuggestDelegate;
class NSuggestBox;
class QWidget;
class QPropertyAnimation;

// 下拉列表容器
class NSuggestPopup : public QWidget {
    Q_OBJECT

  public:
    explicit NSuggestPopup(QWidget* parent = nullptr);
    ~NSuggestPopup();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    NThemeType::ThemeMode _themeMode{NThemeType::Light};
};

class NSuggestBoxPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NSuggestBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)

  public:
    explicit NSuggestBoxPrivate(QObject* parent = nullptr);
    ~NSuggestBoxPrivate();

  public slots:
    void onTextChanged(const QString& text);
    void onFocusChanged();
    void onSuggestionSelected(const QModelIndex& index);

  private:
    void setupUI();
    void updatePopupPosition();
    void startExpandAnimation();
    void startCollapseAnimation();

    NThemeType::ThemeMode _themeMode{NThemeType::Light};
    NLineEdit*            _lineEdit{nullptr};
    NSuggestPopup*        _popup{nullptr};
    QListView*            _listView{nullptr};
    NSuggestModel*        _model{nullptr};
    NSuggestDelegate*     _delegate{nullptr};
    QVBoxLayout*          _popupLayout{nullptr};

    QVector<NSuggestion*> _suggestions;
    QVector<NSuggestion*> _filteredSuggestions;

    bool                _isAnimating{false};
    QSize               _lastSize;
    QPropertyAnimation* _sizeAnimation{nullptr};
    QPropertyAnimation* _posAnimation{nullptr};

    bool _isExpandAnimationFinished{true};
    bool _isCollapseAnimationFinished{true};
};

#endif // NSUGGESTBOXPRIVATE_H