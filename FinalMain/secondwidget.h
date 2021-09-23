#ifndef SECONDWIDGET_H
#define SECONDWIDGET_H

#include <QWidget>
#include "mainwidget.h"

namespace Ui {
class secondwidget;
}

class secondwidget : public QWidget
{
    Q_OBJECT

public:
    explicit secondwidget(QWidget *parent = nullptr);
    ~secondwidget();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_fre_clicked();

    void on_sec_clicked();

    void on_back_clicked();

private:
    Ui::secondwidget *ui;
};

#endif // SECONDWIDGET_H
