/**
  *  The Widget adsopent automatically
  */
#ifndef MEANSADSORPENT_H
#define MEANSADSORPENT_H

#include <QWidget>
#include "meansdrager.h"
#include "musicwidget.h"

class MeansAdsorpent : public QWidget
{
    Q_OBJECT
public:
    enum Mode{NoMode=0,TopMode=1,BottomMode=2,LeftMode=4,RightMode=8};

    explicit MeansAdsorpent(QWidget *parent = 0);
    ~MeansAdsorpent();

    void set_music(MusicInterface*);
    void set_mode(Mode mode);
    Mode mode();
    void adsopent(Mode mode);
protected:
    MeansDrager *MainWidget;

private:
    MusicInterface *musicWidget;
    Mode thismode;
    bool isDraging;

    Mode isNear();
signals:
    void clicked();
    void pressed();

private slots:
    void mouse_press();
    void mouse_move();
    void mouse_release();
};

#endif // MEANSADSORPENT_H
