
#include "servermanager.h"
#include <QProcess>
#include <algorithm>
#include <QDebug>
#include <QJsonDocument>
#include <QBuffer>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>


#define CLI_WRAPPER CServerManager::SCliWrap;

//constexpr define
using TDockCmdLine = char[255];

//constansts
constexpr TDockCmdLine DOCKER_NAME{"DockerServer%1"};