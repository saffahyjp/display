#include "mousebindingsdialog.h"
#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QStringList>
#include <QDialogButtonBox>

void MouseBindingsDialog::setDateLeft(int x)
{
    this->bind->dateActionLeft = (MouseBindings::DateAction) x;
}
void MouseBindingsDialog::setDateRight(int x)
{
    this->bind->dateActionRight = (MouseBindings::DateAction) x;
}
void MouseBindingsDialog::setFileLeft(int x)
{
    this->bind->fileActionLeft = (MouseBindings::FileAction) x;
}
void MouseBindingsDialog::setFileRight(int x)
{
    this->bind->fileActionRight = (MouseBindings::FileAction) x;
}

MouseBindingsDialog::MouseBindingsDialog(MouseBindings *bind_, QWidget *parent): QDialog(parent)
{
    this->bind = bind_;
    this->setWindowTitle(tr("Mouse Bindings"));
    QStringList dates = {tr("Popup Menu"), tr("Set Day's Color"), tr("Add New Event")};
    QStringList files = {tr("Popup Menu"), tr("Open This File"), tr("Delete This File")};
    QLayout *lo = new QVBoxLayout();
        QFrame *topF = new QFrame();
        QGridLayout *top = new QGridLayout(topF);
        top->addWidget(new QLabel(tr("Left Click")), 0, 1, Qt::AlignHCenter);
        top->addWidget(new QLabel(tr("Right Click")), 0, 2, Qt::AlignHCenter);
        top->addWidget(new QLabel(tr("On Dates")), 1, 0, Qt::AlignHCenter);
        top->addWidget(new QLabel(tr("On Files")), 2, 0, Qt::AlignHCenter);
            QComboBox *dl = new QComboBox();
            dl->addItems(dates);
            dl->setCurrentIndex(this->bind->dateActionLeft);
            connect(dl, SIGNAL(currentIndexChanged(int)), this, SLOT(setDateLeft(int)));
        top->addWidget(dl, 1, 1, Qt::AlignHCenter);
            QComboBox *dr = new QComboBox();
            dr->addItems(dates);
            dr->setCurrentIndex(this->bind->dateActionRight);
            connect(dr, SIGNAL(currentIndexChanged(int)), this, SLOT(setDateRight(int)));
        top->addWidget(dr, 1, 2, Qt::AlignHCenter);
            QComboBox *fl = new QComboBox();
            fl->addItems(files);
            fl->setCurrentIndex(this->bind->fileActionLeft);
            connect(fl, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileLeft(int)));
        top->addWidget(fl, 2, 1, Qt::AlignHCenter);
            QComboBox *fr = new QComboBox();
            fr->addItems(files);
            fr->setCurrentIndex(this->bind->fileActionRight);
            connect(fr, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileRight(int)));
        top->addWidget(fr, 2, 2, Qt::AlignHCenter);
    lo->addWidget(topF);
        QFrame *buttonF = new QFrame();
        QHBoxLayout *button = new QHBoxLayout(buttonF);
        button->addStretch();
            QDialogButtonBox *buttons = new QDialogButtonBox();
            buttons->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
            buttons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
            connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
            connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
        button->addWidget(buttons);
        button->addStretch();
    lo->addWidget(buttonF);
    this->setLayout(lo);
}
