#include <algorithm>
#include "imglabel.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

ImgLabel::ImgLabel(QWidget *parent) :
    MeansDrager(parent)
{
    listMaker.read_list("./config/list.jel");
    setWindowFlags(Qt::FramelessWindowHint |Qt::Tool |Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setScaledContents(true);
    scaleRate=100;
    dynamicImg=new QMovie(this);
    rollable=true;
    connect(dynamicImg,SIGNAL(frameChanged(int)),this,SLOT(show_dynamic_slot()));

    if(parent==0)
        move((QApplication::desktop()->width()-width())/2,30);
}

void ImgLabel::set_rollable(bool flag)
{
    rollable=flag;
}

void ImgLabel::refresh()
{
    show_picture(lastDir,lastImg);
}

int ImgLabel::parentHeight()
{
    if(parent()==NULL)
        return QApplication::desktop()->height()*6/7;
    else
        return ((QWidget*)parent())->height()*6/7;
}

void ImgLabel::show_img(QString code)
{
    QJsonObject object=listMaker.object(code);
    if(object.size()!=0)
    {
        QString dir=QString("[%1] %2").arg(object["code"].toString()).arg(object["name"].toString());
        QString image=object["image"].toString();
        if(dir.size() && image.size())
            show_picture(dir,image);
    }
}

void ImgLabel::show_picture(QString dir, QString image)
{
    QDir directory("./MultiData/"+dir);
    if(!directory.exists())
        return;
    if(image.section('.',-1,-1)!="gif")
        show_static_picture(dir,image);
    else
        show_dynamic_picture(dir,image);
}

void ImgLabel::show_static_picture(QString dir,QString fileName)
{
    if(QFile::exists("./MultiData/"+dir+'/'+fileName))
    {        
        QPixmap pix("./MultiData/"+dir+'/'+fileName);
        int width=parentHeight()*pix.width()*scaleRate/(100*pix.height());
        move(x()+(this->width()-width)/2,y()+(height()-parentHeight()*scaleRate/100)/2);
        resize(width,parentHeight()*scaleRate/100);
        setPixmap(pix);
        emit img_show();
    }
    else
    {
        //MeansWaring::warns("(つд⊂)图片貌似找不到了");
    }
}

void ImgLabel::show_dynamic_picture(QString dir, QString fileName)
{
    if(QFile::exists("./MultiData/"+dir+'/'+fileName))
    {
        dynamicImg->setFileName("./MultiData/"+dir+'/'+fileName);
        dynamicImg->start();
    }
    else
    {
        //MeansWaring::warns("(つд⊂)图片貌似找不到了");
    }
}

void ImgLabel::show_dynamic_slot()
{
    QPixmap pix=dynamicImg->currentPixmap();
    int width=parentHeight()*pix.width()*scaleRate/(100*pix.height());
    move(x()+(this->width()-width)/2,y()+(height()-parentHeight()*scaleRate/100)/2);
    resize(width,parentHeight()*scaleRate/100);
    setPixmap(pix);
    emit img_show();
}

void ImgLabel::refresh_list()
{
    listMaker.read_list("./config/list.jel");
}

///protected
void ImgLabel::wheelEvent(QWheelEvent *event)
{
    int delta=event->delta();
    if(rollable)
    {
        if(!((scaleRate<=30 && delta>0)||(scaleRate>=190 &&delta<0)))
        {
            scaleRate=std::max(scaleRate-event->delta()/12,30);
            int w=width(),h=height();
            int w2=parentHeight()*scaleRate*w/(100*h),h2=parentHeight()*scaleRate/100;
            int x2=x()-(w2-w)/2,y2=y()-(h2-h)/2;
            setGeometry(x2,y2,w2,h2);
        }
    }
    event->accept();
}

void ImgLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    MeansDrager::mouseDoubleClickEvent(event);
    emit mouse_doubleclick();
    event->accept();
}
