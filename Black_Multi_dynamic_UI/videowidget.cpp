#include "videowidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
VideoWidget::VideoWidget(ImgIndex *imgIndex,MusicWidget *musicWidget, QWidget *parent) :
    QWidget(parent)
{
    listMaker.read_list("./config/list.jel");
    QDesktopWidget *w=QApplication::desktop();
    resize(w->height()*16/15,w->height()*3/5);
    px=(w->width()-width())/2+60;
    py=(w->height()-height())*2/5;
    setWindowFlags(Qt::FramelessWindowHint);
    QFile style("./styles/videowidget.qss");
    style.open(QIODevice::ReadOnly);
    setStyleSheet(QString(style.readAll()));

    vPlayer=new QMediaPlayer(this);
    VideoOut=new MeansVideoWidget(this);
    VideoOut->resize(456,265);
    vPlayer->setVideoOutput(VideoOut);
    vPlayer->setParent(this);
    set_friend(musicWidget,imgIndex);
}

VideoWidget::~VideoWidget()
{
    delete vPlayer;
    delete VideoOut;
}

void VideoWidget::set_friend(MusicWidget *musicWidget, ImgIndex *imgIndex)
{
    connect(musicWidget,SIGNAL(set_position(int)),this,SLOT(set_media_position(int)));
    connect(musicWidget,SIGNAL(change_state(QMediaPlayer::State)),this,SLOT(change_state(QMediaPlayer::State)));
    connect(musicWidget,SIGNAL(full_screen(bool)),VideoOut,SLOT(setFullScreen(bool)));

    connect(musicWidget,SIGNAL(pressed()),this,SLOT(raise_widget()));
    connect(imgIndex,SIGNAL(pressed()),this,SLOT(raise_widget()));

    VideoOut->set_player(musicWidget);
    this->musicWidget=musicWidget;
    this->imgIndex=imgIndex;
}

///slots
void VideoWidget::change_state(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::PlayingState:
        if(vPlayer->mediaStatus()==QMediaPlayer::NoMedia)
        {
            if(this->isVisible())
            {;}//MeansWaring::warns("(*´iωi｀*)没有视频",2000,250,60);
        }
        else
        {
            vPlayer->play();
        }
        break;
    case QMediaPlayer::PausedState:
        vPlayer->pause();
        break;
    case QMediaPlayer::StoppedState:
        vPlayer->stop();
        break;
    }
}

void VideoWidget::set_media_position(int position)
{
    vPlayer->setPosition(position);
}

void VideoWidget::set_file_name(QString code)
{
    QJsonObject object=listMaker.object(code);
    QString dir=QString("./MultiData/[%1] %2/").arg(code).arg(object["name"].toString());
    QString fileName=object["video"].toString();

    if(QFile::exists(dir+fileName))
    {
        vPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(dir+fileName)));
    }
    else
    {
        if(VideoOut->isVisible())
        {}//MeansWaring::warns("(-∀-｀;)视频找不到了",2000,250,60,this);
    }
}

void VideoWidget::raise_widget()
{
    musicWidget->raise();
    imgIndex->raise();
    this->raise();
}

void VideoWidget::refresh_list()
{
    listMaker.read_list("./config/list.jel");
}

/**               *
 *MeansVideoWidget*
 */
MeansVideoWidget::MeansVideoWidget(QWidget *parent) :
    QVideoWidget(parent)
{
    time=new QTimer(this);
    time->setSingleShot(true);
    setStyleSheet("background-color:#000;");
}

MeansVideoWidget::~MeansVideoWidget()
{
    delete time;
}

void MeansVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    time->stop();
    setFocusPolicy(Qt::StrongFocus);
    if(event->buttons()==Qt::LeftButton)
    {
        if(isFullScreen())
        {
            setFullScreen(false);
            setGeometry(0,0,456,265);
        }
        else
            setFullScreen(true);
    }
    event->accept();
}

void MeansVideoWidget::mousePressEvent(QMouseEvent *event)
{
    emit pressed();
    setFocusPolicy(Qt::StrongFocus);
    time->start(300);
    event->accept();
}

void MeansVideoWidget::keyPressEvent(QKeyEvent *event)
{
    if(isFullScreen() && event->key()==Qt::Key_Escape)
    {
        setFullScreen(false);
        setGeometry(0,0,456,265);
    }
    event->accept();
}


void MeansVideoWidget::set_player(MusicWidget *musicWidget)
{
    connect(time,SIGNAL(timeout()),musicWidget,SLOT(click_p_button()));
}

