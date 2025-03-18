//
// Created by Yang1206 on 2025/3/17.
//

#ifndef QTNATIVEUI_EXAMPLE_CHECKBOX_H
#define QTNATIVEUI_EXAMPLE_CHECKBOX_H

#include <QWidget>

class CheckBoxExample : public QWidget {
    Q_OBJECT
public:
    explicit CheckBoxExample(QWidget* parent = nullptr);
    ~CheckBoxExample();

private:
    void setupUI();
};

#endif // QTNATIVEUI_EXAMPLE_CHECKBOX_H
