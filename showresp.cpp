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