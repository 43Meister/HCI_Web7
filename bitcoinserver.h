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