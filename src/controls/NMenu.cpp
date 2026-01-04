#include "QtNativeUI/NMenu.h"

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "../private/nmenu_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")

#ifndef DWMWA_NCRENDERING_POLICY
#define DWMWA_NCRENDERING_POLICY 2
#endif

#ifndef DWMNCRP_ENABLED
#define DWMNCRP_ENABLED 2
#endif

#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

#ifndef DWMSBT_MAINWINDOW
#define DWMSBT_MAINWINDOW 2
#endif
#endif

Q_PROPERTY_CREATE_Q_CPP(NMenu, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NMenu, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NMenu, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NMenu, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NMenu, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NMenu, QColor, LightSeparatorColor)
Q_PROPERTY_CREATE_Q_CPP(NMenu, QColor, DarkSeparatorColor)
NMenu::NMenu(QWidget* parent) : QMenu(parent), d_ptr(new NMenuPrivate()) { init(); }

NMenu::NMenu(const QString& title, QWidget* parent) : NMenu(parent) { setTitle(title); }

NMenu::~NMenu() {}

void NMenu::setDropDownAnimation(bool enabled) {
    Q_D(NMenu);
    d->_enableDropDownAnimation = enabled;
}

bool NMenu::dropDownAnimation() const {
    Q_D(const NMenu);
    return d->_enableDropDownAnimation;
}

void NMenu::init() {
    Q_D(NMenu);
    d->q_ptr = this;

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pBorderRadius         = NDesignToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);
    d->_pLightBorderColor     = NThemeColor(NFluentColorKey::ControlStrokeColorSecondary, NThemeType::Light);
    d->_pDarkBorderColor      = NThemeColor(NFluentColorKey::ControlStrokeColorSecondary, NThemeType::Dark);
    d->_pLightSeparatorColor  = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkSeparatorColor   = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    d->_menuStyle = new NMenuPrivate::Style(d, style());

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setObjectName("NMenu");
    setStyle(d->_menuStyle);

#ifdef Q_OS_MAC
    QMenu::setNoReplayFor(this);
    QMenu::setAsDockMenu();
    menuAction()->setIconVisibleInMenu(true);
    menuAction()->setShortcutVisibleInContextMenu(true);

    for (QAction* action : actions()) {
        action->setIconVisibleInMenu(true);
        action->setShortcutVisibleInContextMenu(true);
    }
#endif
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NMenu);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

QAction* NMenu::addItem(const QString& text) { return createAction(text); }

QAction* NMenu::addItem(const QString& text, NRegularIconType::Icon icon) {
    return createAction(text, nIcon->fromRegular(icon));
}

QAction* NMenu::addItem(const QString& text, NFilledIconType::Icon icon) {
    return createAction(text, nIcon->fromFilled(icon));
}

QAction* NMenu::addItem(const QString& text, const QKeySequence& shortcut) {
    QAction* action = createAction(text);
    action->setShortcut(shortcut);
    return action;
}

QAction* NMenu::addItem(const QString& text, NRegularIconType::Icon icon, const QKeySequence& shortcut) {
    QAction* action = createAction(text, nIcon->fromRegular(icon));
    action->setShortcut(shortcut);
    return action;
}

QAction* NMenu::addItem(const QString& text, NFilledIconType::Icon icon, const QKeySequence& shortcut) {
    QAction* action = createAction(text, nIcon->fromFilled(icon));
    action->setShortcut(shortcut);
    return action;
}

NMenu* NMenu::addSubMenu(const QString& text) {
    NMenu* subMenu = new NMenu(text, this);
    QMenu::addMenu(subMenu);
    return subMenu;
}

QAction* NMenu::addCheckableItem(const QString& text, bool checked) {
    QAction* action = createAction(text);
    action->setCheckable(true);
    action->setChecked(checked);
    return action;
}

QAction* NMenu::addCheckableItem(const QString& text, NRegularIconType::Icon icon, bool checked) {
    QAction* action = createAction(text, nIcon->fromRegular(icon));
    action->setCheckable(true);
    action->setChecked(checked);
    return action;
}

QAction* NMenu::addCheckableItem(const QString& text, NFilledIconType::Icon icon, bool checked) {
    QAction* action = createAction(text, nIcon->fromFilled(icon));
    action->setCheckable(true);
    action->setChecked(checked);
    return action;
}

void NMenu::setItemEnabled(QAction* action, bool enabled) {
    if (action) {
        action->setEnabled(enabled);
    }
}

void NMenu::setItemVisible(QAction* action, bool visible) {
    if (action) {
        action->setVisible(visible);
    }
}

void NMenu::setItemToolTip(QAction* action, const QString& toolTip) {
    if (action) {
        action->setToolTip(toolTip);
    }
}

NMenu* NMenu::addSubMenu(const QString& text, NRegularIconType::Icon icon) {
    NMenu* subMenu = addSubMenu(text);
    subMenu->setIcon(nIcon->fromRegular(icon));
    return subMenu;
}

NMenu* NMenu::addSubMenu(const QString& text, NFilledIconType::Icon icon) {
    NMenu* subMenu = addSubMenu(text);
    subMenu->setIcon(nIcon->fromFilled(icon));
    return subMenu;
}

void NMenu::addSeparator() { QMenu::addSeparator(); }

QAction* NMenu::createAction(const QString& text, const QIcon& icon) {
    QAction* action = new QAction(text, this);
    if (!icon.isNull()) {
#ifdef Q_OS_MAC
        action->setIconVisibleInMenu(true);
        action->setShortcutVisibleInContextMenu(true);
#endif
        action->setIcon(icon);
    }
    addAction(action);
    return action;
}

void NMenu::paintEvent(QPaintEvent* event) { QMenu::paintEvent(event); }

void NMenu::showEvent(QShowEvent* event) {
    Q_D(NMenu);
    
    auto* effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);

    if (d->_enableDropDownAnimation) {
        QPoint startPos = pos() - QPoint(0, 20);
        move(startPos);
        
        auto* posAni = new QPropertyAnimation(this, "pos");
        posAni->setDuration(300);
        posAni->setStartValue(startPos);
        posAni->setEndValue(startPos + QPoint(0, 20));
        posAni->setEasingCurve(QEasingCurve::OutCubic);
        posAni->start(QAbstractAnimation::DeleteWhenStopped);
        
        auto* opacityAni = new QPropertyAnimation(effect, "opacity");
        opacityAni->setDuration(250);
        opacityAni->setStartValue(0.0);
        opacityAni->setEndValue(1.0);
        opacityAni->setEasingCurve(QEasingCurve::OutCubic);
        opacityAni->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        auto* ani = new QPropertyAnimation(effect, "opacity");
        ani->setDuration(150);
        ani->setStartValue(0.0);
        ani->setEndValue(1.0);
        ani->setEasingCurve(QEasingCurve::OutCubic);
        ani->start(QAbstractAnimation::DeleteWhenStopped);
    }
    
    QMenu::showEvent(event);
}

bool NMenu::event(QEvent* event) {
    // if (event->type() == QEvent::WinIdChange) {
    //     addMenuShadow();
    // }
    return QMenu::event(event);
}

void NMenu::addMenuShadow() {
#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(winId());
    if (!hwnd) {
        return;
    }

    auto policy = static_cast<DWMNCRENDERINGPOLICY>(DWMNCRP_ENABLED);
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));

    MARGINS margins = {-1, -1, -1, -1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    DWORD backdrop = DWMSBT_MAINWINDOW;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(backdrop));
#endif
}
