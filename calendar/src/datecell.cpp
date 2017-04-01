#include "datecell.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QApplication>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QDrag>
#include <QDir>
#include "fileinfo.h"

DateCell::DateCell(int x_, int y_, Calendar *calendar_, QWidget *parent): QTextBrowser(parent)
{
    this->x = x_; this->y = y_;
    this->calendar = calendar_;
    this->dragging = false;
    this->realDragging = false;
    connect(this, SIGNAL(highlighted(QUrl)), this, SLOT(setCurrentUrl(QUrl)));
}

void DateCell::setCurrentUrl(const QUrl &url)
{
    this->currentUrl = url;
}

void DateCell::mouseReleaseEvent(QMouseEvent *ev){
    if(this->dragging && !this->realDragging)
    {
        emit anchorClicked(this->currentUrl);
        this->setCurrentUrl(QUrl());
        this->dragging = false;
        return;
    }
    this->setCurrentUrl(QUrl());
    this->dragging = false;
    QTextBrowser::mouseReleaseEvent(ev);
}

void DateCell::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton && this->currentUrl.url().contains("file_clicked"))
    {
        this->dragStartPoint = ev->pos();
        this->dragging = true;
        return;
    }
    this->dragging = false;
    if(ev->button() == Qt::RightButton)
        this->contextMenuEvent();
    else
        QTextBrowser::mousePressEvent(ev);
}

void DateCell::mouseMoveEvent(QMouseEvent *ev)
{
    if(!(ev->buttons() & Qt::LeftButton))
    {
        QTextBrowser::mouseMoveEvent(ev);
        return;
    }
    if(!this->dragging)
    {
        QTextBrowser::mouseMoveEvent(ev);
        return;
    }
    if((ev->pos() - this->dragStartPoint).manhattanLength() < QApplication::startDragDistance())
        return;
    if(!this->currentUrl.url().contains("file_clicked"))
        return;
    if(!this->calendar->mainWindow->dragEnabled)
        return;
    this->realDragging = true;
    QDrag *drag = new QDrag(this);
    int id = this->currentUrl.url().split('_')[7].toInt();
    FileInfo &info = this->calendar->data->fileInfos[id];
    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls((QList<QUrl>) {QUrl::fromLocalFile(QDir(QString("savedFiles/%1").arg(id)).absoluteFilePath(info.fileName))});
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
    this->dragging = false;
    this->realDragging = false;
}

void DateCell::contextMenuEvent(QContextMenuEvent *)
{
    emit anchorClicked(QUrl(this->currentUrl.url() + "_right"));
    this->setCurrentUrl(QUrl());
}

void DateCell::setDateColor()
{
    emit anchorClicked(QUrl(QString("#set_date_color_%1_%2_%3_%4_%5").arg(this->date.year()).arg(this->date.month()).arg(this->date.day()).arg(this->x).arg(this->y)));
}

void DateCell::addEvent()
{
    emit anchorClicked(QUrl(QString("#add_event_%1_%2_%3_%4_%5").arg(this->date.year()).arg(this->date.month()).arg(this->date.day()).arg(this->x).arg(this->y)));
}

void DateCell::dragEnterEvent(QDragEnterEvent *e)
{
    if(!this->calendar->mainWindow->dragEnabled)
        e->ignore();
    else if(this->canInsertFromMimeData(e->mimeData()))
        e->accept();
    else
        e->ignore();
}

void DateCell::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
}

void DateCell::dropEvent(QDropEvent *e)
{
    if(this->calendar->mainWindow->dragEnabled && this->canInsertFromMimeData(e->mimeData()))
    {
        this->insertFromMimeData(e->mimeData());
        /*if(e->proposedAction() == Qt::CopyAction)
            e->acceptProposedAction();
        else if(e->proposedAction() == Qt::MoveAction)
            e->acceptProposedAction();
        else
        {*/
        qDebug() << e->possibleActions();
        e->setDropAction(Qt::CopyAction);
        e->accept();
        //}
    }
    else
        e->ignore();
    /*foreach(QString type, e->mimeData()->formats())
    {
        qDebug() << type << e->mimeData()->data(type);
    }
    qDebug() << e->mimeData()->text();
    qDebug() << e->mimeData()->urls();*/
}

QMimeData *DateCell::cloneMimeData(const QMimeData *data)
{
    QMimeData *ret = new QMimeData();
    foreach(QString type, data->formats())
        ret->setData(type, data->data(type));
    return ret;
}

void DateCell::insertFromMimeData(const QMimeData *source)
{
    qDebug() << source;
    FileInfo info;
    info.date = this->date;
    QUrl url = source->urls().first();
    info.fileName = url.fileName();
    this->calendar->data->addFile(info, url);
    this->calendar->paintCells();
    /*qDebug() << this->textCursor().position();
    this->moveCursor(QTextCursor::End);
    if(!source->urls().empty())
        this->insertHtml(source->urls().first().toString() + "<br>");*/
    //return QTextBrowser::insertFromMimeData(source);
}

bool DateCell::canInsertFromMimeData(const QMimeData *source) const
{
    QList<QUrl> ul = source->urls();
    if(ul.size() != 1)
        return false;
    if(!ul.first().isLocalFile())
        return false;
    QString localFile = ul.first().toLocalFile();
    QFile file(localFile);
    return file.open(QIODevice::ReadOnly);
}
