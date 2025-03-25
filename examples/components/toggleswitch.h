//
// Created by Yang1206 on 2025/3/24.
//

#ifndef QTNATIVEUI_EXAMPLE_TOGGLESWITCH_H
#define QTNATIVEUI_EXAMPLE_TOGGLESWITCH_H

#include <QWidget>

class ToggleSwitchExample : public QWidget {
    Q_OBJECT
public:
    explicit ToggleSwitchExample(QWidget* parent = nullptr);
    ~ToggleSwitchExample();

private:
    void setupUI();
};

#endif // QTNATIVEUI_EXAMPLE_TOGGLESWITCH_H 