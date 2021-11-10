
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

    return ref;
}

void CServerManager::registerClient()
{
    m_cliMap.emplace(TCliPair(s_id, std::move(SCliWrap(s_id, s_port))));
    s_id++;
    s_port++;
}

void CServerManager::deleteClient(uint id)
{
    auto iter = m_cliMap.find(id);

    if (iter != m_cliMap.end())
    {
        (iter->second).remove();
        //m_cliMap.erase(iter);
    }
}

void CServerManager::startClient(const uint id)
{
    auto iter = m_cliMap.find(id);

    if (iter != m_cliMap.end())
    {
        (iter->second).run();
    }
}

void CServerManager::startAll()
{
    forEach([](TCliPair& pair){(pair.second).run();});
}

void CServerManager::deleteAll()
{
    forEach([](TCliPair& pair){(pair.second).remove();});
}

QStringList CServerManager::toString()
{
    QStringList rv;

    forEach([&rv](TCliPair& pair){
        rv << std::move(pair.second.toString());
    });


    return std::move(rv);
}


CServerManager::TServTable CServerManager::getTableData()
{
    CServerManager::TServTable rv;
    int id(0);

    for (auto& cli : m_cliMap)
    {
       std::map<QString, QString> child;
       child.emplace(std::pair<QString,QString>(SERVER_NAME, (cli.second).getName()));
       child.emplace(std::pair<QString,QString>(END_POINT, (cli.second).getEndPoint()));
       child.emplace(std::pair<QString,QString>(ACCOUNT, (cli.second).getAddress()));
       std::string balance(std::to_string((cli.second.getBalance())));
       child.emplace(std::pair<QString,QString>(BALANCE, QString(balance.c_str())));
       child.emplace(std::pair<QString,QString>(STATUS, (cli.second).isActive()));

       rv.emplace(std::pair<int, std::map<QString, QString> >(id++, std::move(child)));
    }

    return rv;
}

CServerManager::TStringMap CServerManager::sendMsg(int idx, QString cmd, QString args, QByteArray* rawJason)
{
    TStringMap rv;
    auto cli(m_cliMap.find(idx));

    if (cli != m_cliMap.end())
    {

        rv = cli->second.sendMsg(std::move(cmd), std::move(args), rawJason);
    }

    return rv;
}

QString CServerManager::getIp(int idx)
{
    QString rv(INVALID_IP);

    auto cli = m_cliMap.find(idx);

    if (cli != m_cliMap.end())
    {
        rv = QString(BASE_IP).arg(BASE_IP_STATION + idx);
    }

    return rv;
}


//wrapper implementation
CServerManager::SCliWrap::SCliWrap(unsigned int id, unsigned int port) : CLogable("ServerMangerCLILogger")
{
    m_dockName = std::move(QString(DOCKER_NAME).arg(QString(std::to_string(id).c_str())));
    m_dockRm = std::move(QString(DOCKER_RM).arg(m_dockName));
    m_dockCmd = std::move(QString(DOCKER_CMD).arg(QString(std::to_string(port).c_str())).arg(m_dockName).arg(m_dockName).arg(BASE_PORT1 + id).arg(BASE_PORT2 + id));
    m_endPoint = std::move(QString("http://172.17.0.1:%1").arg(std::to_string(port).c_str()));
    m_cli.reset(new CJasonHttpClient(m_endPoint));

    std::string dirName = std::string("bitcoinLV/") + m_dockName.toStdString();

    mkdir(dirName.c_str(), ACCESSPERMS);
}



bool CServerManager::SCliWrap::run()
{
    bool rv(true);

    QString dockStr(std::move(this->toString()));

    LOGGER_HELPER(DEBUG, dockStr.toStdString());

    //build docker
    QString buildDocker(DOCKER_BUILD);
    //create a string with the correct docker build info.
    runDockerCmd(std::move(buildDocker.arg(DOCKER_IMG).arg(DOCKER_LOCK)));

    //remove old docker instance
    remove();

    //run deamon
    QString arguments(std::move(m_dockCmd + " -d=true " + DOCKER_IMG + " " + RUN_DEMON));
    runDockerCmd(std::move(arguments));

    m_isActive = true;

    return rv;
}

bool CServerManager::SCliWrap::remove()
{
    bool rv(true);

    //remove old docker instance
    QString rmArgs = QString(DOCKER_RM).arg(m_dockName);
    runDockerCmd(std::move(rmArgs));

    m_isActive = false;

    return rv;
}

CServerManager::TStringMap CServerManager::SCliWrap::sendMsg(QString cmd, QString args, QByteArray* rawJason)
{
    TStringMap rv;

    QStringList arguments = args.split(" ");

    QJsonRpcMessage sendMsg = std::move(m_cli->prepareMessage(std::move(cmd), std::move(arguments)));

    QJsonRpcMessage rep = std::move(m_cli->sendAndWaitForResp(std::move(sendMsg)));