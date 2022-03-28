#include "showresp.h"
#include "ui_showresp.h"
#include "servermanager.h"

CShowResp::CShowResp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CShowResp)
{
    ui->setupUi(this);
}

CShowResp::~CShowResp()
{
    delete ui;
}


void CShowResp::showFullResp(const QString& respName, const QString& resp, const QString& cmd)
{
    //create string list from resp
    auto respLst(resp.split(CServerManager::RESP_SEPERATOR));

    auto lable(respName + " For: " + cmd);

    ui->label->setText(std::move(lable));

    ui->listWidget->addItems(respLst);

    QDialog::show();
}


void CShowResp::on_buttonBox_accepted()
{
    ui->listWidget->clear();
    hide();
}

void CShowResp::on_buttonBox_rejected()
{
    on_buttonBox_accepted();
}
