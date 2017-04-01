#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include "eventinfo.h"

class AddEventDialog: public QDialog
{
    Q_OBJECT
public:
    explicit AddEventDialog(QDate date = QDate::currentDate(), QWidget *parent = Q_NULLPTR);
    void setEvent(const EventInfo &event);
    QLineEdit *titleE;
    QDateEdit *dateE;
    QDateTimeEdit *timeE1, *timeE2;
    QTextEdit *descE;
    QComboBox *repeatE;
    QComboBox *excludeE;
    bool deleted;
public slots:
    void okClicked();
    void deleteClicked();
    void addExcludeClicked();
    void removeExcludeClicked();
};

#endif // ADDEVENTDIALOG_H
