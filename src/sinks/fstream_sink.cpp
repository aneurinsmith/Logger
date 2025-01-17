
#include "fstream_sink.h"
#include <iostream>

namespace LOG
{
	void FStreamSink::write(std::string msg)
	{
		std::cout << msg << std::endl;;
	}
}
