#include <QClipboard>
#include <QGuiApplication>
#include <QLineEdit>
#include <QStyleOptionComboBox>
#include <QtNativeUI/NComboBox.h>
#include <qevent.h>
#include "../private/ncombobox_p.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"
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

Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownBgColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownBgColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownPressColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownPressColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, LightDropdownDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NComboBox, QColor, DarkDropdownDisabledColor)

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

    // 初始化各种颜色
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

    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    d->_pLightDropdownBgColor       = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Light);
    d->_pDarkDropdownBgColor        = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Dark);
    d->_pLightDropdownHoverColor    = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkDropdownHoverColor     = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightDropdownPressColor    = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkDropdownPressColor     = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightDropdownDisabledColor = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Light);
    d->_pDarkDropdownDisabledColor  = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Dark);

    d->_pLightSelectionBackgroundColor = nTheme->accentColor().normal();
    d->_pDarkSelectionBackgroundColor  = nTheme->accentColor().normal();

    bool useDarkTextOnLight = d->_pLightSelectionBackgroundColor.lightnessF() > 0.5;
    bool useDarkTextOnDark  = d->_pDarkSelectionBackgroundColor.lightnessF() > 0.5;

    d->_pLightSelectionTextColor = useDarkTextOnLight ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);
    d->_pDarkSelectionTextColor  = useDarkTextOnDark ? QColor(0x00, 0x00, 0x00) : QColor(0xFF, 0xFF, 0xFF);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;

    // 设置样式
    d->_comboBoxStyle = new NComboBoxStyle(d, style());
    setStyle(d->_comboBoxStyle);

    setObjectName("NComboBox");

    if (isEditable()) {
        lineEdit()->setStyleSheet("border: none; background-color: transparent;");
    }

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_MacShowFocusRect, false);

    setMinimumHeight(35);

    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NComboBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    // 连接强调色变化信号
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

void NComboBox::showPopup() {
    Q_D(NComboBox);
    d->_isDropdownVisible = true;
    QComboBox::showPopup();
    update();
}

void NComboBox::hidePopup() {
    Q_D(NComboBox);
    d->_isDropdownVisible = false;
    QComboBox::hidePopup();
    update();
}

void NComboBox::contextMenuEvent(QContextMenuEvent* event) {
    if (!isEditable()) {
        // 对于不可编辑的ComboBox，使用默认菜单
        QComboBox::contextMenuEvent(event);
        return;
    }

    // 对于可编辑的ComboBox，创建自定义菜单
    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("撤销"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(lineEdit()->isUndoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::undo);

        action = menu->addItem(tr("恢复"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(lineEdit()->isRedoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("剪切"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!lineEdit()->isReadOnly() && lineEdit()->hasSelectedText() &&
                           lineEdit()->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::cut);
    }

    action = menu->addItem(tr("复制"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(lineEdit()->hasSelectedText() && lineEdit()->echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, lineEdit(), &QLineEdit::copy);

    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("粘贴"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!lineEdit()->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, lineEdit(), &QLineEdit::paste);
    }
#endif

    if (!lineEdit()->isReadOnly()) {
        action = menu->addItem(tr("删除"), NRegularIconType::Delete16Regular);
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

    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!lineEdit()->text().isEmpty() && !(lineEdit()->selectedText() == lineEdit()->text()));
    connect(action, &QAction::triggered, lineEdit(), &QLineEdit::selectAll);

    menu->exec(event->globalPos());
}