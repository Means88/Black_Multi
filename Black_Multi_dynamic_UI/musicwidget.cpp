#include "musicwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QTime>
MusicWidget::MusicWidget(QWidget *parent) :
    QWidget(parent)
{
    listMaker.read_list("./config/list.jel");
    int w=QApplication::desktop()->width(),h=QApplication::desktop()->height();
    int tempw=(h-100)*1240/1754;
    px=(w+tempw)/2+100;
    py=(h-170)/2;
    isbegin=true;
    listState=true;
    lyricState=true;
    musicSwitch=false;
    init_others();
    init_timenum();
    init_icon();
    init_slider();
    init_button();
    init_geometry();
}

MusicInterface *MusicWidget::interface()
{
    return (MusicInterface*)(this);
}

void MusicWidget::set_dragable(bool flag)
{
    MainWidget->set_dragable(flag);
}

void MusicWidget::set_object(QWidget *object)
{
    MainWidget->set_object(object);
}

void MusicWidget::set_object(QList<QWidget *> list)
{
    MainWidget->set_object(list);
}

void MusicWidget::add_object(QWidget *object)
{
    MainWidget->add_object(object);
}

void MusicWidget::add_object(QList<QWidget *> list)
{
    MainWidget->add_object(list);
}

void MusicWidget::del_object(QWidget *object)
{
    MainWidget->del_object(object);
}

///private
void MusicWidget::init_timenum()
{
    for(int i=0;i<5;i++)
    {
        TimeNum[i]=new QLabel(MainWidget);
        TimeNum[i]->setGeometry(153+14*i,87,14,18);
        if(i==2)
            TimeNum[i]->setPixmap(QPixmap("./src/number/maohao.png"));
        else
            TimeNum[i]->setPixmap(QPixmap("./src/number/0.png"));
    }
}

void MusicWidget::init_others()
{
    ///main
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QFile style("./styles/musicwidget.qss");
    style.open(QIODevice::ReadOnly);
    setStyleSheet(QString(style.readAll()));
    MainWidget=new MeansDrager(this);
    connect(MainWidget,SIGNAL(clicked()),this,SIGNAL(clicked()));
    connect(MainWidget,SIGNAL(pressed()),this,SIGNAL(pressed()));

    ///player
    mPlayer=new QMediaPlayer(this);
    mPlayer->setVolume(100);
    connect(mPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(m_state_changed(QMediaPlayer::State)));

    prePlayer=new QMediaPlayer(this);
    prePlayer->setVolume(100);
    prePlayer->setMedia(QMediaContent(QUrl::fromLocalFile("./src/musicprefix.mp3")));
    connect(prePlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(prefix_stopped(QMediaPlayer::State)));
    connect(prePlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(pre_state_changed(QMediaPlayer::State)));

    ///MessageLabel
    MessageLabel=new QLabel(MainWidget);
    MessageLabel->setObjectName("MessageLabel");
    refresh_message();
}

void MusicWidget::init_icon()
{
    start_normal=QIcon("./src/button/start.png");
    start_hover=QIcon("./src/button/start_hover.png");
    start_press=QIcon("./src/button/start_press.png");

    pause_normal=QIcon("./src/button/pause.png");
    pause_hover=QIcon("./src/button/pause_hover.png");
    pause_press=QIcon("./src/button/pause_press.png");

    stop_normal=QIcon("./src/button/stop.png");
    stop_hover=QIcon("./src/button/stop_hover.png");
    stop_press=QIcon("./src/button/stop_press.png");

    list_normal=QIcon("./src/button/list.png");
    list_hover=QIcon("./src/button/list_hover.png");
    list_press=QIcon("./src/button/list_press.png");
    list_disabled=QIcon("./src/button/list_disabled.png");

    lyric_normal=QIcon("./src/button/lyric.png");
    lyric_hover=QIcon("./src/button/lyric_hover.png");
    lyric_press=QIcon("./src/button/lyric_press.png");
    lyric_disabled=QIcon("./src/button/lyric_disabled.png");

    minimize_normal=QIcon("./src/button/minimize.png");
    minimize_hover=QIcon("./src/button/minimize_hover.png");
    minimize_press=QIcon("./src/button/minimize_press.png");

    minimode_normal=QIcon("./src/button/minimode.png");
    minimode_hover=QIcon("./src/button/minimode_hover.png");
    minimode_press=QIcon("./src/button/minimode_press.png");

    prev_normal=QIcon("./src/button/prev.png");
    prev_hover=QIcon("./src/button/prev_hover.png");
    prev_press=QIcon("./src/button/prev_press.png");

    next_normal=QIcon("./src/button/next.png");
    next_hover=QIcon("./src/button/next_hover.png");
    next_press=QIcon("./src/button/next_press.png");

    volume_normal=QIcon("./src/button/volume.png");
    volume_hover=QIcon("./src/button/volume_hover.png");
    mute_normal=QIcon("./src/button/mute.png");
    mute_hover=QIcon("./src/button/mute_hover.png");
    video_normal=QIcon("./src/button/video.png");
    video_hover=QIcon("./src/button/video_hover.png");
    fullscreen_normal=QIcon("./src/button/fullscreen.png");
    fullscreen_hover=QIcon("./src/button/fullscreen_hover.png");
    back_normal=QIcon("./src/button/back.png");
    back_hover=QIcon("./src/button/back_hover.png");
}

void MusicWidget::init_slider()
{
    SeekSlider=new MeansSlider(MainWidget);
    SeekSlider->setOrientation(Qt::Horizontal);
    SeekSlider->setObjectName("SeekSlider");
    SeekSlider->set_handle_width(14);
    connect(mPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(seek_set_range(qint64)));
    connect(mPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(seek_set_value(qint64)));
    connect(SeekSlider,SIGNAL(sliderMoved(int)),this,SLOT(player_set_position()));
    connect(SeekSlider,SIGNAL(select_value(qint64)),mPlayer,SLOT(setPosition(qint64)));
    connect(SeekSlider,SIGNAL(select_value(qint64)),this,SLOT(player_set_position()));

    VolumeSlider=new MeansSlider(MainWidget);
    VolumeSlider->setOrientation(Qt::Horizontal);
    VolumeSlider->setObjectName("VolumeSlider");
    VolumeSlider->setRange(0,100);
    VolumeSlider->setValue(100);
    connect(VolumeSlider,SIGNAL(valueChanged(int)),this,SLOT(volume_changed(int)));
}

void MusicWidget::init_button()
{
    init_playbutton();
    init_stopbutton();
    init_fullscreenbutton();
    init_backbutton();
    init_voicebutton();
    init_closebutton();
    init_listbutton();
    init_lyricbutton();
    init_minimizebutton();
    init_minimodebutton();
    init_prevbutton();
    init_nextbutton();
}

///init buttons

void MusicWidget::init_playbutton()
{
    PlayButton=new MeansButton(MainWidget);
    PlayButton->setIcon(start_normal);
    PlayButton->setIconSize(QSize(24,18));
    connect(PlayButton,SIGNAL(clicked()),this,SLOT(click_p_button()));
    connect(PlayButton,SIGNAL(mouse_enter()),this,SLOT(enter_p_button()));
    connect(PlayButton,SIGNAL(mouse_leave()),this,SLOT(leave_p_button()));
    connect(PlayButton,SIGNAL(mouse_press()),this,SLOT(press_p_button()));
    connect(PlayButton,SIGNAL(mouse_release()),this,SLOT(release_p_button()));
}

void MusicWidget::init_stopbutton()
{
    StopButton=new MeansButton(MainWidget);
    StopButton->setIcon(stop_normal);
    StopButton->setIconSize(QSize(24,18));
    connect(StopButton,SIGNAL(clicked()),this,SLOT(click_s_button()));
    connect(StopButton,SIGNAL(mouse_enter()),this,SLOT(enter_s_button()));
    connect(StopButton,SIGNAL(mouse_leave()),this,SLOT(leave_s_button()));
    connect(StopButton,SIGNAL(mouse_press()),this,SLOT(press_s_button()));
    connect(StopButton,SIGNAL(mouse_release()),this,SLOT(release_s_button()));
}

void MusicWidget::init_voicebutton()
{
    VoiceButton=new MeansButton(MainWidget);
    VoiceButton->setIcon(volume_normal);
    VoiceButton->setIconSize(QSize(19,16));
    VoiceButton->setToolTip("音量");
    ismute=false;
    connect(VoiceButton,SIGNAL(clicked()),this,SLOT(click_v_button()));
    connect(VoiceButton,SIGNAL(mouse_enter()),this,SLOT(enter_v_button()));
    connect(VoiceButton,SIGNAL(mouse_leave()),this,SLOT(leave_v_button()));
}

void MusicWidget::init_fullscreenbutton()
{
    FullScreenButton=new MeansButton(MainWidget);
    FullScreenButton->setIcon(fullscreen_normal);
    FullScreenButton->setIconSize(QSize(28,28));
    FullScreenButton->setToolTip("全屏");
    connect(FullScreenButton,SIGNAL(clicked()),this,SLOT(click_full_button()));
    connect(FullScreenButton,SIGNAL(mouse_enter()),this,SLOT(enter_full_button()));
    connect(FullScreenButton,SIGNAL(mouse_leave()),this,SLOT(leave_full_button()));
}

void MusicWidget::init_backbutton()
{
    BackButton=new MeansButton(MainWidget);
    BackButton->setIcon(back_normal);
    BackButton->setIconSize(QSize(48,48));
    BackButton->setToolTip("返回");
    connect(BackButton,SIGNAL(clicked()),this,SIGNAL(back_music()));
    connect(BackButton,SIGNAL(mouse_enter()),this,SLOT(enter_back_button()));
    connect(BackButton,SIGNAL(mouse_leave()),this,SLOT(leave_back_button()));
}

void MusicWidget::init_closebutton()
{
    CloseButton=new MeansCloseButton(MainWidget);
    CloseButton->set_mode(MeansCloseButton::CloseMode);
}

void MusicWidget::init_listbutton()
{
    ListButton=new MeansButton(MainWidget);
    ListButton->setIcon(list_normal);
    ListButton->setIconSize(QSize(14,14));
    connect(ListButton,SIGNAL(clicked()),this,SLOT(click_l_button()));
    connect(ListButton,SIGNAL(mouse_enter()),this,SLOT(enter_l_button()));
    connect(ListButton,SIGNAL(mouse_leave()),this,SLOT(leave_l_button()));
    connect(ListButton,SIGNAL(mouse_press()),this,SLOT(press_l_button()));
    connect(ListButton,SIGNAL(mouse_release()),this,SLOT(release_l_button()));
}

void MusicWidget::init_lyricbutton()
{
    LyricButton=new MeansButton(MainWidget);
    LyricButton->setIcon(lyric_normal);
    LyricButton->setIconSize(QSize(14,14));
    connect(LyricButton,SIGNAL(clicked()),this,SLOT(click_lyric_button()));
    connect(LyricButton,SIGNAL(mouse_enter()),this,SLOT(enter_lyric_button()));
    connect(LyricButton,SIGNAL(mouse_leave()),this,SLOT(leave_lyric_button()));
    connect(LyricButton,SIGNAL(mouse_press()),this,SLOT(press_lyric_button()));
    connect(LyricButton,SIGNAL(mouse_release()),this,SLOT(release_lyric_button()));
}

void MusicWidget::init_minimizebutton()
{
    MinimizeButton=new MeansButton(MainWidget);
    MinimizeButton->setIcon(minimize_normal);
    MinimizeButton->setIconSize(QSize(12,12));
    connect(MinimizeButton,SIGNAL(clicked()),this,SLOT(click_m_button()));
    connect(MinimizeButton,SIGNAL(mouse_enter()),this,SLOT(enter_m_button()));
    connect(MinimizeButton,SIGNAL(mouse_leave()),this,SLOT(leave_m_button()));
    connect(MinimizeButton,SIGNAL(mouse_press()),this,SLOT(press_m_button()));
    connect(MinimizeButton,SIGNAL(mouse_release()),this,SLOT(release_m_button()));
}

void MusicWidget::init_minimodebutton()
{
    MinimodeButton=new MeansButton(MainWidget);
    MinimodeButton->setIcon(minimode_normal);
    MinimodeButton->setIconSize(QSize(14,14));
    //connect(MinimodeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(MinimodeButton,SIGNAL(mouse_enter()),this,SLOT(enter_mini_button()));
    connect(MinimodeButton,SIGNAL(mouse_leave()),this,SLOT(leave_mini_button()));
    connect(MinimodeButton,SIGNAL(mouse_press()),this,SLOT(press_mini_button()));
    connect(MinimodeButton,SIGNAL(mouse_release()),this,SLOT(release_mini_button()));
}

void MusicWidget::init_prevbutton()
{
    PrevButton=new MeansButton(MainWidget);
    PrevButton->setIcon(prev_normal);
    PrevButton->setIconSize(QSize(24,18));
    connect(PrevButton,SIGNAL(clicked()),this,SLOT(click_prev_button()));
    connect(PrevButton,SIGNAL(mouse_enter()),this,SLOT(enter_prev_button()));
    connect(PrevButton,SIGNAL(mouse_leave()),this,SLOT(leave_prev_button()));
    connect(PrevButton,SIGNAL(mouse_press()),this,SLOT(press_prev_button()));
    connect(PrevButton,SIGNAL(mouse_release()),this,SLOT(release_prev_button()));
}

void MusicWidget::init_nextbutton()
{
    NextButton=new MeansButton(MainWidget);
    NextButton->setIcon(next_normal);
    NextButton->setIconSize(QSize(24,18));
    connect(NextButton,SIGNAL(clicked()),this,SLOT(click_next_button()));
    connect(NextButton,SIGNAL(mouse_enter()),this,SLOT(enter_next_button()));
    connect(NextButton,SIGNAL(mouse_leave()),this,SLOT(leave_next_button()));
    connect(NextButton,SIGNAL(mouse_press()),this,SLOT(press_next_button()));
    connect(NextButton,SIGNAL(mouse_release()),this,SLOT(release_next_button()));
}

///init buttons over

void MusicWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    emit window_state_change(windowState());
}

void MusicWidget::refresh_time()
{
    QString time(QTime(0,mPlayer->position()/60000,mPlayer->position()%60000/1000,0).toString("mm:ss"));
    for(int i=0;i<5;i++)
    {
        if(i==2)
            TimeNum[i]->setPixmap(QPixmap("./src/number/maohao.png"));
        else
            TimeNum[i]->setPixmap(QPixmap(QString("./src/number/")+QChar(time[i])+".png"));
    }
}

///slots
void MusicWidget::init_geometry()
{
    set_dragable(true);
    this->setParent(NULL);
    this->resize(345,195);
    FullScreenButton->hide();
    FullScreenButton->resize(30,30);
    BackButton->hide();
    BackButton->resize(50,50);
    MainWidget->resize(345,195);
    MessageLabel->setGeometry(230,85,100,25);
    MessageLabel->setWordWrap(true);
    SeekSlider->setGeometry(148,113,183,11);
    VolumeSlider->setGeometry(275,171,62,10);
    PlayButton->setGeometry(172,168,24,18);
    StopButton->setGeometry(196,168,24,18);
    VoiceButton->setGeometry(256,170,19,16);
    CloseButton->setGeometry(327,48,14,14);
    ListButton->setGeometry(313,134,14,14);
    LyricButton->setGeometry(313,148,14,14);
    MinimizeButton->setGeometry(298,48,12,12);
    MinimodeButton->setGeometry(312,48,14,14);
    PrevButton->setGeometry(148,168,24,18);
    NextButton->setGeometry(222,168,24,18);
    //VideoButton->setGeometry(132,103,45,45);
}

void MusicWidget::set_file_name(QString code)
{
    QJsonObject object=listMaker.object(code);
    fileDir=QString("./MultiData/[%1] %2/").arg(code).arg(object["name"].toString());
    fileName[0]=object["music1"].toString();
    fileName[1]=object["music2"].toString();
    fileName[2]=object["music3"].toString();

    set_media_name(1);
}

bool MusicWidget::set_media_name(int id)
{
    QString mediaName=fileDir+fileName[id-1];
    QFile file(mediaName);
    if(fileName[id-1]!="" && file.exists())
    {
        if(mPlayer->state()!=QMediaPlayer::StoppedState)
        {
            qint64 time=mPlayer->position();
            musicSwitch=true;
            disconnect(mPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(seek_set_range(qint64)));
            disconnect(mPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(seek_set_value(qint64)));
            mPlayer->stop();
            mPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(mediaName)));
            mPlayer->setPosition(time);
            mPlayer->play();
            connect(mPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(seek_set_range(qint64)));
            connect(mPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(seek_set_value(qint64)));
        }
        else
        {
            mPlayer->stop();
            prePlayer->stop();
            mPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(mediaName)));
            mPlayer->setPosition(10);
        }
        currentMediaId=id;
        refresh_message();
        return true;
    }
    else
    {
        if(this->isVisible())
        {;}//MeansWaring::warns("Σ(ﾟдﾟ；)音乐找不到了",2000,150,35,this);
        return false;
    }
}

void MusicWidget::seek_set_range(qint64 duration)
{
    SeekSlider->setRange(0,duration);
    refresh_time();
}

void MusicWidget::seek_set_value(qint64 position)
{
    SeekSlider->setValue(position);
    refresh_time();
}

void MusicWidget::player_set_position()
{
    mPlayer->setPosition(SeekSlider->value());
    emit set_position(SeekSlider->value());
}

void MusicWidget::volume_changed(int value)
{
    if(ismute && value!=0)
        ismute=false;
    leave_v_button();
    prePlayer->setVolume(value);
    mPlayer->setVolume(value);
}

/// button p
void MusicWidget::click_p_button()
{
    if(mPlayer->state()==QMediaPlayer::PlayingState)
        mPlayer->pause();
    else if(prePlayer->state()==QMediaPlayer::PlayingState)
        prePlayer->pause();
    else if(isbegin && SeekSlider->value()<=15)
        prePlayer->play();
    else
        mPlayer->play();
}

void MusicWidget::enter_p_button()
{
    if(mPlayer->state()==QMediaPlayer::PlayingState || prePlayer->state()==QMediaPlayer::PlayingState)
        PlayButton->setIcon(pause_hover);
    else
        PlayButton->setIcon(start_hover);
}

void MusicWidget::leave_p_button()
{
    if(mPlayer->state()==QMediaPlayer::PlayingState || prePlayer->state()==QMediaPlayer::PlayingState)
        PlayButton->setIcon(pause_normal);
    else
        PlayButton->setIcon(start_normal);
}

void MusicWidget::press_p_button()
{
    if(mPlayer->state()==QMediaPlayer::PlayingState || prePlayer->state()==QMediaPlayer::PlayingState)
        PlayButton->setIcon(pause_press);
    else
        PlayButton->setIcon(start_press);
}

void MusicWidget::release_p_button()
{
    if(mPlayer->state()==QMediaPlayer::PlayingState || prePlayer->state()==QMediaPlayer::PlayingState)
        PlayButton->setIcon(pause_normal);
    else
        PlayButton->setIcon(start_normal);
}

/// button s
void MusicWidget::click_s_button()
{
    mPlayer->stop();
    prePlayer->stop();
}

void MusicWidget::enter_s_button()
{
    StopButton->setIcon(stop_hover);
}

void MusicWidget::leave_s_button()
{
    StopButton->setIcon(stop_normal);
}

void MusicWidget::press_s_button()
{
    StopButton->setIcon(stop_press);
}

void MusicWidget::release_s_button()
{
    StopButton->setIcon(stop_normal);
}

/// button v
void MusicWidget::click_v_button()
{
    ismute=!ismute;
    if(ismute)
    {
        mPlayer->setVolume(0);
        prePlayer->setVolume(0);
        VoiceButton->setIcon(mute_hover);
    }
    else
    {
        mPlayer->setVolume(VolumeSlider->value());
        prePlayer->setVolume(VolumeSlider->value());
        VoiceButton->setIcon(volume_hover);
    }
}

void MusicWidget::enter_v_button()
{
    if(ismute)
        VoiceButton->setIcon(mute_hover);
    else
        VoiceButton->setIcon(volume_hover);
}

void MusicWidget::leave_v_button()
{
    if(ismute)
        VoiceButton->setIcon(mute_normal);
    else
        VoiceButton->setIcon(volume_normal);
}

///button l
void MusicWidget::click_l_button()
{
    listState=!listState;
    emit switch_list();
}

void MusicWidget::enter_l_button()
{
    ListButton->setIcon(list_hover);
}

void MusicWidget::leave_l_button()
{
    if(listState)
        ListButton->setIcon(list_normal);
    else
        ListButton->setIcon(list_disabled);
}

void MusicWidget::press_l_button()
{
    ListButton->setIcon(list_press);
}

void MusicWidget::release_l_button()
{
    if(listState)
        ListButton->setIcon(list_normal);
    else
        ListButton->setIcon(list_disabled);
}

///button lyric
void MusicWidget::click_lyric_button()
{
    lyricState=!lyricState;
    emit switch_lyric();
}

void MusicWidget::enter_lyric_button()
{
    LyricButton->setIcon(lyric_hover);
}

void MusicWidget::leave_lyric_button()
{
    if(lyricState)
        LyricButton->setIcon(lyric_normal);
    else
        LyricButton->setIcon(lyric_disabled);
}

void MusicWidget::press_lyric_button()
{
    LyricButton->setIcon(lyric_press);
}

void MusicWidget::release_lyric_button()
{
    if(lyricState)
        LyricButton->setIcon(lyric_normal);
    else
        LyricButton->setIcon(lyric_disabled);
}

///button m
void MusicWidget::click_m_button()
{
    showMinimized();
}

void MusicWidget::enter_m_button()
{
    MinimizeButton->setIcon(minimize_hover);
}

void MusicWidget::leave_m_button()
{
    MinimizeButton->setIcon(minimize_normal);
}

void MusicWidget::press_m_button()
{
    MinimizeButton->setIcon(minimize_press);
}

void MusicWidget::release_m_button()
{
    MinimizeButton->setIcon(minimize_normal);
}

///button mini
void MusicWidget::click_mini_button()
{

}

void MusicWidget::enter_mini_button()
{
    MinimodeButton->setIcon(minimode_hover);
}

void MusicWidget::leave_mini_button()
{
    MinimodeButton->setIcon(minimode_normal);
}

void MusicWidget::press_mini_button()
{
    MinimodeButton->setIcon(minimode_press);
}

void MusicWidget::release_mini_button()
{
    MinimodeButton->setIcon(minimode_normal);
}

///button prev
void MusicWidget::click_prev_button()
{
    set_media_name((currentMediaId+1)%3+1);
}

void MusicWidget::enter_prev_button()
{
    PrevButton->setIcon(prev_hover);
}

void MusicWidget::leave_prev_button()
{
    PrevButton->setIcon(prev_normal);
}

void MusicWidget::press_prev_button()
{
    PrevButton->setIcon(prev_press);
}

void MusicWidget::release_prev_button()
{
    PrevButton->setIcon(prev_normal);
}

///button next
void MusicWidget::click_next_button()
{
    set_media_name((currentMediaId%3)+1);
}

void MusicWidget::enter_next_button()
{
    NextButton->setIcon(next_hover);
}

void MusicWidget::leave_next_button()
{
    NextButton->setIcon(next_normal);
}

void MusicWidget::press_next_button()
{
    NextButton->setIcon(next_press);
}

void MusicWidget::release_next_button()
{
    NextButton->setIcon(next_normal);
}

///button fullscreen
void MusicWidget::click_full_button()
{
    emit full_screen(true);
}

void MusicWidget::enter_full_button()
{
    FullScreenButton->setIcon(fullscreen_hover);
}

void MusicWidget::leave_full_button()
{
    FullScreenButton->setIcon(fullscreen_normal);
}

///button back
void MusicWidget::enter_back_button()
{
    BackButton->setIcon(back_hover);
}

void MusicWidget::leave_back_button()
{
    BackButton->setIcon(back_normal);
}

void MusicWidget::refresh_message()
{
    QString state,media;
    if(mPlayer->state()==QMediaPlayer::PlayingState || prePlayer->state()==QMediaPlayer::PlayingState)
        state="播放";
    else if(mPlayer->state()==QMediaPlayer::StoppedState && prePlayer->state()==QMediaPlayer::StoppedState)
        state="停止";
    else
        state="暂停";

    switch(currentMediaId)
    {
    case 1:
        media="原曲";
        break;
    case 2:
        media="伴奏";
        break;
    case 3:
        media="示例";
        break;
    default:
        media="无";
        break;
    }

    MessageLabel->setText("状态:"+media+"("+state+")");
}

void MusicWidget::play_music()
{
    click_p_button();
}

///interface
void MusicWidget::pre_state_changed(QMediaPlayer::State state)
{
    if(mPlayer->state()==QMediaPlayer::StoppedState)
        isbegin=true;
    int px=QCursor::pos().x(),py=QCursor::pos().y();
    int x=PlayButton->x()+this->x(),y=PlayButton->y()+this->y();
    if(mPlayer->state()==QMediaPlayer::PlayingState || prePlayer->state()==QMediaPlayer::PlayingState)
    {
        if(x<px && px<x+PlayButton->width()&& y<py && py<y+PlayButton->height())
            PlayButton->setIcon(pause_hover);
        else
            PlayButton->setIcon(pause_normal);
    }
    else
    {
        if(x<px && px<x+PlayButton->width()&& y<py && py<y+PlayButton->height())
            PlayButton->setIcon(start_hover);
        else
            PlayButton->setIcon(start_normal);
    }
    refresh_message();
}

void MusicWidget::m_state_changed(QMediaPlayer::State state)
{
    pre_state_changed(state);
    if(musicSwitch)
        musicSwitch=false;
    else
        emit change_state(state);
}

void MusicWidget::list_off()
{
    listState=false;
    ListButton->setIcon(list_disabled);
}

void MusicWidget::lyric_off()
{
    lyricState=false;
    LyricButton->setIcon(lyric_disabled);
}

void MusicWidget::prefix_stopped(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
        isbegin=false;
        click_p_button();
    }
}

void MusicWidget::refresh_list()
{
    listMaker.read_list("./config/list.jel");
}

///slots over


///Interface
MusicInterface::MusicInterface(QWidget *parent) :
    MusicWidget(parent)
{

}

QRect MusicInterface::geometry()
{
    return MusicWidget::geometry();
}

QPoint MusicInterface::pos()
{
    return MusicWidget::pos();
}

int MusicInterface::x()
{
    return MusicWidget::x();
}

int MusicInterface::y()
{
    return MusicWidget::y();
}

int MusicInterface::width()
{
    return MusicWidget::width();
}

int MusicInterface::height()
{
    return MusicWidget::height();
}

QSize MusicInterface::size()
{
    return MusicWidget::size();
}

void MusicInterface::set_object(QWidget *object)
{
    MusicWidget::set_object(object);
}

void MusicInterface::set_object(QList<QWidget *> list)
{
    MusicWidget::set_object(list);
}

void MusicInterface::add_object(QWidget *object)
{
    MusicWidget::add_object(object);
}

void MusicInterface::add_object(QList<QWidget *> list)
{
    MusicWidget::add_object(list);
}

void MusicInterface::del_object(QWidget *object)
{
    MusicWidget::del_object(object);
}
