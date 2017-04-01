#include "datecellmenu.h"
#include <QAction>
#include <QApplication>
#include <QDebug>

DateCellMenu::DateCellMenu(DateCell *cell_, QWidget *parent): QMenu(parent)
{
    this->cell = cell_;
    QAction *tmpAction = this->addAction(QString("%1 %2 %3").arg(cell_->date.year()).arg(cell_->date.month()).arg(cell_->date.day()));
    tmpAction->setEnabled(false);
    this->addSeparator();
    connect(this->addAction(tr("Set Day's Color")), SIGNAL(triggered(bool)), this->cell, SLOT(setDateColor()));
    connect(this->addAction(tr("Add New Event")), SIGNAL(triggered(bool)), this->cell, SLOT(addEvent()));
    //connect(this->addAction(tr("Test")), SIGNAL(triggered(bool)), this, SLOT(test()));
}

void DateCellMenu::test()
{
    qDebug() << QApplication::allWidgets();
}
