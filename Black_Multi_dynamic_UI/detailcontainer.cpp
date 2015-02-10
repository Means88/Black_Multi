#include "detailcontainer.h"
#include <QFile>
#include <QPalette>
#include <QBitmap>
DetailContainer::DetailContainer(QWidget *parent) :
    MeansAdsorpent(parent)
{
    resize(476,300);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    //setAttribute(Qt::WA_TranslucentBackground);

    init_mainwidget();
    init_tabwidget();
    init_imgtab();
    init_videotab();
    init_thirdtab();
    init_closebutton();

    QFile style("./styles/detailcontainer.qss");
    style.open(QIODevice::ReadOnly);
    setStyleSheet(style.readAll());

    QPixmap pix;
    pix.load("./src/ui/detail.png",0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    setMask(QBitmap(pix.mask()));

    setAutoFillBackground(false);
    set_mode(RightMode);
}

void DetailContainer::init_mainwidget()
{
    MainWidget->resize(476,300);
    MainWidget->setObjectName("MainWidget");
}

void DetailContainer::init_tabwidget()
{
    TabWidget=new QTabWidget(MainWidget);
    TabWidget->move(11,5);
    TabWidget->resize(456,270);
}

void DetailContainer::init_imgtab()
{
    ImgTab=new QLabel(TabWidget);
    ImgTab->resize(456,265);
    imgLabel=new ImgLabel(ImgTab);
    imgLabel->setWindowFlags(Qt::Widget);
    imgLabel->setGeometry(140,37,140,190);
    imgLabel->set_rollable(false);
    imgLabel->set_dragable(false);
    connect(imgLabel,SIGNAL(img_show()),ImgTab,SLOT(update()));
    TabWidget->addTab(ImgTab,"图片");
}

void DetailContainer::init_videotab()
{
    VideoTab=new QLabel(TabWidget);
    VideoTab->resize(456,265);
    TabWidget->addTab(VideoTab,"视频");
}

void DetailContainer::init_thirdtab()
{
    ThirdTab=new QLabel(TabWidget);
    TabWidget->addTab(ThirdTab,"");
}

void DetailContainer::init_closebutton()
{
    CloseButton=new MeansCloseButton(MainWidget);
    CloseButton->setGeometry(458,4,14,14);
    CloseButton->set_close_parent(this);
    connect(CloseButton,SIGNAL(closed()),this,SIGNAL(switch_off()));
}

///slots

