#include "eventinfo.h"

QDataStream &operator <<(QDataStream &out, const EventInfo &e)
{
    out << QString::number(e.id) << QString::number((int) e.repeatMode) << e.date << e.startTime << e.stopTime << e.title << e.desc << e.excludeDays.size();
    foreach(QDate date, e.excludeDays)
        out << date;
    return out << e.isNull << e.deleted;
}

QDataStream &operator >>(QDataStream &in, EventInfo &e)
{
    QString tmr, tmp, tmq;
    in >> tmr >> tmp >> e.date >> e.startTime >> e.stopTime >> e.title >> e.desc >> tmq;
    e.repeatMode = (EventInfo::RepeatMode) tmp.toInt();
    e.id = tmr.toInt();
    int tms = tmq.toInt();
    while(tms--)
    {
        QDate date; in >> date;
        e.excludeDays << date;
    }
    return in;
}

EventInfo::EventInfo()
{
    this->isNull = true;
}

bool EventInfo::containsDate(const QDate &date) const
{
    if(this->excludeDays.count(date))
        return false;
    if(this->repeatMode == EventInfo::NoRepeat)
        return date == this->date;
    if(this->repeatMode == EventInfo::Daily)
        return true;
    if(this->repeatMode == EventInfo::Weekly)
        return date.dayOfWeek() == this->date.dayOfWeek();
    if(this->repeatMode == EventInfo::Monthly)
        return date.day() == this->date.day();
    if(this->repeatMode == EventInfo::Yearly)
        return date.day() == this->date.day() && date.month() == this->date.month();
    Q_ASSERT(false);
    return false;
}

QString EventInfo::toString() const
{
    return QString("%1-%2 %3").arg(this->startTime.toString("hh:mm")).arg(this->stopTime.toString("hh:mm")).arg(this->title);
}

bool EventInfo::operator <(const EventInfo &rhs) const
{
    if(this->date != rhs.date)
        return this->date < rhs.date;
    if(this->startTime != rhs.startTime)
        return this->startTime < rhs.startTime;
    if(this->stopTime != rhs.stopTime)
        return this->stopTime < rhs.stopTime;
    if(this->title != rhs.title)
        return this->title < rhs.title;
    if(this->desc != rhs.desc)
        return this->desc < rhs.desc;
    return this->id < rhs.id;
}
