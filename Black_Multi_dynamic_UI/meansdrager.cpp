#include "meansdrager.h"
MeansDrager::MeansDrager(QWidget *parent) :
    QLabel(parent)
{
    setParent(parent);
    ismoved=flag=false;
    isDragable=true;
    objectList.clear();
}

MeansDrager::~MeansDrager()
{

}

bool MeansDrager::is_single()
{
    return single;
}

void MeansDrager::set_single(bool flag)
{
    single=flag;
}

bool MeansDrager::is_dragable()
{
    return isDragable;
}

void MeansDrager::set_dragable(bool flag)
{
    isDragable=flag;
}

void MeansDrager::set_object(QWidget *object)
{
    objectList.clear();
    objectList.push_back(object);
}

void MeansDrager::set_object(QList<QWidget *> list)
{
    objectList=list;
}

void MeansDrager::add_object(QWidget *object)
{
    if(!objectList.contains(object))
        objectList.push_back(object);
}

void MeansDrager::add_object(QList<QWidget *> list)
{
    for(int i=0;i<list.count();i++)
    {
        if(!objectList.contains(list[i]))
            objectList.push_back(list[i]);
    }
}

void MeansDrager::del_object(QWidget *object)
{
    objectList.removeOne(object);
}

void MeansDrager::setParent(QWidget *parent)
{
    QLabel::setParent(parent);
    set_single(parent==0);
}

void MeansDrager::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    ismoved=false;
    if(event->button() == Qt::LeftButton)
        emit pressed();
    if(isDragable && event->button() == Qt::LeftButton)
    {
        flag=true;
        if(single)
        {
            dragPosition=event->globalPos()-frameGeometry().topLeft();
        }
        else
        {
            dragPosition=event->globalPos()-((QWidget*)parent())->frameGeometry().topLeft();
        }
        positionList.clear();
        for(int i=0;i<objectList.size();i++)
        {
            if(objectList[i]!=NULL)
            {
                positionList.push_back(event->globalPos()-objectList[i]->frameGeometry().topLeft());
                objectList[i]->raise();
            }
            else
            {
                positionList.push_back(QPoint(0,0));
            }
        }
         event->accept();
    }
    if(single)
        raise();
    else
        ((QWidget*)parent())->raise();
}

void MeansDrager::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
    if(flag && event->buttons() == Qt::LeftButton)
    {
        ismoved=true;
        emit moved();
        if(single)
        {
            move(event->globalPos()-dragPosition);
        }
        else
        {
            ((QWidget*)parent())->move(event->globalPos()-dragPosition);
        }

        for(int i=0;i<objectList.size();i++)
        {
            if(objectList[i]!=NULL)
            {
                objectList[i]->move(event->globalPos()-positionList[i]);
            }
        }
         event->accept();
    }
}

void MeansDrager::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    if(flag && !ismoved)
        emit clicked();
    emit released();
    ismoved=flag=false;
    event->accept();
}
