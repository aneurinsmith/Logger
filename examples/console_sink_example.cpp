
#include <logger.h>
#include <chrono>
#include <thread>
#include <iostream>

LOG::Logger logger;

int main()
{
    std::cout << "app start" << std::endl;
    logger.add_sink(LOG::consoleSink());
    logger.add_sink(LOG::consoleSink());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "we are now looping" << std::endl;
    for (int i = 0; i < 10000; i++) {
        std::string msg = "Terminal window message ";
        msg += std::to_string(i);
        logger.print(LOG::TRACE, msg);
    }
    std::cout << "done" << std::endl;

    std::this_thread::sleep_for(std::chrono::minutes(10));

    return 0;
}
