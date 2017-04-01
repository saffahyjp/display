#include "mousebindings.h"
#include <QString>

MouseBindings::MouseBindings()
{
    this->dateActionLeft = MouseBindings::PopupDateMenu;
    this->dateActionRight = MouseBindings::PopupDateMenu;
    this->fileActionLeft = MouseBindings::PopupFileMenu;
    this->fileActionRight = MouseBindings::PopupFileMenu;
}

QDataStream &operator <<(QDataStream &out, const MouseBindings &bind)
{
    return out << QString::number((int) bind.dateActionLeft) << QString::number((int) bind.dateActionRight) << QString::number((int) bind.fileActionLeft) << QString::number((int) bind.fileActionRight);
}

QDataStream &operator >>(QDataStream &in, MouseBindings &bind)
{
    QString a, b, c, d;
    in >> a >> b >> c >> d;
    bind.dateActionLeft = (MouseBindings::DateAction) a.toInt();
    bind.dateActionRight = (MouseBindings::DateAction) b.toInt();
    bind.fileActionLeft = (MouseBindings::FileAction) c.toInt();
    bind.fileActionRight = (MouseBindings::FileAction) d.toInt();
    return in;
}
