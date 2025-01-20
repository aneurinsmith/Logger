
#include <logger.h>
#include <chrono>
#include <thread>

LOG::Logger logger(LOG::oStreamSink());

int main()
{
    logger.add_sink(LOG::terminalSink(LOG::NONE));

    for (int i = 0; i < 10000; i++) {
        std::string msg = "Terminal window message ";
        msg += std::to_string(i);
        logger.print(LOG::TRACE, msg);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
