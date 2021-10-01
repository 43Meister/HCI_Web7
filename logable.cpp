#include "logable.h"

CLogable::CLogable(QString logName) :
    m_logger(log4cxx::Logger::getLogger(logName.toStdString()))
{}
