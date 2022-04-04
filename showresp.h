#ifndef SHOWRESP_H
#define SHOWRESP_H

#include <QDialog>
#include <QString>

namespace Ui {
class CShowResp;
}

class CShowResp : public QDialog
{
    Q_OBJECT

public:
    explicit CShowResp(QWidget *parent = 0);
    ~CShowResp();

    void showFullResp(const QString& respName, 