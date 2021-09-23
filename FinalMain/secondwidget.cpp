#include "secondwidget.h"
#include "ui_secondwidget.h"
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QtCore/qtextcodec.h>
#include <QColor>
#include "mainwidget.h"


secondwidget::secondwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::secondwidget)
{
    ui->setupUi(this);
    this->setWindowTitle("测试结果");
    this->setWindowIcon(QIcon(".\\logo.ico"));
}

secondwidget::~secondwidget()
{
    delete ui;
}

void secondwidget::on_comboBox_currentIndexChanged(int index)
{
    QString path;
    extern QString frequence;
    if(index!=0)
    {

    switch(ui->comboBox->currentIndex()){
        case 1:
            path=frequence+"/out_copyfile.txt";
            break;
        case 2:
            path=frequence+"/out_creatfile.txt";
            break;
        case 3:
            path=frequence+"/out_deletefile.txt";
            break;
        case 4:
            path=frequence+"/out_heapcreate.txt";
            break;
        case 5:
            path=frequence+"/out_heapdestroy.txt";
            break;
        case 6:
            path=frequence+"/out_heapfree.txt";
            break;
        case 7:
            path=frequence+"/out_messageboxa.txt";
            break;
        case 8:
            path=frequence+"/out_messageboxw.txt";
            break;
        case 9:
            path=frequence+"/out_readfile.txt";
            break;
        case 10:
            path=frequence+"/out_regclosekey.txt";
            break;
        case 11:
            path=frequence+"/out_regcreatekey.txt";
            break;
        case 12:
            path=frequence+"/out_regdeletevalue.txt";
            break;
        case 13:
            path=frequence+"/out_regopenkey.txt";
            break;
        case 14:
            path=frequence+"/out_regsetvalue.txt";
            break;
        case 15:
            path=frequence+"/out_writefile.txt";
            break;
        default:
            break;

    }
    QTextCodec *codec =QTextCodec::codecForName("GBK");
    QFile file(path);
    bool isOK=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(isOK==true)
    {
        ui->TX1->setText(" ");
        QByteArray array =file.readAll();
        QString str=codec->toUnicode(array);
        ui->TX1->setText(str);
    }
    file.close();
    }

}


void secondwidget::on_fre_clicked()
{
    extern QString frequence;
    QString path=frequence+"/num.txt";
    QTextCodec *codec =QTextCodec::codecForName("GBK");
    QFile file(path);
    bool isOK=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(isOK==true)
    {
         QByteArray array =file.readAll();
         QString str=codec->toUnicode(array);
         ui->TX2->setTextColor(Qt::blue);
         ui->TX2->setText(str);
    }
    file.close();
}


void secondwidget::on_sec_clicked()
{
    extern QString frequence;
    QString path=frequence+"/warning.txt";
    QTextCodec *codec =QTextCodec::codecForName("GBK");
    QFile file(path);
    bool isOK=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(isOK==true)
    {

         QByteArray array =file.readAll();
         QString str=codec->toUnicode(array);
         ui->TX3->setTextColor(Qt::red);
         ui->TX3->setText(str);
    }
    file.close();
}


void secondwidget::on_back_clicked()
{
    MainWidget::final="";
    MainWidget *fater = new MainWidget();
    this->close();
    fater->show();
}

