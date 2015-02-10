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

void ListMaker::read_list(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    playList=QJsonDocument::fromJson(QByteArray::fromPercentEncoding(file.readAll())).array();
    file.close();
}

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
            flag = flag&&(object["parent0"].toString()==parent);
            break;
        case 2:
            flag = flag&&(object["parent1"].toString()==parent);
        default:
            break;
        }

        if(flag)
            list.push_back(i);
    }

    return list;
}

QJsonObject ListMaker::object(int index)
{
    return playList[index].toObject();
}

QJsonObject ListMaker::object(QString code)
{
    for(int i=0; i<playList.size(); i++)
    {
        if(playList[i].toObject()["code"].toString()==code)
            return playList[i].toObject();
    }
    return QJsonObject();
}
