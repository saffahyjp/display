#ifndef SSGETTER_H
#define SSGETTER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMainWindow>
#include "socketstream.h"

class SSGetter: public QObject
{
    Q_OBJECT
public:
    explicit SSGetter(QMainWindow *mainWindow, QObject *parent = Q_NULLPTR);
    QMainWindow *mainWindow;
    QTcpServer *server;
    QTcpSocket *client;
public slots:
    void onServerConnected();
    void onClientConnected();
    void setServerMode();
    void setClientMode();
signals:
    void gotStream(SocketStream *ss);
};

#endif // SSGETTER_H
