#ifndef DATECELLMENU_H
#define DATECELLMENU_H

#include <QMenu>
#include <QDate>
#include "datecell.h"

class DateCellMenu : public QMenu
{
    Q_OBJECT
public:
    DateCellMenu(DateCell *cell_, QWidget *parent = Q_NULLPTR);
    DateCell *cell;
public slots:
    void test();
};

#endif // DATECELLMENU_H
