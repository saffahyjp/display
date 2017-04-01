#ifndef DATA_H
#define DATA_H

#include <QMetaType>
#include <QDataStream>

class Data
{
public:
    Data();
    bool myTurn;
    int lx, ly;
    bool finish;
    bool dang[15][15], safe[15][15];
    enum Chess{
        NoChess, BlackChess, WhiteChess
    } map[15][15], myChess, othChess;
    bool isValid(int x, int y);
    bool isFinish();
    void calcFinish();
    bool isDang(int x, int y);
    bool canDefend(int x, int y);
    bool readyDefeat(int x, int y);
    void calcDang();
    void clearSwap();
    friend QDataStream &operator <<(QDataStream &out, const Data &data);
    friend QDataStream &operator >>(QDataStream &in, Data &data);
};
Q_DECLARE_METATYPE(Data)

#endif // DATA_H
