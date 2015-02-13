#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QCheckBox>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QProcess>
#include "meanslineedit.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void dropEvent(QDropEvent *event);

private slots:
    void on_OkButton_clicked();

    void on_LevelBox_currentIndexChanged(int index);

    void on_LoadButton_clicked();

    void on_MusicCheck_stateChanged(int arg1);

    void on_VideoCheck_stateChanged(int arg1);

private:
    Ui::Widget *ui;

    QString get_md5code(QString dir);
    QString hex_to_36(QString);
    void load(QString fileName);
    void check();
};

#endif // WIDGET_H
