#include "meansadsorpent.h"

MeansAdsorpent::MeansAdsorpent(QWidget *parent) :
    QWidget(parent)
{
    musicWidget=NULL;
    MainWidget=new MeansDrager(this);
    isDraging=false;
    connect(MainWidget,SIGNAL(released()),this,SLOT(mouse_release()));
    connect(MainWidget,SIGNAL(pressed()),this,SLOT(mouse_press()));
    connect(MainWidget,SIGNAL(moved()),this,SLOT(mouse_move()));
}

MeansAdsorpent::~MeansAdsorpent()
{

}

void MeansAdsorpent::set_music(MusicInterface *musicWidget)
{
    this->musicWidget=musicWidget;
}

void MeansAdsorpent::set_mode(Mode mode)
{
    thismode=mode;
}

MeansAdsorpent::Mode MeansAdsorpent::mode()
{
    return thismode;
}

///private
MeansAdsorpent::Mode MeansAdsorpent::isNear()
{
    if(musicWidget==NULL)
        return NoMode;
    Mode result=NoMode;
    if(thismode & TopMode)
    {
        if(abs(y()+height()-musicWidget->y())<25 && abs(x()-musicWidget->x()))
            result=TopMode;
    }
    else if(thismode & BottomMode)
    {
        if(abs(y()-musicWidget->y()-musicWidget->height())<25 && abs(x()-musicWidget->x())<25)
            result=BottomMode;
    }
    else if(thismode & LeftMode)
    {
        if(abs(x()+width()-musicWidget->x())<25 && abs(y()-musicWidget->y()-45)<25)
            result=LeftMode;
    }
    else if(thismode & RightMode)
    {
        if(abs(x()-musicWidget->x()-musicWidget->width())<25 && abs(y()-musicWidget->y()-45)<25)
            result=RightMode;
    }
    return result;
}

void MeansAdsorpent::adsopent(Mode mode)
{
    if(musicWidget==NULL)
        return;
    switch (mode)
    {
    case TopMode:
        move(musicWidget->x(),musicWidget->y()-height());
        break;
    case BottomMode:
        move(musicWidget->x(),musicWidget->y()+musicWidget->height());
        break;
    case LeftMode:
        move(musicWidget->x()-width(),musicWidget->y()+45);
        break;
    case RightMode:
        move(musicWidget->x()+musicWidget->width(),musicWidget->y()+45);
        break;
    case NoMode:
        musicWidget->del_object(this);
        return;
        break;
    }
    musicWidget->add_object(this);
}

///slot吸附
void MeansAdsorpent::mouse_press()
{
    if(musicWidget!=NULL)
        isDraging=true;
    emit pressed();
}

void MeansAdsorpent::mouse_move()
{
    if(musicWidget!=NULL && isDraging && isNear()!=NoMode)
    {
        if(windowOpacity()==1)
            setWindowOpacity(0.7);
    }
    else if(windowOpacity()==0.7)
        setWindowOpacity(1);
}

void MeansAdsorpent::mouse_release()
{
    isDraging=false;
    setWindowOpacity(1);
    if(musicWidget!=NULL)
        adsopent(isNear());
}
