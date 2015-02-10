#include "meanswaring.h"
#include <QFile>
#include <QApplication>
#include <QDesktopWidget>
MeansWaring::MeansWaring(QString text, int time, int w, int h, QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QFile style("./styles/meanswarning.qss");
    style.open(QIODevice::ReadOnly);
    setStyleSheet(QString(style.readAll()));

    resize(w,h);
    label=new QLabel(this);
    label->resize(w,h);
    label->setAlignment(Qt::AlignCenter);
    label->setText(text);

    this->time=time;
    timer=new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(delete_this()));
    timer->start(time);

    int pw,ph;
    if(parent==NULL)
    {
        pw=QApplication::desktop()->width();
        ph=QApplication::desktop()->height();
    }
    else
    {
        pw=parent->width();
        ph=parent->height();
    }
    move(parent->x()+(pw-width())/2,parent->y()+(ph-height())/2);
}

MeansWaring::~MeansWaring()
{
}

void MeansWaring::warns(QString text, int time, int w, int h, QWidget *parent)
{
    (new MeansWaring(text,time,w,h,parent))->show();
}

void MeansWaring::delete_this()
{
    delete this;
}
