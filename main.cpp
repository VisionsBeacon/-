#include "mainwindow.h"
#include "application.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    Application::instance(argc, argv);
    MainWindow w;
    w.show();
    return Application::instance()->exec();
}
