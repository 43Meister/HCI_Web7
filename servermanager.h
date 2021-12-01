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
    static const char* BALANCE;
    static const char* STATUS;
    static const char* BASE_IP;
    static const char* INVALID_IP;
    static const uint32_t BASE_IP_STATION;

    using TStringPair = std::pair<QString, QString>;

    using TStringMap = std::map<QString, QString>;

    using TServTable = st