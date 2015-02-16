#include "meansmusic.h"

using namespace NoteSpace;
MeansMusic::MeansMusic()
{
}

void MeansMusic::set_bd_music(QString src)
{
    int length=src.size();
    bool isPush=false;
    int ord=0;

    data.clear();
    for(int i=0;i<length;i++)
    {
        if(src[i]==L'（' || src[i]==L'）' || src[i]=='(' || src[i]==')')
        {
            isPush=!isPush;
        }
        else if(src[i]=='b' || src[i]=='B')
        {
            data.push_back(NoteClass::note_from_bd(isPush,Blow,ord));
            ord=0;
        }
        else if(src[i]=='d' || src[i]=='D')
        {
            data.push_back(NoteClass::note_from_bd(isPush,Draw,ord));
            ord=0;
        }
        else if(src[i].isDigit())
        {
            ord=ord*10+(src[i].toLatin1()-'0');
        }
        else
        {
            QString text;
            char temp[20];
            if(ord!=0)
            {
                text=itoa(ord,temp,10);
            }
            text+=src[i];
            data.push_back(NoteClass(text));
        }
    }
}

void MeansMusic::set_simple_music(QString src)
{
    int length=src.size();
    int shell=2;

    data.clear();
    for(int i=0;i<length;i++)
    {
        if(src[i]=='(' || src[i]==']' || src[i]==L'（' || src[i]==L'】')
        {
            shell--;
        }
        else if(src[i]==')' || src[i]=='[' || src[i]==L'）' || src[i]==L'【')
        {
            shell++;
        }
        else if(src[i]=='#')
        {
            if(++i<length)
            {
                int core=src[i].toLatin1()-'0';
                data.push_back(NoteClass::note_from_simple(true,shell,core));
            }
            else
            {
                data.push_back(NoteClass("#"));
            }
        }
        else if(src[i].isDigit())
        {
            int core=src[i].toLatin1()-'0';
            data.push_back(NoteClass::note_from_simple(false,shell,core));
        }
        else
        {
            data.push_back(NoteClass(QString(src[i])));
        }
    }
}

QString MeansMusic::to_bd_music(bool flag1, bool flag2, bool flag0)
{
    QString result;
    for(int i=0;i<data.size();i++)
    {
        result+=data[i].turn_bd_note(flag1,flag2,flag0);
    }
    return result;
}

QString MeansMusic::to_simple_music(bool flag1, bool flag2)
{
    QString result;
    for(int i=0;i<data.size();i++)
    {
        result+=data[i].turn_simple_note(flag1,flag2);
    }
    return result;
}

void MeansMusic::change_tonality(Note src, Note dst)
{
    change_tonality(int(src),int(dst));
}

void MeansMusic::change_tonality(int src, int dst)
{
    int diff=src-dst;
    if(diff!=NoteErrorCode)
    {
        for(int i=0;i<data.size();i++)
        {
            data[i]=data[i]+diff;
        }
    }
}

void MeansMusic::dec_tonality()
{
    for(int i=0;i<data.size();i++)
    {
        data[i]=data[i]-1;
    }
}

void MeansMusic::inc_tonality()
{
    for(int i=0;i<data.size();i++)
    {
        data[i]=data[i]+1;
    }
}

QString MeansMusic::test()
{
    return QString();
}




///Note
int note_msg(Note note)
{
    const int msg[]=
    {
        -1,
        0,1,2,3,4,6,7,8,9,10,11,14,
        16,17,18,19,20,22,23,24,25,26,27,30,
        32,33,34,35,36,38,39,40,41,42,43,46,
        48,49,50,51,52,54,55,56,57,58,59,62,
        64,65,66,67,68,70,71,72,73,74,75,78
    };
    return msg[int(note)];
}

Note operator +(Note note,int i)
{
    int result=int(note)+i;
    if(note==NoteError || result<=int(NoteError) || result>int(ttSi))
        return NoteError;
    else
        return Note(result);
}

Note operator -(Note note,int i)
{
    int result=int(note)-i;
    if(note==NoteError || result<=int(NoteError) || result>int(ttSi))
        return NoteError;
    else
        return Note(result);
}

int operator -(Note a,Note b)
{
    if(a==NoteError || b==NoteError)
        return NoteErrorCode;
    else
        return int(a)-int(b);
}




///NoteClass
NoteClass::NoteClass(const NoteClass * const src)
{
    this->note=src->note;
    this->text=src->text;
}

NoteClass::NoteClass(QString s, Note note)
{
    this->note=note;
    this->text=s;
}

NoteClass NoteClass::operator +(int i)
{
    NoteClass result(this);
    result.note=this->note+i;
    return result;
}

NoteClass NoteClass::operator -(int i)
{
    NoteClass result(this);
    result.note=this->note-i;
    return result;
}

NoteClass NoteClass::note_from_bd(bool isPush, BDState state, int ord)
{
    NoteClass result;
    Note note;

    if(state==Blow)
    {
        switch(ord)
        {
        case 1: note=dDo; break;
        case 2: note=dMi; break;
        case 3: note=dSo; break;
        case 4:
        case 5: note=Do;  break;
        case 6: note=Mi;  break;
        case 7: note=So;  break;
        case 8:
        case 9: note=tDo; break;
        case 10:note=tMi; break;
        case 11:note=tSo; break;
        case 12:note=ttDo;break;
        default:note=NoteError;
        }
    }
    else
    {
        switch(ord)
        {
        case 1: note=dRe; break;
        case 2: note=dFa; break;
        case 3: note=dLa; break;
        case 4: note=dSi; break;
        case 5: note=Re;  break;
        case 6: note=Fa;  break;
        case 7: note=La;  break;
        case 8: note=Si;  break;
        case 9: note=tRe; break;
        case 10:note=tFa; break;
        case 11:note=tLa; break;
        case 12:note=tSi; break;
        default:note=NoteError;
        }
    }

    result.note=note;
    if(note==NoteError)
    {
        result.note=NoteError;
        result.text="*";
    }
    else if(isPush)
    {
        if(note==tSi)
            result.note=ttuRe;
        else
            result.note=note+1;
    }
    return result;
}

NoteClass NoteClass::note_from_simple(bool isPush, int shell, int core)
{
    NoteClass result;
    const int note[]={-1,1,3,5,6,8,10,12};

    if(core<1 || core>7)
    {
        result.note=NoteError;
        result.text="*";
    }
    else
    {
        result.note=Note(note[core]+12*shell);
        if(isPush)
            result.note=result.note+1;
    }
    return result;
}


QString NoteClass::turn_bd_note(bool flag1, bool flag2, bool flag0)
{
    if(note!=NoteError)
    {
        int ord;
        bool isPush;
        BDState state;

        ord=note_msg(note)/4+1-4;
        isPush=note_msg(note)&1;
        if(note_msg(note)%4<2)
            state=Blow;
        else
            state=Draw;

        if(flag1 && !isPush && state==Draw &&(ord==2 || ord==6 || ord==10))
        {
            state=Blow;
            isPush=true;
        }
        else if(!isPush && state==Blow &&(ord==5 || ord==9))
        {
            if(flag2)
            {
                state=Draw;
                isPush=true;
                ord--;
            }
            else if(flag0)
            {
                ord--;
            }
        }

        QString result;

        char num[3];
        result+=itoa(ord,num,10);
        if(state==Blow)
        {
            result+="B";
        }
        else
        {
            result+="D";
        }
        if(isPush)
        {
            result='('+result+')';
        }

        return result;
    }
    else
        return text;
}

QString NoteClass::turn_simple_note(bool flag1, bool flag2)
{
    if(note!=NoteError)
    {
        int shell=int(note-1)/12-2;
        int core=int(note-1)%12;

        QString simpleNote[]={"1","#1","2","#2","3","4","#4","5","#5","6","#6","7"};
        if(flag1)
            simpleNote[5]="#3";
        if(flag2)
        {
            shell--;
            simpleNote[0]="#7";
        }

        QString result;

        if(shell>0)
        {
            for(int i=0;i<shell;i++)
                result+='[';
        }
        else if(shell<0)
        {
            for(int i=0;i>shell;i--)
                result+='(';
        }

        result+=simpleNote[core];

        if(shell>0)
        {
            for(int i=0;i<shell;i++)
                result+=']';
        }
        else if(shell<0)
        {
            for(int i=0;i>shell;i--)
                result+=')';
        }

        return result;
    }
    else
        return text;
}
