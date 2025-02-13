
#include <logger.h>
#include <thread>

LOG::Logger logger(LOG::fStreamSink("session"));
LOG::Logger ossLogger(LOG::oStreamSink());

int main()
{
    for (int i = 0; i < 100; i++) {
        logger.print(LOG::DEBUG, "Hello World!");
    }

    return 0;
}
