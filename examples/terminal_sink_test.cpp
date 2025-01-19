
#include <logger.h>
#include <chrono>
#include <thread>

LOG::Logger logger;

int main()
{
    logger.add_sink(LOG::terminalSink(LOG::NONE));

    logger.print(LOG::TRACE, "Terminal window message");

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
