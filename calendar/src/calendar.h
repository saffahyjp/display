#ifndef CALENDER_H
#define CALENDER_H

#include <QFrame>
#include <QUrl>
#include <QLayout>
#include <QDateTimeEdit>
#include "datamanager.h"
#include "mainwindow.h"

class Calendar: public QFrame
{
    Q_OBJECT

public:
    Calendar(MainWindow *mainWindow_, DataManager *dm, QWidget *parent = Q_NULLPTR);
    DataManager *data;
    QGridLayout *table;
    QDateTimeEdit *monthEdit;
    MainWindow *mainWindow;
public slots:
    void onAnchorClicked(const QUrl &link);
    void onAnchorHighlighted(const QUrl &link);
    void changeDateColor(const QDate &date);
    void addNewEvent(const QDate &date);
    void paintCells();
    void popupMenu(int x, int y);
    void popupFileMenu(int id);
    void editEvent(int id, const QDate &date);
};

#endif // CALENDER_H
