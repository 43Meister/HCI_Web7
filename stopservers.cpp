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
            //qDebug() << "the status is: " <<  outer.second[CServerManager::STATUS];
            LOGGER_HELPER(DEBUG, errMsg, "the status is:", outer.second[CServerManager::STATUS]);

            if (outer.second[CServerManager::STATUS] == CServerManager::ACTIVE)
            {
                ui->comboBox->addItem(outer.second[CServerManager::SERVER_NAME], QVariant(outer.first));
            }
        }
    }

    QDialog::show();
}

void CStopServers::on_pushButton_clicked()
{
    std::string errMsg("");
    auto currVal = ui->comboBox->currentData().toString();
    (currVal == "-1") ? currVal = "All" : "";

    //qDebug() << "Stoping server: [ "  << currVal << "]";
    LOGGER_HELPER(DEBUG, errMsg, "Stopping server: [ " , currVal, " ]");

    if (currVal == "All")
    {
        CServerManager::getReference().deleteAll();
    }
    else //we just need to start one server
    {
        CServerManager::getReference().deleteClient(currVal.toInt());
    }

    hide();
}
