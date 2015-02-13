#include "listmaker.h"
#include <QCoreApplication>
ListMaker::ListMaker()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
}

ListMaker::ListMaker(QString fileName)
{
    read_list(fileName);
}

///read list from file
void ListMaker::read_list(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    playList=QJsonDocument::fromJson(QByteArray::fromPercentEncoding(file.readAll())).array();
    file.close();
}

///get sub-list's index by parent
QVector<int> ListMaker::get_list(int level, QString parent)
{
    QVector<int> list;

    for(int i=0; i<playList.size(); i++)
    {
        QJsonObject object = playList[i].toObject();
        bool flag = (object["level"].toInt()==level);

        switch(level)
        {
        case 0:
            break;
        case 1:
            flag = flag&&(object["parent0"].toString()==parent || parent=="-1");
            break;
        case 2:
            flag = flag&&(object["parent1"].toString()==parent || parent=="-2");
        default:
            break;
        }

        if(flag)
            list.push_back(i);
    }

    return list;
}

///return object by index
QJsonObject ListMaker::object(int index)
{
    return playList[index].toObject();
}

///return object by code
QJsonObject ListMaker::object(QString code)
{
    for(int i=0; i<playList.size(); i++)
    {
        if(playList[i].toObject()["code"].toString()==code)
            return playList[i].toObject();
    }
    return QJsonObject();
}

///write list to file
void ListMaker::save_list()
{
    QFile file("./config/list.jel");
    file.open(QIODevice::WriteOnly);
    QByteArray data=QString(QJsonDocument(playList).toJson()).toUtf8().toPercentEncoding();
    file.write(data);
    file.close();
}

///change name of an object
void ListMaker::list_edit(QString code, QString name)
{
    for(int i=0; i<playList.size(); i++)
    {
        if(playList[i].toObject()["code"].toString()==code)
        {
            QJsonObject object=playList[i].toObject();
            object["name"]=name;
            playList.replace(i,object);
            break;
        }
    }
    save_list();
}

///remove an object from list
void ListMaker::list_remove(QString code)
{
    for(int i=0; i<playList.size(); i++)
    {
        if(playList[i].toObject()["code"]==code)
        {
            playList.removeAt(i);
            break;
        }
    }
    save_list();
}
