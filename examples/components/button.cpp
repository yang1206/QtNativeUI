//
// Created by admin on 2025/3/10.
//

#include "button.h"
#include <QGridLayout>
#include <QtNativeUI/NPushButton.h>

ButtonExample::ButtonExample(QWidget* parent) : QWidget(parent) { initUI(); }

void ButtonExample::initUI() {
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // 添加按钮
    QPushButton* btn = new NPushButton("Click Me");

    QPushButton* btn2 = new NPushButton("Click Me");
    btn2->setDisabled(true);

    mainLayout->addWidget(btn, 0, 0);
    mainLayout->addWidget(btn2, 1, 0);
}
