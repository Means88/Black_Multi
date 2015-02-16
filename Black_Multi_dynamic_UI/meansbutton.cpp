#include "meansbutton.h"
#include <QApplication>
#include <QMessageBox>

///button
MeansButton::MeansButton(QWidget *parent) :
    QPushButton(parent)
{
}

MeansButton::~MeansButton()
{
}

void MeansButton::enterEvent(QEvent *event)
{
    emit mouse_enter();
    event->accept();
}

void MeansButton::leaveEvent(QEvent *event)
{
    emit mouse_leave();
    event->accept();
}

void MeansButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    emit mouse_press();
}

void MeansButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    emit mouse_release();
}

///slider
MeansSlider::MeansSlider(QWidget *parent) :
    QSlider(parent)
{
    entered=false;
    pressed=false;
    time=new QTimer(this);
    time->setSingleShot(true);
    handleWidth=0;
    setRange(0,1);
    connect(time,SIGNAL(timeout()),this,SLOT(time_out()));
}

MeansSlider::~MeansSlider()
{
    delete time;
}

void MeansSlider::set_handle_width(int width)
{
    handleWidth=width;
}

void MeansSlider::enterEvent(QEvent *event)
{
    entered=true;
    emit mouse_enter();
    event->accept();
}

void MeansSlider::leaveEvent(QEvent *event)
{
    entered=false;
    emit mouse_leave();
    event->accept();
}

void MeansSlider::set_position(QPoint pos)
{
    if(this->orientation()==Qt::Horizontal)
    {
        double px=(pos.x()-handleWidth/2) / double(width()-handleWidth);
        setValue(px*(maximum()-minimum()) + minimum());
    }
    else
    {
        double py=(pos.y()-handleWidth/2) / double(height()-handleWidth);
        setValue(py*(maximum()-minimum()) + minimum());
    }
}

void MeansSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    set_position(event->pos());
    pressed=true;
    emit select_value(qint64(value()));
    emit mouse_press();
}

void MeansSlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    if(pressed)
    {
        set_position(event->pos());
        emit select_value(qint64(value()));
        emit mouse_drag();
    }
    emit mouse_move();
}

void MeansSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    pressed=false;
    emit mouse_release();
    //QMessageBox::information(this,"",itoa(maximum(),new char[10],10));
}

bool MeansSlider::is_entered()
{
    return entered;
}

void MeansSlider::time_begin()
{
    time->start(150);
}

///slots
void MeansSlider::time_out()
{
    if(!entered)
        hide();
}

///MeansCloseButton
MeansCloseButton::MeansCloseButton(QWidget *parent) :
    MeansButton(parent)
{
    this->closeParent=parent;
    thismode=HideMode;

    close_normal=QIcon("./src/button/close.png");
    close_hover=QIcon("./src/button/close_hover.png");
    close_press=QIcon("./src/button/close_press.png");

    setIcon(close_normal);
    setIconSize(QSize(14,14));
    connect(this,SIGNAL(clicked()),this,SLOT(click_c_button()));
    connect(this,SIGNAL(mouse_enter()),this,SLOT(enter_c_button()));
    connect(this,SIGNAL(mouse_leave()),this,SLOT(leave_c_button()));
    connect(this,SIGNAL(mouse_press()),this,SLOT(press_c_button()));
    connect(this,SIGNAL(mouse_release()),this,SLOT(release_s_button()));
}

MeansCloseButton::~MeansCloseButton()
{
}

MeansCloseButton::Mode MeansCloseButton::mode()
{
    return thismode;
}

void MeansCloseButton::set_mode(Mode mode)
{
    thismode=mode;
}

QWidget *MeansCloseButton::close_parent()
{
    return close_parent();
}

void MeansCloseButton::set_close_parent(QWidget *parent)
{
    closeParent=parent;
}

void MeansCloseButton::click_c_button()
{
    emit closed();
    if(thismode==HideMode)
        closeParent->hide();
    else
        qApp->quit();
}

void MeansCloseButton::enter_c_button()
{
   setIcon(close_hover);
}

void MeansCloseButton::leave_c_button()
{
    setIcon(close_normal);
}

void MeansCloseButton::press_c_button()
{
    setIcon(close_press);
}

void MeansCloseButton::release_c_button()
{
    setIcon(close_normal);
}

///MeansListWidget
MeansListWidget::MeansListWidget(QWidget *parent) :
    QListWidget(parent)
{
    editAction=new QAction("编辑",this);
    removeAction=new QAction("删除",this);
    connect(editAction,SIGNAL(triggered()),this,SLOT(edit_slot()));
    connect(removeAction,SIGNAL(triggered()),this,SLOT(remove_slot()));
}

MeansListWidget::~MeansListWidget()
{
    delete editAction;
    delete removeAction;
}

void MeansListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    contextItem=this->itemAt(event->pos());
    if(contextItem!=NULL)
    {
        QMenu *menu=new QMenu(this);
        menu->addAction(editAction);
        menu->addAction(removeAction);
        menu->exec(QCursor::pos());
    }
}

void MeansListWidget::edit_slot()
{
    emit edit(contextItem);
}

void MeansListWidget::remove_slot()
{
    emit remove(contextItem);
}

///MeansTimer
MeansTimer::MeansTimer(QObject *parent) :
    QTimer(parent)
{
}

MeansTimer::~MeansTimer()
{
}

MeansTimer::MeansTimer(QString msg, QObject *parent) :
    QTimer(parent)
{
    this->msg=msg;
}

void MeansTimer::set_message(QString msg)
{
    this->msg=msg;
}

QString MeansTimer::message()
{
    return msg;
}
