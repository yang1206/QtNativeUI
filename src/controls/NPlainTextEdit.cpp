#include <QClipboard>
#include <QEnterEvent>
#include <QtNativeUI/NPlainTextEdit.h>
#include "../private/nplaintextedit_p.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkTextDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, LightSelectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, QColor, DarkSelectionTextColor)

Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NPlainTextEdit, int, BorderWidth)

NPlainTextEdit::NPlainTextEdit(QWidget* parent) : QPlainTextEdit(parent), d_ptr(new NPlainTextEditPrivate()) { init(); }

NPlainTextEdit::NPlainTextEdit(const QString& text, QWidget* parent) : NPlainTextEdit(parent) { setPlainText(text); }

NPlainTextEdit::~NPlainTextEdit() {}

void NPlainTextEdit::init() {
    Q_D(NPlainTextEdit);
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

    setObjectName("NPlainTextEdit");
    setStyleSheet("#NPlainTextEdit{background-color:transparent;}");

    d->_plainTextEditStyle = new NEditStyle(d, style());
    setStyle(d->_plainTextEditStyle);

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
        Q_D(NPlainTextEdit);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NPlainTextEdit);
        d->_pLightSelectionBackgroundColor = accentColor.normal();
        d->_pDarkSelectionBackgroundColor  = accentColor.normal();

        bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
        bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

        d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
        d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

        update();
    });
}

void NPlainTextEdit::contextMenuEvent(QContextMenuEvent* event) {
    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    if (!isReadOnly()) {
        action = menu->addItem(tr("撤销"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(document()->isUndoAvailable());
        connect(action, &QAction::triggered, this, &NPlainTextEdit::undo);

        action = menu->addItem(tr("恢复"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(document()->isRedoAvailable());
        connect(action, &QAction::triggered, this, &NPlainTextEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly()) {
        action = menu->addItem(tr("剪切"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && textCursor().hasSelection());
        connect(action, &QAction::triggered, this, &NPlainTextEdit::cut);
    }

    action = menu->addItem(tr("复制"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(textCursor().hasSelection());
    connect(action, &QAction::triggered, this, &NPlainTextEdit::copy);

    if (!isReadOnly()) {
        action = menu->addItem(tr("粘贴"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &NPlainTextEdit::paste);
    }
#endif

    if (!isReadOnly()) {
        action = menu->addItem(tr("删除"), NRegularIconType::Delete16Regular);
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

    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    bool hasText = !document()->isEmpty();
    bool isAllSelected =
        hasText && textCursor().hasSelection() && textCursor().selectedText().length() == toPlainText().length();
    action->setEnabled(hasText && !isAllSelected);
    connect(action, &QAction::triggered, this, &NPlainTextEdit::selectAll);

    menu->exec(event->globalPos());
}