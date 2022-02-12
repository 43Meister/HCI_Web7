
#include "settingswin.h"
#include "ui_settingswin.h"
#include "qdebug.h"
#include "servermanager.h"

CSettingsWin::CSettingsWin(QWidget *parent) :
    QDialog(parent),
    CLogable("SettingDialogLog"),
    ui(new Ui::CSettingsWin)
{
    ui->setupUi(this);
}

CSettingsWin::~CSettingsWin()
{
    delete ui;
}

void CSettingsWin::on_buttonBox_accepted()
{
    int numOfDockers = ((ui->lineEdit)->text()).toInt();
    std::string errMsg("");

    for (int docker(0); docker < numOfDockers; ++docker)
    {
        CServerManager::getReference().registerClient();
    }

    //qDebug() << "Created: " << numOfDockers << " Servers";
    LOGGER_HELPER(DEBUG, errMsg, "Created: " , numOfDockers, " Servers");

}

void CSettingsWin::on_buttonBox_rejected()
{
    std::string errMsg("");
    LOGGER_HELPER(DEBUG, errMsg, "Oper canceled");

}

void CSettingsWin::on_lineEdit_editingFinished()
{
    m_numOfDockers = ((ui->lineEdit)->text()).toInt();