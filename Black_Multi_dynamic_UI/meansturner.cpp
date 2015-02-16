#include "meansturner.h"

MeansTurner::MeansTurner(QWidget *parent) :
    QWidget(parent)
{
    resize(456,265);
    music=new MeansMusic;
    type=Simple;
    label=new QLabel(">>>>>>>>>>>>",this);
    label->setGeometry(192,60,75,20);
    init_in();
    init_out();
    init_mode();
}

MeansTurner::~MeansTurner()
{

}

void MeansTurner::init_in()
{
    QStringList items;
    const QString itemArray[]={QString("A"),QString("#A"),QString("B"),QString("C"),QString("#C"),QString("D"),
                               QString("#D"),QString("E"),QString("F"),QString("#F"),QString("G"),QString("#G")};
    for(int i=0;i<12;i++)
        items.push_back(itemArray[i]);

    InText=new QTextEdit(this);
    InText->setGeometry(2,65,185,190);
    InCombo=new QComboBox(this);
    InCombo->setGeometry(152,12,50,20);
    InCombo->addItems(items);
    InCombo->setCurrentIndex(3);

    InSimpleRadio=new QRadioButton("简谱",this);
    InBdRadio=new QRadioButton("BD谱",this);
    InGroup=new QButtonGroup(this);
    InGroup->addButton(InSimpleRadio,0);
    InGroup->addButton(InBdRadio,1);
    InSimpleRadio->setChecked(true);
    InSimpleRadio->move(52,12);
    InBdRadio->move(52,32);
}

void MeansTurner::init_out()
{
    QStringList items;
    const QString itemArray[]={QString("A"),QString("#A"),QString("B"),QString("C"),QString("#C"),QString("D"),
                               QString("#D"),QString("E"),QString("F"),QString("#F"),QString("G"),QString("#G")};
    for(int i=0;i<12;i++)
        items.push_back(itemArray[i]);

    OutText=new QTextEdit(this);
    OutText->setGeometry(269,65,185,190);
    OutCombo=new QComboBox(this);
    OutCombo->setGeometry(254,12,50,20);
    OutCombo->addItems(items);
    OutCombo->setCurrentIndex(3);

    OutSimpleRadio=new QRadioButton("简谱",this);
    OutBdRadio=new QRadioButton("BD谱",this);
    OutGroup=new QButtonGroup(this);
    OutGroup->addButton(OutSimpleRadio,0);
    OutGroup->addButton(OutBdRadio,1);
    OutSimpleRadio->setChecked(true);
    OutSimpleRadio->move(350,12);
    OutBdRadio->move(350,32);
    connect(OutGroup,SIGNAL(buttonClicked(int)),this,SLOT(change_out_mode(int)));
}

void MeansTurner::init_mode()
{
    Check0=new QCheckBox("#3代替4",this);
    Check1=new QCheckBox("#7代替1",this);
    Check2=new QCheckBox("B4代替B5",this);
    Check0->move(192,108);
    Check1->move(192,128);
    Check2->move(192,88);
    Check2->hide();

    IncButton=new QPushButton("升半音",this);
    DecButton=new QPushButton("降半音",this);
    TurnButton=new QPushButton("直接转调",this);
    IncButton->setGeometry(192,148,75,20);
    DecButton->setGeometry(192,170,75,20);
    TurnButton->setGeometry(192,192,75,20);
    connect(IncButton,SIGNAL(clicked()),this,SLOT(click_inc()));
    connect(DecButton,SIGNAL(clicked()),this,SLOT(click_dec()));
    connect(TurnButton,SIGNAL(clicked()),this,SLOT(click_turn()));
}

void MeansTurner::read_music(MODE mode)
{
    if(mode==TurnMode)
    {
        if(InGroup->checkedId()==0)
            music->set_simple_music(InText->toPlainText());
        else
            music->set_bd_music(InText->toPlainText());
    }
    else
    {
        if(OutText->toPlainText()==QString())
            read_music(TurnMode);
        else
        {
            if(type==Simple)
                music->set_simple_music(OutText->toPlainText());
            else
                music->set_bd_music(OutText->toPlainText());
        }
    }
}

void MeansTurner::write_music()
{
    if(OutGroup->checkedId()==0)
        OutText->setText(music->to_simple_music(Check0->isChecked(),Check1->isChecked()));
    else
        OutText->setText(music->to_bd_music(Check0->isChecked(),Check1->isChecked(),Check2->isChecked()));

    if(OutGroup->checkedId()==0)
        type=Simple;
    else
        type=Bd;
}

void MeansTurner::change_out_mode(int id)
{
    if(id==0)
        Check2->hide();
    else
        Check2->show();
}

void MeansTurner::click_inc()
{
    read_music(OtherMode);
    music->inc_tonality();
    write_music();
}

void MeansTurner::click_dec()
{
    read_music(OtherMode);
    music->dec_tonality();
    write_music();
}

void MeansTurner::click_turn()
{
    read_music(TurnMode);
    music->change_tonality(InCombo->currentIndex()+1,OutCombo->currentIndex()+1);
    write_music();
}

