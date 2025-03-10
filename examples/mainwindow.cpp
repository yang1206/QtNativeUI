#include "mainwindow.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "components/button.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    auto btn = new ButtonExample(this);
    m_tabWidget->addTab(btn, "Button");

    // 设置窗口属性
    setWindowTitle("QtNativeUI by Yang1206");
    resize(900, 700);
}