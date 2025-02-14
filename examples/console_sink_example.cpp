
#include <logger.h>
#include <timer.h>
#include <thread>

LOG::Logger consoleLogger;
LOG::Logger ossLogger(LOG::oStreamSink());

int main()
{
    ossLogger.print(LOG::TRACE, "app start");
    consoleLogger.add_sink(LOG::consoleSink("test", 20));
    consoleLogger.add_sink(LOG::consoleSink());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    LOG::Timer timer;
    timer.start();
    ossLogger.print(LOG::TRACE, "we are now looping");
    for (int i = 0; i < 10000; i++) {
        std::string msg = "Terminal window message ";
        msg += std::to_string(i);
        consoleLogger.print(LOG::TRACE, msg);
    }
    ossLogger.print(LOG::TRACE, "done");
    ossLogger.print(LOG::TRACE, "time taken (ms): " + std::to_string(timer.stop()));

    std::this_thread::sleep_for(std::chrono::minutes(10));

    return 0;
}
