#include <QApplication>
#include <windows.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}