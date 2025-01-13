
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	void OStreamSink::write(std::string msg)
	{
		std::cout << msg << std::endl;;
	}
}
