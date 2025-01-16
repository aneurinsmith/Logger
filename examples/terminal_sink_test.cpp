
#include <logger.h>

LOG::Logger logger(LOG::oStreamSink());

int main()
{
    logger.add_sink(LOG::terminalSink(LOG::DEBUG));
    logger.add_sink(LOG::terminalSink(LOG::WARN));

    logger.print(LOG::TRACE, "Neither terminal sink should receive this message");
    logger.print(LOG::DEBUG, "The debug level terminal will recieve this message");
    logger.print(LOG::WARN, "Both sinks will recieve this message, sent by the logger");

    LOG::warn("Both sinks will recieve this message, sent by an inline fucntion");

    return 0;
}
