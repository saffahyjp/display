#include "ssgetter.h"
#include "serverdialog.h"
#include "clientdialog.h"
#include <QInputDialog>

SSGetter::SSGetter(QMainWindow *mainWindow, QObject *parent): QObject(parent)
{
    this->mainWindow = mainWindow;
    this->server = new QTcpServer(this);
    this->server->listen(QHostAddress::Any, 2366);
    this->server->pauseAccepting();
    connect(this->server, SIGNAL(newConnection()), this, SLOT(onServerConnected()));
    
    this->client = new QTcpSocket(this);
    connect(this->client, SIGNAL(connected()), this, SLOT(onClientConnected()));
}

void SSGetter::onServerConnected()
{
    this->server->pauseAccepting();
    Q_ASSERT(this->server->hasPendingConnections());
    emit gotStream(new SocketStream(this->server->nextPendingConnection(), true));
}

void SSGetter::setServerMode()
{
    //this->client->close();
    this->server->resumeAccepting();
    ServerDialog *dialog = new ServerDialog(this->mainWindow);
    connect(this->server, SIGNAL(newConnection()), dialog, SLOT(accept()));
    if(dialog->exec() == QDialog::Rejected)
        this->server->pauseAccepting();
    delete dialog;
}

void SSGetter::setClientMode()
{
    //this->server->pauseAccepting();
    QString addr = ClientDialog::getText(this->mainWindow);
    if(!addr.isNull())
        this->client->connectToHost(addr, 2366);
}

void SSGetter::onClientConnected()
{
    emit gotStream(new SocketStream(this->client, false));
}
