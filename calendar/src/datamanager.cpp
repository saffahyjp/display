#include "datamanager.h"
#include <QDate>
#include <QDebug>
#include <QList>
#include <QDir>
#include <QUrl>

DataManager::DataManager(QObject *parent): QObject(parent)
{
}

void DataManager::addEvent(EventInfo info)
{
    Q_ASSERT(!info.isNull);
    info.id = this->eventInfos.size();
    this->eventInfos << info;
}

void DataManager::addFile(FileInfo info, const QUrl &url)
{
    info.id = this->fileInfos.size();
    QDir dir;
    while(!dir.cd("savedFiles"))
        if(!dir.mkdir("savedFiles"))
            qDebug() << "Fail to create directory savedFiles!";
    while(!dir.cd(QString::number(info.id)))
        if(!dir.mkdir(QString::number(info.id)))
            qDebug() << QString("Fail to create subdirectory ") + QString::number(info.id);
    /*QUrl url = info.mimeData->urls().first();*/
    QFile *file = new QFile(url.toLocalFile());
    QString localFile = dir.absoluteFilePath(info.fileName);
    if(!file->copy(localFile))
        qDebug() << file->errorString();
    /*QList<QUrl> ul;
    ul << QUrl::fromLocalFile(localFile);
    info.mimeData->setUrls(ul);*/
    this->fileInfos << info;
}

QString DataManager::getDateHtml(const QDate &date, int x, int y)
{
    QString ans = "";
    ans += QString("<head><style type=\"text/css\">a {text-decoration: none}</style></head>");
    ans += QString("<body bgcolor=\"%1\">").arg(this->getDateColor(date).name());
    ans += QString("<center><h1><a href=\"#day_clicked_%1_%2_%3_%4_%5\" style=\"color:#FF0000\">%3</a></h1></center>").arg(date.year()).arg(date.month()).arg(date.day()).arg(x).arg(y);
    foreach(const FileInfo &info, this->fileInfos)
        if(info.date == date && !info.deleted)
            ans += QString("<a href=\"#file_clicked_%1_%2_%3_%4_%5_%6\" style=\"color:#FF00FF\">%7</a><br>").arg(date.year()).arg(date.month()).arg(date.day()).arg(x).arg(y).arg(info.id).arg(info.toString());
    //ans += QString("</body>");
    QList<EventInfo> containers;
    foreach(const EventInfo &info, this->eventInfos)
        if(info.containsDate(date) && !info.deleted)
            containers << info;
    qSort(containers);
    foreach(const EventInfo &info, containers)
        ans += QString("<a href=\"#event_clicked_%1_%2_%3_%4_%5_%6\" style=\"color:#0000FF\" title=\"%8\">%7</a><br>").arg(date.year()).arg(date.month()).arg(date.day()).arg(x).arg(y).arg(info.id).arg(info.toString()).arg(info.desc.toHtmlEscaped());
    return ans;
}

QColor DataManager::getDateColor(const QDate &date)
{
    return this->dateInfos.value(date).color;
}

void DataManager::setDateColor(const QDate &date, const QColor &color)
{
    this->dateInfos[date].color = color;
}
