#include "addeventdialog.h"
#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QEvent>

void AddEventDialog::setEvent(const EventInfo &event)
{
    this->setWindowTitle(tr("Modify Event"));
    this->titleE->setText(event.title);
    this->dateE->setDate(event.date);
    this->timeE1->setTime(event.startTime);
    this->timeE2->setTime(event.stopTime);
    this->descE->setPlainText(event.desc);
    this->repeatE->setCurrentIndex(event.repeatMode);
    foreach(const QDate &date, event.excludeDays)
        this->excludeE->addItem(date.toString(), QVariant(date));
}

AddEventDialog::AddEventDialog(QDate date, QWidget *parent): QDialog(parent)
{
    this->setWindowTitle(tr("New Event"));
    QVBoxLayout *layout = new QVBoxLayout();
        QFrame *titleF = new QFrame();
        QHBoxLayout *title = new QHBoxLayout(titleF);
        title->addWidget(new QLabel(tr("Event name:")));
            this->titleE = new QLineEdit();
        title->addWidget(this->titleE);
    layout->addWidget(titleF);
        QFrame *dateF = new QFrame();
        QHBoxLayout *dateLO = new QHBoxLayout(dateF);
        dateLO->addWidget(new QLabel(tr("Date:")));
            this->dateE = new QDateEdit(date);
            this->dateE->setCalendarPopup(true);
        dateLO->addWidget(this->dateE);
    layout->addWidget(dateF);
        QFrame *timeF = new QFrame();
        QHBoxLayout *time = new QHBoxLayout(timeF);
        time->addWidget(new QLabel(tr("Time:")));
            this->timeE1 = new QDateTimeEdit();
            this->timeE1->setDisplayFormat("hh:mm");
        time->addWidget(this->timeE1);
        time->addWidget(new QLabel(tr("to")));
            this->timeE2 = new QDateTimeEdit();
            this->timeE2->setDisplayFormat("hh:mm");
        time->addWidget(this->timeE2);
    layout->addWidget(timeF);
        QFrame *repeatF = new QFrame();
        QHBoxLayout *repeat = new QHBoxLayout(repeatF);
        repeat->addWidget(new QLabel(tr("Repeat:")));
            this->repeatE = new QComboBox();
            this->repeatE->addItem(tr("Don't Repeat"));
            this->repeatE->addItem(tr("Daily"));
            this->repeatE->addItem(tr("Weekly"));
            this->repeatE->addItem(tr("Monthly"));
            this->repeatE->addItem(tr("Yearly"));
        repeat->addWidget(this->repeatE);
    layout->addWidget(repeatF);
        QFrame *excludeF = new QFrame();
        QHBoxLayout *exclude = new QHBoxLayout(excludeF);
        exclude->addWidget(new QLabel(tr("Days to exclude:")));
            this->excludeE = new QComboBox();
        exclude->addWidget(this->excludeE);
        QFrame *excludeF2 = new QFrame();
        QHBoxLayout *exclude2 = new QHBoxLayout(excludeF2);
            QPushButton *addExclude = new QPushButton(tr("Exclude Current Day"));
            connect(addExclude, SIGNAL(clicked(bool)), this, SLOT(addExcludeClicked()));
        exclude2->addWidget(addExclude);
            QPushButton *removeExclude = new QPushButton(tr("Re-include Selected Day"));
            connect(removeExclude, SIGNAL(clicked(bool)), this, SLOT(removeExcludeClicked()));
        exclude2->addWidget(removeExclude);
    layout->addWidget(excludeF);
    layout->addWidget(excludeF2);
    layout->addWidget(new QLabel(tr("Description:")));
        this->descE = new QTextEdit();
    layout->addWidget(this->descE);
        QFrame *buttonF = new QFrame();
        QHBoxLayout *button = new QHBoxLayout(buttonF);
        button->addStretch();
            QDialogButtonBox *buttons = new QDialogButtonBox();
            buttons->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
            buttons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
            buttons->addButton(tr("Delete Event"), QDialogButtonBox::HelpRole);
            connect(buttons, SIGNAL(accepted()), this, SLOT(okClicked()));
            connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
            connect(buttons, SIGNAL(helpRequested()), this, SLOT(deleteClicked()));
        button->addWidget(buttons);
        button->addStretch();
    layout->addWidget(buttonF);
    this->setLayout(layout);
}

void AddEventDialog::okClicked()
{
    if(titleE->text().isEmpty())
    {
        QMessageBox::critical(this, this->windowTitle(), tr("The event name cannot be empty!"));
        return;
    }
    if(!dateE->date().isValid())
    {
        QMessageBox::critical(this, this->windowTitle(), tr("The date is invalid!"));
        return;
    }
    if(!timeE1->time().isValid())
    {
        QMessageBox::critical(this, this->windowTitle(), tr("The start time is invalid!"));
        return;
    }
    if(!timeE2->time().isValid())
    {
        QMessageBox::critical(this, this->windowTitle(), tr("The end time is invalid!"));
        return;
    }
    if(timeE1->time() > timeE2->time())
    {
        QMessageBox::critical(this, this->windowTitle(), tr("The start time is earlier than the end time!"));
        return;
    }
    this->deleted = false;
    this->accept();
}

void AddEventDialog::addExcludeClicked()
{
    QDate toAdd = this->dateE->date();
    int pos = this->excludeE->findData(QVariant(toAdd));
    //qDebug() << toAdd << pos;
    if(pos != -1)
        this->excludeE->setCurrentIndex(pos);
    else
    {
        this->excludeE->addItem(toAdd.toString(), QVariant(toAdd));
        this->excludeE->setCurrentIndex(this->excludeE->count() - 1);
    }
}

void AddEventDialog::removeExcludeClicked()
{
    int pos = this->excludeE->currentIndex();
    if(pos != -1)
        this->excludeE->removeItem(pos);
}

void AddEventDialog::deleteClicked()
{
    QMessageBox::StandardButton result = QMessageBox::question(this, this->windowTitle(), tr("Are you sure to delete this event?"));
    if(result == QMessageBox::Yes)
    {
        this->deleted = true;
        this->accept();
    }
}
