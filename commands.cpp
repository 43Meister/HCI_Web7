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

void CCommands::initCombo(QComboBox* combo, CServerManager::TServTa