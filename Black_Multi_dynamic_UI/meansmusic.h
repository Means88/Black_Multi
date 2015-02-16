#ifndef MEANSMUSIC_H
#define MEANSMUSIC_H

#include <QString>
#include <QVector>

/**
* @brief Note
* prefix:
* u:#   d:()    t:[]
*/
namespace NoteSpace
{
    enum Note
    {
        NoteError,
        ddDo,dduDo,ddRe,dduRe,ddMi,ddFa,dduFa,ddSo,dduSo,ddLa,dduLa,ddSi,
        dDo,duDo,dRe,duRe,dMi,dFa,duFa,dSo,duSo,dLa,duLa,dSi,
        Do,uDo,Re,uRe,Mi,Fa,uFa,So,uSo,La,uLa,Si,
        tDo,tuDo,tRe,tuRe,tMi,tFa,tuFa,tSo,tuSo,tLa,tuLa,tSi,
        ttDo,ttuDo,ttRe,ttuRe,ttMi,ttFa,ttuFa,ttSo,ttuSo,ttLa,ttuLa,ttSi
    };
    const int NoteErrorCode=233;

}

NoteSpace::Note operator +(NoteSpace::Note note,int i);
NoteSpace::Note operator -(NoteSpace::Note note,int i);
int operator -(NoteSpace::Note a,NoteSpace::Note b);
int note_msg(NoteSpace::Note note);

enum BDState{Blow,Draw};



///-----    NoteClass   ----------
class NoteClass
{
public:
    NoteClass(const NoteClass * const src);
    NoteClass(QString s=QString(""),NoteSpace::Note note=NoteSpace::NoteError);

    NoteClass operator +(int i);
    NoteClass operator -(int i);

    static NoteClass note_from_bd(bool isPush,BDState state,int ord);
    static NoteClass note_from_simple(bool isPush,int shell,int core);

    QString turn_bd_note(bool flag1, bool flag2, bool flag0);     //true: use B4/B8 instead of B5/B9
    QString turn_simple_note(bool flag1,bool flag2);
        //flag1: true: use #3   instead of 4
        //flag2: true: use (#7) instead of 1

private:
    NoteSpace::Note note;
    QString text;
};




///-----    MeansMusic  ----------
class MeansMusic
{
public:
    MeansMusic();
    void set_bd_music(QString src);
    void set_simple_music(QString src);
    QString to_bd_music(bool flag1, bool flag2, bool flag0);
    QString to_simple_music(bool flag1,bool flag2);
    void change_tonality(NoteSpace::Note src,NoteSpace::Note dst);
    void change_tonality(int src,int dst);
    void inc_tonality();
    void dec_tonality();

    QString test();

private:
    QVector<NoteClass> data;

};

#endif // MEANSMUSIC_H
