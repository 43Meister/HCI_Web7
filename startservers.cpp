#include "startservers.h"
#include "ui_startservers.h"
#include <stdlib.h>
#include <iostream>
#include "servermanager.h"

CStartServers::CStartServers(QWidget *parent) :
    QDialog(parent),
    CLogable("StartServerDialogLog"),
    ui(new Ui::StartServers)
{
    ui->setupUi(this);
}

CStartServers::~CStartServers()
{
    delete ui;
}

void CStartServers::show()
{
    std::string errMsg("");
    ui->comboBox->clear();

    auto data = std::move(CServerManager::getReference().getTableData());
    ui->comboBox->addItem("All", QVariant(-1));


    for (auto& outer : data)
    {
        if (ui->comboBox->findText(outer.second[CServerManager::SERVER_NAME]) == -1)
        {
            //qDebug() << "the status is: " <<  outer.second["Status"];
            LOGGER_HELPER(DEBUG, errMsg, QString("the status is "),  outer.second["Status"]);
            if (outer.second[CServerManager::STATUS] == CServerManager::DEACTIVE)
            {
                ui->comboBox->addItem(outer.second[CServerManager::SERVER_NAME], QVariant(outer.first));
            }
        }
    }

    QDialog::show();
}

void CStartServers::on_pushButton_clicked()
{

    std::string errMsg("");
    auto currVal = ui->comboBox->currentData().toString();
    (currVal == "-1") ? currVal = "All" : "";

    //qDebug() << "Strting server: [ "  << currVal << "]";
    LOGGER_HELPER(DEBUG, errMsg, "Starting server: [ ", currVal ," ]");

    if (currVal == "All")
    {
        CServerManager::getReference().startAll();
    }
    else //we just need to start one server
    {
        CServerManager::getReference().startClient(currVal.toInt());
    }

    hide();
}
