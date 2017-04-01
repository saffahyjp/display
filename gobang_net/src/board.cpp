#include "board.h"
#include <QPainter>
#include <QPixmap>

Board::Board(Game *game, QWidget *parent) : QWidget(parent)
{
    this->game = game;
    this->enDang = false; this->enSafe = false;
}

void Board::setEnDang(bool e)
{
    this->enDang = e;
    this->game->refresh();
}
void Board::setEnSafe(bool e)
{
    this->enSafe = e;
    this->game->refresh();
}

int Board::getPos(qreal x)
{
    for(int i = 0; i < 15; i++)
        if(abs(x - pos[i]) <= pos[0] / 2.5)
            return i;
    return -1;
}

void Board::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
    {
        QWidget::mousePressEvent(event);
        return;
    }
    if(!this->game->data.myTurn)
        return;
    QPointF pos = QPointF(event->pos()) - this->offset;
    int x = getPos(pos.x()), y = getPos(pos.y());
    if(x == -1 || y == -1)
        return;
    this->game->onBoardClicked(x, y);
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    
    qreal a;
    if(this->width() > this->height())
    {
        a = this->height();
        this->offset = QPointF((qreal) (this->width() - this->height()) / 2.0, 0);
    }
    else
    {
        a = this->width();
        this->offset = QPointF(0, (qreal) (this->height() - this->width()) / 2.0);
    }
    p.translate(this->offset);
    
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(255, 204, 153)));
    p.drawRect(0, 0, a, a);
    p.restore();
    
    p.save();
    p.setPen(Qt::black);
    p.setBrush(Qt::NoBrush);
    for(int i = 0; i < 15; i++)
        this->pos[i] = a * (i + 1) / 16.0;
    for(int i = 0; i < 15; i++)
        p.drawLine(this->pos[i], this->pos[0], this->pos[i], this->pos[14]);
    for(int j = 0; j < 15; j++)
        p.drawLine(this->pos[0], this->pos[j], this->pos[14], this->pos[j]);
    p.restore();
    
    p.save();
    p.setPen(QPen(Qt::black));
    p.setBrush(QBrush(Qt::white));
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(this->game->data.map[i][j] == Data::WhiteChess)
                p.drawEllipse(QPointF(this->pos[i], this->pos[j]), this->pos[0] / 2.0, this->pos[0] / 2.0);
    p.restore();
    
    p.save();
    p.setPen(QPen(Qt::white));
    p.setBrush(QBrush(Qt::black));
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(this->game->data.map[i][j] == Data::BlackChess)
                p.drawEllipse(QPointF(this->pos[i], this->pos[j]), this->pos[0] / 2.0, this->pos[0] / 2.0);
    p.restore();
    
    p.save();
    p.setPen(QPen(Qt::red));
    p.setBrush(Qt::NoBrush);
    if(this->game->data.lx != -1 && this->game->data.ly != -1)
        p.drawRect(this->pos[this->game->data.lx] - this->pos[0] / 2.0, this->pos[this->game->data.ly] - this->pos[0] / 2.0, this->pos[0], this->pos[0]);
    p.restore();
    
    p.save();
    //p.setPen(Qt::NoPen);
    //p.setBrush(QBrush(Qt::red));
    QPixmap redBomb = QPixmap("redbomb.png").scaled(this->pos[0] * 1.4, this->pos[0] * 1.4);
    if(this->enDang && !this->game->data.finish)
        for(int i = 0; i < 15; i++)
            for(int j = 0; j < 15; j++)
                if(this->game->data.dang[i][j])
                    p.drawPixmap(this->pos[i] - this->pos[0] * 0.7, this->pos[j] - this->pos[0] * 0.7, redBomb);
                    //p.drawEllipse(QPointF(this->pos[i], this->pos[j]), this->pos[0] / 4.0, this->pos[0] / 4.0);
    p.restore();
    
    p.save();
    //p.setPen(Qt::NoPen);
    //p.setBrush(QBrush(Qt::green));
    QPixmap greenBomb = QPixmap("greenbomb.png").scaled(this->pos[0] * 1.4, this->pos[0] * 1.4);
    if(this->enSafe && !this->game->data.finish)
        for(int i = 0; i < 15; i++)
            for(int j = 0; j < 15; j++)
                if(this->game->data.safe[i][j])
                    p.drawPixmap(this->pos[i] - this->pos[0] * 0.7, this->pos[j] - this->pos[0] * 0.7, greenBomb);
                    //p.drawEllipse(QPointF(this->pos[i], this->pos[j]), this->pos[0] / 4.0, this->pos[0] / 4.0);
    p.restore();
}
