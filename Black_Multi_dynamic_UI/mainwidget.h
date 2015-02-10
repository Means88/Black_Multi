/*!
 * Connect other widgets
 */

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "imglabel.h"
#include "imgindex.h"
#include "musicwidget.h"
#include "videowidget.h"
#include "detailcontainer.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    ImgLabel *imgLabel;
    ImgIndex *imgIndex;
    MusicWidget *musicWidget;
    VideoWidget *videoWidget;
    DetailContainer *detailContainer;

    void connect_objects();

signals:
    void refresh();

private slots:
    void show_widget();
    void raise_widget();

    void switch_list();
    void switch_lyric();

    void set_imglabel();
    void refresh_object();
};

#endif // MAINWIDGET_H
