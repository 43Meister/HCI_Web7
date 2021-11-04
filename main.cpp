#include "bitcoinserver.h"
#include <QApplication>
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

LoggerPtr g_logger(Logger::getLogger("BitcoinLogger"));

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    log4cxx::PropertyConfigurator::configure("bitCoinserver.log4cxx");

    LOG4CXX_INFO(g_logger, std::string("==================================================================\n\t\t") +
                 "Wellcome to BITNet \n" +
                 "==================================================================");

    BitcoinServer w;
    w.show();

    return a.exec();
}
