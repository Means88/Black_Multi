/**
 * VideoPlayer
 */

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QJsonObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEngine>
#include <QPalette>
#include <QTimer>
#include "musicwidget.h"
#include "imgindex.h"
#include "listmaker.h"

class MeansVideoWidget;
class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(ImgIndex *imgIndex, MusicWidget *musicWidget, QWidget *parent = 0);
    virtual ~VideoWidget();

protected:

private:
    int px,py;
    QMediaPlayer *vPlayer;
    MeansVideoWidget *VideoOut;

    MusicWidget *musicWidget;
    ImgIndex *imgIndex;

    ListMaker listMaker;

    void set_friend(MusicWidget *,ImgIndex *);

signals:
    void click_back();
    void change_imgindex(QRect,QList<QWidget*>);
public slots:
    void change_state(QMediaPlayer::State);
    void set_media_position(int);
    void set_file_name(QString code);
    void raise_widget();
    void refresh_list();
};

///MeansVideoWidget
class MeansVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit MeansVideoWidget(QWidget *parent = 0);
    virtual ~MeansVideoWidget();

    void set_player(MusicWidget *);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
private:
    QTimer *time;

signals:
    void clicked();
    void pressed();

};

#endif // VIDEOWIDGET_H
