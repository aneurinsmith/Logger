
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	OStreamSink::OStreamSink(std::string fmt) :
		basesink(fmt)
	{

	}

	void OStreamSink::write(Message msg)
	{
	#ifdef win32
		std::cout << "\033[90m";
	#endif

		std::cout << msg.get_tsString();

	#ifdef win32
		switch (msg.get_lvl()) {
		case LOG::TRACE:
			break;
		case LOG::DEBUG:
			std::cout << "\033[97m";
			break;
		case LOG::INFO:
			std::cout << "\033[97;46m";
			break;
		case LOG::WARN:
			std::cout << "\033[43;30m";
			break;
		case LOG::ERROR:
			std::cout << "\033[97;101m";
			break;
		case LOG::FATAL:
			std::cout << "\033[97;41m";
			break;
		}
	#endif

		std::cout << msg.get_lvlString();

	#ifdef win32
		std::cout << "\033[0m";
	#endif

		std::cout << msg.get_msgString() << std::endl;
	}
}
