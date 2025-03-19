#include "QtNativeUI/NMenu.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>

#include "../private/nmenu_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

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

void NMenu::init() {
    Q_D(NMenu);
    d->q_ptr = this;

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    // 初始化样式属性
    d->_pBorderRadius         = NDesignToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightBorderColor     = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor      = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pLightSeparatorColor  = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkSeparatorColor   = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    // 设置自定义样式
    d->_menuStyle = new NMenuPrivate::Style(d, style());
    setStyle(d->_menuStyle);
#ifdef Q_OS_MAC
    QMenu::setNoReplayFor(this);
    QMenu::setAsDockMenu();
    menuAction()->setIconVisibleInMenu(true);
    menuAction()->setShortcutVisibleInContextMenu(true);

    // 对所有菜单项启用图标
    for (QAction* action : actions()) {
        action->setIconVisibleInMenu(true);
        action->setShortcutVisibleInContextMenu(true);
    }
#endif
    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NMenu);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    // 设置属性
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
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

bool NMenu::event(QEvent* event) {
    // 处理特定事件
    return QMenu::event(event);
}

void NMenu::showEvent(QShowEvent* event) {
    // 菜单显示时的处理
    QMenu::showEvent(event);
}

void NMenu::hideEvent(QHideEvent* event) {
    // 菜单隐藏时的处理
    QMenu::hideEvent(event);
}
