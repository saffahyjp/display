#include "data.h"
#include <QDebug>

Data::Data()
{
    this->myTurn = true;
    this->myChess = Data::BlackChess;
    this->othChess = Data::WhiteChess;
    this->finish = false;
    this->lx = -1; this->ly = -1;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
        {
            this->map[i][j] = Data::NoChess;
            this->dang[i][j] = false;
            this->safe[i][j] = false;
        }
}

void Data::clearSwap()
{
    std::swap(this->myChess, this->othChess);
    this->myTurn = (this->myChess == Data::BlackChess);
    this->finish = false;
    this->lx = -1; this->ly = -1;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
        {
            this->map[i][j] = Data::NoChess;
            this->dang[i][j] = false;
            this->safe[i][j] = false;
        }
}

bool Data::readyDefeat(int x, int y)
{
    if(this->map[x][y] != Data::NoChess)
        return false;
    const int idx[4] = {0, 1, 1, 1};
    const int idy[4] = {1, -1, 0, 1};
    for(int dir = 0; dir < 4; dir++)
    {
        int px = x + idx[dir], py = y + idy[dir];
        int nx = x - idx[dir], ny = y - idy[dir];
        while(this->isValid(px, py) && this->map[px][py] == this->othChess)
        {
            px += idx[dir]; py += idy[dir];
        }
        while(this->isValid(nx, ny) && this->map[nx][ny] == this->othChess)
        {
            nx -= idx[dir]; ny -= idy[dir];
        }
        int len = std::max(abs(px - nx), abs(py - ny)) - 1;
        if(len >= 5 || (len == 4 && this->isValid(px, py) && this->map[px][py] == Data::NoChess && this->isValid(nx, ny) && this->map[nx][ny] == Data::NoChess))
            return true;
    }
    return false;
}

bool Data::canDefend(int x, int y)
{
    if(this->map[x][y] != Data::NoChess)
        return false;
    this->map[x][y] = this->myChess;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(this->readyDefeat(i, j))
                goto returnfalse;
    this->map[x][y] = Data::NoChess;
    return true;
    returnfalse:
    this->map[x][y] = Data::NoChess;
    return false;
}

bool Data::isDang(int x, int y)
{
    if(this->map[x][y] != Data::NoChess)
        return false;
    this->map[x][y] = this->othChess;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(this->canDefend(i, j))
                goto returnfalse;
    this->map[x][y] = Data::NoChess;
    return true;
    returnfalse:
    this->map[x][y] = Data::NoChess;
    return false;
}

void Data::calcDang()
{
    std::swap(this->myChess, this->othChess);
    bool ok = false;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(this->safe[i][j] = this->readyDefeat(i, j))
                ok = true;
    if(!ok)
        for(int i = 0; i < 15; i++)
            for(int j = 0; j < 15; j++)
                this->safe[i][j] = this->isDang(i, j);
    std::swap(this->myChess, this->othChess);
    ok = false;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            if(this->dang[i][j] = this->readyDefeat(i, j))
                ok = true;
    if(!ok)
        for(int i = 0; i < 15; i++)
            for(int j = 0; j < 15; j++)
                this->dang[i][j] = this->isDang(i, j);
}

bool Data::isFinish()
{
    for(int x = 0; x < 15; x++)
        for(int y = 0; y < 15; y++)
            if(this->map[x][y] != Data::NoChess)
                for(int dx = -1; dx <= 1; dx++)
                    for(int dy = -1; dy <= 1; dy++)
                        if(dx || dy)
                        {
                            bool ok = true;
                            for(int i = 1; i <= 4; i++)
                            {
                                int tx = x + dx * i, ty = y + dy * i;
                                if(!this->isValid(tx, ty) || this->map[tx][ty] != this->map[x][y])
                                    ok = false;
                            }
                            if(ok)
                                return true;
                        }
    return false;
}

void Data::calcFinish()
{
    this->finish = this->isFinish();
    qDebug() << "finish" << this->finish;
}

bool Data::isValid(int x, int y)
{
    return x >= 0 && x < 15 && y >= 0 && y < 15;
}

QDataStream &operator <<(QDataStream &out, const Data &data)
{
    out << !data.myTurn;
    out << (qint32) data.othChess << (qint32) data.myChess;
    out << data.finish;
    out << data.lx << data.ly;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            out << (qint32) data.map[i][j];
    return out;
}

QDataStream &operator >>(QDataStream &in, Data &data)
{
    in >> data.myTurn;
    {
        qint32 tmp, tmq;
        in >> tmp >> tmq;
        data.myChess = (Data::Chess) tmp;
        data.othChess = (Data::Chess) tmq;
    }
    in >> data.finish;
    in >> data.lx >> data.ly;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
        {
            qint32 tmp; in >> tmp;
            data.map[i][j] = (Data::Chess) tmp;
        }
    data.calcDang();
    return in;
}
