#ifndef DATECELL_H
#define DATECELL_H

#include <QObject>
#include <QTextBrowser>
#include <QDate>
#include <QPoint>
#include <QMimeData>
#include "calendar.h"

class DateCell: public QTextBrowser
{
    Q_OBJECT
public:
    DateCell(int x_, int y_, Calendar *calendar_, QWidget *parent = Q_NULLPTR);
    QDate date;
    int x, y;
    Calendar *calendar;
    static QMimeData *cloneMimeData(const QMimeData *data);
protected:
    virtual void contextMenuEvent(QContextMenuEvent *ev = Q_NULLPTR);
    virtual void dragEnterEvent(QDragEnterEvent *e);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dropEvent(QDropEvent *e);
    virtual void insertFromMimeData(const QMimeData *source);
    virtual bool canInsertFromMimeData(const QMimeData *source) const;
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
private:
    bool dragging;
    bool realDragging;
    QPoint dragStartPoint;
    QUrl currentUrl;
public slots:
    void setDateColor();
    void addEvent();
    void setCurrentUrl(const QUrl &url);
};

#endif // DATECELL_H
