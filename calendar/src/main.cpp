#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include "eventinfo.h"
#include "fileinfo.h"
#include "mousebindings.h"

/*class SBTranslator: public QTranslator
{
public:
    virtual QString translate(const char *, const char *sourceText, const char *, int ) const
    {
        return sourceText;
    }
};*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaTypeStreamOperators<pddi>("pddi");
    qRegisterMetaTypeStreamOperators<EventInfo>("EventInfo");
    qRegisterMetaTypeStreamOperators<FileInfo>("FileInfo");
    qRegisterMetaTypeStreamOperators<MouseBindings>("MouseBindings");

    /*SBTranslator sb;
    a.installTranslator(&sb);*/

    MainWindow w(QApplication::desktop());
    w.show();

    return a.exec();
}
