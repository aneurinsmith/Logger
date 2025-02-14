
#include <logger.h>
#include <thread>

LOG::Logger logger1;
LOG::Logger logger2;

int main() 
{
    auto oss = LOG::oStreamSink();
    auto cs = LOG::consoleSink("console", 100, "%B %d %Y %H:%M");
    logger2.add_sink(cs);
    logger2.print(LOG::INFO, "This console logger has a different datetime format");

    logger1.add_sink(oss);
    logger1.add_sink(cs);
    logger1.print(LOG::NONE, "Multiple sinks can be added to a logger");

    logger1.set_level(LOG::NONE);
    logger1.print(LOG::NONE, "At this level, all messages will be logged");
    logger1.set_level(LOG::FATAL);
    logger1.print(LOG::NONE, "At this level, only fatal message will be shown");
    logger1.set_level(LOG::NONE);

    LOG::print("The default logger, which can be accessed through the inline functions, is also an oStreamSink");

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
