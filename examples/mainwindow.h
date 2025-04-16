#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class NTabWidget;
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);

  private:
    NTabWidget* m_tabWidget;
};

#endif // MAINWINDOW_H