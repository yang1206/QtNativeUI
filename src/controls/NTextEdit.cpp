//
// Created by Yang1206 on 2025/4/1.
//
#include <QClipboard>
#include <QEnterEvent>
#include <QtNativeUI/NTextEdit.h>
#include "../private/ntextedit_p.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkTextDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, LightSelectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, QColor, DarkSelectionTextColor)

Q_PROPERTY_CREATE_Q_CPP(NTextEdit, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NTextEdit, int, BorderWidth)

NTextEdit::NTextEdit(QWidget* parent) : QTextEdit(parent), d_ptr(new NTextEditPrivate()) { init(); }

NTextEdit::NTextEdit(const QString& text, QWidget* parent) : NTextEdit(parent) { setText(text); }

NTextEdit::~NTextEdit() {}

void NTextEdit::init() {
    Q_D(NTextEdit);
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

    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    d->_pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightSelectionBackgroundColor = nTheme->accentColor().normal();
    d->_pDarkSelectionBackgroundColor  = nTheme->accentColor().normal();

    bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
    bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

    d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
    d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;
    setObjectName("NTextEdit");
    setStyleSheet("#QTextEdit{border: none;}");
    setStyleSheet("#NTextEdit{background-color:transparent;}");
    d->_textEditStyle = new NEditStyle(d, style());
    setStyle(d->_textEditStyle);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);

    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingM).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setViewportMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);

    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NTextEdit);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NTextEdit);
        d->_pLightSelectionBackgroundColor = accentColor.normal();
        d->_pDarkSelectionBackgroundColor  = accentColor.normal();

        bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
        bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

        d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
        d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

        update();
    });
}

void NTextEdit::contextMenuEvent(QContextMenuEvent* event) {
    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    if (!isReadOnly()) {
        action = menu->addItem(tr("Undo"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(document()->isUndoAvailable());
        connect(action, &QAction::triggered, this, &NTextEdit::undo);

        action = menu->addItem(tr("Redo"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(document()->isRedoAvailable());
        connect(action, &QAction::triggered, this, &NTextEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly()) {
        action = menu->addItem(tr("Cut"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && textCursor().hasSelection());
        connect(action, &QAction::triggered, this, &NTextEdit::cut);
    }

    action = menu->addItem(tr("Copy"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(textCursor().hasSelection());
    connect(action, &QAction::triggered, this, &NTextEdit::copy);

    if (!isReadOnly()) {
        action = menu->addItem(tr("Paste"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &NTextEdit::paste);
    }
#endif

    if (!isReadOnly()) {
        action = menu->addItem(tr("Delete"), NRegularIconType::Delete16Regular);
        action->setEnabled(!isReadOnly() && !document()->isEmpty() && textCursor().hasSelection());
        connect(action, &QAction::triggered, this, [this]() {
            if (textCursor().hasSelection()) {
                textCursor().removeSelectedText();
            }
        });
    }

    if (!menu->isEmpty()) {
        menu->addSeparator();
    }

    action = menu->addAction(tr("Select All"));
    action->setShortcut(QKeySequence::SelectAll);
    bool hasText = !document()->isEmpty();
    bool isAllSelected =
        hasText && textCursor().hasSelection() && textCursor().selectedText().length() == toPlainText().length();
    action->setEnabled(hasText && !isAllSelected);
    connect(action, &QAction::triggered, this, &NTextEdit::selectAll);

    menu->exec(event->globalPos());
}