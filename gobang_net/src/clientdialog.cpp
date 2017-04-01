#include "clientdialog.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>

ClientDialog::ClientDialog(QString *str, QWidget *parent): QDialog(parent)
{
    this->str = str;
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Please input host address:"));
    
    this->le = new QLineEdit();
    this->le->setText(*this->str);
    connect(this->le, SIGNAL(textChanged(QString)), this, SLOT(setStr(QString)));
    layout->addWidget(this->le);
    
    this->table = new QGridLayout();
    this->sm = new QSignalMapper(this);
    for(int i = 1; i <= 9; i++)
        this->addButton(QString::number(i), (i - 1) / 3, (i - 1) % 3);
    this->addButton(".", 3, 0);
    this->addButton("0", 3, 1);
    this->addButton("OK", 3, 2);
    
    connect(this->sm, SIGNAL(mapped(QString)), this, SLOT(onButtonClicked(QString)));
    
    layout->addLayout(this->table);
    this->setLayout(layout);
}

void ClientDialog::addButton(const QString &text, int x, int y)
{
    QPushButton *pb = new QPushButton(text);
    if(text == "OK")
        pb->setDefault(true);
    this->table->addWidget(pb, x, y);
    connect(pb, SIGNAL(clicked()), this->sm, SLOT(map()));
    this->sm->setMapping(pb, text);
}

void ClientDialog::setStr(const QString &str)
{
    *this->str = str;
}

QString ClientDialog::getText(QWidget *parent)
{
    QString ans = "127.0.0.1";
    ClientDialog cd(&ans, parent);
    if(cd.exec() == QDialog::Accepted)
        return ans;
    else
        return QString();
}

void ClientDialog::onButtonClicked(const QString &text)
{
    if(text == "OK")
        this->accept();
    else
        this->le->setText(this->le->text() + text);
}
