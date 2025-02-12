
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	OStreamSink::OStreamSink(LOG::Level lvl, std::string fmt) :
		basesink(lvl, fmt) 
	{

	}

	void OStreamSink::write(std::string msg)
	{
		std::cout << msg << std::endl;
	}
}
