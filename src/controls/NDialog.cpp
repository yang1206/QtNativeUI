#include "QtNativeUI/NDialog.h"

#include <QPainter>
#include <QVBoxLayout>
#include <qevent.h>

#include "../private/ndialog_p.h"
#include "../private/nframelesshelper_p.h"
#include "QtNativeUI/NWindowBar.h"

NDialog::NDialog(QWidget* parent) : QDialog(parent), d_ptr(new NDialogPrivate()) {
    d_ptr->q_ptr = this;

    Q_D(NDialog);
    d->setupFrameless();
    d->setupLayout();
    d->setupDefaultWindowBar();

    connect(d->windowBar, &NWindowBar::closeRequested, this, &QDialog::reject);
}

NDialog::~NDialog() { delete d_ptr; }

void NDialog::paintEvent(QPaintEvent* event) {
    Q_D(NDialog);

    if (d->frameless->windowEffect() == NFramelessHelper::None) {
        QPainter painter(this);
        painter.fillRect(event->rect(), d->frameless->backgroundColor());
    }

    QDialog::paintEvent(event);
}

void NDialog::setWindowEffect(WindowEffectType type) {
    Q_D(NDialog);
    d->frameless->setWindowEffect(static_cast<NFramelessHelper::WindowEffectType>(type));
    if (d->frameless->windowEffect() == static_cast<NFramelessHelper::WindowEffectType>(type)) {
        emit windowEffectChanged(type);
    }
}

NDialog::WindowEffectType NDialog::windowEffect() const {
    Q_D(const NDialog);
    return static_cast<WindowEffectType>(d->frameless->windowEffect());
}

int NDialog::borderThickness() const {
    Q_D(const NDialog);
    return d->frameless->borderThickness();
}

int NDialog::titleBarHeight() const {
    Q_D(const NDialog);
    return d->frameless->titleBarHeight();
}

#ifdef Q_OS_MAC
void NDialog::setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback) {
    Q_D(NDialog);
    d->frameless->setSystemButtonAreaCallback(callback);
}

void NDialog::setNativeSystemButtonsVisible(bool visible) {
    Q_D(NDialog);
    d->frameless->setNativeSystemButtonsVisible(visible);
}
#endif

bool NDialog::setWindowAttribute(const QString& key, const QVariant& value) {
    Q_D(NDialog);
    return d->frameless->setWindowAttribute(key, value);
}

QVariant NDialog::windowAttribute(const QString& key) const {
    Q_D(const NDialog);
    return d->frameless->windowAttribute(key);
}

void NDialog::setTitleBar(QWidget* titleBar) {
    Q_D(NDialog);

    if (d->windowBar) {
        d->windowBar->hide();
        d->mainLayout->removeWidget(d->windowBar);
    }

    if (d->titleBar) {
        d->mainLayout->removeWidget(d->titleBar);
    }

    d->titleBar = titleBar;
    if (titleBar) {
        d->mainLayout->insertWidget(0, titleBar);
        d->frameless->setTitleBar(titleBar);
    }
}

QWidget* NDialog::titleBar() const {
    Q_D(const NDialog);
    return d->titleBar ? d->titleBar : d->windowBar;
}

NWindowBar* NDialog::windowBar() const {
    Q_D(const NDialog);
    return d->windowBar;
}

void NDialog::setWindowBarVisible(bool visible) {
    Q_D(NDialog);
    if (d->windowBar) {
        d->windowBar->setVisible(visible);
    }
}

bool NDialog::windowBarVisible() const {
    Q_D(const NDialog);
    return d->windowBar ? d->windowBar->isVisible() : false;
}

void NDialog::setContentWidget(QWidget* widget) {
    Q_D(NDialog);

    if (d->contentWidget) {
        d->mainLayout->removeWidget(d->contentWidget);
    }

    d->contentWidget = widget;
    if (widget) {
        d->mainLayout->addWidget(widget, 1);
    }
}

QWidget* NDialog::contentWidget() const {
    Q_D(const NDialog);
    return d->contentWidget;
}

void NDialog::setHitTestVisible(QWidget* widget, bool visible) {
    Q_D(NDialog);
    d->frameless->setHitTestVisible(widget, visible);
}

void NDialog::setSystemButton(SystemButtonType type, QAbstractButton* button) {
    Q_D(NDialog);

    NFramelessHelper::SystemButtonType helperType;
    switch (type) {
        case Minimize:
            helperType = NFramelessHelper::Minimize;
            break;
        case Maximize:
            helperType = NFramelessHelper::Maximize;
            break;
        case Close:
            helperType = NFramelessHelper::Close;
            break;
        default:
            return;
    }

    d->frameless->setSystemButton(helperType, button);
}

QWK::WidgetWindowAgent* NDialog::windowAgent() const {
    Q_D(const NDialog);
    return d->frameless->windowAgent();
}
