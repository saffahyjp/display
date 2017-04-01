#include "serverdialog.h"
#include <QLayout>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QTextStream>
#include <QPushButton>

ServerDialog::ServerDialog(QWidget *parent): QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QString text = "Your IP Addresses are:";
    foreach(const QHostAddress &addr, QNetworkInterface::allAddresses())
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            text += QString("\n") + addr.toString();
    layout->addWidget(new QLabel(text));
    this->ipLabel = new QLabel();
    layout->addWidget(this->ipLabel);
    QPushButton *button = new QPushButton("Cancel");
    layout->addWidget(button);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(reject()));
    this->setLayout(layout);
    
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    this->reply = manager->get(QNetworkRequest(QUrl("http://www.3322.org/dyndns/getip")));
    connect(this->reply, SIGNAL(readyRead()), this, SLOT(gotIP()));
}

void ServerDialog::gotIP()
{
    QTextStream stream(this->reply);
    this->ipLabel->setText(QString("Your public IP is:\n") + stream.readAll());
}
