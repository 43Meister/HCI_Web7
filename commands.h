
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