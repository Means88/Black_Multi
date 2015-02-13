#include "imgindex.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFile>
#include <iostream>
#include <string>
ImgIndex::ImgIndex(QWidget *parent) :
    MeansAdsorpent(parent)
{
    create_object();
    init_object();
    set_mode(BottomMode);
}

void ImgIndex::set_object(QWidget *object)
{
    MainWidget->set_object(object);
}

void ImgIndex::set_object(QList<QWidget *> list)
{
    MainWidget->set_object(list);
}

///private
void ImgIndex::create_object()
{
    treeTitle=new QLabel(MainWidget);
    CloseButton=new MeansCloseButton(MainWidget);
    MainListWidget=new MeansListWidget(MainWidget);
    Level1ListWidget=new MeansListWidget(MainWidget);
    Level2ListWidget=new MeansListWidget(MainWidget);
    MainListWidget->setGeometry(9,46,51,91);
    Level1ListWidget->setGeometry(70,46,90,91);
    Level2ListWidget->setGeometry(160,46,166,91);

    connect(MainListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(load_level1(QListWidgetItem*)));
    connect(Level1ListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(load_level2(QListWidgetItem*)));
    connect(Level2ListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(click_img(QListWidgetItem*)),Qt::DirectConnection);
    connect(Level2ListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(double_click_img(QListWidgetItem*)),Qt::DirectConnection);
    connect(Level1ListWidget,SIGNAL(edit(QListWidgetItem*)),this,SLOT(edit_item(QListWidgetItem*)));
    connect(Level2ListWidget,SIGNAL(edit(QListWidgetItem*)),this,SLOT(edit_item(QListWidgetItem*)));
    connect(Level1ListWidget,SIGNAL(remove(QListWidgetItem*)),this,SLOT(remove_item(QListWidgetItem*)));
    connect(Level2ListWidget,SIGNAL(remove(QListWidgetItem*)),this,SLOT(remove_item(QListWidgetItem*)));

    Splitter1=new QLabel(MainWidget);
    Splitter1->setGeometry(63,46,7,91);
    Splitter1->setPixmap(QPixmap("./src/ui/splitter.png"));
    Splitter2=new QLabel(MainWidget);
    Splitter2->setGeometry(153,46,7,91);
    Splitter2->setPixmap(QPixmap("./src/ui/splitter.png"));
}

void ImgIndex::init_object()
{
    init_this();
    init_main();
    init_title();
    init_list();
    init_time();
}

void ImgIndex::init_this()
{
    resize(345,150);
    //move((w-tempw)/2-210,(h-height())/2);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    setAttribute(Qt::WA_TranslucentBackground);
    QFile style("./styles/imgindex.qss");
    style.open(QIODevice::ReadOnly);
    setStyleSheet(QString(style.readAll()));
}

void ImgIndex::init_title()
{
    treeTitle->setGeometry(0,0,160,50);
    treeTitle->setObjectName("treeTitle");
    treeTitle->setText("  (*´○`)o¶♪");
    treeTitle->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    treeTitle->setMargin(3);
    connect(treeTitle,SIGNAL(clicked()),this,SIGNAL(clicked()));
    connect(treeTitle,SIGNAL(pressed()),this,SIGNAL(pressed()));

    CloseButton->setGeometry(327,5,14,14);
    CloseButton->set_close_parent(this);
    connect(CloseButton,SIGNAL(closed()),this,SIGNAL(switch_off()));
}

void ImgIndex::init_main()
{
    MainWidget->setGeometry(0,0,345,150);
    MainWidget->setObjectName("mainWidget");

/*
 *  int y=51;
    mainShow=true;
    mainState=TreeState;

    mainShowAnime=new QPropertyAnimation(mainWidget,"size");
    mainShowAnime->setDuration(800);
    mainShowAnime->setStartValue(QSize(320,0));

    mainShowAnime->setKeyValueAt(0.28089,QSize(320,280));
    mainShowAnime->setKeyValueAt(0.42135,QSize(320,210));
    mainShowAnime->setKeyValueAt(0.56180,QSize(320,280));
    mainShowAnime->setKeyValueAt(0.661095,QSize(320,245));
    mainShowAnime->setKeyValueAt(0.76039,QSize(320,280));
    mainShowAnime->setKeyValueAt(0.83062,QSize(320,262));
    mainShowAnime->setKeyValueAt(0.90084,QSize(320,280));
    mainShowAnime->setKeyValueAt(0.95035,QSize(320,271));

    mainShowAnime->setEndValue(QSize(320,280));

    mainHideAnime=new QPropertyAnimation(mainWidget,"size");
    mainHideAnime->setDuration(450);
    mainHideAnime->setStartValue(QSize(320,280));
    mainHideAnime->setKeyValueAt(0.3,QSize(320,250));
    mainHideAnime->setKeyValueAt(0.5,QSize(320,240));
    mainHideAnime->setEndValue(QSize(320,0));
    connect(mainHideAnime,SIGNAL(finished()),this,SLOT(update()));

    turnListAnime=new QPropertyAnimation(mainWidget,"pos");
    turnListAnime->setDuration(800);
    turnListAnime->setStartValue(QPoint(0,y));
    turnListAnime->setKeyValueAt(0.375,QPoint(40,y));
    turnListAnime->setKeyValueAt(0.625,QPoint(50,y));
    turnListAnime->setEndValue(QPoint(-160,y));
    connect(turnListAnime,SIGNAL(finished()),this,SLOT(update()));

    turnTreeAnime=new QPropertyAnimation(mainWidget,"pos");
    turnTreeAnime->setDuration(800);
    turnTreeAnime->setStartValue(QPoint(-160,y));
    turnTreeAnime->setKeyValueAt(0.375,QPoint(-200,y));
    turnTreeAnime->setKeyValueAt(0.625,QPoint(-210,y));
    turnTreeAnime->setEndValue(QPoint(0,y));
    connect(turnTreeAnime,SIGNAL(finished()),this,SLOT(update()));
*/
}

void ImgIndex::init_list()
{
    listMaker.read_list("./config/list.jel");
    MainListWidget->clear();
    Level1ListWidget->clear();
    Level2ListWidget->clear();

    QListWidgetItem *item=new QListWidgetItem("全部");
    item->setSizeHint(QSize(50,13));
    item->setStatusTip("-1");
    MainListWidget->addItem(item);

    QVector<int> list=listMaker.get_list(0);
    for(int i=0; i<list.size(); i++)
    {
        QJsonObject object=listMaker.object(list[i]);
        QListWidgetItem *item=new QListWidgetItem(object["name"].toString());
        item->setSizeHint(QSize(50,13));
        item->setStatusTip(object["code"].toString());
        MainListWidget->addItem(item);
    }
}

void ImgIndex::init_time()
{
    time=new MeansTimer(this);
    time->setSingleShot(true);
    connect(time,SIGNAL(timeout()),this,SLOT(time_out()),Qt::DirectConnection);
}

///slots

void ImgIndex::load_level1(QListWidgetItem *item)
{
    Level1ListWidget->clear();
    Level2ListWidget->clear();
    //treeTitle->setText(QJsonDocument(level1List).toJson());
    QListWidgetItem *itm=new QListWidgetItem("全部");
    itm->setSizeHint(QSize(83,13));
    itm->setStatusTip("-2");
    Level1ListWidget->addItem(itm);

    QVector<int> list = listMaker.get_list(1,item->statusTip());
    for(int i=0; i<list.size(); i++)
    {
        QJsonObject object=listMaker.object(list[i]);
        QListWidgetItem *itm=new QListWidgetItem(object["name"].toString());
        itm->setSizeHint(QSize(83,13));
        itm->setStatusTip(object["code"].toString());
        Level1ListWidget->addItem(itm);
    }
    emit select_img(item->statusTip(),false);
}

void ImgIndex::load_level2(QListWidgetItem *item)
{
    Level2ListWidget->clear();

    QVector<int> list = listMaker.get_list(2,item->statusTip());
    for(int i=0; i<list.size(); i++)
    {
        QJsonObject object=listMaker.object(list[i]);
        QString name=object["name"].toString();
        if(object["video"].toString().size())
            name = "[MAD] "+name;
        else if(object["music1"].toString().size() || object["music2"].toString().size() || object["music3"].toString().size())
            name = "♪ "+name;
        else
            name = " "+name;
        QListWidgetItem *itm=new QListWidgetItem(name);
        itm->setSizeHint(QSize(165,13));
        itm->setStatusTip(object["code"].toString());
        Level2ListWidget->addItem(itm);
    }
    emit select_img(item->statusTip(),false);
}

void ImgIndex::click_img(QListWidgetItem *item)
{
    if(item!=NULL)
    {
        time->set_message(item->statusTip());
        time->start(250);
    }
}

void ImgIndex::double_click_img(QListWidgetItem *item)
{
    time->stop();
    if(item!=NULL)
        emit select_img(item->statusTip(),true);
}

void ImgIndex::time_out()
{
    emit select_img(time->message(),false);
}

void ImgIndex::edit_item(QListWidgetItem *item)
{
    QInputDialog *dialog=new QInputDialog;
    if(item->listWidget()==Level2ListWidget)
        dialog->setTextValue(item->text().section(' ',1));
    else
        dialog->setTextValue(item->text());
    dialog->setLabelText("输入新名称");
    dialog->setOkButtonText("确定");
    dialog->setCancelButtonText("取消");

    if(dialog->exec()==QInputDialog::Accepted)
    {
        if(item->listWidget()==Level2ListWidget)
            item->setText(item->text().section(' ',0,0)+' '+dialog->textValue());
        else
            item->setText(dialog->textValue());
        listMaker.list_edit(item->statusTip(),dialog->textValue());
    }
}

void ImgIndex::remove_item(QListWidgetItem *item)
{
    QMessageBox *dialog=new QMessageBox;
    dialog->setIcon(QMessageBox::NoIcon);
    dialog->setText("确定要删除吗");
    dialog->addButton("确定",QMessageBox::AcceptRole);
    dialog->addButton("取消",QMessageBox::RejectRole);
    if(dialog->exec()==QMessageBox::AcceptRole)
    {
        listMaker.list_remove(item->statusTip());
        item->listWidget()->removeItemWidget(item);
        delete item;
    }

}
