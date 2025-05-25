//
// Created by Yang1206 on 2025/4/10.
//

#include <QClipboard>
#include <QStyleOptionSpinBox>
#include <QtGui/qevent.h>
#include <QtGui/qguiapplication.h>
#include <QtNativeUI/NSpinBox.h>
#include "../private/nspinbox_p.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkTextDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkSelectionBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightSelectionTextColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkSelectionTextColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, int, BorderWidth)

NSpinBox::NSpinBox(QWidget* parent) : QSpinBox(parent), d_ptr(new NSpinBoxPrivate()) { init(); }

NSpinBox::~NSpinBox() {}

void NSpinBox::init() {
    Q_D(NSpinBox);
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
    d->_pLightBorderColor             = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor              = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderDisabledColor     = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderDisabledColor      = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    d->_pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightBottomLineColor = nTheme->accentColor().normal();
    d->_pDarkBottomLineColor  = nTheme->accentColor().normal();
    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    d->_pLightButtonBgColor       = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Light);
    d->_pDarkButtonBgColor        = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Dark);
    d->_pLightButtonHoverColor    = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkButtonHoverColor     = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightButtonPressColor    = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkButtonPressColor     = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightButtonDisabledColor = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Light);
    d->_pDarkButtonDisabledColor  = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Dark);

    d->_pLightSelectionBackgroundColor = nTheme->accentColor().normal();
    d->_pDarkSelectionBackgroundColor  = nTheme->accentColor().normal();

    bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
    bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

    d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
    d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;

    d->_spinBoxStyle = new NSpinBoxStyle(d, style());
    setStyle(d->_spinBoxStyle);
    setObjectName("NSpinBox");
    lineEdit()->setStyleSheet("border: none; background-color:transparent;");
    lineEdit()->setAlignment(Qt::AlignCenter);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setButtonSymbols(QAbstractSpinBox::UpDownArrows);

    setMinimumHeight(35);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NSpinBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NSpinBox);

        d->_pLightSelectionBackgroundColor = accentColor.normal();
        d->_pDarkSelectionBackgroundColor  = accentColor.normal();

        bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
        bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

        d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
        d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

        update();
    });
}

void NSpinBox::contextMenuEvent(QContextMenuEvent* event) {
    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    if (!isReadOnly()) {
        action = menu->addItem(tr("Undo"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(lineEdit()->isUndoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::undo);

        action = menu->addItem(tr("Redo"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(lineEdit()->isRedoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly()) {
        action = menu->addItem(tr("Cut"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && lineEdit()->hasSelectedText() &&
                           lineEdit()->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::cut);
    }

    action = menu->addItem(tr("Copy"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(lineEdit()->hasSelectedText() && lineEdit()->echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, lineEdit(), &NLineEdit::copy);

    if (!isReadOnly()) {
        action = menu->addItem(tr("Paste"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::paste);
    }
#endif

    if (!isReadOnly()) {
        action = menu->addItem(tr("Delete"), NRegularIconType::Delete16Regular);
        action->setEnabled(!isReadOnly() && !lineEdit()->text().isEmpty() && lineEdit()->hasSelectedText());
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

    if (!menu->isEmpty()) {
        menu->addSeparator();
    }

    action = menu->addItem(tr("Increase"), NRegularIconType::ChevronUp20Regular);
    action->setEnabled(isEnabled() && value() < maximum());
    connect(action, &QAction::triggered, this, &NSpinBox::stepUp);

    action = menu->addItem(tr("Decrease"), NRegularIconType::ChevronDown20Regular);
    action->setEnabled(isEnabled() && value() > minimum());
    connect(action, &QAction::triggered, this, &NSpinBox::stepDown);

    if (singleStep() != 1) {
        menu->addSeparator();

        QString stepInfo = tr("Step: %1").arg(singleStep());
        QAction* stepInfoAction = menu->addAction(stepInfo);
        stepInfoAction->setEnabled(false);
    }

    menu->addSeparator();

    action = menu->addAction(tr("Select All"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!lineEdit()->text().isEmpty() && !(lineEdit()->selectedText() == lineEdit()->text()));
    connect(action, &QAction::triggered, lineEdit(), &QLineEdit::selectAll);

    menu->exec(event->globalPos());
}
