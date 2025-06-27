#include <QClipboard>
#include <QEnterEvent>
#include <QFocusEvent>
#include <QStyle>
#include <QtNativeUI/NLineEdit.h>
#include "../private/nlineedit_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkTextDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightSelectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkSelectionTextColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, int, BorderWidth)

NLineEdit::NLineEdit(QWidget* parent) : QLineEdit(parent), d_ptr(new NLineEditPrivate()) { init(); }

NLineEdit::NLineEdit(const QString& text, QWidget* parent) : NLineEdit(parent) { setText(text); }

NLineEdit::~NLineEdit() {}

void NLineEdit::init() {
    Q_D(NLineEdit);
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

    d->_pLightBottomLineColor = nTheme->accentColor().normal();
    d->_pDarkBottomLineColor  = nTheme->accentColor().normal();
    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    d->_pLightSelectionBackgroundColor = nTheme->accentColor().normal();
    d->_pDarkSelectionBackgroundColor  = nTheme->accentColor().normal();

    bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
    bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

    d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
    d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

    d->_pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;
    setObjectName("NLineEdit");
    setStyleSheet("#QLineEdit{border: none;}");
    setStyleSheet("#NLineEdit{background-color:transparent;}");

    d->_lineEditStyle = new NEditStyle(d, style());
    setStyle(d->_lineEditStyle);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumHeight(35);
    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingM).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setTextMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);
    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NLineEdit);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NLineEdit);
        d->_pLightSelectionBackgroundColor = accentColor.normal();
        d->_pDarkSelectionBackgroundColor  = accentColor.normal();

        bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
        bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

        d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
        d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

        update();
    });
}

void NLineEdit::addAction(QAction* action, ActionPosition position) { QLineEdit::addAction(action, position); }

QAction* NLineEdit::addAction(NFilledIconType::Icon icon, ActionPosition position) {
    QAction* action = new QAction(this);
    action->setIcon(nIcon->fromFilled(icon));
    addAction(action, position);
    return action;
}

QAction* NLineEdit::addAction(NRegularIconType::Icon icon, ActionPosition position) {
    QAction* action = new QAction(this);
    action->setIcon(nIcon->fromRegular(icon));
    addAction(action, position);
    return action;
}

void NLineEdit::focusInEvent(QFocusEvent* event) {
    Q_EMIT focusIn(text());
    QLineEdit::focusInEvent(event);
}

void NLineEdit::focusOutEvent(QFocusEvent* event) {
    Q_EMIT focusOut(text());
    QLineEdit::focusOutEvent(event);
}

void NLineEdit::contextMenuEvent(QContextMenuEvent* event) {
    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly()) {
        action = menu->addItem(tr("Undo"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &NLineEdit::undo);

        action = menu->addItem(tr("Redo"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &NLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly()) {
        action = menu->addItem(tr("Cut"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, &NLineEdit::cut);
    }

    action = menu->addItem(tr("Copy"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &NLineEdit::copy);

    if (!isReadOnly()) {
        action = menu->addItem(tr("Paste"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &NLineEdit::paste);
    }
#endif
    if (!isReadOnly()) {
        action = menu->addItem(tr("Delete"), NRegularIconType::Delete16Regular);
        action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
        connect(action, &QAction::triggered, this, [this]() {
            if (hasSelectedText()) {
                int startIndex = selectionStart();
                int endIndex   = selectionEnd();
                setText(text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if (!menu->isEmpty()) {
        menu->addSeparator();
    }
    action = menu->addAction(tr("Select All"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
    connect(action, &QAction::triggered, this, &NLineEdit::selectAll);
    menu->exec(event->globalPos());
}