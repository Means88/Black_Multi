#ifndef DETAILCONTAINER_H
#define DETAILCONTAINER_H

#include <QTabWidget>
#include <QTabBar>
#include <QSizeGrip>
#include "meansdrager.h"
#include "meansbutton.h"
#include "meansadsorpent.h"
#include "meansturner.h"
#include "imglabel.h"

class DetailContainer : public MeansAdsorpent
{
    Q_OBJECT
public:
    explicit DetailContainer(QWidget *parent = 0);

    QTabWidget *TabWidget;
    QLabel *VideoTab,*TurnTab,*ImgTab;
    ImgLabel *imgLabel;
private:
    MeansCloseButton *CloseButton;
    MeansTurner *MusicTuner;
    //QSizeGrip *SizeGrip;

    void init_mainwidget();
    void init_tabwidget();
    void init_imgtab();
    void init_videotab();
    void init_turntab();
    void init_closebutton();

signals:
    void switch_off();

public slots:

};

#endif // DETAILCONTAINER_H
