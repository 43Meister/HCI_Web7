#include "show.h"
#include "ui_show.h"
#include "servermanager.h"

CShow::CShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CShow)
{
    ui->setupUi(this);
}

CShow::~CShow()
{
    delete ui;
}

void CShow::on_pushButton_clicked()
{
    hide();
}

void CShow::on_pushButton_2_clicked()
{
    auto tableData = std::move(CServerManager::getReference().getTableData());

    ui->tableWidget->setRowCount(tableData.size());
    ui->tableWidget->setColumnCount(std::begin(tableData)->second.size());

    QStringList lables;

    for (auto& lable : std::begin(tableData)->second)
    {
       lables << ((lable.first).split("_"))[1];
    }

    ui->tableWidget->setHorizontalH