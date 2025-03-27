#include <QApplication>
#include <QtNativeUI/NConfig.h>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    nConfig->initialize(&a);

    MainWindow window;
    window.show();

    return QApplication::exec();
}