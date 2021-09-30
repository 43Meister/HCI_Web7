#include "logable.h"

CLogable::CLogable(QString logName) :
    m_logger(log4cxx::Logger::get