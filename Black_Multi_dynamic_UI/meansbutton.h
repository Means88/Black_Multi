/*!
 * The button can catch the mouse Enter/Leave/Press/Release event
 * Tthe slider: Enter/Leave
 */

#ifndef MEANSBUTTON_H
#define MEANSBUTTON_H

#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QSlider>
#include <QEvent>
#include <QMouseEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QIcon>

/// button
class MeansButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MeansButton(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void mouse_enter();
    void mouse_leave();
    void mouse_press();
    void mouse_release();

public slots:

};

///slider
class MeansSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MeansSlider(QWidget *parent = 0);

    bool is_entered();
    void time_begin();
    void set_handle_width(int width);
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    bool entered,pressed;
    int handleWidth;
    QTimer *time;

    void set_position(QPoint pos);
signals:
    void mouse_enter();
    void mouse_leave();
    void select_value(qint64);

public slots:
    void time_out();
};

///MeansCloseButton
class MeansCloseButton : public MeansButton
{
    Q_OBJECT
public:
    explicit MeansCloseButton(QWidget *parent);

    enum Mode{CloseMode,HideMode};
    Mode mode();
    void set_mode(Mode mode);
    QWidget *close_parent();
    void set_close_parent(QWidget*);

private:
    QIcon close_normal,close_hover,close_press;
    QWidget *closeParent;
    Mode thismode;

signals:
    void closed();

public slots:
    void click_c_button();
    void enter_c_button();
    void leave_c_button();
    void press_c_button();
    void release_c_button();
};

///MeansListWidget
class MeansListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MeansListWidget(QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QAction *editAction,*removeAction;
    QListWidgetItem *contextItem;

signals:
    void edit(QListWidgetItem *);
    void remove(QListWidgetItem *);

private slots:
    void edit_slot();
    void remove_slot();
};

#endif // MEANSBUTTON_H
