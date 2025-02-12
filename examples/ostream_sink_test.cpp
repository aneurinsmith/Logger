
#include <logger.h>

LOG::Logger logger;

int main() 
{
    auto oss = LOG::oStreamSink(LOG::DEBUG);
    //oss->print(LOG::DEBUG, "This message will be logged to the output stream sink");
    //oss->print(LOG::TRACE, "This message, however, is too low level to be logged");

    logger.add_sink(oss);
    logger.print(LOG::DEBUG, "Multiple sinks can be added to a logger");
    logger.set_level(LOG::NONE);
    logger.print(LOG::DEBUG, "At this level, all messages will be logged");
    logger.set_format("  %B %d %Y %H:%M");
    logger.print(LOG::DEBUG, "This message will have a different date format");

    LOG::debug("All sinks that are added to a logger can be logged to using the inline funtions");

    return 0;
}
