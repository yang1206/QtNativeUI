#include "nautosuggestbox_p.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QUuid>
#include <QVBoxLayout>

#include "QtNativeUI/NAutoSuggestBox.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"
#include "nautosuggestmodel.h"
#include "nbaselistview.h"

NAutoSuggestion::NAutoSuggestion(QObject* parent) : QObject(parent) {
    _pIcon = NRegularIconType::None;
    _pText = "";
    _pKey  = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _pData = QVariantMap();
}

NAutoSuggestion::~NAutoSuggestion() {}

NAutoSuggestPopup::NAutoSuggestPopup(QWidget* parent) : QWidget(parent) {
    setObjectName("NAutoSuggestPopup");
    setStyleSheet("#NAutoSuggestPopup{background-color:transparent}");
    setContentsMargins(8, 8, 8, 8);

    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) { _themeMode = mode; });
}

NAutoSuggestPopup::~NAutoSuggestPopup() {}

void NAutoSuggestPopup::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    // 绘制背景
    painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorSecondary, _themeMode));
    painter.setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, _themeMode));
    QRect foregroundRect(6, 0, rect().width() - 2 * 6, rect().height() - 6);
    painter.drawRoundedRect(foregroundRect, 8, 8);
    painter.restore();
}

// NAutoSuggestBoxPrivate 实现
NAutoSuggestBoxPrivate::NAutoSuggestBoxPrivate(QObject* parent) : QObject(parent) { _themeMode = nTheme->themeMode(); }

NAutoSuggestBoxPrivate::~NAutoSuggestBoxPrivate() {}

void NAutoSuggestBoxPrivate::setupUI() {
    Q_Q(NAutoSuggestBox);
    q->setFixedHeight(35);

    // 创建输入框
    _lineEdit = new NLineEdit(q);
    _lineEdit->setFixedHeight(35);
    _lineEdit->setPlaceholderText("查找功能");
    _lineEdit->setClearButtonEnabled(true);
    _lineEdit->addAction(NRegularIconType::Search16Regular, NLineEdit::TrailingPosition);

    // 创建布局
    QVBoxLayout* mainLayout = new QVBoxLayout(q);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_lineEdit);

    // 创建弹出窗口
    _popup = new NAutoSuggestPopup(q->window());
    // 设置弹出窗口布局
    _popupLayout = new QVBoxLayout(_popup);
    _popupLayout->setContentsMargins(0, 0, 0, 0);
    // 创建列表视图
    _listView                   = new NBaseListView(_popup);
    NScrollBar* floatVScrollBar = new NScrollBar(_listView->verticalScrollBar(), _listView);
    // _listView->setVerticalScrollBar(floatVScrollBar);
    _popup->resize(292, 300);
    _popupLayout->addWidget(_listView);
    _model    = new NAutoSuggestModel(q);
    _delegate = new NAutoSuggestDelegate(q);
    _listView->setModel(_model);
    _listView->setItemDelegate(_delegate);
    _listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _popup->hide();
    // 连接信号
    connect(_lineEdit, &NLineEdit::textChanged, this, &NAutoSuggestBoxPrivate::onTextChanged);
    connect(_lineEdit, &NLineEdit::focusIn, this, &NAutoSuggestBoxPrivate::onTextChanged);
    connect(_lineEdit, &NLineEdit::focusOut, this, [this]() { _startCloseAnimation(); });
    connect(_listView, &QListView::clicked, this, &NAutoSuggestBoxPrivate::onSuggestionSelected);
}

void NAutoSuggestBoxPrivate::onTextChanged(const QString& text) {
    Q_Q(NAutoSuggestBox);
    if (text.isEmpty()) {
        _startCloseAnimation();
        return;
    }

    QVector<NAutoSuggestion*> suggestionVector;
    for (NAutoSuggestion* suggestion : _suggestions) {
        if (suggestion->getText().contains(text, _pCaseSensitivity)) {
            suggestionVector.append(suggestion);
        }
    }

    if (!suggestionVector.isEmpty()) {
        _model->setSuggestions(suggestionVector);
        int rowCount = suggestionVector.count();
        if (rowCount > 4) {
            rowCount = 4;
        }
        if (!_popup->isVisible()) {
            q->raise();
            _popup->show();
            _popup->raise();
            QPoint cyclePoint = _popup->mapFromGlobal(q->mapToGlobal(QPoint(-5, q->height())));
            if (cyclePoint != QPoint(0, 0)) {
                _popup->move(cyclePoint);
            }
            _startSizeAnimation(QSize(q->width() + 10, 0), QSize(q->width() + 10, 40 * rowCount + 16));
            _listView->move(_popup->x(), -(40 * rowCount + 16));
        } else {
            _startSizeAnimation(_popup->size(), QSize(q->width() + 12, 40 * rowCount + 16));
        }
        _startExpandAnimation();
    } else {
        _startCloseAnimation();
    }
}

void NAutoSuggestBoxPrivate::onSuggestionSelected(const QModelIndex& index) {
    Q_Q(NAutoSuggestBox);
    _lineEdit->clear();
    _listView->clearSelection();
    if (!index.isValid()) {
        return;
    }
    NAutoSuggestion* suggest = _model->getSuggestion(index.row());
    Q_EMIT q->suggestionClicked(suggest->getText(), suggest->getData());
    _startCloseAnimation();
}

void NAutoSuggestBoxPrivate::_startSizeAnimation(QSize oldSize, QSize newSize) {
    if (_lastSize.isValid() && _lastSize == newSize) {
        return;
    }
    _popupLayout->removeWidget(_listView);
    QPropertyAnimation* expandAnimation = new QPropertyAnimation(_popup, "size");
    connect(expandAnimation, &QPropertyAnimation::valueChanged, this, [=]() { _listView->resize(_popup->size()); });
    connect(expandAnimation, &QPropertyAnimation::finished, this, [=]() {
        _popupLayout->addWidget(_listView);
        qDebug() << "Popup height:" << _popup->height() << "ListView height:" << _listView->height()
                 << "Item count:" << _model->rowCount();
    });
    expandAnimation->setDuration(300);
    expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
    expandAnimation->setStartValue(oldSize);
    expandAnimation->setEndValue(newSize);
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _lastSize = newSize;
}

void NAutoSuggestBoxPrivate::_startExpandAnimation() {
    if (!_isExpandAnimationFinished) {
        return;
    }
    _isCloseAnimationFinished           = true;
    _isExpandAnimationFinished          = false;
    QPropertyAnimation* expandAnimation = new QPropertyAnimation(_listView, "pos");
    connect(expandAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isExpandAnimationFinished = true;
        _listView->clearSelection();
    });
    expandAnimation->setDuration(300);
    expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
    expandAnimation->setStartValue(_listView->pos());
    expandAnimation->setEndValue(QPoint(8, 8));
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NAutoSuggestBoxPrivate::_startCloseAnimation() {
    if (!_isCloseAnimationFinished) {
        return;
    }
    _isExpandAnimationFinished               = true;
    _isCloseAnimationFinished                = false;
    QPropertyAnimation* baseWidgetsAnimation = new QPropertyAnimation(_popup, "size");
    baseWidgetsAnimation->setDuration(300);
    baseWidgetsAnimation->setEasingCurve(QEasingCurve::InOutSine);
    baseWidgetsAnimation->setStartValue(_popup->size());
    baseWidgetsAnimation->setEndValue(QSize(_popup->width(), 0));
    baseWidgetsAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation* closeAnimation = new QPropertyAnimation(_listView, "pos");
    connect(closeAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isCloseAnimationFinished = true;
        _model->clearSuggestions();
        _popup->hide();
    });
    closeAnimation->setDuration(300);
    closeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closeAnimation->setStartValue(_listView->pos());
    closeAnimation->setEndValue(QPoint(_listView->pos().x(), -_listView->height()));
    closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _lastSize = baseWidgetsAnimation->endValue().toSize();
}