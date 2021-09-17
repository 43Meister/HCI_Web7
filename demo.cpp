
#include "demo.h"
#include "ui_demo.h"
#include "servermanager.h"
#include "commands.h"
#include <thread>
#include <chrono>

//globals for demo usage
static std::unique_ptr<CCommands> g_commands(nullptr);

//using chrono literals
using namespace std::chrono_literals;

CDemo::CDemo(QWidget *parent) :
    QDialog(parent),
    CLogable("DemoLogger"),
    ui(new Ui::CDemo),
    m_serverManager(CServerManager::getReference()),
    m_miner(std::make_shared<SMiner>()),
    m_player(std::make_shared<SPlayer>()),
    m_logReader(std::make_shared<SLoggingTask>()),
    m_logFile("bitcoinServer.log"),
    m_stream(&m_logFile)
{
    ui->setupUi(this);
    //connect the logging task to the actual ui container
    QObject::connect(m_logReader.get(), SIGNAL(loggerTrigger(QString)), this, SLOT(drawLogger(QString)));
}

void CDemo::show()
{
    //init commands global
    m_logFile.open(QIODevice::ReadOnly|QIODevice::Text);
    m_logFile.seek(m_logFile.size()); //go to the end of the file

    if (!g_commands)
    {
        g_commands.reset(new CCommands);
    }

    std::string errMsg("");
    LOGGER_HELPER(INFO, errMsg,
    std::string("==================================================================\n\t\t") +
    "!!!!DEMO START!!!! \n" +
    "==================================================================");

    //start the logreader task
    m_logReaderTask = std::make_unique<std::thread>
    (std::thread([this]()
    {
        this->m_logReader->mainFunc(this->m_stream);
    }));

    m_logReader->start();

    m_logReaderTask->detach();

    QDialog::show();
}

CDemo::~CDemo()
{
    delete ui;
}

void CDemo::on_pushButton_clicked()
{
    m_miner->stop();
    m_player->stop();
    m_logReader->stop();
    hide();
}

void CDemo::on_pushButton_2_clicked()
{
    std::string errMsg("");
    auto servers = g_commands->getActiveServerList();

    if (servers.size() > 1)
    {
        //create miner and players
        auto miner = servers[0];
        servers.pop_front(); //remove the miner

        //create the sync point
        m_playersFeture = m_minerPromise.get_future();

        //first step is to register all servers
        g_commands->regAllServer();

        //now create accounts for all servers
        g_commands->createAddForAll();

        //creating the demo taks
        m_minerTask = std::make_unique<std::thread>
        (std::thread([this, miner, servers]()
        {
            (this->m_miner)->minerMain(miner, std::move(servers), this->m_minerPromise);
        }));

        m_playerTask = std::make_unique<std::thread>
        (std::thread([this, servers]()
        {
            (this->m_player)->playerMain(this->m_miner, std::move(servers), this->m_playersFeture);
        }));

        m_minerTask->detach();
        m_playerTask->detach();

    }
    else
    {
        LOGGER_HELPER(ERROR, errMsg, "Not enugh active server for demo to run properly , need at least 2 server");
    }
}

void CDemo::SMiner::distCash()
{
    std::string errMsg("");

    auto balance = g_commands->getBalance(miner, false);
    LOGGER_HELPER(INFO, errMsg, "Distrebuting [ ", balance, " ] bitcoins between users");

    for (auto serv : players)
    {
        auto amount = CDemo::getAmount(balance);
        balance -= amount;
        LOGGER_HELPER(INFO, errMsg, QString("Sending [ "), amount, " ] coins to server [ ", serv , " ]");
        LOGGER_HELPER(INFO, errMsg, QString("Amount left"), balance, " ]");

        g_commands->sendCoins(miner, serv, amount, false);