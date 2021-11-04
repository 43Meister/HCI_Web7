
#include "servermanager.h"
#include <QProcess>
#include <algorithm>
#include <QDebug>
#include <QJsonDocument>
#include <QBuffer>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>


#define CLI_WRAPPER CServerManager::SCliWrap;

//constexpr define
using TDockCmdLine = char[255];

//constansts
constexpr TDockCmdLine DOCKER_NAME{"DockerServer%1"};
constexpr TDockCmdLine DOCKER_IMG{"bitcoin-regtest"};
constexpr TDockCmdLine DOCKER_LOCK{"/home/alexd/gak/docker-bitcoin-regtest/bitcoin-regtest"};
constexpr TDockCmdLine DOCKER_CMD{"run -t -p %1:8888 -p %4:18444 -p %5:18332 --name=%2 --hostname=%3 -v /home/alexd/BitcoinServerManager/bitcoinLV/%2:/root/.bitcoin"};
constexpr TDockCmdLine DOCKER_RM{"rm -f %1"};
constexpr TDockCmdLine RUN_DEMON{"bitcoind -regtest -server -rpcuser=test -rpcpassword=test -rpcallowip=0.0.0.0/0 -rpcport=8888 -printtoconsole"};
constexpr TDockCmdLine DOCKER_BUILD{"build -t %1 %2"};
constexpr TDockCmdLine DOCKER{"docker"};
constexpr qlonglong BASE_PORT1{18444};
constexpr qlonglong BASE_PORT2(18332); 

const char* CServerManager::ACTIVE = "Active";
const char* CServerManager::DEACTIVE = "Deactive";
const char* CServerManager::RESP_SEPERATOR = "?!?!";
const char* CServerManager::SERVER_NAME = "1_NAME";
const char* CServerManager::END_POINT = "2_End Point";
const char* CServerManager::ACCOUNT = "3_Account";
const char* CServerManager::BALANCE = "4_Balance";
const char* CServerManager::STATUS = "5_Status";
const char* CServerManager::BASE_IP = "172.17.0.%1"; //BASE IP OF THE DOCKER SERVERS
const char* CServerManager::INVALID_IP = "BAD_IP_ADDR";

const uint32_t CServerManager::BASE_IP_STATION = 3;

//static definitions
unsigned int CServerManager::s_id = 0;
unsigned int CServerManager::s_port = 8888;

using namespace NBitcoinServer;

CServerManager::CServerManager(QObject *parent) : QObject(parent), CLogable("ServerManagerLogger")
{}

CServerManager& CServerManager::getReference()
{
    static CServerManager ref;