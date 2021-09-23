#include "mainwidget.h"
#include "ui_mainwidget.h"
#include<iostream>
#include <string>
#include <stdio.h>
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
QString MainWidget::final;
QString MainWidget::inject;
QStringList MainWidget::list;
QString frequence;
using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    QIcon("logo.ico");
    ui->setupUi(this);
    this->setWindowTitle("选择参数");
    this->setWindowIcon(QIcon(".\\logo.ico"));
}

MainWidget::~MainWidget()
{
    delete ui;
}



void MainWidget::on_ReadInject_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,
     "打开文件","G:\\User\\Desktop\\Final\\");
    inject=path;
    ui->Inject->setText(path);
}


void MainWidget::on_ReadDLL_clicked()
{
    QString path=QFileDialog::getExistingDirectory(this,"打开文件","G:\\User\\Desktop\\Final\\");
    list.append(path);
    ui->DLL->setText(path);
     frequence=path+"/log";
}


void MainWidget::on_ReadTestAPP_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,
    "打开文件","G:\\User\\Desktop\\Final\\");
    list.append(path);
    ui->TestAPP->setText(path);

}




void MainWidget::on_ReadDLL2_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,
     "打开文件","G:\\User\\Desktop\\Final\\");
    list.append(path);
    //final=final+path+" ";
    ui->Ldll->setText(path);

}

void MainWidget::on_pushButton_clicked()
{
    myProcess = new QProcess(this);
    myProcess->start(inject,list);
    this->close();
    secondwidget *p1 =new secondwidget();
    p1->show();
}




