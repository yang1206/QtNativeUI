//
// Created by Yang1206 on 2025/4/1.
//

#include <QtNativeUI/NTextEdit.h>
#include "../private/ntextedit_p.h"

NTextEdit::NTextEdit(QWidget* parent) : QTextEdit(parent), d_ptr(new NTextEditPrivate()) {
    Q_D(NTextEdit);
    d->q_ptr = this;
}

NTextEdit::NTextEdit(const QString& text, QWidget* parent) : NTextEdit(parent) {
    setText(text);
}

NTextEdit::~NTextEdit() {}

void NTextEdit::paintEvent(QPaintEvent* event) {
    QTextEdit::paintEvent(event);
}