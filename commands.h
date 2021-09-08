
#ifndef COMMANDS_H
#define COMMANDS_H

#include <QDialog>
#include "servermanager.h"
#include "showresp.h"
#include <utility>
#include <iostream>
#include <QComboBox>
#include <logable.h>
#include <QVector>

namespace Ui {
class CCommands;
}

class CCommands : public QDialog, CLogable
{

    //this is my friend
    friend class CDemo;

    Q_OBJECT

public:
    explicit CCommands(QWidget *parent = 0);
    ~CCommands();

    //overides the widget show function
    void show();

    void addNode(quint32 sndrId, quint32 rcvrId, bool showResp = true);

    //add all serveres to all nodes
    void regAllServer();

    void createAddress(quint32 serv, bool showResp = true);

    //create an address for all active servers
    void createAddForAll();

private slots:
    void on_comboBox_activated(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

private:

    bool runCommand(const uint32_t ind, bool showResp, const QString& cmd, const QString& args, QString& reslt);

    void initCombo(QComboBox* combo, CServerManager::TServTable& data);

    QVector<quint32> getActiveServerList();

    template <typename TFunc, typename TRet = decltype((std::declval<TFunc>())()), typename... TArgs>
    TRet execute(int32_t ind, TFunc exe, TRet badVal, bool showResp, TArgs&&... args)
    {
        TRet rv(badVal);

        if (ind > -1)
        {
           rv = exe(ind, showResp, std::forward<TArgs>(args)...);
        }

        return rv;
    }

    void executeHelper(QString cmd, QComboBox* box1, QComboBox* box2 = nullptr, bool showeReps = true, QString args = "");

    void mine(qint32 ind, qint32 blocks);

    double getBalance(qint32 ind, bool showResp = true);

    void sendCoins(qint32 sendr, qint32 rcvr, quint32 amount, bool showResp = true);

    QStringList getMiningInfo(qint32 ind);

    void addNode(quint32 sndrId, QString& sndrIp, QString& rcvrIp, bool showResp = true);

    Ui::CCommands *ui;

    CServerManager& m_serverMng = CServerManager::getReference();
    int m_index = -1;
    CShowResp m_respShower;
};

#endif // COMMANDS_H