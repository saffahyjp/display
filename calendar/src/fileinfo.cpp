#include "fileinfo.h"
#include <QList>
#include <QUrl>

QDataStream &operator <<(QDataStream &out, const FileInfo &f)
{
    return out << f.fileName << f.date << QString::number(f.id) << QString::number((int) f.deleted);
}

QDataStream &operator >>(QDataStream &in, FileInfo &f)
{
    QString tmp, tmq;
    in >> f.fileName >> f.date >> tmp >> tmq;
    f.id = tmp.toInt();
    f.deleted = (bool) tmq.toInt();
    return in;
}

FileInfo::FileInfo()
{
    this->id = -1;
    this->deleted = false;
}

QString FileInfo::toString() const
{
    return this->fileName;
    /*QList<QUrl> ul = this->mimeData->urls();
    if(ul.isEmpty())
        return QString("<Invalid>");
    QString fn = ul.first().fileName();
    if(fn.isEmpty())
        return QString("<Empty>");
    return fn;*/
}
