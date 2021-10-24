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
        QDebug dbgStream(&streamStr);

        createMsgImpl(dbgStream, std::forward<TArgs>(args)...);

        return streamStr.toStdString();
    }


    log4cxx::LoggerPtr m_logger;

//prvate
    template <typename TFirst, typename ...TArgs>
    void createMsgImpl(QDebug& dbg, TFirst&& first, TArgs&&... args)
    {
        dbg << first << " ";
        createMsgImpl(dbg, std::forward<TArgs>(args)...);
    }

    template<typename ...TArgs>
    void createMsgImpl(QDebug& dbg, std::string str, TArgs&&... args)
    {
        dbg << str.c_str() << " ";
        createMsgImpl(dbg, std::forward<TArgs>(args)...);
    }


    //stop recursion
    void createMsgImpl(QDebug& /*dbg*/)
    {}
};

#endif // CLOGABLE_H
