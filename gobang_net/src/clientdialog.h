#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSignalMapper>
#include <QGridLayout>

class ClientDialog: public QDialog
{
    Q_OBJECT
public:
    explicit ClientDialog(QString *str, QWidget *parent = Q_NULLPTR);
    QLineEdit *le;
    QString *str;
    QSignalMapper *sm;
    QGridLayout *table;
    void addButton(const QString &text, int x, int y);
    static QString getText(QWidget *parent = Q_NULLPTR);
public slots:
    void setStr(const QString &str);
    void onButtonClicked(const QString &text);
};

#endif // CLIENTDIALOG_H
