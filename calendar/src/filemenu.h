#ifndef FILEMENU_H
#define FILEMENU_H

#include <QMenu>
#include "calendar.h"

class FileMenu : public QMenu
{
    Q_OBJECT
public:
    FileMenu(Calendar *calendar_, int id_, QWidget *parent = Q_NULLPTR);
    Calendar *calendar;
    int id;
public slots:
    void openFile();
    void deleteFile();
};

#endif // FILEMENU_H
