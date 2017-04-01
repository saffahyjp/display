#include "mainwindow.h"
#include <QApplication>
#include "data.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontUseNativeMenuBar, true);
    
    qRegisterMetaType<Data>("Data");
    qRegisterMetaTypeStreamOperators<Data>("Data");
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
