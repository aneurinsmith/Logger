
#include <stdio.h>
#include <logger.h>

LOG::Logger logger;

int main() {
    logger.set_level(TRACE);
    logger.print("Hello World!");

    LOG::print("Hi");

    return 0;
}