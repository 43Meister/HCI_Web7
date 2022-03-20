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


void CShowResp::showFullResp(const QString& respName, c