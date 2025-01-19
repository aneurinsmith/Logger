
#include <logger.h>

LOG::Logger logger;

int main()
{
    logger.add_sink(LOG::terminalSink(LOG::NONE));

    logger.print(LOG::TRACE, "Terminal window message");

    return 0;
}
