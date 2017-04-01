#include "socketstream.h"
#include <QDebug>

SocketStream::SocketStream(QTcpSocket *socket, bool isHost, QObject *parent): QObject(parent), stream(socket)
{
    this->isHost = isHost;
    this->socket = socket;
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(socketRecv()));
}

void SocketStream::socketRecv()
{
    QVariant var;
    this->stream >> var;
    emit recv(var);
}

void SocketStream::send(const QVariant &var)
{
    this->stream << var;
    this->socket->waitForBytesWritten(1000);
}
