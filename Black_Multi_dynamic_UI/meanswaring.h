/**
 * The label shows warning message
 */

#ifndef MEANSWARING_H
#define MEANSWARING_H

#include <QLabel>
#include <QTimer>

class MeansWaring : protected QWidget
{
    Q_OBJECT
public:
    ~MeansWaring();

    static void warns(QString text,int time=2000, int w=250, int h=60, QWidget *parent=0);
private:
    explicit MeansWaring(QString text, int time=2000, int w=250, int h=60, QWidget *parent=0);
    QLabel *label;
    QTimer *timer;
    int time;
signals:

private slots:
    void delete_this();
};

#endif // MEANSWARING_H
