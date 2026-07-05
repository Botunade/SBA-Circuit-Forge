#include <QApplication>
#include "MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::cout << "Starting SBA Circuit Forge UI..." << std::endl;

    MainWindow window;
    window.show();

    return app.exec();
}