#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    //Temporary tests etc. are best done in 'mainwindow.cpp'
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
