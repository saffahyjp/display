#include "game.h"
#include <QLayout>
#include <QDebug>
#include <QMessageBox>

Game::Game(SocketStream *ss, QWidget *parent): QWidget(parent)
{
    //qDebug() << "newGame";
    this->ss = ss;
    QVBoxLayout *layout = new QVBoxLayout();
        this->colorL = new QLabel();
    layout->addWidget(this->colorL);
        this->turnL = new QLabel();
    layout->addWidget(this->turnL);
        this->le = new QLineEdit();
        connect(this->le, SIGNAL(textChanged(QString)), this, SLOT(sendStr(QString)));  
    //layout->addWidget(this->le);
        connect(this->ss, SIGNAL(recv(QVariant)), this, SLOT(recv(QVariant)));
        this->lb = new QLabel();
    //layout->addWidget(this->lb);
        this->board = new Board(this);
    layout->addWidget(this->board, 1);
    this->setLayout(layout);
    
    this->data.myTurn = ss->isHost;
    if(!this->data.myTurn)
        std::swap(this->data.myChess, this->data.othChess);
    
    this->refresh();
}

void Game::refresh()
{
    this->colorL->setText(QString("You Are ") + (this->data.myChess == Data::WhiteChess ? "White" : "Black"));
    this->turnL->setText(this->data.finish ? "Game Finished" : (this->data.myTurn ? "Your Turn" : "Your Opponent's Turn"));
    this->repaint();
}

void Game::onBoardClicked(int x, int y)
{
    if(this->data.finish || this->data.map[x][y] != Data::NoChess)
        return;
    this->data.myTurn = false;
    this->data.map[x][y] = this->data.myChess;
    this->data.lx = x; this->data.ly = y;
    this->data.calcFinish();
    this->data.calcDang();
    this->ss->send(QVariant::fromValue(this->data));
    this->refresh();
    if(this->data.finish)
    {
        this->ss->send(QString("You Lost!"));
        QMessageBox::information(this, "Result", "You Won!");
    }
}

void Game::onRestartClicked()
{
    if(!this->data.finish)
        return;
    this->data.clearSwap();
    this->ss->send(QVariant::fromValue(this->data));
    this->refresh();
}

void Game::sendStr(const QString &str)
{
    this->ss->send(str);
}

void Game::recv(const QVariant &var)
{
    if(var.canConvert<QString>())
        QMessageBox::information(this, "Result", var.toString());
    else if(var.canConvert<Data>())
    {
        this->data = var.value<Data>();
        this->refresh();
    }
    //this->lb->setText(var.toString());
}
