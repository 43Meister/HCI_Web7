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
        runComma