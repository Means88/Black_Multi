/*!
 *Where shows the pictures
 */

#ifndef IMGLABEL_H
#define IMGLABEL_H

#include "meansdrager.h"
#include "meanswaring.h"
#include "listmaker.h"
#include <QMovie>
#include <QPixmap>
#include <QImage>
#include <QTreeWidgetItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QJsonObject>

class ImgLabel : public MeansDrager
{
    Q_OBJECT
public:
    explicit ImgLabel(QWidget *parent = 0);

    void refresh();
    void set_rollable(bool);
protected:
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    int labelHeight,scaleRate;
    QMovie *dynamicImg;
    QString lastDir,lastImg;

    ListMaker listMaker;

    bool rollable;

    int parentHeight();
    void show_picture(QString dir,QString image);
    void show_static_picture(QString dir,QString fileName);
    void show_dynamic_picture(QString dir,QString fileName);

signals:
    void mouse_doubleclick();
    void img_show();

public slots:
    void show_img(QString code);
    void refresh_list();

private slots:
    void show_dynamic_slot();
};

#endif // IMGLABEL_H
