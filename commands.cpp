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
    ui->setupUi