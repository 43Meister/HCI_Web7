
#include "bitcoinserver.h"
#include "ui_bitcoinserver.h"
#include <qdebug.h>
#include <QDir>
#include <sys/mount.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>

const QString MNT_DIR_NAME("bitcoinLV");
const QString LV_NAME("/dev/bitCoinVG/bitcoinLV");
const QString MNT_FULL(MNT_DIR_NAME);
const QString FS("ext4");

BitcoinServer::BitcoinServer(QWidget *parent) :
    QMainWindow(parent),
    CLogable("BitcoinLogger"),
    ui(new Ui::BitcoinServer),
    m_manager(CServerManager::getReference())
{
    ui->setupUi(this);

    if (!mountVolume())
    {
        std::logic_error exp("Couldnt mount lvm stopping!!!");
        throw exp;
    }
}

BitcoinServer::~BitcoinServer()
{
    delete ui;
}

void BitcoinServer::on_pushButton_clicked()
{
    std::string errMsg("");
    LOGGER_HELPER(INFO, errMsg,
                  std::string("==================================================================\n\t\t") +
                  "Exiting Application \n" +
                  "==================================================================");
    m_manager.deleteAll();

    if( umount2(MNT_FULL.toStdString().c_str(), MNT_FORCE) < 0 )
    {
          LOGGER_HELPER(FATAL, errMsg, "unmount failed with error: ", strerror(errno));
    }

    exit(0);
}

void BitcoinServer::on_pushButton_2_clicked()
{
    m_settings.show();
}

void BitcoinServer::on_pushButton_3_clicked()
{
   m_startServer.show();
}

void BitcoinServer::on_pushButton_4_clicked()
{
    m_stopServers.show();
}

void BitcoinServer::on_pushButton_5_clicked()
{
   m_show.show();
}

void BitcoinServer::on_pushButton_6_clicked()
{
    m_commands.show();
}

void BitcoinServer::on_pushButton_7_clicked()
{
    m_demo.show();
}

bool BitcoinServer::mountVolume()
{
    std::string errMsg("");

    struct stat fileStat;
    auto status = stat(MNT_FULL.toStdString().c_str(), &fileStat);
    auto dirStat = 0;

    if (status < 0)
    {
        dirStat = mkdir(MNT_FULL.toStdString().c_str(), ACCESSPERMS);
    }

    if ( dirStat < 0 )
    {
        LOGGER_HELPER(FATAL, errMsg, "mkdir failed with error: ", strerror(errno));
        return false;
    }

    if( -1 == mount(LV_NAME.toStdString().c_str(), MNT_FULL.toStdString().c_str(),
                    FS.toStdString().c_str(), MS_MGC_VAL | MS_SILENT, "") )
    {
       LOGGER_HELPER(ERROR, errMsg, "mount failed with error: ", strerror(errno));
       //return false;
       //for now just for debug we can work without an lv
    }

    return true;
}