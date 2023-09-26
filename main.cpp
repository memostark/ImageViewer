
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("Image Viewer");
    QGuiApplication::setOrganizationName("NA");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
