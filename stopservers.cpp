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


    for (auto& outer : data)
    {
        if (ui->comboBox->findText(outer.second[CServerManager::SERVER_NAME]) == -1)
        {
            //qDebug() << "the status is: " <<  outer.second[CServerMana