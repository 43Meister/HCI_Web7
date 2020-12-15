
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