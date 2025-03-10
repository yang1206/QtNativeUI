#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    a.setStyle("Fusion");

    MainWindow window;
    window.show();

    return QApplication::exec();
}