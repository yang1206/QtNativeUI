//
// Created by yang1206 on 2022/3/10.
//

#ifndef BUTTON_H
#define BUTTON_H
#include <QWidget>


class ButtonExample : public QWidget {
    Q_OBJECT
public:
    explicit ButtonExample(QWidget *parent = nullptr);

private:
    void initUI();
};



#endif //BUTTON_H
