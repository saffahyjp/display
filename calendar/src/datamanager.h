#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QMap>
#include <QDate>
#include <QVector>
#include "dateinfo.h"
#include "eventinfo.h"
#include "fileinfo.h"

class DataManager: public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = 0);
    QString getDateHtml(const QDate &date, int x, int y);
    QColor getDateColor(const QDate &date);
    void setDateColor(const QDate &date, const QColor &color);
    void addEvent(EventInfo info);
    void addFile(FileInfo info, const QUrl &url);
    QMap<QDate, DateInfo> dateInfos;
    QVector<EventInfo> eventInfos;
    QVector<FileInfo> fileInfos;
signals:

public slots:
private:
};

#endif // DATAMANAGER_H
