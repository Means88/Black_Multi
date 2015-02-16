#ifndef MEANSTURNER_H
#define MEANSTURNER_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include "meansmusic.h"

class MeansTurner : public QWidget
{
    Q_OBJECT
public:
    explicit MeansTurner(QWidget *parent = 0);
    virtual ~MeansTurner();

private:
    enum MusicType{Simple,Bd};
    enum MODE{TurnMode,OtherMode};

    MeansMusic *music;
    QTextEdit *InText,*OutText;
    QComboBox *InCombo,*OutCombo;
    QCheckBox *Check0,*Check1,*Check2;
    QRadioButton *InSimpleRadio,*InBdRadio,*OutSimpleRadio,*OutBdRadio;
    QButtonGroup *InGroup,*OutGroup;
    QPushButton *IncButton,*DecButton,*TurnButton;
    QLabel *label;
    MusicType type;

    void init_in();
    void init_out();
    void init_mode();
    void read_music(MODE mode);
    void write_music();

signals:

private slots:
    void change_out_mode(int id);
    void click_inc();
    void click_dec();
    void click_turn();

};

#endif // MEANSTURNER_H
