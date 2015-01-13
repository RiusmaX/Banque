#include <QtGui/QApplication>
#include "dab_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DAB_MainWindow w;
    w.show();
    
    return a.exec();
}
