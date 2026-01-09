#include <QApplication>
#include <QtNativeUI/NConfig.h>
#include "QtNativeUI/NPushButton.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    nConfig->initialize();

    MainWindow window;
    window.show();
    return a.exec();
}