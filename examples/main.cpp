#include <QApplication>
#include <QtNativeUI/NConfig.h>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    nConfig->initialize(&a);
    nConfig->useDefaultStyleOnPlatform(NConfig::MacOS, true);

    // a.setStyle("Fusion");

    MainWindow window;
    window.show();

    return QApplication::exec();
}