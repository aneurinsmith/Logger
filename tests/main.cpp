
#include <logger.h>

LOG::Logger logger;

int main() {

    logger.add_sink(LOG::oStreamSink());
    LOG::set_level(NONE);

    LOG::trace("Trace");
    LOG::print("");

    LOG::trace("Trace");
    LOG::debug("Debug");
    LOG::info("Info");
    LOG::warn("Warn");
    LOG::error("Error");
    LOG::fatal("Fatal");

    return 0;
}