#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include "socketstream.h"
#include "board.h"
#include "data.h"

class Board;

class Game: public QWidget
{
    Q_OBJECT
public:
    explicit Game(SocketStream *ss, QWidget *parent = 0);
    SocketStream *ss;
    QLineEdit *le;
    QLabel *lb, *colorL, *turnL;
    Board *board;
    Data data;
    void onBoardClicked(int x, int y);
    void refresh();
signals:
    
public slots:
    void onRestartClicked();
    void sendStr(const QString &str);
    void recv(const QVariant &var);
};

#endif // GAME_H
