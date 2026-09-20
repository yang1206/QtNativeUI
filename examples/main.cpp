#include <QApplication>
#include <QCoreApplication>
#include <QtNativeUI/NConfig.h>
#include "QtNativeUI/NPushButton.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    NConfig::prepareQtEnvironment();
    QApplication a(argc, argv);
    nConfig->initialize();

    MainWindow window;
    QCoreApplication::processEvents();
    window.show();
    return a.exec();
}