
#include <logger.h>
#include <thread>

LOG::Logger logger;

int main() 
{
    auto oss = LOG::oStreamSink();

    logger.add_sink(LOG::consoleSink());
    logger.add_sink(oss);
    logger.print(LOG::DEBUG, "Multiple sinks can be added to a logger");
    logger.set_level(LOG::NONE);
    logger.print(LOG::TRACE, "At this level, all messages will be logged");
    logger.set_level(LOG::FATAL);
    logger.print(LOG::DEBUG, "At this level, only fatal message will be shown");
    logger.set_level(LOG::DEBUG);
    logger.set_format("%B %d %Y %H:%M");
    logger.print(LOG::DEBUG, "This message will have a different date format");
    logger.set_level(LOG::NONE);

    LOG::debug("All sinks that are added to a logger can be logged to using the inline funtions");

    LOG::print("none");
    LOG::trace("trace");
    LOG::debug("debug");
    LOG::info("info");
    LOG::warn("warn");
    LOG::error("error");
    LOG::fatal("fatal");

    std::this_thread::sleep_for(std::chrono::minutes(10));

    return 0;
}
