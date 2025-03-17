#include <QPainter>
#include <QtNativeUI/NCheckBox.h>
#include "../private/ncheckbox_p.h"

NCheckBox::NCheckBox(QWidget* parent) : QCheckBox(parent), d_ptr(new NCheckBoxPrivate()) { init(); }

NCheckBox::NCheckBox(const QString& text, QWidget* parent) : NCheckBox(parent) { setText(text); }

NCheckBox::~NCheckBox() {}

void NCheckBox::init() {
    Q_D(NCheckBox);
    d->q_ptr = this;
}

void NCheckBox::paintEvent(QPaintEvent* event) { QCheckBox::paintEvent(event); }