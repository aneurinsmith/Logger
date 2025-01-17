
#include <logger.h>

LOG::Logger logger;

int main()
{
    logger.add_sink(LOG::fStreamSink());
    LOG::print("Hello World!");

    return 0;
}
