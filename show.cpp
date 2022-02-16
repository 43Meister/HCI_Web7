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
    auto tableData = std::move(CServerManager::getRefe