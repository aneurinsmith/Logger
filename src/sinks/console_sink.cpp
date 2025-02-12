
#include "console_sink.h"
#include "../console/console.h"

namespace LOG
{
	ConsoleSink::ConsoleSink() : 
		console(new Console()) 
	{

	}

	void ConsoleSink::print(Message msg)
	{
		console->push(msg);
	}
}
