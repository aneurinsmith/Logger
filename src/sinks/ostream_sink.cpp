
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	OStreamSink::OStreamSink()
	{

	}

	void OStreamSink::print(Message msg)
	{
		std::cout << msg.msg << std::endl;
	}
}
