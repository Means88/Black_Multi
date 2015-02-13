/*!
 * The index of the player
 */

#ifndef IMGINDEX_H
#define IMGINDEX_H

#include "meansdrager.h"
#include "meansbutton.h"
#include "musicwidget.h"
#include "meansadsorpent.h"
#include "listmaker.h"
#include <QWidget>
#include <QVector>
#include <QIcon>
#include <QTimer>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QInputDialog>

class ImgIndex : public MeansAdsorpent
{
    Q_OBJECT
public:
    enum State{TreeState,ListState};
    explicit ImgIndex(QWidget *parent = 0);

    void set_object(QWidget*);
    void set_object(QList<QWidget*>);
protected:

private:
    int px,py;
    QLabel *treeTitle;
    MeansListWidget *MainListWidget,*Level1ListWidget,*Level2ListWidget;
    QLabel *Splitter1,*Splitter2;
    State mainState;
    MeansCloseButton *CloseButton;
    ListMaker listMaker;
    MeansTimer *time;

    bool mainShow;

    void create_object();
    void init_object();
    void init_this();
    void init_title();
    void init_main();
    void init_time();

signals:
    void select_img(QString fileName, bool play);
    void switch_off();

public slots:
    void init_list();

private slots:
    void load_level1(QListWidgetItem *);
    void load_level2(QListWidgetItem *);
    void click_img(QListWidgetItem *);
    void double_click_img(QListWidgetItem *);
    void time_out();

    void edit_item(QListWidgetItem *);
    void remove_item(QListWidgetItem *);
};

#endif // IMGINDEX_H
