#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <secondwidget.h>
#include <QProcess>

extern QString frequence;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    static QString final;
    static QString inject;
    static QStringList list;
    QProcess* myProcess;

private slots:

    void on_ReadDLL_clicked();

    void on_ReadInject_clicked();

    void on_ReadTestAPP_clicked();

    void on_ReadDLL2_clicked();

    void on_pushButton_clicked();

private:

    Ui::MainWidget *ui;
};
#endif // MAINWIDGET_H
