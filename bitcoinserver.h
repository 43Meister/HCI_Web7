#ifndef BITCOINSERVER_H
#define BITCOINSERVER_H

#include <QMainWindow>
#include "servermanager.h"
#include "settingswin.h"
#include "startservers.h"
#include "stopservers.h"
#include "show.h"
#include "commands.h"
#include "logable.h"
#include "demo.h"

namespace Ui {
class BitcoinServer;
}

class BitcoinServer : public QMainWindow, CLogable
{
    Q_OBJECT

public:
    explicit BitcoinServer(QWidget *parent = 0);
    ~BitcoinServer();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

