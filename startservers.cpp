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


    for (auto& outer : 