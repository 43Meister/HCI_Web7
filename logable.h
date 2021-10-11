#ifndef CLOGABLE_H
#define CLOGABLE_H
#include "log4cxx/logger.h"
#include <QString>
#include <QBuffer>
#include <QDebug>
#include <string>
#include <utility>
#include <QTextStream>
#include <QDataStream>

//helper LoggerMacros
#define LOGGER_HELPER(LVL, msg ,ARGS...)           \
    msg = createMsg(ARGS);                         \
    LOG4CXX_##LVL(m_logger, msg)

class CLogable
{
public:
    CLogable(QString logName);

//protected member tahat defines a log
protected:

    template <typename ...TArgs>
    std::string createMsg(TArgs&&... args)
    {
        QString streamStr;
        //QTextStream stream(&streamStr);
        QDebug dbg