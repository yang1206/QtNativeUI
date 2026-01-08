#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNativeUI/NMainWindow.h>

class NNavigationView;

class MainWindow : public NMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onAbout();

private:
    void setupMenuBar();
    
    NNavigationView* m_navigationView;
};

#endif // MAINWINDOW_H
