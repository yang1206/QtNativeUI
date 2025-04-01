//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NTEXTEDIT_H
#define NTEXTEDIT_H

#include <QTextEdit>
#include "stdafx.h"

class NTextEditPrivate;
class QTNATIVEUI_EXPORT NTextEdit : public QTextEdit {
    Q_OBJECT
    Q_Q_CREATE(NTextEdit)

  public:
    NTextEdit(QWidget* parent = nullptr);
    NTextEdit(const QString& text, QWidget* parent = nullptr);
    ~NTextEdit();

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NTEXTEDIT_H