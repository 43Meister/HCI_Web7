#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QString>
#include <memory>
#include <algorithm>
#include "jasonHttpclient.h"
#include "logable.h"

namespace NBitcoinServer
{
    class serverManager;
}

class CServerManager : public QObject, CLogable
{
    Q_OBJECT
public:


    //