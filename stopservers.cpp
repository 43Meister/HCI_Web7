#include "stopservers.h"
#include "ui_stopservers.h"
#include "servermanager.h"

CStopServers::CStopServers(QWidget *parent) :
    QDialog(parent),
    CLogable("StopServerLogger"),
    ui(new Ui::CStopServers)
{
    ui->setupUi(this);
}

CStopServers::~CStopServers()
{
    delete ui;
}


void CStopServers::show()
{
    std::string errMsg("");
    ui->comboBox->clear();

    auto data = std::move(CServerManager::getReference().getTableData());
    ui->comboBox->addItem("All", QVariant(-1));


    for (auto& 