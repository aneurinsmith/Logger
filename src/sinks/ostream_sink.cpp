
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	void OStreamSink::print(LOG::Level lvl, std::string msg)
	{
		std::cout << msg << std::endl;;
	}
}
