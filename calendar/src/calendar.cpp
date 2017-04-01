#include "calendar.h"
#include <QLayout>
#include <QSpinBox>
#include <QDate>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QLocale>
#include <QTextBrowser>
#include <QDateTimeEdit>
#include <QtGlobal>
#include <QStringList>
#include <QColorDialog>
#include <QCalendarWidget>
#include "datecell.h"
#include "datecellmenu.h"
#include "filemenu.h"
#include "addeventdialogwrapper.h"

void Calendar::paintCells()
{
    QDate date = this->monthEdit->date();
    //qDebug() << date;
    while(date.day() > 1)
        date = date.addDays(-1);
    while(date.dayOfWeek() > 1)
        date = date.addDays(-1);
    for(int i = 1; i <= 5; i++)
    {
        QWidget *label = this->table->itemAtPosition(i, 0)->widget();
        Q_ASSERT(label != Q_NULLPTR);
        Q_ASSERT(label->inherits("QLabel"));
        static_cast<QLabel *>(label)->setText(QString::number(date.weekNumber()));
        for(int j = 1; j <= 7; j++)
        {
            QWidget *tb = this->table->itemAtPosition(i, j)->widget();
            Q_ASSERT(tb != Q_NULLPTR);
            Q_ASSERT(tb->inherits("DateCell"));
            static_cast<DateCell *>(tb)->setHtml(this->data->getDateHtml(date, i, j));
            static_cast<DateCell *>(tb)->date = date;
            date = date.addDays(1);
        }
    }
}

Calendar::Calendar(MainWindow *mainWindow_, DataManager *dm, QWidget *parent): QFrame(parent)
{
    this->mainWindow = mainWindow_;
    this->data = dm;
    QVBoxLayout *lo = new QVBoxLayout(this);
        QWidget *topBarW = new QWidget();
        QHBoxLayout *topBar = new QHBoxLayout(topBarW);
                /*
                QSpinBox *year = new QSpinBox();
                    year->setRange(1, 9999);
                    year->setValue(QDate::currentDate().year());
                    year->setPrefix(tr("Year "));
                    year->setSuffix(tr(""));
                QSpinBox *mon = new QSpinBox();
                    mon->setRange(1, 12);
                    mon->setValue(QDate::currentDate().month());
                    mon->setPrefix(tr("Month "));
                    mon->setSuffix(tr(""));
                QLabel *slash = new QLabel(" / ");
                */
                this->monthEdit = new QDateTimeEdit(QDate::currentDate());
                this->monthEdit->setDisplayFormat("yyyy MM");
                //this->monthEdit->setCalendarPopup(true);
            topBar->addStretch();
            topBar->addWidget(monthEdit);
            /*topBar->addWidget(year);
            topBar->addWidget(slash);
            topBar->addWidget(mon);*/
            topBar->addStretch();
        QWidget *tableW = new QWidget();
        this->table = new QGridLayout(tableW);
        this->table->setVerticalSpacing(0);
        this->table->setHorizontalSpacing(0);
        for(int i = 1; i <= 7; i++)
        {
            QFrame *tmpFrame = new QFrame();
            QHBoxLayout *tmpLayout = new QHBoxLayout(tmpFrame);
            tmpLayout->addStretch();
            tmpLayout->addWidget(new QLabel(QLocale().dayName(i)));
            tmpLayout->addStretch();
            this->table->addWidget(tmpFrame, 0, i);
        }
        for(int i = 1; i <= 5; i++)
            this->table->addWidget(new QLabel(""), i, 0);
        for(int i = 1; i <= 5; i++)
            for(int j = 1; j <= 7; j++)
            {
                DateCell *tb = new DateCell(i, j, this);
                this->table->addWidget(tb, i, j);
                tb->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                tb->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                tb->setFrameStyle(QFrame::NoFrame);
                tb->setAcceptDrops(true);
                connect(tb, SIGNAL(anchorClicked(QUrl)), this, SLOT(onAnchorClicked(QUrl)));
            }
        this->paintCells();
        connect(this->monthEdit, SIGNAL(dateChanged(QDate)), this, SLOT(paintCells()));
    lo->addWidget(topBarW);
    lo->addWidget(tableW);
    //lo->addWidget(new QCalendarWidget());
    this->setLayout(lo);
}

void Calendar::changeDateColor(const QDate &date)
{
    QColor color = QColorDialog::getColor(data->getDateColor(date), this, "", QColorDialog::DontUseNativeDialog);
    if(color.isValid())
        data->setDateColor(date, color);
    this->paintCells();
}

void Calendar::addNewEvent(const QDate &date)
{
    EventInfo info = AddEventDialogWrapper::getEvent(date);
    if(!info.isNull)
        data->addEvent(info);
    this->paintCells();
}

void Calendar::popupFileMenu(int id)
{
    FileMenu *menu = new FileMenu(this, id);
    menu->exec(QCursor::pos());
    delete menu;
}

void Calendar::popupMenu(int x, int y)
{
    QWidget *widget = this->table->itemAtPosition(x, y)->widget();
    Q_ASSERT(widget != Q_NULLPTR);
    Q_ASSERT(widget->inherits("DateCell"));
    DateCellMenu *menu = new DateCellMenu(static_cast<DateCell *>(widget));
    menu->exec(QCursor::pos());
    delete menu;
}

void Calendar::editEvent(int id, const QDate &date)
{
    Q_ASSERT(id >= 0);
    Q_ASSERT(id < this->data->eventInfos.size());
    this->data->eventInfos[id].date = date;
    EventInfo newEvent = AddEventDialogWrapper::getEvent(this->data->eventInfos[id]);
    if(!newEvent.isNull)
    {
        newEvent.id = id;
        this->data->eventInfos[id] = newEvent;
    }
    this->paintCells();
}

void Calendar::onAnchorHighlighted(const QUrl &link)
{
    qDebug() << "high" << link;
}

void Calendar::onAnchorClicked(const QUrl &link)
{
    qDebug() << link;
    QString url = link.url();
    if(url.contains("day_clicked"))
    {
        QStringList spl = url.split('_');
        MouseBindings::DateAction action = url.contains("right") ? this->mainWindow->bind.dateActionRight : this->mainWindow->bind.dateActionLeft;
        if(action == MouseBindings::PopupDateMenu)
            this->popupMenu(spl[5].toInt(), spl[6].toInt());
        else if(action == MouseBindings::SetDateColor)
            this->changeDateColor(QDate(spl[2].toInt(), spl[3].toInt(), spl[4].toInt()));
        else if(action == MouseBindings::AddDateEvent)
            this->addNewEvent(QDate(spl[2].toInt(), spl[3].toInt(), spl[4].toInt()));
    }
    else if(url.contains("set_date_color"))
    {
        QStringList spl = url.split('_');
        QDate date = QDate(spl[3].toInt(), spl[4].toInt(), spl[5].toInt());
        this->changeDateColor(date);
    }
    else if(url.contains("add_event"))
    {
        QStringList spl = url.split('_');
        QDate date = QDate(spl[2].toInt(), spl[3].toInt(), spl[4].toInt());
        this->addNewEvent(date);
    }
    else if(url.contains("event_clicked"))
    {
        QStringList spl = url.split('_');
        QDate date = QDate(spl[2].toInt(), spl[3].toInt(), spl[4].toInt());
        if(url.contains("right"))
            this->editEvent(spl[7].toInt(), date);
        else
            this->editEvent(spl[7].toInt(), date);
    }
    else if(url.contains("file_clicked"))
    {
        QStringList spl = url.split('_');
        MouseBindings::FileAction action = url.contains("right") ? this->mainWindow->bind.fileActionRight : this->mainWindow->bind.fileActionLeft;
        if(action == MouseBindings::PopupFileMenu)
            this->popupFileMenu(spl[7].toInt());
        else if(action == MouseBindings::OpenFile)
            FileMenu(this, spl[7].toInt()).openFile();
        else if(action == MouseBindings::DeleteFile)
            FileMenu(this, spl[7].toInt()).deleteFile();
    }
    else
        qDebug() << link;
}
