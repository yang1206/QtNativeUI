#include <QClipboard>
#include <QtGui/qevent.h>
#include <QtGui/qguiapplication.h>
#include <QtNativeUI/NDoubleSpinBox.h>
#include "../private/ndoublespinbox_p.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NTheme.h"

// 属性创建
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, LightButtonDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, QColor, DarkButtonDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NDoubleSpinBox, int, BorderWidth)

NDoubleSpinBox::NDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent), d_ptr(new NDoubleSpinBoxPrivate()) { init(); }

NDoubleSpinBox::~NDoubleSpinBox() {}

void NDoubleSpinBox::init() {
    Q_D(NDoubleSpinBox);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    // 初始化颜色
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

    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    // 按钮颜色
    d->_pLightButtonBgColor       = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Light);
    d->_pDarkButtonBgColor        = NThemeColor(NFluentColorKey::ControlFillColorTransparent, NThemeType::Dark);
    d->_pLightButtonHoverColor    = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkButtonHoverColor     = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightButtonPressColor    = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkButtonPressColor     = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightButtonDisabledColor = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Light);
    d->_pDarkButtonDisabledColor  = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;

    // 设置小数精度和显示格式
    setDecimals(2);

    // 设置样式
    d->_spinBoxStyle = new NSpinBoxStyle(d, style());
    setStyle(d->_spinBoxStyle);
    setObjectName("NDoubleSpinBox");
    lineEdit()->setStyleSheet("border: none; background-color:transparent;");
    lineEdit()->setAlignment(Qt::AlignCenter);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);

    setButtonSymbols(QAbstractSpinBox::UpDownArrows);

    setMinimumHeight(35);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NDoubleSpinBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

void NDoubleSpinBox::contextMenuEvent(QContextMenuEvent* event) {
    NMenu* menu = new NMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    if (!isReadOnly()) {
        action = menu->addItem(tr("撤销"), NRegularIconType::ArrowUndo16Regular, QKeySequence::Undo);
        action->setEnabled(lineEdit()->isUndoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::undo);

        action = menu->addItem(tr("恢复"), NRegularIconType::ArrowRedo16Regular, QKeySequence::Redo);
        action->setEnabled(lineEdit()->isRedoAvailable());
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly()) {
        action = menu->addItem(tr("剪切"), NRegularIconType::Cut16Regular, QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && lineEdit()->hasSelectedText() &&
                           lineEdit()->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::cut);
    }

    action = menu->addItem(tr("复制"), NRegularIconType::Copy16Regular, QKeySequence::Copy);
    action->setEnabled(lineEdit()->hasSelectedText() && lineEdit()->echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, lineEdit(), &NLineEdit::copy);

    if (!isReadOnly()) {
        action = menu->addItem(tr("粘贴"), NRegularIconType::ClipboardPaste16Regular, QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, lineEdit(), &NLineEdit::paste);
    }
#endif

    if (!isReadOnly()) {
        action = menu->addItem(tr("删除"), NRegularIconType::Delete16Regular);
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

    // 数值操作菜单项
    action = menu->addItem(tr("增加"), NRegularIconType::ChevronUp20Regular);
    action->setEnabled(isEnabled() && value() < maximum());
    connect(action, &QAction::triggered, this, &NDoubleSpinBox::stepUp);

    action = menu->addItem(tr("减少"), NRegularIconType::ChevronDown20Regular);
    action->setEnabled(isEnabled() && value() > minimum());
    connect(action, &QAction::triggered, this, &NDoubleSpinBox::stepDown);

    // 小数步长信息
    menu->addSeparator();
    QString  stepInfo       = tr("步长: %1").arg(QString::number(singleStep(), 'f', decimals()));
    QAction* stepInfoAction = menu->addAction(stepInfo);
    stepInfoAction->setEnabled(false);

    // 小数位数信息
    QString  decimalInfo       = tr("小数位数: %1").arg(decimals());
    QAction* decimalInfoAction = menu->addAction(decimalInfo);
    decimalInfoAction->setEnabled(false);

    menu->addSeparator();

    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!lineEdit()->text().isEmpty() && !(lineEdit()->selectedText() == lineEdit()->text()));
    connect(action, &QAction::triggered, lineEdit(), &QLineEdit::selectAll);

    menu->exec(event->globalPos());
    delete menu;
    event->accept();
}