/*!
 * The Label can be dragged
 * And has signals:
 *      clicked()
 *      moved()
 *      pressed()
 */

#ifndef MEANSDRAGER_H
#define MEANSDRAGER_H

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

class MeansDrager : public QLabel
{
    Q_OBJECT
public:
    explicit MeansDrager(QWidget *parent = 0);
    virtual ~MeansDrager();

    bool is_single();
    bool is_dragable();
    void set_single(bool);
    void set_dragable(bool);
    void set_object(QWidget*);
    void set_object(QList<QWidget*>);
    void add_object(QWidget*);
    void add_object(QList<QWidget*>);
    void del_object(QWidget*);
    void setParent(QWidget *parent);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    QPoint dragPosition;
    bool single,flag,isDragable,ismoved;

    QList<QWidget *> objectList;
    QList<QPoint> positionList;
signals:
    void clicked();
    void moved();
    void pressed();
    void released();

};

#endif // MEANSDRAGER_H
