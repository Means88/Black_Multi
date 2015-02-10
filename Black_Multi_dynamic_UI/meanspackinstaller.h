#ifndef MEANSPACKINSTALLER_H
#define MEANSPACKINSTALLER_H

#include <QCoreApplication>
#include <QString>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QVariantList>
#include <QVariantMap>
#include <QProcess>

class MeansPackInstaller
{
public:
    MeansPackInstaller(QString src);
    MeansPackInstaller(char *src);

    bool setfile(QString fileName);
    bool unpack();
    bool install();
private:
    QString packName;

};

#endif // MEANSPACKINSTALLER_H
