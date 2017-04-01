#ifndef MOUSEBINDINGS_H
#define MOUSEBINDINGS_H

#include <QMetaType>

class MouseBindings
{
public:
    MouseBindings();
    enum DateAction
    {
        PopupDateMenu, SetDateColor, AddDateEvent
    } dateActionLeft, dateActionRight;
    enum FileAction
    {
        PopupFileMenu, OpenFile, DeleteFile
    } fileActionLeft, fileActionRight;
    friend QDataStream &operator <<(QDataStream &out, const MouseBindings &bind);
    friend QDataStream &operator >>(QDataStream &in, MouseBindings &bind);
};
Q_DECLARE_METATYPE(MouseBindings)

#endif // MOUSEBINDINGS_H
