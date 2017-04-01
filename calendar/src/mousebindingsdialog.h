#ifndef MOUSEBINDINGSDIALOG_H
#define MOUSEBINDINGSDIALOG_H

#include <QDialog>
#include "mousebindings.h"

class MouseBindingsDialog: public QDialog
{
    Q_OBJECT
public:
    MouseBindingsDialog(MouseBindings *bind_, QWidget *parent = Q_NULLPTR);
    MouseBindings *bind;
public slots:
    void setDateLeft(int x);
    void setDateRight(int x);
    void setFileLeft(int x);
    void setFileRight(int x);
};

#endif // MOUSEBINDINGSDIALOG_H
