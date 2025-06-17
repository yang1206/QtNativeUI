#include "nsuggestbox_p.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QUuid>
#include <QVBoxLayout>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NSuggestBox.h"
#include "QtNativeUI/NTheme.h"
#include "nbaselistview.h"
#include "nsuggestmodel.h"

// NSuggestion 实现
NSuggestion::NSuggestion(QObject* parent) : QObject(parent) {
    _pIcon = NRegularIconType::None;
    _pText = "";
    _pKey  = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _pData = QVariantMap();
}

NSuggestion::~NSuggestion() {}

// NSuggestPopup 实现
NSuggestPopup::NSuggestPopup(QWidget* parent)
    : QWidget(parent, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint) {
    setObjectName("NSuggestPopup");
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(8, 8, 8, 8);

    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        _themeMode = mode;
        update();
    });
}

NSuggestPopup::~NSuggestPopup() {}

void NSuggestPopup::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制阴影
    QColor shadowColor = nTheme->isDarkMode() ? QColor(0, 0, 0, 80) : QColor(0, 0, 0, 30);
    for (int i = 0; i < 10; i++) {
        painter.setPen(
            QPen(QColor(shadowColor.red(), shadowColor.green(), shadowColor.blue(), shadowColor.alpha() / (i + 1)), 1));
        painter.drawRoundedRect(QRect(8 - i, 8 - i, width() - 16 + i * 2, height() - 16 + i * 2), 8, 8);
    }

    // 绘制背景
    painter.setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, _themeMode));
    painter.setBrush(NThemeColor(NFluentColorKey::LayerFillColorDefault, _themeMode));
    painter.drawRoundedRect(QRect(8, 8, width() - 16, height() - 16), 8, 8);
}

// NSuggestBoxPrivate 实现
NSuggestBoxPrivate::NSuggestBoxPrivate(QObject* parent) : QObject(parent) {
    _themeMode                   = nTheme->themeMode();
    _isExpandAnimationFinished   = true;
    _isCollapseAnimationFinished = true;
}

NSuggestBoxPrivate::~NSuggestBoxPrivate() { qDeleteAll(_suggestions); }

void NSuggestBoxPrivate::setupUI() {
    Q_Q(NSuggestBox);
    q->setFixedHeight(35);

    // 创建输入框
    _lineEdit = new NLineEdit(q);
    _lineEdit->setFixedHeight(35);
    _lineEdit->setPlaceholderText("搜索...");
    _lineEdit->setClearButtonEnabled(true);

    // 创建搜索图标，使用 NLineEdit 的 addAction 方法
    _lineEdit->addAction(NRegularIconType::Search16Regular, NLineEdit::TrailingPosition);

    // 创建布局
    QVBoxLayout* layout = new QVBoxLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(_lineEdit);

    // 创建弹出窗口
    _popup = new NSuggestPopup(q->window());

    // 创建列表视图
    _listView = new NBaseListView(_popup);
    _listView->setFrameShape(QFrame::NoFrame);
    _listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _listView->setSelectionMode(QAbstractItemView::SingleSelection);

    // 设置滚动条
    NScrollBar* scrollBar = new NScrollBar(_listView);
    _listView->setVerticalScrollBar(scrollBar);

    // 创建模型和委托
    _model    = new NSuggestModel(q);
    _delegate = new NSuggestDelegate(q);
    _listView->setModel(_model);
    _listView->setItemDelegate(_delegate);

    // 设置弹出窗口布局
    _popupLayout = new QVBoxLayout(_popup);
    _popupLayout->setContentsMargins(8, 8, 8, 8);
    _popupLayout->addWidget(_listView);

    // 连接信号
    connect(_lineEdit, &NLineEdit::textChanged, this, &NSuggestBoxPrivate::onTextChanged);
    connect(_lineEdit, &NLineEdit::focusIn, this, &NSuggestBoxPrivate::onFocusChanged);
    connect(_lineEdit, &NLineEdit::focusOut, this, [this]() {
        // 延迟关闭，避免点击列表项时因失焦而关闭
        QTimer::singleShot(100, this, &NSuggestBoxPrivate::onFocusChanged);
    });
    connect(_listView, &QListView::clicked, this, &NSuggestBoxPrivate::onSuggestionSelected);

    // 创建动画
    _sizeAnimation = new QPropertyAnimation(_popup, "size");
    _sizeAnimation->setDuration(200);
    _sizeAnimation->setEasingCurve(QEasingCurve::OutCubic);

    _posAnimation = new QPropertyAnimation(_listView, "pos");
    _posAnimation->setDuration(200);
    _posAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(_sizeAnimation, &QPropertyAnimation::finished, this, [this]() {
        _isAnimating = false;
        if (_sizeAnimation->endValue().toSize().height() == 0) {
            _popup->hide();
        }
    });
}

void NSuggestBoxPrivate::onTextChanged(const QString& text) {
    if (text.isEmpty()) {
        startCollapseAnimation();
        return;
    }

    // 过滤建议项
    _filteredSuggestions.clear();
    for (NSuggestion* suggestion : _suggestions) {
        if (suggestion->getText().contains(text, _pCaseSensitivity)) {
            _filteredSuggestions.append(suggestion);
        }
    }

    if (_filteredSuggestions.isEmpty()) {
        startCollapseAnimation();
        return;
    }

    // 更新模型
    _model->setSuggestions(_filteredSuggestions);

    // 显示弹出窗口
    if (!_popup->isVisible()) {
        updatePopupPosition();
        _popup->show();
        startExpandAnimation();
    } else {
        // 调整弹出窗口大小
        int rowCount = qMin(_filteredSuggestions.count(), 5);
        int height   = rowCount * 40 + 16;

        if (_lastSize.isValid() && _lastSize.height() != height) {
            _sizeAnimation->setStartValue(_popup->size());
            _sizeAnimation->setEndValue(QSize(_popup->width(), height));
            _sizeAnimation->start();
            _lastSize = _sizeAnimation->endValue().toSize();
        }
    }
}

void NSuggestBoxPrivate::onFocusChanged() {
    Q_Q(NSuggestBox);
    if (!_lineEdit->hasFocus() && !_listView->hasFocus()) {
        startCollapseAnimation();
    } else if (_lineEdit->hasFocus() && !_lineEdit->text().isEmpty() && !_filteredSuggestions.isEmpty()) {
        updatePopupPosition();
        _popup->show();
        startExpandAnimation();
    }
}

void NSuggestBoxPrivate::onSuggestionSelected(const QModelIndex& index) {
    Q_Q(NSuggestBox);
    if (!index.isValid())
        return;

    NSuggestion* suggestion = _model->getSuggestion(index.row());
    if (suggestion) {
        _lineEdit->setText(suggestion->getText());
        emit q->suggestionClicked(suggestion->getText(), suggestion->getData());
        startCollapseAnimation();
    }
}

void NSuggestBoxPrivate::updatePopupPosition() {
    Q_Q(NSuggestBox);
    QPoint pos = q->mapToGlobal(QPoint(0, q->height()));

    // 确保弹出窗口不会超出屏幕边界
    QRect screenGeometry = QApplication::screenAt(pos)->geometry();
    if (pos.y() + 300 > screenGeometry.height()) {
        pos.setY(q->mapToGlobal(QPoint(0, 0)).y() - 300);
    }

    _popup->setFixedWidth(q->width());
    _popup->move(pos);
}

void NSuggestBoxPrivate::startExpandAnimation() {
    if (_isAnimating || !_isExpandAnimationFinished)
        return;

    _isAnimating                 = true;
    _isExpandAnimationFinished   = false;
    _isCollapseAnimationFinished = true;

    // 计算高度
    int rowCount     = qMin(_filteredSuggestions.count(), 5);
    int targetHeight = rowCount * 40 + 16;

    // 设置初始位置
    if (!_popup->isVisible() || _popup->height() == 0) {
        _popup->resize(_popup->width(), 0);
        _listView->move(8, -targetHeight);
    }

    // 开始大小动画
    _sizeAnimation->setStartValue(QSize(_popup->width(), _popup->height()));
    _sizeAnimation->setEndValue(QSize(_popup->width(), targetHeight));
    _sizeAnimation->start();

    // 开始位置动画
    _posAnimation->setStartValue(_listView->pos());
    _posAnimation->setEndValue(QPoint(8, 8));
    _posAnimation->start();

    connect(_posAnimation, &QPropertyAnimation::finished, this, [this]() { _isExpandAnimationFinished = true; });

    _lastSize = _sizeAnimation->endValue().toSize();
}

void NSuggestBoxPrivate::startCollapseAnimation() {
    if (_isAnimating || !_isCollapseAnimationFinished || !_popup->isVisible())
        return;

    _isAnimating                 = true;
    _isCollapseAnimationFinished = false;
    _isExpandAnimationFinished   = true;

    // 开始大小动画
    _sizeAnimation->setStartValue(QSize(_popup->width(), _popup->height()));
    _sizeAnimation->setEndValue(QSize(_popup->width(), 0));
    _sizeAnimation->start();

    // 开始位置动画
    _posAnimation->setStartValue(_listView->pos());
    _posAnimation->setEndValue(QPoint(8, -_listView->height()));
    _posAnimation->start();

    connect(_posAnimation, &QPropertyAnimation::finished, this, [this]() {
        _isCollapseAnimationFinished = true;
        _model->clearSuggestions();
    });

    _lastSize = _sizeAnimation->endValue().toSize();
}