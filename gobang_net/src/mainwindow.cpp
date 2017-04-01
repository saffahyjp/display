#include "mainwindow.h"
#include "game.h"
#include <QFrame>
#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->ssg = new SSGetter(this);
    connect(this->ssg, SIGNAL(gotStream(SocketStream*)), this, SLOT(setSocketStream(SocketStream*)));
    QFrame *frame = new QFrame();
    this->layout = new QHBoxLayout(frame);
    this->setCentralWidget(frame);
    
    QMenuBar *mb = new QMenuBar();
    mb->setNativeMenuBar(false);
    QMenu *menu = new QMenu("Network");
    mb->addMenu(menu);
    QAction *aServer = menu->addAction("Start Server");
    connect(aServer, SIGNAL(triggered(bool)), this->ssg, SLOT(setServerMode()));
    QAction *aClient = menu->addAction("Start Client");
    connect(aClient, SIGNAL(triggered(bool)), this->ssg, SLOT(setClientMode()));
    QMenu *menuG = new QMenu("Game");
    mb->addMenu(menuG);
    this->aRestart = menuG->addAction("Restart");
    menuG->addSeparator();
    this->aDang = menuG->addAction("Enable Dangerous Hints");
    this->aDang->setCheckable(true);
    this->aDang->setChecked(false);
    //connect(this->aDang, SIGNAL(triggered(bool)), this->aDang, SLOT(toggle()));
    this->aSafe = menuG->addAction("Enable \"Safe\" Hints");
    this->aSafe->setCheckable(true);
    this->aSafe->setChecked(false);
    //connect(this->aSafe, SIGNAL(triggered(bool)), this->aSafe, SLOT(toggle()));
    this->setMenuBar(mb);
    mb->setNativeMenuBar(false);
}

void MainWindow::setSocketStream(SocketStream *ss)
{
    Game *game = new Game(ss);
    this->games << game;
    this->layout->addWidget(game);
    connect(this->aRestart, SIGNAL(triggered(bool)), game, SLOT(onRestartClicked()));
    connect(this->aDang, SIGNAL(toggled(bool)), game->board, SLOT(setEnDang(bool)));
    connect(this->aSafe, SIGNAL(toggled(bool)), game->board, SLOT(setEnSafe(bool)));
}

MainWindow::~MainWindow()
{
}
