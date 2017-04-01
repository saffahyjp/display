#include "filemenu.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QDir>

FileMenu::FileMenu(Calendar *calendar_, int id_, QWidget *parent): QMenu(parent)
{
    this->calendar = calendar_;
    this->id = id_;
    QAction *tmpAction = this->addAction(this->calendar->data->fileInfos[id].toString());
    tmpAction->setEnabled(false);
    this->addSeparator();
    connect(this->addAction(tr("Open This File")), SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(this->addAction(tr("Delete This File")), SIGNAL(triggered(bool)), this, SLOT(deleteFile()));
}

void FileMenu::openFile()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir(QString("savedFiles/%1").arg(this->id)).absoluteFilePath(this->calendar->data->fileInfos[this->id].fileName)));
}

void FileMenu::deleteFile()
{
    QMessageBox::StandardButton result = QMessageBox::question(this, this->windowTitle(), tr("Are you sure to delete this file?"));
    if(result == QMessageBox::Yes)
    {
        this->calendar->data->fileInfos[id].deleted = true;
        this->calendar->paintCells();
    }
}
