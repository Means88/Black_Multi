/*!
 *Get the list of pictures
 */

#ifndef LISTMAKER_H
#define LISTMAKER_H

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QMap>
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <map>
#include <algorithm>
#include <iostream>
class ListMaker
{
public:
    ListMaker();
    ListMaker(QString fileName);

    void read_list(QString fileName);
    QVector<int> get_list(int level,QString parent=QString());
    QJsonObject object(int index);
    QJsonObject object(QString code);
private:
    QJsonArray playList;
};

#endif // LISTMAKER_H
