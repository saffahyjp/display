#ifndef ADDEVENTDIALOGWRAPPER_H
#define ADDEVENTDIALOGWRAPPER_H

#include <QObject>
#include "addeventdialog.h"
#include "eventinfo.h"

class AddEventDialogWrapper : public QObject
{
    Q_OBJECT
public:
    explicit AddEventDialogWrapper(QObject *parent = 0);
    EventInfo getEvent_(const QDate &date);
    EventInfo getEvent_(const EventInfo &event);
    static EventInfo getEvent(const QDate &date);
    static EventInfo getEvent(const EventInfo &event);
signals:
public slots:
    void getAccepted();
private:
    AddEventDialog *dlg;
    EventInfo ret;
};

#endif // ADDEVENTDIALOGWRAPPER_H
