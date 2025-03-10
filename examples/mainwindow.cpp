#include "mainwindow.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    auto btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(20, 20, 20, 20);
    btnLayout->setSpacing(15);
    mainLayout->addLayout(btnLayout);


    // 添加按钮
    QPushButton* btn = new QtNativeUI::NPushButton("Click Me");

    QPushButton* btn2 = new QtNativeUI::NPushButton("Click Me");
    btn2->setEnabled(false);

    btnLayout->addWidget(btn);
    btnLayout->addWidget(btn2);



    // 设置窗口属性
    setWindowTitle("QtNativeUI by Yang1206");
    resize(900, 700);
}