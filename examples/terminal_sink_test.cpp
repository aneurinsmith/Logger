
#include <logger.h>
#include <chrono>
#include <thread>

LOG::Logger logger;

int main()
{
    logger.add_sink(LOG::terminalSink(LOG::NONE));
    //logger.add_sink(LOG::oStreamSink(LOG::NONE));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (int i = 0; i < 10000; i++) {
        std::string msg = "Terminal window message ";
        msg += std::to_string(i);
        logger.print(LOG::TRACE, msg);
    }

    std::this_thread::sleep_for(std::chrono::minutes(10));

    return 0;
}
