#include "meanslineedit.h"

MeansLineEdit::MeansLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    flag=false;
    connect(this,SIGNAL(clicked()),this,SLOT(on_click_slot()));
}

void MeansLineEdit::set_mode(Mode mode)
{
    this->mode=mode;
}

void MeansLineEdit::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    flag=true;
}

void MeansLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    QLineEdit::mouseReleaseEvent(event);
    if(flag)
    {
        flag=false;
        emit clicked();
    }
}

void MeansLineEdit::on_click_slot()
{
    if(text().size()==0 && isEnabled())
    {
        QStringList filters;

        switch(mode)
        {
        case NoMode:
            filters << "所有文件 (*)";
            break;
        case Image:
            filters << "图片 (*.jpg *.png *.gif *.bmp)"
                    << "所有文件 (*)";
            break;

        case Music:
            filters << "音乐 (*.mp3)"
                    << "所有文件 (*)";
            break;

        case Video:
            filters << "视频 (*.mp4)"
                    << "所有文件 (*)";
            break;
        }

        QFileDialog dialog(this);
        dialog.setNameFilters(filters);
        dialog.setFileMode(QFileDialog::ExistingFile);

        if(dialog.exec())
        {
            setText(dialog.selectedFiles()[0].replace('/','\\'));
        }
    }
}
