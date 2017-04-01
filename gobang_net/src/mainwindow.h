#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QList>
#include <QAction>
#include "ssgetter.h"
#include "game.h"

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SSGetter *ssg;
    QHBoxLayout *layout;
    QList<Game *> games;
    QAction *aRestart, *aDang, *aSafe;
public slots:
    void setSocketStream(SocketStream *ss);
};

#endif // MAINWINDOW_H
