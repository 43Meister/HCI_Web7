
#ifndef STARTSERVERS_H
#define STARTSERVERS_H

#include <QDialog>
#include "servermanager.h"
#include "logable.h"

namespace Ui {
class StartServers;
}

class CStartServers : public QDialog, CLogable
{
    Q_OBJECT

public:
    explicit CStartServers(QWidget *parent = 0);
    ~CStartServers();

    virtual void show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartServers *ui;
};

#endif // STARTSERVERS_H