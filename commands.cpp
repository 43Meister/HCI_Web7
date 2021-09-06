#include "commands.h"
#include "ui_commands.h"
#include "servermanager.h"
#include "argwindowmine.h"

#define EXE_LAMBDA QString reslt; \
                   auto exe([this, &reslt](int32_t ind, bool showResp, const QString& cmd, const QString& args)->bool \
                   {return this->runCommand(ind, showResp, cmd, args, reslt);});

CCommands::CCommands(QWidget *parent) :
    QDialog(parent),
    CLogable("CommandsLog"),
    ui(new Ui::CCommands)
{
    ui->setupUi(this);
}

CCommands::~CCommands()
{
    delete ui;
}

void CCommands::show()
{
    auto data = std::move(CServerManager::getReference().getTableData());

    initCombo(ui->comboBox, data);
    initCombo(ui->comboBox_2, data);
    initCombo(ui->comboBox_3, data);

    QDialog::show();
}

void CCommands::initCombo(QComboBox* combo, CServerManager::TServTable &data)
{
    combo->clear();
    combo->addItem("None", QVariant(-1));

    for (auto& outer : data)
    {
        if (combo->findText(outer.second[CServerManager::SERVER_NAME]) == -1)
        {
            if (outer.second[CServerManager::STATUS] == CServerManager::ACTIVE)
            {
                combo->addItem(outer.second[CServerManager::SERVER_NAME], QVariant(outer.first));
            }
        }
    }
}

void CCommands::on_comboBox_activated(int index)
{
    m_index = index;
}



void CCommands::on_pushButton_clicked()
{
    auto ind(ui->comboBox_3->currentData().toInt());

    getBalance(ind);
    /*EXE_LAMBDA;

    auto status = execute(ind, std::move(exe), false, true, QString("getbalance"), QString(""));

    if (status)
    {
        auto balance(reslt.split(CServerManager::RESP_SEPERATOR));
        m_serverMng.setBalance(ind, balance[0].toFloat());
    }

    if (ind > -1)
    {
        runCommand(ind, "getbalance", "");
    }*/

}

void CCommands::on_pushButton_2_clicked()
{
    hide();
}

void CCommands::createAddress(quint32 serv, bool showResp)
{
    EXE_LAMBDA;

    auto status = execute(serv, std::move(exe), false, showResp, QString("getnewaddress"), QString(""));

    if (status)
    {
        m_serverMng.setAddress(serv, reslt.split(CServerManager::RESP_SEPERATOR)[0]);
    }
}

void CCommands::on_pushButton_3_clicked()
{
    auto ind(ui->comboBox_3->currentData().toInt());

    createAddress(ind);
}

bool CCommands::runCommand(const uint32_t ind, bool showResp, const QString& cmd, const QString& args, QString& reslt)
{
    bool rv(true);
    std::string errMsg("");

    QByteArray rawJason;

    CServerManager::TStringMap resp(m_serverMng.sendMsg(ind, cmd, args, &rawJason));

    //qDebug() << "raw Response: [ " << rawJason << " ]";
    LOGGER_HELPER(TRACE, errMsg, "Raw response from server: [ " , rawJason, " ]");

    auto res = (resp.begin());

    //qDebug() << res->first << ": [ " << res->second << " ]";
    LOGGER_HELPER(TRACE, errMsg, res->first , ": [ ", res->second, " ]");

    if (res->first != "Response")
    {
        rv = false;
    }

    reslt = res->second;

    if (showResp)
    {
        m_respShower.showFullResp(res->first, res->second, cmd);
    }

    return rv;
}

void CCommands::on_pushButton_4_clicked()
{
    executeHelper("getpeerinfo", ui->comboBox_3);
}

void CCommands::on_pushButton_5_clicked()
{
    //auto ind(ui->comboBox->currentData().toInt());

    QStringList commandAndArgs((ui->lineEdit->text()).split(" ")); //create List of command variants

    QString cmd = commandAndArgs[0];

    commandAndArgs.removeFirst();

    QString args = commandAndArgs.join(" ");

    executeHelper(std::move(cmd), ui->comboBox_3, nullptr, true, std::move(args));

    //EXE_LAMBDA;

    //execute(ind, std::move(exe), false, true, cmd, args);
}

void CCommands::on_pushButton_6_clicked()
{
    static cargWindow argWin;

    auto ind(ui->comboBox_3->currentData().toInt());

    QString cmd("generate");

    auto subExeFunc([this, ind, cmd](const QString& args)->bool
    {
        EXE_LAMBDA;
        return execute(ind, std::move(exe), false, true, cmd, args);
    });

    argWin.show(std::move(subExeFunc), "Number Of Blocks");
}

void CCommands::mine(qint32 ind, qint32 blocks)
{
    QString cmd("generate");

    EXE_LAMBDA;

    QString args;

    QTextStream ts(&args);

    ts << blocks;

    execute(ind, std::move(exe), false, false, cmd, args);

}

void CCommands::on_pushButton_7_clicked()
{
    static cargWindow argWin;
    auto indSender(ui->comboBox->currentData().toInt());
    auto indRecvr(ui->comboBox_2->currentData().toInt());

    QString cmd("sendtoaddress");

    //only if the rcvr is selected
    if (indRecvr != -1)
    {
        QString add(m_serverMng.getAddress(indRecvr));
        add += " ";

        auto subExeFunc([this, indSender, cmd, add](const QString& args)->bool
        {
            EXE_LAMBDA;
            return execute(indSender, std::move(exe), false, true, cmd, add + args);
        });

        argWin.show(std::move(subExeFunc), "Number Of Coins");
    }
}

void CCommands::addNode(quint32 sndrId, quint32 rcvrId, bool showResp)
{
    auto sndrIp(m_serverMng.getIp(sndrId));
    auto rcvrIp(m_serverMng.getIp(rcvrId));

    addNode(sndrId ,sndrIp, rcvrIp, showResp);
}

void CCommands::addNode(quint32 sndrId, QString& sndrIp, QString& rcvrIp, bool showResp)
{
    auto cmd("addnode");

    if (sndrIp != CServerManager::INVALID_IP && rcvrIp != CServerManager::INVALID_IP)
    {
        auto args = rcvrIp + " onetry";
        EXE_LAMBDA;
        execute(sndrId, std::move(exe), false, showResp, cmd, args);
    }
}

void CCommands::on_pushButton_8_clicked()
{
    auto sndrInd(ui->comboBox->currentData().toInt());
    auto rcvrInd(ui->comboBox_2->currentData().toInt());

    addNode(sndrInd, rcvrInd);
}

void CCommands::on_pushButton_9_clicked()
{
    executeHelper("getinfo", ui->comboBox_3);
}

void CCommands::on_pushButton_10_clicked()
{
    executeHelper("getblockchaininfo", ui->comboBox_3);
}

QVector<quint32> CCommands::getActiveServerList()
{
    QVector<quint32> rv;

    auto data = std::move(CServerManager::getReference().getTableData());

    for (auto& outer : data)
    {
        if (outer.second[CServerManager::STATUS] == CServerManager::ACTIVE)
        {
            rv.push_back(outer.first);
        }
    }

    return rv;
}

void CCommands::regAllServer()
{
    std::string errMsg("");

    auto servers = getActiveServerList();

    for (auto regIter(servers.begin()); regIter != servers.end(); regIter++)
    {
        for (auto acceptIter(regIter+1); acceptIter != servers.end(); acceptIter++)
        {
            LOGGER_HELPER(TRACE, errMsg, "Registering servers: [ ", *regIter, " ]" ,
                          " and [ ", *acceptIter, " ]");
            addNode(*regIter, *acceptIter, false);
        }
    }
}

void CCommands::createAddForAll()
{
    std::string errMsg("");

    auto servers = getActiveServerList();

    for (auto serv : servers)
    {
        createAddress(serv, false);
    }
}

void CCommands::on_pushButton_11_clicked()
{
    regAllServer();
}

void CCommands::on_pushButton_12_clicked()
{
    executeHelper("listtransactions", ui->comboBox_3);
}

void CCommands::on_pushButton_13_clicked()
{
    executeHelper("dumpwallet", ui->comboBox_3, nullptr, true, QString("walletInfo.txt"));
}


void CCommands::executeHelper(QString cmd, QComboBox* box1, QComboBox* /*box2*/, bool showReps, QString args)
{
    auto ind1(box1->currentData().toInt());

    EXE_LAMBDA;

    execute(ind1, std::move(exe), false, showReps, cmd, args);
}

double CCommands::getBalance(qint32 ind, bool showResp)
{
    EXE_LAMBDA;

    auto status = execute(ind, std::move(exe), false, showResp, QString("getbalance"), QString(""));

    QStringList balance;

    if (status)
    {
        balance = reslt.split(CServerManager::RESP_SEPERATOR);
        m_serverMng.setBalance(ind, balance[0].toFloat());
    }

    return balance[0].toFloat();
}

void CCommands::sendCoins(qint32 sendr, qint32 rcvr, quint32 amount, bool showResp)
{
    QString cmd("sendtoaddress");

    QString add(m_serverMng.getAddress(rcvr));
    QString args = QString::number(amount);
    add += " ";
    EXE_LAMBDA;

    execute(sendr, std::move(exe), false, showResp, cmd, add + args);
}

void CCommands::on_pushButton_14_clicked()
{
    createAddForAll();
}

QStringList CCommands::getMiningInfo(qint32 ind)
{
    QStringList rv;
    QString cmd{"getmininginfo"};
    auto args{QString("")};

    //we want to have an empty string list as a referance
    rv.clear();

    EXE_LAMBDA;

    auto res =  execute(ind, std::move(exe), false, false, cmd, args);

    if  (res)
    {
        rv = std::move(reslt.split(CServerManager::RESP_SEPERATOR));
    }

    return rv;
}


void CCommands::on_pushButton_15_clicked()
{
    std::string errMsg("");
    QString MASTER_IP("172.17.0.2");

    auto servers = getActiveServerList();

    for (auto serv : servers)
    {
        auto sndrIp(m_serverMng.getIp(serv));

        addNode(serv, sndrIp, MASTER_IP, false);
    }
}
