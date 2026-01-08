#include "QtNativeUI/NWindowBar.h"
#include <QEvent>
#include "../private/nwindowbar_p.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NWindowButton.h"

NWindowBarPrivate::NWindowBarPrivate()  = default;
NWindowBarPrivate::~NWindowBarPrivate() = default;

NWindowBar::NWindowBar(QWidget* parent) : QFrame(parent), d_ptr(new NWindowBarPrivate()) {
    d_ptr->q_ptr = this;
    setupUI();
}

NWindowBar::~NWindowBar() = default;

void NWindowBar::setupUI() {
    Q_D(NWindowBar);

    setFixedHeight(32);
    setObjectName(QStringLiteral("NWindowBar"));

    d->mainLayout = new QHBoxLayout(this);
    d->mainLayout->setContentsMargins(0, 0, 0, 0);
    d->mainLayout->setSpacing(0);

    d->leftLayout = new QHBoxLayout();
    d->leftLayout->setContentsMargins(8, 0, 0, 0);
    d->leftLayout->setSpacing(8);

    d->centerLayout = new QHBoxLayout();
    d->centerLayout->setContentsMargins(0, 0, 0, 0);
    d->centerLayout->setSpacing(0);

    d->rightLayout = new QHBoxLayout();
    d->rightLayout->setContentsMargins(0, 0, 0, 0);
    d->rightLayout->setSpacing(0);

#ifndef Q_OS_MAC
    d->iconLabel = new QLabel(this);
    d->iconLabel->setFixedSize(16, 16);
    d->iconLabel->setScaledContents(true);
    d->leftLayout->addWidget(d->iconLabel);
#endif

    d->titleLabel = new QLabel(this);
    d->titleLabel->setObjectName(QStringLiteral("NWindowBarTitle"));
    d->titleLabel->setAlignment(Qt::AlignCenter);

#ifdef Q_OS_MAC
    d->centerLayout->addStretch();
    d->centerLayout->addWidget(d->titleLabel);
    d->centerLayout->addStretch();
    d->leftLayout->addStretch();
#else
    d->leftLayout->addWidget(d->titleLabel);
    d->leftLayout->addStretch();
#endif

#ifndef Q_OS_MAC
    d->pinButton = new NWindowButton(NWindowButton::Pin, this);
    d->pinButton->setCheckable(true);
    d->themeButton = new NWindowButton(NWindowButton::Theme, this);
    d->themeButton->setCheckable(true);
    d->minButton   = new NWindowButton(NWindowButton::Minimize, this);
    d->maxButton   = new NWindowButton(NWindowButton::Maximize, this);
    d->closeButton = new NWindowButton(NWindowButton::Close, this);

    d->rightLayout->addWidget(d->themeButton);
    d->rightLayout->addWidget(d->pinButton);
    d->rightLayout->addWidget(d->minButton);
    d->rightLayout->addWidget(d->maxButton);
    d->rightLayout->addWidget(d->closeButton);

    connect(d->pinButton, &NWindowButton::clicked, this, [this](bool checked) { emit pinRequested(checked); });
    connect(d->themeButton, &NWindowButton::clicked, this, &NWindowBar::themeRequested);
    connect(d->minButton, &NWindowButton::clicked, this, &NWindowBar::minimizeRequested);
    connect(d->maxButton, &NWindowButton::clicked, this, &NWindowBar::maximizeRequested);
    connect(d->closeButton, &NWindowButton::clicked, this, &NWindowBar::closeRequested);
#endif

    d->mainLayout->addLayout(d->leftLayout, 0);
    d->mainLayout->addLayout(d->centerLayout, 1);
    d->mainLayout->addLayout(d->rightLayout, 0);
}

QWidget* NWindowBar::hostWidget() const {
    Q_D(const NWindowBar);
    return d->hostWidget;
}

void NWindowBar::setHostWidget(QWidget* widget) {
    Q_D(NWindowBar);

    if (d->hostWidget) {
        d->hostWidget->removeEventFilter(this);
    }

    d->hostWidget = widget;

    if (widget) {
        widget->installEventFilter(this);
        setTitle(widget->windowTitle());
        setIcon(widget->windowIcon());
    }
}

QString NWindowBar::title() const {
    Q_D(const NWindowBar);
    return d->titleLabel->text();
}

void NWindowBar::setTitle(const QString& title) {
    Q_D(NWindowBar);
    d->titleLabel->setText(title);
}

QIcon NWindowBar::icon() const {
    Q_D(const NWindowBar);
    if (!d->iconLabel) return QIcon();
    return d->iconLabel->pixmap().isNull() ? QIcon() : QIcon(d->iconLabel->pixmap());
}

void NWindowBar::setIcon(const QIcon& icon) {
    Q_D(NWindowBar);
    if (!d->iconLabel) return;
    if (icon.isNull()) {
        d->iconLabel->clear();
    } else {
        d->iconLabel->setPixmap(icon.pixmap(16, 16));
    }
}

bool NWindowBar::titleVisible() const {
    Q_D(const NWindowBar);
    return d->titleVisible;
}

void NWindowBar::setTitleVisible(bool visible) {
    Q_D(NWindowBar);
    d->titleVisible = visible;
    d->titleLabel->setVisible(visible);
}

bool NWindowBar::iconVisible() const {
    Q_D(const NWindowBar);
    return d->iconLabel ? d->iconLabel->isVisible() : false;
}

void NWindowBar::setIconVisible(bool visible) {
    Q_D(NWindowBar);
    if (d->iconLabel) {
        d->iconLabel->setVisible(visible);
    }
}

void NWindowBar::setSystemButtonVisible(NWindowButton::SystemButtonType type, bool visible) {
    Q_D(NWindowBar);
    NWindowButton* button = systemButton(type);
    if (button) {
        button->setVisible(visible);
    }
}

bool NWindowBar::systemButtonVisible(NWindowButton::SystemButtonType type) const {
    Q_D(const NWindowBar);
    NWindowButton* button = systemButton(type);
    return button ? button->isVisible() : false;
}

NWindowButton* NWindowBar::systemButton(NWindowButton::SystemButtonType type) const {
    Q_D(const NWindowBar);
    switch (type) {
        case NWindowButton::Minimize:
            return d->minButton;
        case NWindowButton::Maximize:
            return d->maxButton;
        case NWindowButton::Close:
            return d->closeButton;
        case NWindowButton::Pin:
            return d->pinButton;
        case NWindowButton::Theme:
            return d->themeButton;
        default:
            return nullptr;
    }
}

void NWindowBar::setMenuBar(QMenuBar* menuBar) {
    Q_D(NWindowBar);

    if (d->menuBar) {
        d->leftLayout->removeWidget(d->menuBar);
        d->menuBar->deleteLater();
    }

    d->menuBar = menuBar;

    if (menuBar) {
        d->leftLayout->insertWidget(2, menuBar);
    }
}

QMenuBar* NWindowBar::menuBar() const {
    Q_D(const NWindowBar);
    return d->menuBar;
}

void NWindowBar::addWidget(QWidget* widget, int stretch) {
    Q_D(NWindowBar);
    d->centerLayout->addWidget(widget, stretch);
}

void NWindowBar::insertWidget(int index, QWidget* widget, int stretch) {
    Q_D(NWindowBar);
    d->centerLayout->insertWidget(index, widget, stretch);
}

bool NWindowBar::eventFilter(QObject* obj, QEvent* event) {
    Q_D(NWindowBar);

    if (obj == d->hostWidget) {
        switch (event->type()) {
            case QEvent::WindowTitleChange:
                setTitle(d->hostWidget->windowTitle());
                break;
            case QEvent::WindowIconChange:
                setIcon(d->hostWidget->windowIcon());
                break;
            case QEvent::WindowStateChange:
                updateButtonStates();
                break;
            default:
                break;
        }
    }

    return QFrame::eventFilter(obj, event);
}

void NWindowBar::updateButtonStates() {
    Q_D(NWindowBar);

    if (d->hostWidget && d->maxButton) {
        bool maximized = d->hostWidget->isMaximized();
        d->maxButton->setMaximized(maximized);
    }
}
