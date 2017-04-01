#include "addeventdialogwrapper.h"

AddEventDialogWrapper::AddEventDialogWrapper(QObject *parent) : QObject(parent)
{
}

EventInfo AddEventDialogWrapper::getEvent(const QDate &date)
{
    return AddEventDialogWrapper().getEvent_(date);
}

EventInfo AddEventDialogWrapper::getEvent(const EventInfo &event)
{
    return AddEventDialogWrapper().getEvent_(event);
}

EventInfo AddEventDialogWrapper::getEvent_(const QDate &date)
{
    this->dlg = new AddEventDialog(date);
    this->ret = EventInfo();
    connect(this->dlg, SIGNAL(accepted()), this, SLOT(getAccepted()));
    dlg->exec();
    delete dlg;
    return this->ret;
}

EventInfo AddEventDialogWrapper::getEvent_(const EventInfo &event)
{
    this->dlg = new AddEventDialog();
    this->dlg->setEvent(event);
    this->ret = EventInfo();
    connect(this->dlg, SIGNAL(accepted()), this, SLOT(getAccepted()));
    dlg->exec();
    delete dlg;
    return this->ret;
}

void AddEventDialogWrapper::getAccepted()
{
    this->ret.date = this->dlg->dateE->date();
    this->ret.startTime = this->dlg->timeE1->time();
    this->ret.stopTime = this->dlg->timeE2->time();
    this->ret.desc = this->dlg->descE->toPlainText();
    this->ret.title = this->dlg->titleE->text();
    this->ret.repeatMode = (EventInfo::RepeatMode) this->dlg->repeatE->currentIndex();
    for(int i = 0; i < this->dlg->excludeE->count(); i++)
    {
        Q_ASSERT(this->dlg->excludeE->itemData(i).canConvert<QDate>());
        this->ret.excludeDays << this->dlg->excludeE->itemData(i).toDate();
    }
    this->ret.isNull = false;
    this->ret.deleted = this->dlg->deleted;
    this->ret.id = -1;
}
