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


    //static constans
    static const char* ACTIVE;
    static const char* DEACTIVE;
    static const char* RESP_SEPERATOR;
    static const char* SERVER_NAME;
    static const char* END_POINT;
    static const char* ACCOUNT;
    static const c