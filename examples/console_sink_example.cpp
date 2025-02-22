
#include "logger.h"
#include "timer.h"
#include <thread>

LOG::Logger consoleLogger(LOG::INFO, LOG::consoleSink("INFO"));

int main()
{
    LOG::trace("app start");
    consoleLogger.add_sink(LOG::consoleSink("test", 20));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    LOG::Timer timer;
    timer.start();
    LOG::trace("we are now looping");
    for (int i = 0; i < 10000; i++) {
        std::string msg = "Terminal window message ";
        msg += std::to_string(i);
        consoleLogger.print(LOG::INFO, msg);
    }
    LOG::trace("done");
    LOG::trace("time taken (ms): ", timer.stop());

    std::this_thread::sleep_for(std::chrono::minutes(10));

    return 0;
}
