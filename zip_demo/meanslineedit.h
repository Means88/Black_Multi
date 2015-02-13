#ifndef MEANSLINEEDIT_H
#define MEANSLINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>
#include <QFileDialog>
#include <QStringList>

class MeansLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MeansLineEdit(QWidget *parent = 0);
    enum Mode{NoMode,Image,Music,Video};
    void set_mode(Mode mode);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool flag;
    Mode mode;

private slots:
    void on_click_slot();

signals:
    void clicked();

public slots:

};

#endif // MEANSLINEEDIT_H
