
#include "console_sink.h"
#include "../console/console.h"

namespace LOG
{
	ConsoleSink::ConsoleSink(std::string fmt) :
		basesink(fmt),
		console(new Console()) 
	{

	}

	void ConsoleSink::write(Message msg)
	{
		console->push(msg);
	}
}
