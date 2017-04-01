#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <QDate>
#include <QTime>
#include <QString>
#include <QList>
#include <QMetaType>

class EventInfo
{
public:
    EventInfo();
    enum RepeatMode{
        NoRepeat, Daily, Weekly, Monthly, Yearly
    } repeatMode;
    int id;
    QDate date;
    QTime startTime, stopTime;
    QString title, desc;
    QList<QDate> excludeDays;
    bool isNull;
    bool deleted;
    bool containsDate(const QDate &date) const;
    bool operator <(const EventInfo &rhs) const;
    QString toString() const;
    friend QDataStream &operator <<(QDataStream &out, const EventInfo &e);
    friend QDataStream &operator >>(QDataStream &in, EventInfo &e);
};

Q_DECLARE_METATYPE(EventInfo)

#endif // EVENTINFO_H
