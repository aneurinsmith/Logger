
#include "console_sink.h"
#include "../console/console.h"

namespace LOG
{
	ConsoleSink::ConsoleSink(LOG::Level lvl, std::string fmt) : 
		basesink(lvl, fmt), console(new Console()) 
	{

	}

	void ConsoleSink::write(std::string msg)
	{
		console->push(msg);
	}
}
