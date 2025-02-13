
#include <logger.h>
#include <thread>

LOG::Logger logger1;

int main() 
{
    auto oss1 = LOG::oStreamSink();
    auto cs = LOG::consoleSink();

    logger1.add_sink(oss1);
    logger1.add_sink(cs);
    logger1.print(LOG::DEBUG, "Multiple sinks can be added to a logger");
    logger1.set_level(LOG::NONE);
    logger1.print(LOG::TRACE, "At this level, all messages will be logged");
    logger1.set_level(LOG::FATAL);
    logger1.print(LOG::DEBUG, "At this level, only fatal message will be shown");
    logger1.set_level(LOG::NONE);

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
