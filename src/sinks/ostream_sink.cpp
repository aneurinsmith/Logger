
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	void OStreamSink::write(Message msg)
	{
		std::string log_level = "";

		switch (msg.lvl) {
		case LOG::TRACE: log_level = "\033[90m[TRACE]\033[0m  "; break;
		case LOG::DEBUG: log_level = "\033[97m[DEBUG]\033[0m  "; break;
		case LOG::INFO:  log_level = "\033[97;46m [INFO]\033[0m  "; break;
		case LOG::WARN:  log_level = "\033[43;30m [WARN]\033[0m  "; break;
		case LOG::ERROR: log_level = "\033[101m[ERROR]\033[0m  "; break;
		case LOG::FATAL: log_level = "\033[41m[FATAL]\033[0m  "; break;
		}

		std::cout << "\033[90m" << msg.ts << "\033[0m\t" << log_level << "\033[0m" << msg.msg << "" << std::endl;
	}
}
