#ifndef FILEINFO_H
#define FILEINFO_H

#include <QMimeData>
#include <QDate>
#include <QString>

class FileInfo
{
public:
    FileInfo();
    //QMimeData *mimeData;
    QString fileName;
    QDate date;
    int id;
    bool deleted;
    QString toString() const;
    friend QDataStream &operator <<(QDataStream &out, const FileInfo &f);
    friend QDataStream &operator >>(QDataStream &in, FileInfo &f);
};
Q_DECLARE_METATYPE(FileInfo)

#endif // FILEINFO_H
