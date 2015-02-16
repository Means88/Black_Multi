#include "mainwidget.h"
#include <QApplication>
#include <QDesktopWidget>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    int w=QApplication::desktop()->width(),h=QApplication::desktop()->height();
    resize(w,h);
    setWindowFlags(Qt::FramelessWindowHint |Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    imgIndex=new ImgIndex;
    musicWidget=new MusicWidget;
    detailContainer=new DetailContainer;
    videoWidget=new VideoWidget(imgIndex,musicWidget,detailContainer->VideoTab);
    imgLabel=new ImgLabel;

    videoWidget->setGeometry(0,0,456,265);

    imgIndex->set_music(musicWidget->interface());
    detailContainer->set_music(musicWidget->interface());
    connect_objects();

    musicWidget->move(w/2-musicWidget->width(),h/2-musicWidget->height());
    imgIndex->adsopent(MeansAdsorpent::BottomMode);
    detailContainer->adsopent(MeansAdsorpent::RightMode);

    imgIndex->show();
    musicWidget->show();
    detailContainer->show();

    detailContainer->setWindowOpacity(0.7);
    detailContainer->setWindowOpacity(1);
}

MainWidget::~MainWidget()
{
    delete imgIndex;
    delete musicWidget;
    delete detailContainer;
    delete videoWidget;
    delete imgLabel;
}

void MainWidget::connect_objects()
{
    connect(imgIndex,SIGNAL(select_img(QString,bool)),imgLabel,SLOT(show_img(QString)));
    connect(imgIndex,SIGNAL(select_img(QString,bool)),detailContainer->imgLabel,SLOT(show_img(QString)));
    connect(imgIndex,SIGNAL(select_img(QString,bool)),musicWidget,SLOT(set_file_name(QString,bool)));
    connect(imgIndex,SIGNAL(select_img(QString,bool)),videoWidget,SLOT(set_file_name(QString)));
    connect(detailContainer->imgLabel,SIGNAL(mouse_doubleclick()),imgLabel,SLOT(show()));
    connect(imgLabel,SIGNAL(mouse_doubleclick()),imgLabel,SLOT(hide()));

    connect(musicWidget,SIGNAL(switch_list()),this,SLOT(switch_list()));
    connect(imgIndex,SIGNAL(switch_off()),musicWidget,SLOT(list_off()));
    connect(musicWidget,SIGNAL(switch_lyric()),this,SLOT(switch_lyric()));
    connect(detailContainer,SIGNAL(switch_off()),musicWidget,SLOT(lyric_off()));

    connect(detailContainer,SIGNAL(pressed()),musicWidget,SLOT(raise()));
    connect(detailContainer,SIGNAL(pressed()),imgIndex,SLOT(raise()));
    connect(imgIndex,SIGNAL(pressed()),musicWidget,SLOT(raise()));
    connect(imgIndex,SIGNAL(pressed()),detailContainer,SLOT(raise()));

    connect(this,SIGNAL(refresh()),this,SLOT(refresh_object()));
}

///slots
void MainWidget::show_widget()
{
    imgLabel->setVisible(!imgLabel->isVisible());
    musicWidget->setVisible(!musicWidget->isVisible());
}

void MainWidget::raise_widget()
{
    imgIndex->raise();
    musicWidget->raise();
    imgLabel->raise();
    detailContainer->raise();
}

void MainWidget::switch_list()
{
    imgIndex->setVisible(!imgIndex->isVisible());
}

void MainWidget::switch_lyric()
{
    detailContainer->setVisible(!detailContainer->isVisible());
    detailContainer->setWindowOpacity(0.7);
    detailContainer->setWindowOpacity(1);
}

void MainWidget::set_imglabel()
{

}

void MainWidget::refresh_object()
{
    imgIndex->init_list();
    imgLabel->refresh_list();
    musicWidget->refresh_list();
    videoWidget->refresh_list();
    detailContainer->imgLabel->refresh_list();
}
