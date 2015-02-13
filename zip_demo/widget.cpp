#include "widget.h"
#include "ui_widget.h"
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->ImageEdit->set_mode(MeansLineEdit::Image);
    ui->MusicEdit1->set_mode(MeansLineEdit::Music);
    ui->MusicEdit2->set_mode(MeansLineEdit::Music);
    ui->MusicEdit3->set_mode(MeansLineEdit::Music);
    ui->VideoEdit->set_mode(MeansLineEdit::Video);
    ui->MusicEdit1->setDisabled(true);
    ui->MusicEdit2->setDisabled(true);
    ui->MusicEdit3->setDisabled(true);
    ui->VideoEdit->setDisabled(true);
    ui->LevelBox->addItem("Level0");
    ui->LevelBox->addItem("Level1");
    ui->LevelBox->addItem("Level2");
    ui->LevelBox->setCurrentIndex(2);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_OkButton_clicked()
{
    if(ui->ImageEdit->text().size()==0 ||
       ui->NameEdit->text().size()==0 ||
       (ui->MusicCheck->isChecked() && ui->MusicEdit1->text().size()==0 && ui->MusicEdit2->text().size()==0 && ui->MusicEdit3->text().size()==0) ||
       (ui->VideoCheck->isChecked() && ui->VideoEdit->text().size()==0) ||
       (ui->LevelBox->currentIndex()==2 && ui->Parent1Edit->text()==0) ||
       (ui->LevelBox->currentIndex()>=1 && ui->Parent0Edit->text()==0))
    {
        QMessageBox::warning(this,"","请将信息补充完整");
    }
    else
    {
        QJsonObject configObject;

        QString name=ui->NameEdit->text();
        QString image=ui->ImageEdit->text().section('\\',-1);

        QString imageCode=get_md5code(ui->ImageEdit->text());
        QString music1Code,music2Code,music3Code,videoCode;

        if(ui->MusicCheck->isChecked())
        {
            if(ui->MusicEdit1->text().size())
                music1Code=get_md5code(ui->MusicEdit1->text());
            if(ui->MusicEdit2->text().size())
                music2Code=get_md5code(ui->MusicEdit2->text());
            if(ui->MusicEdit3->text().size())
                music3Code=get_md5code(ui->MusicEdit3->text());
        }
        if(ui->VideoCheck->isChecked())
        {
            videoCode=get_md5code(ui->VideoEdit->text());
        }

        QString music1,music2,music3,video;
        if(ui->MusicCheck->isChecked())
        {
            if(ui->MusicEdit1->text().size())
                music1=ui->MusicEdit1->text().section('\\',-1);
            if(ui->MusicEdit2->text().size())
                music2=ui->MusicEdit2->text().section('\\',-1);
            if(ui->MusicEdit3->text().size())
                music3=ui->MusicEdit3->text().section('\\',-1);
        }
        if(ui->VideoCheck->isChecked() && ui->VideoEdit->text().size())
        {
             video=ui->VideoEdit->text().section('\\',-1);
        }

        QString parent0,parent1;
        int level=ui->LevelBox->currentIndex();

        if(level==2 && ui->Parent1Edit->text().size())
            parent1=ui->Parent1Edit->text();

        if(level>=1 && ui->Parent0Edit->text().size())
            parent0=ui->Parent0Edit->text();

        QString nameSha1=QString(QCryptographicHash::hash(name.toUtf8(),QCryptographicHash::Sha1).toHex());
        QString code=hex_to_36(nameSha1).right(2)+imageCode.right(5);
        ui->CodeEdit->setText(code);

        configObject.insert("code",code);
        configObject.insert("name",name);
        configObject.insert("image",image);
        configObject.insert("music1",music1);
        configObject.insert("music2",music2);
        configObject.insert("music3",music3);
        configObject.insert("video",video);
        configObject.insert("level",level);
        configObject.insert("parent0",parent0);
        configObject.insert("parent1",parent1);
        configObject.insert("imageCode",imageCode);
        configObject.insert("music1Code",music1Code);
        configObject.insert("music2Code",music2Code);
        configObject.insert("music3Code",music3Code);
        configObject.insert("videoCode",videoCode);

        QFile file("imfo.jez");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << QString(QJsonDocument(configObject).toJson()).toUtf8().toPercentEncoding();
        file.close();

        QString command;
        command = QString("7za a \"[%1] %2.je\" ").arg(code).arg(name);

        QProcess::execute(command + "imfo.jez");
        QFile::remove("imfo.jez");
        if(image.size())
            QProcess::execute(command + "\""+ui->ImageEdit->text()+"\" ");
        if(music1.size())
            QProcess::execute(command + "\""+ui->MusicEdit1->text()+"\" ");
        if(music2.size())
            QProcess::execute(command + "\""+ui->MusicEdit2->text()+"\" ");
        if(music3.size())
            QProcess::execute(command + "\""+ui->MusicEdit3->text()+"\" ");
        if(video.size())
            QProcess::execute(command + "\""+ui->VideoEdit->text()+"\" ");

        QMessageBox::information(this,"","生成完毕");
    }
}

QString Widget::get_md5code(QString dir)
{
    QString result;

    QFile file(dir.replace('\\','/'));
    file.open(QIODevice::ReadOnly);
    result.append(QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5).toHex());
    file.close();

    return hex_to_36(result);
}

QString Widget::hex_to_36(QString origin)
{
    unsigned long long tempull = strtoull(origin.right(16).toStdString().c_str(),NULL,16);
    char temps[37];
    QString result = ulltoa(tempull,temps,36);

    //QMessageBox::information(this,"",QString("%1\n%2\n%3").arg(tempull).arg(origin).arg(result));
    return result;
}

void Widget::on_LevelBox_currentIndexChanged(int index)
{
    if(index==2)
    {
        ui->MusicCheck->setEnabled(true);
        ui->VideoCheck->setEnabled(true);
        ui->Parent0Edit->setEnabled(true);
        ui->Parent1Edit->setEnabled(true);
    }
    else
    {
        ui->MusicCheck->setEnabled(false);
        ui->VideoCheck->setEnabled(false);
        ui->Parent1Edit->setEnabled(false);
        if(index==0)
            ui->Parent0Edit->setEnabled(false);
        else
            ui->Parent0Edit->setEnabled(true);
    }
}

void Widget::on_LoadButton_clicked()
{
    QStringList filter;
    filter << "谱册文件 (*.je)";

    QFileDialog dialog(this);
    dialog.setNameFilters(filter);
    dialog.setFileMode(QFileDialog::ExistingFile);

    if(dialog.exec())
    {
        load(dialog.selectedFiles()[0]);
    }
}

void Widget::on_MusicCheck_stateChanged(int state)
{
    ui->MusicEdit1->setDisabled(state==Qt::Unchecked);
    ui->MusicEdit2->setDisabled(state==Qt::Unchecked);
    ui->MusicEdit3->setDisabled(state==Qt::Unchecked);
}

void Widget::on_VideoCheck_stateChanged(int state)
{
    ui->VideoEdit->setDisabled(state==Qt::Unchecked);
}

void Widget::dropEvent(QDropEvent *event)
{
    load(event->mimeData()->urls().at(0).toLocalFile());
}

void Widget::load(QString fileName)
{
    QString file=fileName.replace('/','\\');
    QString dir=QString("./mediaData/")+file.section('\\',-1,-1).section('.',0,0);
    if(!QDir(dir).exists())
        QDir::current().mkpath(dir);
    QString command=QString("7za e -y \"%1\" -o\".\\mediaData\\%2\"").arg(file).arg(file.section('\\',-1,-1).section('.',0,0));
    QProcess::execute(command);

    QFile msg(dir.replace('\\','/')+"/imfo.jez");
    msg.open(QIODevice::ReadOnly);
    QJsonObject imfo=QJsonDocument::fromJson(QByteArray::fromPercentEncoding(msg.readAll())).object();
    msg.close();

    dir=(QDir::current().absolutePath()+QString("/mediaData/[%1] %2/").arg(imfo["code"].toString()).arg(imfo["name"].toString()));
    dir.replace('/','\\');
    ui->LevelBox->setCurrentIndex(imfo["level"].toInt());
    ui->NameEdit->setText(imfo["name"].toString());
    ui->Parent0Edit->setText(imfo["parent0"].toString());
    ui->Parent1Edit->setText(imfo["parent1"].toString());

    if(imfo["image"].toString().size()) ui->ImageEdit->setText(dir+imfo["image"].toString());
    else ui->ImageEdit->clear();

    if(imfo["music1"].toString().size()) ui->MusicEdit1->setText(dir+imfo["music1"].toString());
    else ui->MusicEdit1->clear();

    if(imfo["music2"].toString().size()) ui->MusicEdit2->setText(dir+imfo["music2"].toString());
    else ui->MusicEdit2->clear();

    if(imfo["music3"].toString().size()) ui->MusicEdit3->setText(dir+imfo["music3"].toString());
    else ui->MusicEdit3->clear();

    if(imfo["video"].toString().size()) ui->VideoEdit->setText(dir+imfo["video"].toString());
    else ui->VideoEdit->clear();

    ui->CodeEdit->setText(imfo["code"].toString());

    ui->LevelBox->setCurrentIndex(imfo["level"].toInt());
    ui->Parent0Edit->setEnabled(imfo["level"].toInt()>=1);
    ui->Parent1Edit->setEnabled(imfo["level"].toInt()==2);
    ui->MusicCheck->setChecked(imfo["music1"].toString().size() || imfo["music2"].toString().size() || imfo["music3"].toString().size());
    ui->VideoCheck->setChecked(imfo["video"].toString().size());

    if(ui->CheckBox->isChecked())
    {
        ///check
        QStringList list;

        if(imfo["imageCode"].toString()!=get_md5code(dir+imfo["image"].toString()))
            list.push_back("图片");

        if(imfo["music1"].toString().size() && imfo["music1Code"].toString()!=get_md5code(dir+imfo["music1"].toString()))
            list.push_back("原曲");

        if(imfo["music2"].toString().size() && imfo["music2Code"].toString()!=get_md5code(dir+imfo["music2"].toString()))
            list.push_back("伴奏");

        if(imfo["music3"].toString().size() && imfo["music3Code"].toString()!=get_md5code(dir+imfo["music3"].toString()))
            list.push_back("示例");

        if(imfo["video"].toString().size() && imfo["videoCode"].toString()!=get_md5code(dir+imfo["video"].toString()))
            list.push_back("MAD");

        if(list.size())
        {
            QString message;
            for(int i=0; i<list.size(); i++)
                message += (list[i]+' ');
            message += "可能有损坏";
            QMessageBox::information(this,"",message);
        }
        else
        {
            QMessageBox::information(this,"","文件完好");
        }
    }
}

