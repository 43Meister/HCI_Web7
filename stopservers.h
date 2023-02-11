#ifndef STOPSERVERS_H
#define STOPSERVERS_H

#include <QDialog>
#include "logable.h"

namespace Ui {
class CStopServers;
}

class CStopServers : public QDialog, CLogable
{
    Q_OBJECT

public:
    explicit CStopServers(QWidget *parent = 0);
    ~CStopSer