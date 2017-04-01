#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QKeySequenceEdit>
#include <QLocale>
#include <QComboBox>
#include <QTranslator>
#include <QList>
#include <QDataStream>
#include <QSettings>
#include "datamanager.h"
#include "mousebindings.h"

struct pddi
{
    QDate first; DateInfo second;
    friend QDataStream &operator <<(QDataStream &out, const pddi &p);
    friend QDataStream &operator >>(QDataStream &in, pddi &p);
};
Q_DECLARE_METATYPE(pddi)

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPoint dPos;
    bool edges;
    bool moveEnabled;
    bool dragEnabled;
    bool isMoving;
    QShortcut *enMouseKey;
    QKeySequenceEdit *enmedit;
    QLocale locale;
    QComboBox *lang;
    QList<QTranslator *> translations;
    QSettings *settings;
    DataManager *dm;
    MouseBindings bind;
    /*friend QDataStream &operator >>(QDataStream &in, MainWindow *w);
    friend QDataStream &operator <<(QDataStream &out, MainWindow *w);*/
public slots:
    void setPercentageOpacity(int op);
    void changeEdges();
    void disableMouse();
    void enableMouse();
    void setEnMouseKey();
    void onSetColorClicked();
    void refresh();
    void setDragEnabled(bool e);
    void languageChanged(int idx);
    void importDataClicked();
    void exportDataClicked();
    void readSettings();
    void writeSettings();
    void setMouseBindings();
protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void setBackgroundColor(const QColor &color);
};

#endif // MAINWINDOW_H
