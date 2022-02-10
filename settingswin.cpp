
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