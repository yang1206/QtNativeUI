#include "QtNativeUI/NMenu.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

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
    d->_pLightBackgroundColor = QColor(0xFA, 0xFA, 0xFA);
    d->_pDarkBackgroundColor  = QColor(0x2C, 0x2C, 0x2C);
    d->_pLightBorderColor     = NThemeColor(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);
    d->_pDarkBorderColor      = NThemeColor(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark);
    d->_pLightSeparatorColor  = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkSeparatorColor   = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    // 设置自定义样式
    d->_menuStyle = new NMenuPrivate::Style(d, style());

    // 设置属性
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

    setObjectName("NMenu");

    setStyle(d->_menuStyle);

    d->_pAnimationImagePosY = 0;

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

void NMenu::paintEvent(QPaintEvent* event) {
    Q_D(NMenu);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    if (!d->_animationPix.isNull()) {
        painter.drawPixmap(QRect(0, d->_pAnimationImagePosY, width(), height()), d->_animationPix);
    } else {
        QMenu::paintEvent(event);
    }
}

void NMenu::showEvent(QShowEvent* event) {
    Q_D(NMenu);
    // 消除阴影偏移
    move(this->pos().x() - 6, this->pos().y());
    if (!d->_animationPix.isNull()) {
        d->_animationPix = QPixmap();
    }
    d->_animationPix                 = this->grab(this->rect());
    QPropertyAnimation* posAnimation = new QPropertyAnimation(d, "pAnimationImagePosY");
    connect(posAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_animationPix = QPixmap();
        update();
    });
    connect(posAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
    posAnimation->setEasingCurve(NDesignToken(NDesignTokenKey::EasingStandard).value<QEasingCurve>());
    posAnimation->setDuration(NDesignToken(NDesignTokenKey::AnimationSlow).toInt());

    int targetPosY = height();
    if (targetPosY > 160) {
        if (targetPosY < 320) {
            targetPosY = 160;
        } else {
            targetPosY /= 2;
        }
    }

    if (pos().y() + 28 + 9 >= QCursor::pos().y()) {
        posAnimation->setStartValue(-targetPosY);
    } else {
        posAnimation->setStartValue(targetPosY);
    }

    posAnimation->setEndValue(0);
    posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QMenu::showEvent(event);
}
