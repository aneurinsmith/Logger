
#include <stdio.h>
#include <logger.h>

LOG::Logger logger;

int main() {
    logger.set_level(TRACE);
    logger.print("Hello World!");
    LOG::print("");

    LOG::print("Print");
    LOG::trace("Trace");
    LOG::print("Print");
    LOG::print("");

    LOG::trace("Trace");
    LOG::debug("Debug");
    LOG::info("Info");
    LOG::warn("Warn");
    LOG::error("Error");
    LOG::fatal("Fatal");

    return 0;
}