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
    void list_edit(QString code,QString name);
    void list_remove(QString code);

private:
    QJsonArray playList;

    void save_list();
};

#endif // LISTMAKER_H
