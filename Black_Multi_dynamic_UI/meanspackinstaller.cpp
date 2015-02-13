#include "meanspackinstaller.h"
#include <QMessageBox>
MeansPackInstaller::MeansPackInstaller(QString src)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    packName = src;
    unpack();
    install();
}

MeansPackInstaller::MeansPackInstaller(char *src)
{
    MeansPackInstaller(QString::fromLocal8Bit(src));
}

bool MeansPackInstaller::setfile(QString fileName)
{
    QFile file(fileName);
    if(file.exists())
    {
        packName = fileName;
        return true;
    }
    return false;
}

///  ./[x2jqlc6] W.je  -->  ./MultiData/[x2jqlc6] W/
bool MeansPackInstaller::unpack()
{   
    QFile file(QString(packName).replace('\\','/'));
    if(!file.exists())
        return false;

    QString dir = QString("./MultiData/")+packName.section('\\',-1,-1).section('.',0,0);

    if(!QDir(dir).exists())
        QDir::current().mkpath(dir);
    QString command = QString("7za e -y -aoa \"%1\" -o\".\\MultiData\\%2\"").arg(packName).arg(packName.section('\\',-1,-1).section('.',0,0));
    QProcess::execute(command);

    return true;
}

/// add infomation to list.jel
bool MeansPackInstaller::install()
{
    QFile listFile("./config/list.jel");
    if(!listFile.exists())
    {
        listFile.open(QIODevice::WriteOnly);
        listFile.write("[]");
        listFile.close();
    }
    listFile.open(QIODevice::ReadOnly);
    QJsonArray list=QJsonDocument::fromJson(QByteArray::fromPercentEncoding(listFile.readAll())).array();
    listFile.close();

    QFile packFile(QString("./MultiData/")+packName.section('\\',-1,-1).section('.',0,0)+"/imfo.jez");
    if(!packFile.exists())
    {
        return false;
    }
    packFile.open(QIODevice::ReadOnly);
    QJsonObject packMsg=QJsonDocument::fromJson(QByteArray::fromPercentEncoding(packFile.readAll())).object();
    packFile.close();

    for(int i=0; i<list.size(); i++)
    {
        if(list[i].toObject()["code"].toString()==packMsg["code"].toString())
            list.removeAt(i--);
    }

    list.push_back(packMsg);

    listFile.open(QIODevice::WriteOnly);
    QByteArray data=QString(QJsonDocument(list).toJson()).toUtf8().toPercentEncoding();
    listFile.write(data);
    listFile.close();

    return true;
}
