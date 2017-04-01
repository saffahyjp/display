#ifndef SOCKETSTREAM_H
#define SOCKETSTREAM_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QList>
#include <QVariant>

class SocketStream: public QObject
{
    Q_OBJECT
public:
    explicit SocketStream(QTcpSocket *socket, bool isHost, QObject *parent = Q_NULLPTR);
    QTcpSocket *socket;
    QDataStream stream;
    bool isHost;
signals:
    void recv(const QVariant &var);
public slots:
    void send(const QVariant &var);
    void socketRecv();
};

#endif // SOCKETSTREAM_H
