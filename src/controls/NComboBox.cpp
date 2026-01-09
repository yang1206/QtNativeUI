#include <QApplication>
#include <QClipboard>
#include <QGuiApplication>
#include <QLayout>
#include <QListView>
#include <QtNativeUI/NAnimation.h>
#include <QtNativeUI/NComboBox.h>
#include <qevent.h>

#include "../private/ncombobox_p.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkTextDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownButtonDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownButtonDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightSelectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkSelectionTextColor)

Q_PROPERTY_CREATE_Q_CPP(NComboBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, int, BorderWidth)

NComboBox::NComboBox(QWidget* parent) : QComboBox(parent), d_ptr(new NComboBoxPrivate()) { init(); }

NComboBox::NComboBox(const QStringList& items, QWidget* parent) : QComboBox(parent), d_ptr(new NComboBoxPrivate()) {
    init();
    addItems(items);
}

NComboBox::~NComboBox() {}

void NComboBox::init() {
    Q_D(NComboBox);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pLightBackgroundColor         = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor          = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightBackgroundHoverColor    = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkBackgroundHoverColor     = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightBackgroundFocusColor    = NThemeColor(NFluentColorKey::ControlFillColorInputActive, NThemeType::Light);
    d->_pDarkBackgroundFocusColor     = NThemeColor(NFluentColorKey::ControlFillColorInputActive, NThemeType::Dark);
    d->_pLightBackgroundDisabledColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Light);
    d->_pDarkBackgroundDisabledColor  = NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Dark);

    d->_pLightBorderColor         = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor          = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderDisabledColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderDisabledColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    d->_pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightDropdownButtonBgColor    = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Light);
    d->_pDarkDropdownButtonBgColor     = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Dark);
    d->_pLightDropdownButtonHoverColor = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkDropdownButtonHoverColor  = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightDropdownButtonPressColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkDropdownButtonPressColor  = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightDropdownButtonDisabledColor = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Light);
    d->_pDarkDropdownButtonDisabledColor  = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Dark);

    d->_pLightSelectionBackgroundColor = nTheme->accentColor().normal();
    d->_pDarkSelectionBackgroundColor  = nTheme->accentColor().normal();

    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
    bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

    d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
    d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;

    d->_comboBoxStyle = new NComboBoxStyle(d, style());
    setStyle(d->_comboBoxStyle);

    d->_arrowAnimation = new QtNativeUI::NTranslateYAnimation(this, 3.0);
    connect(d->_arrowAnimation, &QtNativeUI::NTranslateYAnimation::yChanged, this, [d](qreal value) {
        d->setArrowYOffset(value);
    });
    setObjectName("NComboBox");
    setMinimumHeight(35);

    setView(new QListView(this));
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    NScrollBar* scrollBar = new NScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);

    comboBoxView->setAutoScroll(true);
    comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
    comboBoxView->setObjectName("NComboBoxView");
    comboBoxView->setStyleSheet("#NComboBoxView{background-color:transparent;}");
    comboBoxView->setStyle(d->_comboBoxStyle);
    QWidget* container = this->findChild<QFrame*>();
    if (container) {
        container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setObjectName("NComboBoxContainer");
        container->setStyle(d->_comboBoxStyle);
        QLayout* layout = container->layout();
        while (layout->count()) {
            layout->takeAt(0);
        }
        layout->addWidget(view());
        layout->setContentsMargins(6, 0, 6, 6);
#ifndef Q_OS_WIN
        container->setStyleSheet("background-color:transparent;");
#endif
    }
    QComboBox::setMaxVisibleItems(5);
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NComboBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NComboBox);

        d->_pLightSelectionBackgroundColor = accentColor.normal();
        d->_pDarkSelectionBackgroundColor  = accentColor.normal();

        bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
        bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

        d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
        d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

        update();
    });
}

QLineEdit* NComboBox::getLineEdit() const { return lineEdit(); }

QSize NComboBox::sizeHint() const { return style()->sizeFromContents(QStyle::CT_ComboBox, nullptr, QSize(), this); }

void NComboBox::showPopup() {
    Q_D(NComboBox);

    d->_isDropdownVisible = true;

    if (d->_arrowAnimation) {
        d->_arrowAnimation->setY(3.0);
    }

    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);

    QWidget* container = this->findChild<QFrame*>();
    if (container && count() > 0) {
        int itemHeight = view()->sizeHintForRow(0);
        if (itemHeight <= 0) {
            itemHeight = 35;
        }

        int viewHeight;
        int containerHeight;
        if (count() >= maxVisibleItems()) {
            viewHeight = maxVisibleItems() * itemHeight;
        } else {
            viewHeight = count() * itemHeight;
        }
        containerHeight = viewHeight + 8;

        view()->setFixedHeight(viewHeight);
        container->setFixedHeight(containerHeight);
        
        auto* effect = new QGraphicsOpacityEffect(container);
        container->setGraphicsEffect(effect);

        QPoint startPos = container->pos() - QPoint(0, 20);
        container->move(startPos);
        
        auto* posAni = new QPropertyAnimation(container, "pos");
        posAni->setDuration(350);
        posAni->setStartValue(startPos);
        posAni->setEndValue(startPos + QPoint(0, 20));
        posAni->setEasingCurve(QEasingCurve::OutCubic);
        posAni->start(QAbstractAnimation::DeleteWhenStopped);

        auto* opacityAni = new QPropertyAnimation(effect, "opacity");
        opacityAni->setDuration(300);
        opacityAni->setStartValue(0.0);
        opacityAni->setEndValue(1.0);
        opacityAni->setEasingCurve(QEasingCurve::OutCubic);
        opacityAni->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void NComboBox::hidePopup() {
    Q_D(NComboBox);

    d->_isDropdownVisible = false;

    if (d->_arrowAnimation) {
        d->_arrowAnimation->setY(0.0);
    }

    QComboBox::hidePopup();
    d->_isDropdownVisible = false;
}

void NComboBox::contextMenuEvent(QContextMenuEvent* event) {
    if (!isEditable()) {
        QComboBox::contextMenuEvent(event);
        return;
    }

    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("Undo"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(lineEdit()->isUndoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::undo);

        action = menu->addItem(tr("Redo"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(lineEdit()->isRedoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("Cut"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!lineEdit()->isReadOnly() && lineEdit()->hasSelectedText() &&
                           lineEdit()->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::cut);
    }

    action = menu->addItem(tr("Copy"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(lineEdit()->hasSelectedText() && lineEdit()->echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, lineEdit(), &QLineEdit::copy);

    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("Paste"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!lineEdit()->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::paste);
    }
#endif

    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("Delete"), NRegularIconType::Delete16Regular);
        action->setEnabled(!lineEdit()->isReadOnly() && !lineEdit()->text().isEmpty() && lineEdit()->hasSelectedText());
        connect(action, &QAction::triggered, this, [this]() {
            if (lineEdit()->hasSelectedText()) {
                int     startIndex = lineEdit()->selectionStart();
                int     endIndex   = startIndex + lineEdit()->selectedText().length();
                QString text       = lineEdit()->text();
                text.remove(startIndex, endIndex - startIndex);
                lineEdit()->setText(text);
            }
        });
    }

    menu->addSeparator();

    action = menu->addAction(tr("Select All"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!lineEdit()->text().isEmpty() && !(lineEdit()->selectedText() == lineEdit()->text()));
    connect(action, &QAction::triggered, lineEdit(), &QLineEdit::selectAll);

    menu->exec(event->globalPos());
}

void NComboBox::mousePressEvent(QMouseEvent* event) {
    Q_D(NComboBox);
    if (d->_arrowAnimation) {
        d->_arrowAnimation->setY(3.0);
    }
    QComboBox::mousePressEvent(event);
}

void NComboBox::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NComboBox);

    if (d->_arrowAnimation && !d->_isDropdownVisible) {
        d->_arrowAnimation->setY(0.0); // 释放时恢复原位
    }

    QComboBox::mouseReleaseEvent(event);
}