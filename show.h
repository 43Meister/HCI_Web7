
#ifndef SHOW_H
#define SHOW_H

#include <QDialog>
#include "servermanager.h"

namespace Ui {
class CShow;
}

class CShow : public QDialog
{
    Q_OBJECT

public:
    explicit CShow(QWidget *parent = 0);
    ~CShow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CShow *ui;
};

#endif // SHOW_H