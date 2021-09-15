
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