#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QNetworkReply>

class ServerDialog: public QDialog
{
    Q_OBJECT
public:
    explicit ServerDialog(QWidget *parent = Q_NULLPTR);
    QLabel *ipLabel;
    QNetworkReply *reply;
public slots:
    void gotIP();
};

#endif // SERVERDIALOG_H
