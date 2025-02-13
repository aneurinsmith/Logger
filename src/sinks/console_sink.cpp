
#include "console_sink.h"
#include "../console/console.h"

namespace LOG
{
	ConsoleSink::ConsoleSink() :
		console(new Console()) 
	{

	}

	void ConsoleSink::write(Message msg)
	{
		console->push(msg);
	}
}
