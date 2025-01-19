
#include <logger.h>

LOG::Logger logger(LOG::oStreamSink());

int main()
{
    logger.add_sink(LOG::fStreamSink());
    LOG::print("Hello World!");

    return 0;
}
