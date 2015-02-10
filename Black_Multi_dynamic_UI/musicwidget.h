/*!
 * Control the music
 */

#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include "listmaker.h"
#include "meansdrager.h"
#include "meansbutton.h"
#include "meanswaring.h"
#include <QButtonGroup>
#include <QMediaPlayer>
#include <QPushButton>
#include <QEvent>
#include <QSlider>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QJsonObject>

class MusicInterface;
class MusicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicWidget(QWidget *parent = 0);
    void set_dragable(bool);
    void set_object(QWidget*);
    void set_object(QList<QWidget*>);
    void add_object(QWidget*);
    void add_object(QList<QWidget*>);
    void del_object(QWidget*);

    MusicInterface *interface();
protected:
    void changeEvent(QEvent *event);

private:
    ListMaker listMaker;

    int px,py;
    MeansDrager *MainWidget;
    QMediaPlayer *mPlayer;
    QString fileDir,fileName[3];
    MeansButton *PlayButton,*StopButton,*VoiceButton,*FullScreenButton,*BackButton;
    MeansButton *MinimizeButton,*MinimodeButton;
    MeansCloseButton *CloseButton;
    MeansButton *PrevButton,*NextButton;
    MeansButton *ListButton,*LyricButton;
    bool ismute;
    MeansSlider *VolumeSlider,*SeekSlider;
    QLabel *MessageLabel;
    QLabel *TimeNum[5];

    ///button icons
    QIcon start_normal,start_hover,start_press;
    QIcon pause_normal,pause_hover,pause_press;
    QIcon stop_normal,stop_hover,stop_press;
    QIcon list_normal,list_hover,list_press,list_disabled;
    QIcon minimize_normal,minimize_hover,minimize_press;
    QIcon minimode_normal,minimode_hover,minimode_press;
    QIcon prev_normal,prev_hover,prev_press;
    QIcon next_normal,next_hover,next_press;
    QIcon lyric_normal,lyric_hover,lyric_press,lyric_disabled;
    QIcon video_normal,video_hover,fullscreen_normal,fullscreen_hover,back_normal,back_hover;
    QIcon volume_normal,volume_hover,mute_normal,mute_hover;

    QString musicName[3];
    int currentMediaId;

    bool isbegin; ///prefix
    bool musicSwitch;///switch the music
    QMediaPlayer *prePlayer;

    void refresh_message();
    void init_timenum();
    void init_others();
    void init_icon();
    void init_slider();
    void init_button();
        void init_playbutton();
        void init_stopbutton();
        void init_voicebutton();
        void init_fullscreenbutton();
        void init_backbutton();
        void init_closebutton();
        void init_listbutton();
        void init_lyricbutton();
        void init_minimizebutton();
        void init_minimodebutton();
        void init_prevbutton();
        void init_nextbutton();

    bool listState,lyricState;

signals:
    void change_state(QMediaPlayer::State);
    void set_position(int);
    void full_screen(bool);
    void clicked();
    void pressed();

    void switch_list();
    void switch_lyric();
    void window_state_change(Qt::WindowStates);

public slots:
    void init_geometry();
    void set_file_name(QString code);
    bool set_media_name(int id);

    void list_off();
    void lyric_off();

    void refresh_list();
    void play_music();

private slots:

    void seek_set_range(qint64 duration);
    void seek_set_value(qint64 position);
    void refresh_time();
    void player_set_position();
    void pre_state_changed(QMediaPlayer::State);
    void m_state_changed(QMediaPlayer::State);
    void prefix_stopped(QMediaPlayer::State);
    void volume_changed(int);

/// p=play s=stop v=volume c=close l=list m=minimize mini=minimode
    void click_p_button();
    void enter_p_button();
    void leave_p_button();
    void press_p_button();
    void release_p_button();

    void click_s_button();
    void enter_s_button();
    void leave_s_button();
    void press_s_button();
    void release_s_button();

    void click_v_button();
    void enter_v_button();
    void leave_v_button();

    void click_l_button();
    void enter_l_button();
    void leave_l_button();
    void press_l_button();
    void release_l_button();

    void click_lyric_button();
    void enter_lyric_button();
    void leave_lyric_button();
    void press_lyric_button();
    void release_lyric_button();

    void click_m_button();
    void enter_m_button();
    void leave_m_button();
    void press_m_button();
    void release_m_button();

    void click_mini_button();
    void enter_mini_button();
    void leave_mini_button();
    void press_mini_button();
    void release_mini_button();

    void click_prev_button();
    void enter_prev_button();
    void leave_prev_button();
    void press_prev_button();
    void release_prev_button();

    void click_next_button();
    void enter_next_button();
    void leave_next_button();
    void press_next_button();
    void release_next_button();

    void click_full_button();
    void enter_full_button();
    void leave_full_button();

    void enter_back_button();
    void leave_back_button();
};

///interface
class MusicInterface : private MusicWidget
{
    Q_OBJECT
public:
    explicit MusicInterface(QWidget *parent = 0);

    QRect geometry();
    QPoint pos();
    int x();
    int y();
    QSize size();
    int width();
    int height();

    void set_object(QWidget*);
    void set_object(QList<QWidget*>);
    void add_object(QWidget*);
    void add_object(QList<QWidget*>);
    void del_object(QWidget*);
};


#endif // MUSICWIDGET_H
