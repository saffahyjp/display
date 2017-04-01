#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPaintEvent>
#include <QPointF>
#include "game.h"

class Game;

class Board: public QWidget
{
    Q_OBJECT
public:
    explicit Board(Game *game, QWidget *parent = 0);
    QPointF offset;
    qreal pos[15];
    Game *game;
    bool enDang, enSafe;
    int getPos(qreal x);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
signals:
    
public slots:
    void setEnDang(bool e);
    void setEnSafe(bool e);
};

#endif // BOARD_H
