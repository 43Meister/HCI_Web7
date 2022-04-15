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

    void showFullResp(const QString& respName, const QString& resp, const QString& cmd);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CShowResp *ui;
};

#endif // SHOWRESP_H
