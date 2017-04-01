#include "mainwindow.h"
#include <QLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QSlider>
#include <QPushButton>
#include <QCheckBox>
#include <QKeySequenceEdit>
#include <QLabel>
#include <QColorDialog>
#include <QComboBox>
#include <QTranslator>
#include <QApplication>
#include <QDebug>
#include <QColor>
#include <QFileDialog>
#include <QMessageBox>
#include "calendar.h"
#include "datamanager.h"
#include "mousebindingsdialog.h"

QDataStream &operator <<(QDataStream &out, const pddi &p)
{
    return out << p.first << p.second.color;
}

QDataStream &operator >>(QDataStream &in, pddi &p)
{
    return in >> p.first >> p.second.color;
}

void MainWindow::importDataClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), tr("Data files (*.ini)"), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if(fileName.isNull())
        return;
    if(fileName == QDir().absoluteFilePath("config.ini"))
        return;
    this->writeSettings();
    this->settings->sync();
    QFile::remove(QDir().absoluteFilePath("config.ini"));
    if(QFile::copy(fileName, QDir().absoluteFilePath("config.ini")))
        QMessageBox::information(this, this->windowTitle(), tr("Import success!"));
    else
        QMessageBox::critical(this, this->windowTitle(), tr("Import failed!"));
    this->settings->sync();
    this->readSettings();
    this->refresh();
}

void MainWindow::exportDataClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString(), QString(), tr("Data files (*.ini)"), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if(fileName.isNull())
        return;
    QFile *file = new QFile(fileName);
    if(file->exists() && !file->remove())
        goto fail;
    if(!QFile::copy(QDir().absoluteFilePath("config.ini"), fileName))
        goto fail;
    QMessageBox::information(this, this->windowTitle(), tr("Export success!"));
    return;
    fail:
    QMessageBox::critical(this, this->windowTitle(), tr("Export failed!"));
}

void MainWindow::setBackgroundColor(const QColor &color)
{
    QPalette pl = this->palette();
    pl.setColor(QPalette::Window, color);
    this->setPalette(pl);
}

void MainWindow::onSetColorClicked()
{
    QColor color = QColorDialog::getColor(this->palette().color(QPalette::Window), this, "", QColorDialog::DontUseNativeDialog);
    if(color.isValid())
        this->setBackgroundColor(color);
}

void MainWindow::setDragEnabled(bool e)
{
    this->dragEnabled = e;
}

void MainWindow::refresh()
{
    this->setWindowTitle(tr("Desktop Calendar"));
    QFrame *frame = new QFrame();
    QLayout *layout = new QVBoxLayout(frame);
        Calendar *calender = new Calendar(this, this->dm);
    layout->addWidget(calender);
    {
        QFrame *hframe = new QFrame();
        QHBoxLayout *hlayout = new QHBoxLayout(hframe);
        hlayout->addStretch();
            QLabel *olabel = new QLabel(tr("Opacity"));
        hlayout->addWidget(olabel);
            QSlider *slider = new QSlider(Qt::Horizontal);
            slider->setMinimum(10);
            slider->setMaximum(100);
            connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setPercentageOpacity(int)));
            slider->setValue(90);
        hlayout->addWidget(slider);
            QCheckBox *drag = new QCheckBox(tr("Enable Drag"));
            drag->setChecked(this->dragEnabled);
            connect(drag, SIGNAL(toggled(bool)), this, SLOT(setDragEnabled(bool)));
        hlayout->addWidget(drag);
            QLabel *enmlab = new QLabel(tr("Shortcut for Enabling Mouse:"));
        hlayout->addWidget(enmlab);
            this->enmedit = new QKeySequenceEdit();
            this->enmedit->setKeySequence(this->enMouseKey->key());
            connect(this->enmedit, SIGNAL(editingFinished()), this, SLOT(setEnMouseKey()));
        hlayout->addWidget(this->enmedit);
        hlayout->addStretch();
    layout->addWidget(hframe);
    }
    {
        QFrame *hframe = new QFrame();
        QHBoxLayout *hlayout = new QHBoxLayout(hframe);
        hlayout->addStretch();
            this->lang = new QComboBox();
            this->lang->addItem(QLocale::languageToString(QLocale("en_US").language()), QVariant(QLocale("en_US")));
            this->lang->addItem(QLocale::languageToString(QLocale("zh_CN").language()), QVariant(QLocale("zh_CN")));
            this->lang->setCurrentIndex(this->lang->findData(QVariant(this->locale)));
            connect(this->lang, SIGNAL(currentIndexChanged(int)), this, SLOT(languageChanged(int)));
        hlayout->addWidget(lang);
            QPushButton *setcolor = new QPushButton(tr("Background Color"));
            connect(setcolor, SIGNAL(clicked(bool)), this, SLOT(onSetColorClicked()));
        hlayout->addWidget(setcolor);
            QPushButton *resize = new QPushButton(tr("Window Frame ON/OFF"));
            connect(resize, SIGNAL(clicked(bool)), this, SLOT(changeEdges()));
        hlayout->addWidget(resize);
            QPushButton *dmouse = new QPushButton(tr("Disable Mouse"));
            connect(dmouse, SIGNAL(clicked(bool)), this, SLOT(disableMouse()));
        hlayout->addWidget(dmouse);
            QPushButton *importData = new QPushButton(tr("Import Data"));
            connect(importData, SIGNAL(clicked(bool)), this, SLOT(importDataClicked()));
        hlayout->addWidget(importData);
            QPushButton *exportData = new QPushButton(tr("Export Data"));
            connect(exportData, SIGNAL(clicked(bool)), this, SLOT(exportDataClicked()));
        hlayout->addWidget(exportData);
            QPushButton *mouseBindings = new QPushButton(tr("Mouse Bindings"));
            connect(mouseBindings, SIGNAL(clicked(bool)), this, SLOT(setMouseBindings()));
        hlayout->addWidget(mouseBindings);
        hlayout->addStretch();
    layout->addWidget(hframe);
    }
    this->setCentralWidget(frame);
}

void MainWindow::setMouseBindings()
{
    MouseBindings tmp = this->bind;
    MouseBindingsDialog *dlg = new MouseBindingsDialog(&tmp, this);
    if(dlg->exec() == QDialog::Accepted)
        this->bind = tmp;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->dm = new DataManager();
    this->moveEnabled = true;
    this->isMoving = false;
    this->edges = true;
    this->enMouseKey = new QShortcut(this);
    connect(this->enMouseKey, SIGNAL(activated()), this, SLOT(enableMouse()));

    this->settings = new QSettings("config.ini", QSettings::IniFormat);
    this->readSettings();
}

void MainWindow::readSettings()
{
    this->dragEnabled = this->settings->value("dragEnabled", true).toBool();
    this->enMouseKey->setKey(this->settings->value("enMouseKey").value<QKeySequence>());
    this->setBackgroundColor(this->settings->value("backGroundColor", QColor(Qt::white)).value<QColor>());
    this->locale = this->settings->value("locale", QLocale("en_US")).toLocale();
    this->bind = this->settings->value("mouseBindings", QVariant::fromValue(MouseBindings())).value<MouseBindings>();

    this->dm->dateInfos.clear();
    this->dm->eventInfos.clear();
    this->dm->fileInfos.clear();

    QList<QVariant> pddis = this->settings->value("pddis").toList();
    foreach(QVariant v, pddis)
    {
        pddi p = v.value<pddi>();
        this->dm->dateInfos[p.first] = p.second;
    }
    QList<QVariant> events = this->settings->value("events").toList();
    foreach(QVariant v, events)
    {
        EventInfo e = v.value<EventInfo>();
        this->dm->eventInfos << e;
    }
    QList<QVariant> files = this->settings->value("files").toList();
    foreach(QVariant v, files)
    {
        FileInfo f = v.value<FileInfo>();
        this->dm->fileInfos << f;
    }

    QLocale::setDefault(this->locale);
    this->refresh();
    this->languageChanged(this->locale != QLocale("en_US"));
}

void MainWindow::languageChanged(int idx)
{
    this->locale = this->lang->itemData(idx).toLocale();
    qDebug() << this->locale.uiLanguages();
    QLocale::setDefault(this->locale);
    foreach(QTranslator *translation, this->translations)
    {
        qApp->removeTranslator(translation);
        qDebug() << "remove" << translation;
        delete translation;
    }
    this->translations.clear();
    QStringList sl = QStringList({"tr", "qt"/*, "assistant", "designer", "linguist", "qt_help", "qt", "qtconfig", "qtcreator"*/});
    foreach(QString str, sl)
    {
        QTranslator *translation = new QTranslator();
        if(!translation->load(this->locale, str, "_"))
            qDebug() << this->locale << QString("Fail to load translation file ") + str;
        qDebug() << "install" << translation;
        qApp->installTranslator(translation);
        this->translations << translation;
    }
    this->refresh();
}

void MainWindow::writeSettings()
{
    this->settings->setValue("dragEnabled", this->dragEnabled);
    this->settings->setValue("enMouseKey", this->enMouseKey->key());
    this->settings->setValue("backgroundColor", this->palette().color(QPalette::Window));
    this->settings->setValue("locale", this->locale);
    this->settings->setValue("mouseBindings", QVariant::fromValue(this->bind));

    QList<QVariant> pddis;
    foreach(QDate date, this->dm->dateInfos.keys())
        pddis << QVariant::fromValue((pddi) {date, this->dm->dateInfos[date]});
    this->settings->setValue("pddis", pddis);
    QList<QVariant> events;
    foreach(EventInfo e, this->dm->eventInfos)
        events << QVariant::fromValue(e);
    this->settings->setValue("events", events);
    QList<QVariant> files;
    foreach(FileInfo f, this->dm->fileInfos)
        files << QVariant::fromValue(f);
    this->settings->setValue("files", files);

    this->settings->sync();
}

MainWindow::~MainWindow()
{
    this->writeSettings();
}

void MainWindow::disableMouse()
{
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->changeEdges();
    this->changeEdges();
}

void MainWindow::enableMouse()
{
    bool tmpEdges = this->edges;
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    this->setWindowFlags(Qt::Window);
    this->edges = true;
    this->show();
    while(this->edges != tmpEdges)
        this->changeEdges();
    /*this->moveEnabled = false;
    this->isMoving = false;
    QSize size = this->size();
    this->setWindowFlags(this->windowFlags());
    this->resize(size);
    this->show();
    this->moveEnabled = true;*/
}

void MainWindow::setEnMouseKey()
{
    this->enMouseKey->setKey(this->enmedit->keySequence());
}

void MainWindow::setPercentageOpacity(int op)
{
    this->setWindowOpacity(op / (qreal) 100);
}

void MainWindow::changeEdges()
{
    this->edges = !this->edges;
    this->moveEnabled = false;
    this->isMoving = false;
    QSize size = this->size();
    if(this->edges)
        this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);
    else
        this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->resize(size);
    this->show();
    this->moveEnabled = true;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(this->moveEnabled)
    {
        this->dPos = event->globalPos() - this->pos();
        this->isMoving = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(this->moveEnabled && this->isMoving)
        this->move(event->globalPos() - this->dPos);
}
