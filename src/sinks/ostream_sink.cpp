
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
		switch (msg.get_lvl()) {
		case LOG::TRACE: 
			break;
		case LOG::DEBUG:
			break;
		case LOG::INFO: 
			break;
		case LOG::WARN: 
			break;
		case LOG::ERROR:
			break;
		case LOG::FATAL: 
			break;
		}

		std::cout << msg.get_fullString() << std::endl;
	}
}
