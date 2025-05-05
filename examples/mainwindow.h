#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNativeUI/NMainWindow.h>

class NTabWidget;

class MainWindow : public NMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    NTabWidget *m_tabWidget;
};

#endif // MAINWINDOW_H
