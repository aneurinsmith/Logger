
#include <logger.h>
#include <thread>

LOG::Logger logger(LOG::oStreamSink());

int main()
{
    logger.add_sink(LOG::fStreamSink());
    for (int i = 0; i < 100; i++) {
        LOG::debug("Hello World!");
    }

    return 0;
}
